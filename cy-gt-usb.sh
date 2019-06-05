##!/bin/bash

set -$-ue${DEBUG+xv}

echo 'devops_scripts/*' >> .mbedignore

# License of ARM_C6
export LM_LICENSE_FILE=8224@armlmd-aus-prod-wan.licenses.cypress.com



windows=$(bash --version | grep msys | wc -l)

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
}

function fetch_psoc6pdl_mbed()
{
    local pdl_branch=${1:-"mt2p0"}
    local pdl_build=${2:-"1.2.0.Latest"}
    local pdl_zip="psoc6pdl-${pdl_build}-mbed.zip"

    fetch_asset "$OUT_DIR/psoc6pdl" "repo/psoc6pdl" "$pdl_branch" "$pdl_zip"
}

branch_name=$(git name-rev --name-only HEAD)
echo "Working with branch: $branch_name"

# Only execute this in *usbdev* branches
if [[ $branch_name == *"usbdev"*  || $branch_name == *"halusb"* ]]; then
  echo "Working branch contains usbdev. Updating BSP/CSP and PSOC6PDL"
  
  ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  # Windows: convert /cygdrive/d/git to D:/git
  if [[ ${OS:-} == Windows_NT ]]; then
      ROOT_DIR="$(cygpath --mixed "$ROOT_PATH")"
  else
      ROOT_DIR="$ROOT_PATH"
  fi
  OUT_DIR="$ROOT_DIR/output"
  ASSET_BASEURL="http://iot-webserver.aus.cypress.com/projects/iot_release/ASSETS"
  
  # Integrate the latest PDL/CSP/BSP
  rm -rf $ROOT_DIR/targets/TARGET_Cypress/TARGET_PSOC6/{psoc6csp,psoc6pdl,psoc6cm0p,TARGET_CY8CKIT_062_WIFI_BT/GeneratedSource,TARGET_CY8CMOD_062_4343W/TARGET_CY8CPROTO_062_4343W/GeneratedSource}
  
  mkdir -p "$OUT_DIR/psoc6pdl_mbed"
  echo 'output/*' >> .mbedignore
  unset http_proxy HTTP_PROXY all_proxy ALL_PROXY
  echo "Downloading BSP/CSP Mbed integration asset"
  curl "$ASSET_BASEURL/repo/bsp_csp/develop/Latest/deploy/TARGET_PSOC6.zip" -o "$OUT_DIR/TARGET_PSOC6.zip"
  
  echo "Downloading PDL Mbed integration asset"
  fetch_psoc6pdl_mbed
  echo "Removing existing asset directories"
  
  echo "Extracting BSP/CSP Mbed integration asset"
  unzip -qbo "$OUT_DIR/TARGET_PSOC6.zip" -d "$ROOT_DIR/targets/TARGET_Cypress"
  echo "Extracting PDL Mbed integration asset"
  rm -rf "$OUT_DIR/psoc6pdl/.timestamp"
  cp -r --copy-contents $OUT_DIR/psoc6pdl/* "$ROOT_DIR/targets/TARGET_Cypress/TARGET_PSOC6/" 
else
  echo "Working branch does not contain usbdev. Updating BSP/CSP and PSOC6PDL was not performed"
fi

# Compile single test case just to check linkage issues
mbed config ROOT .

mbed config --list
#configure mbed for toolchains
mbed config GCC_ARM_PATH "C:\Program Files (x86)\GNU Tools ARM Embedded\7 2018-q2-update\bin"

mbed config IAR_PATH "C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.5\arm"

mbed config ARM_PATH "C:\Keil_v5\ARM\ARMCC"
mbed config ARMC6_PATH "C:\Program Files\ARMCompiler6.11\bin"

mbed config --list

echo "Compiling of Cypress USB Device support files. "
#mbed test --compile -n TESTS-usb_device-* --toolchain GCC_ARM --target NUCLEO_F767ZI
#mbed test --run --toolchain GCC_ARM --target NUCLEO_F767ZI


mbed test --compile -n TESTS-usb_device-* --toolchain GCC_ARM --target CY8CKIT_062_WIFI_BT -vv
mbed test --run --toolchain GCC_ARM --target CY8CKIT_062_WIFI_BT
#
#mbed test --compile -n TESTS-usb_device-* --toolchain IAR --target CY8CKIT_062_WIFI_BT
#mbed test --run --toolchain IAR --target CY8CKIT_062_WIFI_BT
#
#mbed test --compile -n TESTS-usb_device-* --toolchain ARMC6 --target CY8CKIT_062_WIFI_BT
#mbed test --run --toolchain ARMC6 --target CY8CKIT_062_WIFI_BT

echo "... done"
