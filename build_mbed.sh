#!/bin/bash

build_dir="$(dirname "${BASH_SOURCE[0]}")"
source "$build_dir/build_assets.sh"

set -$-ue${DEBUG+xv}

# All paths are relative to ROOT_DIR
pushd "$ROOT_DIR" >/dev/null

# Mbed OS git repository URL
MBED_OS_URL="http://git-ore.aus.cypress.com/repo/mbed-os"

# Mbed OS git repository revision
# Can be a branch, tag, or git commit SHA
MBED_OS_REV_DEFAULT="cy-mbed-os-hal-usbdev"

# Allow overridding from the environment or another script
MBED_OS_REV="${MBED_OS_REV:-$MBED_OS_REV_DEFAULT}"

# Set paths to the mbed-os build directories
MBED_OS_DIR="mbed/mbed-os"
MBED_PSOC6_DIR="$MBED_OS_DIR/targets/TARGET_Cypress/TARGET_PSOC6"

MBED_PSOC6_ZIP="${OUT_DIR}/TARGET_PSOC6.zip"

# Download Mbed OS from gitlab to mbed/mbed-os and checkout $MBED_OS_REV
function mbed_fetch()
{
    if [[ ! -d "$MBED_OS_DIR/.git" ]]; then
        rm -rf "$MBED_OS_DIR"
        echo "[INFO] clone $MBED_OS_URL"
        git clone "$MBED_OS_URL" "$MBED_OS_DIR"
    fi
    pushd "$MBED_OS_DIR" >/dev/null
    # MBED_OS_DIR/.git is cached between CI builds.. ensure the git index is cleaned up
    if [[ -n ${CI_PIPELINE_IID:-} ]]; then
        echo "[INFO] clean $MBED_OS_DIR"
        git checkout -f
        git clean -xdf
    fi
    echo "[INFO] fetch $MBED_OS_DIR"
    git fetch --tags
    echo "[INFO] checkout $MBED_OS_REV"
    git checkout "$MBED_OS_REV"
    # Pull MBED_OS_REV from origin
    echo "[INFO] pull $MBED_OS_DIR"
    git pull --ff-only
    echo "Mbed OS git revision: $(git rev-parse --short=8 HEAD)"
    popd >/dev/null
}

# Integrate the latest PDL asset
function mbed_unpack_psoc6pdl()
{
    local pdl_branch=${1:-"mt2p0"}
    local pdl_build=${2:-"Latest"}
    echo "[INFO] integrate psoc6pdl branch=$pdl_branch build=$pdl_build"
    mkdir -p "$OUT_DIR/psoc6pdl_mbed"
    local psoc6pdl_mbed_url="$ASSET_BASEURL/repo/psoc6pdl/${pdl_branch}/${pdl_build}/deploy/TARGET_PSOC6.zip"
    curl -f "$psoc6pdl_mbed_url" -o "$OUT_DIR/psoc6pdl_mbed/TARGET_PSOC6.zip"
    rm -rf "$MBED_PSOC6_DIR"/{psoc6pdl,psoc6cm0p}
    unzip -qbo "$OUT_DIR/psoc6pdl_mbed/TARGET_PSOC6.zip" -d "$MBED_OS_DIR/targets/TARGET_Cypress"
    (cd "$MBED_PSOC6_DIR" && git checkout -f psoc6pdl/devices/include/{cmsis.h,device.h})
}

# Remove TARGET_PSOC6
function mbed_clean_psoc6()
{
    rm -rf "$MBED_PSOC6_DIR"
}

# Remove TARGET_PSOC6/psoc6csp
function mbed_clean_psoc6csp()
{
    rm -rf "$MBED_PSOC6_DIR/psoc6csp"
}

# Copy CSP from CSP_TOP_DIR to mbed/mbed-os/targets/TARGET_Cypress/TARGET_PSOC6/psoc6csp
function mbed_copy_csp()
{
    local csp_target_dir="$MBED_PSOC6_DIR/psoc6csp"
    rm -rf "$csp_target_dir"
    mkdir -p "$MBED_PSOC6_DIR"
    echo "$ROOT_DIR -> $csp_target_dir"
    perform_copy "$SCRIPTS_DIR/psoc6csp.txt" "$ROOT_DIR" "$MBED_PSOC6_DIR/psoc6csp"

    # Doxygen documentation is not delivered to Mbed OS
    echo "Remove $csp_target_dir/docs"
    rm -rf "$csp_target_dir/docs"
}

