#!/bin/bash

# Set path to the root directory of the git repository
#ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Windows: convert /cygdrive/d/git to D:/git
if [[ ${OS:-} == Windows_NT ]]; then
    ROOT_DIR="$(cygpath --mixed "$ROOT_PATH")"
else
    ROOT_DIR="$ROOT_PATH"
fi

# Apply developer configuration
[[ -e "$ROOT_DIR/build/custom.sh" ]] && source "$ROOT_DIR/build/custom.sh"

# Set path to the build scripts
#SCRIPTS_DIR="$ROOT_DIR/build"
SCRIPTS_DIR="$ROOT_DIR"

# Set path to the asset output directory
OUT_DIR="$ROOT_DIR/output"

# Set path to the PDL source directory
PDL_DIR=${PDL_DIR:-"$OUT_DIR/psoc6pdl"}

# Set packaging variables
PKG_MAJOR=1
PKG_MINOR=0
PKG_PATCH=0
PKG_BUILD=${CI_PIPELINE_IID:-0}
PKG_VERSION=${PKG_MAJOR}.${PKG_MINOR}.${PKG_PATCH}.${PKG_BUILD}

# This function checks if the .timestamp file is up to date
# Arguments:
# 1 - path to the .timestamp file
# 2 - the threshold in seconds to decide if the timestamp is new enough
#     default threshold: 86400 seconds = 60*60*24 = 1 day
# Returns:
# 0 - the timestamp is up to date
# 1 - the timestamp is out of date
#
function check_timestamp()
{
    local timestamp_file=$1
    local threshold_sec=${2:-86400}
    # when the timestamp file doesn't exist, always download the artifacts
    [[ -f "$timestamp_file" ]] || return 1
    # read the timestamp from file
    local timestamp_date=$(< "$timestamp_file")
    # convert timestamp to UNIX time in seconds.
    # GNU date: use "date -d", BSD date: use "date -j -f"
    local timestamp_sec=$(date -d "$timestamp_date" "+%s" 2>/dev/null || \
    date -j -f "%Y-%m-%d %H:%M:%S" "$timestamp_date" "+%s" || echo 0)
    # determine the current UNIX time in seconds
    local current_sec=$(date "+%s")
    # check if the current time exceeds timestamp by threshold
    return $(( $current_sec - $timestamp_sec >= $threshold_sec ))
}

# This function saves the timestamp to file in the following format:
# 2019-04-03 14:09:53
function save_timestamp()
{
    local timestamp_file=$1
    local current_date=$(date "+%Y-%m-%d %H:%M:%S")
    echo "$current_date" > "$timestamp_file"
}

# Avoid calling downloaddeps.sh on each shell script invocation
# by saving the timestamp to CY_DEP_DIR
# When the dependency file
function check_deps()
{
    local deps_file="$1"
    local timestamp_file="$CY_DEP_DIR/.timestamp"
    # Always download dependencies when deps_file is newer
    if [[ "$deps_file" -nt "$timestamp_file" ]] || ! check_timestamp "$timestamp_file"; then
        sh "${SCRIPTS_DIR}/downloaddeps.sh" $@
        save_timestamp "$timestamp_file"
    fi
}

