##!/bin/bash

#set -$-ue${DEBUG+xv}

echo 'devops_scripts/*' >> .mbedignore

#_arm_gnu=""
#[[ $1 = mac ]]     && _arm_gnu=OSX
#[[ $1 = ubuntu ]]  && _arm_gnu=Linux64
#[[ $1 = win ]]     && _arm_gnu=Win32
#_arm_compiler=""
#[[ $1 = ubuntu ]]  && _arm_compiler=Linux64
#[[ $1 = win ]]     && _arm_compiler=Win64

## install ARM_GNU
#if [[ ${_arm_gnu} != "" ]]; then
#  devops_scripts/install_tool.sh git@git-ore.aus.cypress.com:devops/tools/ARM_GNU.git gcc_631 ${_arm_gnu}
#  export MBED_GCC_ARM_PATH=$(pwd)/ARM_GNU/${_arm_gnu}/bin
#  echo 'ARM_GNU/*' >> .mbedignore
#fi
#
## install ARM_Compiler
#if [[ ${_arm_compiler} != "" ]]; then
#  devops_scripts/install_tool.sh git@git-ore.aus.cypress.com:devops/tools/ARM_Compiler.git arm_compiler_611 ${_arm_compiler}
#  export MBED_ARMC6_PATH=$(pwd)/ARM_Compiler/${_arm_compiler}/bin
#  export ARMLMD_LICENSE_FILE=8224@armlmd-aus-prod-wan.licenses.cypress.com
#  echo 'ARM_Compiler/*' >> .mbedignore
#fi
#
## install virtualenv
#[[ $1 = mac ]]     && virtualenv env
#[[ $1 = ubuntu ]]  && python3 -m virtualenv env
#[[ $1 = win ]]     && virtualenv env
#[[ $1 = mac ]]     && source env/bin/activate
#[[ $1 = ubuntu ]]  && source env/bin/activate
#[[ $1 = win ]]     && env/Scripts/activate.bat
#echo 'env/*' >> .mbedignore

## install mbed-cli
#pip install mbed-cli --no-cache-dir
#pip install -r requirements.txt --no-cache-dir
#mbed --version
#wd=`pwd`

windows=$(bash --version | grep msys | wc -l)

# Create a new map drive to work around max-path...
if [[ ${windows} -ne 0 ]]; then
  bash -c "subst A: //D" || :
  subst A: .
  cd A:
fi

# Only execute this in *usbdev* branches
if [[ $(git rev-parse --abbrev-ref HEAD) =~ usbdev ]]; then
  # Integrate the latest PDL/CSP/BSP
  OUT_DIR=output
  ASSET_BASEURL="http://iot-webserver.aus.cypress.com/projects/iot_release/ASSETS"
  mkdir -p "$OUT_DIR/psoc6pdl_mbed"
  echo 'output/*' >> .mbedignore
  unset http_proxy HTTP_PROXY all_proxy ALL_PROXY
  echo "Downloading BSP/CSP Mbed integration asset"
  curl "$ASSET_BASEURL/repo/bsp_csp/develop/Latest/deploy/TARGET_PSOC6.zip" -o "$OUT_DIR/TARGET_PSOC6.zip"
  echo "Downloading PDL Mbed integration asset"
  curl "$ASSET_BASEURL/repo/psoc6pdl/mt2p0/Latest/deploy/TARGET_PSOC6.zip" -o "$OUT_DIR/psoc6pdl_mbed/TARGET_PSOC6.zip"
  echo "Removing existing asset directories"
  rm -rf targets/TARGET_Cypress/{psoc6csp,psoc6pdl,psoc6cm0p,TARGET_CY8CKIT_062_WIFI_BT/GeneratedSource,TARGET_CY8CMOD_062_4343W/TARGET_CY8CPROTO_062_4343W/GeneratedSource}
  echo "Extracting BSP/CSP Mbed integration asset"
  unzip -qbo "$OUT_DIR/TARGET_PSOC6.zip" -d targets/TARGET_Cypress
  echo "Extracting PDL Mbed integration asset"
  unzip -qbo "$OUT_DIR/psoc6pdl_mbed/TARGET_PSOC6.zip" -d targets/TARGET_Cypress
fi

# Compile single test case just to check linkage issues
mbed config ROOT .
mbed config --list
mbed compile --library --clean --no-archive --source usb/device/targets/TARGET_Cypress/TARGET_PSOC6 --profile werror.json --toolchain GCC_ARM --target CY8CKIT_062_WIFI_BT
mbed compile --library --clean --no-archive --source usb/device/targets/TARGET_Cypress/TARGET_PSOC6 --profile werror.json --toolchain IAR --target CY8CKIT_062_WIFI_BT
mbed compile --library --clean --no-archive --source usb/device/targets/TARGET_Cypress/TARGET_PSOC6 --profile werror.json --toolchain ARMC6 --target CY8CKIT_062_WIFI_BT

# Delete the mapped drive...
if [[ ${windows} -ne 0 ]]; then
  subst A: //D
fi

cd ${wd}

## uninstall virtualenv
#[[ $1 = mac ]]     && deactivate
#[[ $1 = ubuntu ]]  && deactivate
#[[ $1 = win ]]     && env/Scripts/deactivate.bat

#
echo "... done"