# Copy BSP from $BSP_TOP_DIR/$bsp to mbed/mbed-os/targets/TARGET_Cypress/TARGET_PSOC6/$bsp_target_dir
function mbed_copy_bsp()
{
    # Allow zero wildcard matches
    shopt -s nullglob

    local bsp=$1
    local bsp_source_dir="${ROOT_DIR}/bsp/$bsp"
    local bsp_target_dir=$2

    echo "$bsp_source_dir -> $bsp_target_dir"
    mkdir -p "$bsp_target_dir"
    # Remove existing directories originating from BSP repository
    rm -rf "$bsp_target_dir"/GeneratedSource
    cp -r "$bsp_source_dir"/{GeneratedSource,design.modus,*.c,*.h} "$bsp_target_dir"
    
    rm -f "$bsp_target_dir/GeneratedSource"/cycfg_capsense.*
    
    # TODO(VMED): workaround for BSP-specific directory
    if [[ -d "$bsp_source_dir/SDIO_HOST" ]]; then
        cp -r "$bsp_source_dir/SDIO_HOST" "$bsp_target_dir"
    fi
}

# Copy HAL and BSPs to Mbed OS
function mbed_copy()
{
    # Copy CSP
    mbed_copy_csp

    # Copy BSPs
    mbed_copy_bsp CY8CKIT-062-WIFI-BT "$MBED_PSOC6_DIR/TARGET_CY8CKIT_062_WIFI_BT"
    mbed_copy_bsp CY8CPROTO-062-4343W "$MBED_PSOC6_DIR/TARGET_CY8CMOD_062_4343W/TARGET_CY8CPROTO_062_4343W"
}

# Create output/TARGET_PSOC6.zip
function mbed_pack()
{
    mkdir -p "${OUT_DIR}"
    [[ -d "$MBED_OS_DIR" ]] || { echo >&2 "[ERROR] $MBED_OS_DIR doesn't exist"; exit 1; }

    echo "Create $MBED_PSOC6_ZIP"
    (cd "$MBED_OS_DIR/targets/TARGET_Cypress" && zip -rqX "$MBED_PSOC6_ZIP" TARGET_PSOC6)
}

# Extract output/TARGET_PSOC6.zip to mbed/mbed-os/targets/TARGET_Cypress
function mbed_unpack()
{
    [[ -f "$MBED_PSOC6_ZIP" ]] || { echo >&2 "[ERROR] artifact $MBED_PSOC6_ZIP does not exist"; exit 1; }

    echo "Extract $MBED_PSOC6_ZIP"
    unzip -qbo "$MBED_PSOC6_ZIP" -d "$MBED_OS_DIR/targets/TARGET_Cypress"
}

# Compile mbed/mbed-os/targets/TARGET_Cypress/TARGET_PSOC6 with a given set of options
function mbed_compile_psoc6()
{
    echo "mbed compile $@"
    mbed compile --library --no-archive \
        --source targets/TARGET_Cypress/TARGET_PSOC6 \
        --profile "$ROOT_DIR/mbed/werror.json" $@
}

# Install Mbed CLI
function mbed_cli_install()
{
    type mbed >/dev/null && return 0

    if [[ $OS == Windows_NT ]]; then
        local pip_conf_dir="C:/ProgramData/pip"
        echo "Installing Cypress certificate to $pip_conf_dir"
        local http_proxy="${http_proxy:-"http://proxy.ua.cypress.com:8080/"}"
        pip config --global set global.proxy "$http_proxy"
        curl -f --connect-timeout 10 http://git-ore.aus.cypress.com/uploads/-/system/temp/b1cb4e0829ea1a316dc488b8842ff579/cacert.pem -o "$pip_conf_dir/cacert.pem"
        pip config --global set global.cert "$pip_conf_dir/cacert.pem"
    fi

    echo "Installing Mbed CLI"
    python -m pip install --upgrade pip setuptools setuptools_scm setuptools_scm_git_archive
    pip install --upgrade PyYAML mbed-cli mbed-ls pyocd
    curl -f --connect-timeout 10 "http://git-ore.aus.cypress.com/repo/mbed-os/raw/master/requirements.txt" -o requirements.txt
    pip install --upgrade -r requirements.txt
    echo "Mbed CLI succesfully installed"
}

if [[ "$0" == "$BASH_SOURCE" ]]; then
    if [[ -n ${CI_PIPELINE_IID:-} ]]; then
        # CI build: create "clean" TARGET_PSOC6.zip with only BSP/HAL sources
        mbed_clean_psoc6
    else
        # Developer build: fetch Mbed OS repository to MBED_OS_DIR
        mbed_fetch
    fi

    mbed_copy
    mbed_pack
fi