# Set OS-specific directories
KERNEL="$($(which uname) -s)"
case "$KERNEL" in
    CYGWIN*|MINGW*|MSYS*)
        # Cygwin, MinGW, Git Bash, MSYS2 provide cygpath command
        command -v cygpath >/dev/null 2>&1 || { echo >&2 "ERROR: cygpath tool not found."; exit 1; }

        # Download Windows dependencies
        export CY_DEP_DIR="${CY_DEP_DIR:-$LOCALAPPDATA/cydep}"
        check_deps "${SCRIPTS_DIR}/dependencies-windows.txt"
        export PATH="$(cygpath --unix "$CY_DEP_DIR/doxygen-1.8.14.windows.x64.bin"):$PATH"

        # Set path to GCC toolchain root directory
        export GCC_DIR="$(cygpath --mixed "${GCC_DIR:-$CY_DEP_DIR/gcc-7.2.1-1.0.0.1-windows}")"

        # Set path to MDK toolchain root directory
        export MDK_DIR="$(cygpath --mixed "${MDK_DIR:-C:/Keil_v5/ARM/ARMCC}")"

        # Set path to IAR toolchain root directory
        export IAR_DIR="$(cygpath --mixed "${IAR_DIR:-C:/Program Files (x86)/IAR Systems/Embedded Workbench 8.2/arm}")"

        # Set path to LLVM toolchain root directory
        export LLVM_DIR="$(cygpath --mixed "${LLVM_DIR:-C:/Program Files/LLVM}")"

        # Set path to ARMCC toolchain root directory
        export ARMCC_DIR="$(cygpath --mixed "${ARMCC_DIR:-C:/Program Files/ARMCompiler6.11}")"

        # Set toolchain paths for Mbed CLI
        export MBED_GCC_ARM_PATH="$GCC_DIR/bin"
        export MBED_ARM_PATH="$MDK_DIR"
        export MBED_IAR_PATH="$IAR_DIR"
        export MBED_ARMC6_PATH="$ARMCC_DIR/bin"
        ;;
    Linux*)
        # Download Linux dependencies
        XDG_CACHE_HOME="${XDG_CACHE_HOME:-$HOME/.cache/cydep}"
        export CY_DEP_DIR="${CY_DEP_DIR:-$XDG_CACHE_HOME}"
        check_deps "${SCRIPTS_DIR}/dependencies-linux.txt"
        export PATH="$CY_DEP_DIR/doxygen-1.8.14.linux.bin:$PATH"
        # Doxygen needs libclang.so.6 but older Ubuntu doesn't have it
        export LD_LIBRARY_PATH="$CY_DEP_DIR/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04/lib"

        # Set path to GCC toolchain root directory
        export GCC_DIR="${GCC_DIR:-$CY_DEP_DIR/gcc-7.2.1-1.0.0.1-linux}"

        # Set toolchain paths for Mbed CLI
        export MBED_GCC_ARM_PATH="$GCC_DIR/bin"
        ;;
    Darwin*)
        # Download MacOS dependencies
        XDG_CACHE_HOME="${XDG_CACHE_HOME:-$HOME/.cache/cydep}"
        export CY_DEP_DIR="${CY_DEP_DIR:-$XDG_CACHE_HOME}"
        check_deps "${SCRIPTS_DIR}/dependencies-osx.txt"
        export PATH="$CY_DEP_DIR/doxygen-1.8.14-osx/Doxygen.app/Contents/Resources:$PATH"

        # Set path to GCC toolchain root directory
        export GCC_DIR="${GCC_DIR:-$CY_DEP_DIR/gcc-7.2.1-1.0.0.1-macos}"

        # Set path to ARMCC toolchain root directory
        export ARMCC_DIR="${ARMCC_DIR:-/Applications/MbedStudio.app/Contents/tools/ac6}"

        # Apply MbedStudio ARMCC license from ac6-license.dat
        # Note: this license can only be used to compile Mbed OS
        [[ -f "${ARMCC_DIR}-license.dat" ]] && ARMLMD_LICENSE_FILE="${ARMCC_DIR}-license.dat"

        # Set toolchain paths for Mbed CLI
        export MBED_GCC_ARM_PATH="$GCC_DIR/bin"
        export MBED_ARMC6_PATH="$ARMCC_DIR/bin"
        ;;
    *)
        echo >&2 "[ERROR]: unsupported OS: $KERNEL"
        exit 1
        ;;
esac

# Set ARM Compiler license server (if not already set)
export ARMLMD_LICENSE_FILE="${ARMLMD_LICENSE_FILE:-"8224@armlmd-aus-prod-wan.licenses.cypress.com"}"
export LM_LICENSE_FILE="${LM_LICENSE_FILE:-"8224@armlmd-aus-prod-wan.licenses.cypress.com"}"

ASSET_BASEURL="http://iot-webserver.aus.cypress.com/projects/iot_release/ASSETS"

# This function downloads the asset from the staging directory
# Arguments:
# 1: local asset directory (absolute, or relative to ROOT_DIR)
# 2: remote asset directory, relative to ASSET_BASEURL (for example: repo/psoc6pdl)
# 3: git branch used to build the asset (for example: mt2p0)
# 4: asset zip file name (for example: psoc6pdl-1.2.0.175.zip)
#    Use "Latest" to download the latest build (for example: psoc6pdl-1.2.0.Latest.zip)
function fetch_asset()
{
    local asset_dir=$1
    local asset_addr=$2
    local git_branch=$3
    local asset_zip=$4
    local asset_baseurl="$ASSET_BASEURL/$asset_addr/$git_branch"
    local asset_timestamp="$asset_dir/.timestamp"

    # Never remove existing git clone
    if [[ -e "$asset_dir/.git" ]]; then
        echo "Found existing git checkout at $asset_dir"
        echo "To bootstrap the $asset_addr artifact from asset directory:"
        echo rm -rf "$asset_dir"
        return
    fi

    # Check if the timestamp is up to date
    check_timestamp "$asset_timestamp" && return

    # Check if asset staging directory is accessible
    # Do not remove existing artifacts when offline
    if [[ -d "$asset_dir" ]] && ! curl -sSf "$ASSET_BASEURL" >/dev/null 2>&1; then
        echo "Skip $asset_dir download: $ASSET_BASEURL is not accessible"
        return
    fi

    local latest_zip_regex='(.*)([0-9]+\.[0-9]+\.[0-9]+)\.Latest(.*)'
    local fixed_zip_regex='(.*)([0-9]+\.[0-9]+\.[0-9]+)\.([0-9]+)'
    if [[ $asset_zip =~ $latest_zip_regex ]]; then
        local zip_prefix="${BASH_REMATCH[1]}${BASH_REMATCH[2]}"
        local zip_suffix="${BASH_REMATCH[3]}"
        local bld_info_url="$asset_baseurl/Latest/_bld_info.txt"
        local bld_ver=$(curl --silent "$bld_info_url" | head -1 | sed -e "s,.* ,,")
        local asset_version=${BASH_REMATCH[2]}.${bld_ver}
        asset_zip="${zip_prefix}.${bld_ver}${zip_suffix}"
    elif [[ $asset_zip =~ $fixed_zip_regex ]]; then
        local bld_ver="${BASH_REMATCH[3]}"
        local asset_version=${BASH_REMATCH[2]}.${bld_ver}
    else
        local bld_info_url="$asset_baseurl/Latest/_bld_info.txt"
        local bld_ver=$(curl --silent "$bld_info_url" | head -1 | sed -e "s,.* ,,")
        local asset_version=${bld_ver}
    fi

    if [[ -d "$asset_dir" ]]; then
        if [[ ! -f "$asset_dir/version.xml" ]]; then
            echo "$asset_addr asset time stamp out of date, downloading $asset_version"
            rm -rf "$asset_dir"
            rm -f "$OUT_DIR/$asset_zip"
        fi

        local asset_version_xml=$(< "$asset_dir/version.xml")
        if [[ "$asset_version_xml" == "<version>${asset_version}</version>" ]]; then
            echo "$asset_addr asset version at $asset_dir is up to date: $asset_version, skip asset download"
            return 0
        fi

        echo "$asset_addr asset version at $asset_dir is out of date, downloading $asset_version"
        rm -rf "$asset_dir"
        rm -f "$OUT_DIR/$asset_zip"
    fi

    local asset_zip_url="$asset_baseurl/$bld_ver/deploy/$asset_zip"
    if [[ ! -f "$OUT_DIR/$asset_zip" ]]; then
        echo "Fetch $asset_zip_url"
        mkdir -p "$OUT_DIR"
        curl -f --connect-timeout 10 "-#" "$asset_zip_url" -o "$OUT_DIR/$asset_zip"
    fi

    echo "Extract $asset_zip to $asset_dir"
    mkdir -p "$(dirname "$asset_dir")"
    unzip -qbo "$OUT_DIR/$asset_zip" -d "$asset_dir"
    save_timestamp "$asset_timestamp"
}

# This function downloads PDL asset
function fetch_psoc6pdl()
{
    local pdl_branch=${1:-"mt2p0"}
    local pdl_build=${2:-"1.2.0.Latest"}
    local pdl_zip="psoc6pdl-${pdl_build}.zip"
    local cm0p_zip="psoc6cm0p-${pdl_build}.zip"

    fetch_asset "$OUT_DIR/psoc6pdl" "repo/psoc6pdl" "$pdl_branch" "$pdl_zip"
    fetch_asset "$OUT_DIR/psoc6cm0p" "repo/psoc6pdl" "$pdl_branch" "$cm0p_zip"
}

function fetch_psoc6pdl()
{
    local pdl_branch=${1:-"mt2p0"}
    local pdl_build=${2:-"1.2.0.Latest"}
    local pdl_zip="psoc6pdl-${pdl_build}.zip"
    local cm0p_zip="psoc6cm0p-${pdl_build}.zip"

    fetch_asset "$OUT_DIR/psoc6pdl" "repo/psoc6pdl" "$pdl_branch" "$pdl_zip"
    fetch_asset "$OUT_DIR/psoc6cm0p" "repo/psoc6pdl" "$pdl_branch" "$cm0p_zip"
}

function fetch_psoc6pdl_mbed()
{
    local pdl_branch=${1:-"mt2p0"}
    local pdl_build=${2:-"1.2.0.Latest"}
    local pdl_zip="psoc6pdl-${pdl_build}-mbed.zip"

    fetch_asset "$OUT_DIR/psoc6pdl" "repo/psoc6pdl" "$pdl_branch" "$pdl_zip"
}

#This function downloads WHD asset
function fetch_whd()
{
    rm -rf "$OUT_DIR/whd"
    fetch_asset "$OUT_DIR/whd" "repo/whd" "master" "whd.zip"
}

#This function downloads capsense and ble middleware
function fetch_middleware()
{
    fetch_asset "$OUT_DIR/capsense" "repo/middleware-capsense" "mt2p0" "middleware-capsense-2.0.0.Latest.zip"
    fetch_asset "$OUT_DIR/ble" "repo/middleware-bless" "mt2p0" "middleware-bless-3.20.0.Latest.zip"
}

function extract_artifacts()
{
    # Discover the extracted assets in the output directory
    # The variables are later used by test scripts and Makefiles
    export CSP_TOP_DIR="$OUT_DIR/psoc6csp"
    export BSP_TOP_DIR="$OUT_DIR/bsp"

    # Extract CSP asset
    local csp_zip="$OUT_DIR/psoc6csp-$PKG_VERSION.zip"
    if [[ ! -d "$CSP_TOP_DIR" ]]; then
        [[ -f "$csp_zip" ]] || { echo "[ERROR] artifact $csp_zip does not exist"; exit 1; }

        echo "Extract $csp_zip"
        unzip -qbo "$csp_zip" -d "$CSP_TOP_DIR"
    fi

    # Extract BSP assets
    for bsp_zip in "$BSP_TOP_DIR"/*-$PKG_VERSION.zip; do
        echo "Extract $bsp_zip"
        bsp_path=${bsp_zip%-$PKG_VERSION.zip} # Strip suffix
        mkdir -p "$BSP_TOP_DIR"
        unzip -qbo "$bsp_zip" -d "$bsp_path"
    done

    # Extract psoc6 make recipe
    local psoc6make_zip="$OUT_DIR/psoc6make-$PKG_VERSION.zip"
    mkdir -p "$OUT_DIR/psoc6make"
    echo "Extract $psoc6make_zip"
    unzip -qbo "$psoc6make_zip" -d "$OUT_DIR/psoc6make/make"

    # Extract 20819-BT make recipe
    local BTmake_zip="$OUT_DIR/20819-BTmake-$PKG_VERSION.zip"
    mkdir -p "$OUT_DIR/20819-BTmake"
    echo "Extract $BTmake_zip"
    unzip -qbo "$BTmake_zip" -d "$OUT_DIR/20819-BTmake/make"

    echo "Extracted artifacts:"
    echo CSP_TOP_DIR=$CSP_TOP_DIR
    echo BSP_TOP_DIR=$BSP_TOP_DIR
}
