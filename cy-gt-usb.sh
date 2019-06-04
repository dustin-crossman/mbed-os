##!/bin/bash

set -$-ue${DEBUG+xv}

echo 'devops_scripts/*' >> .mbedignore

# License of ARM_C6
export LM_LICENSE_FILE=8224@armlmd-aus-prod-wan.licenses.cypress.com


source "setenv.sh"

windows=$(bash --version | grep msys | wc -l)


branch_name=$(git name-rev --name-only HEAD)
echo "Working with branch: $branch_name"

# Only execute this in *usbdev* branches
if [[ $branch_name == *"usbdev"*  || $branch_name == *"halusb"* ]]; then
  echo "Working branch contains usbdev. Updating BSP/CSP and PSOC6PDL"
  # Integrate the latest PDL/CSP/BSP
  
  rm -rf targets/TARGET_Cypress/TARGET_PSOC6/{psoc6csp,psoc6pdl,psoc6cm0p,TARGET_CY8CKIT_062_WIFI_BT/GeneratedSource,TARGET_CY8CMOD_062_4343W/TARGET_CY8CPROTO_062_4343W/GeneratedSource}
  
  mkdir -p "$OUT_DIR/psoc6pdl_mbed"
  echo 'output/*' >> .mbedignore
  unset http_proxy HTTP_PROXY all_proxy ALL_PROXY
  echo "Downloading BSP/CSP Mbed integration asset"
  curl "$ASSET_BASEURL/repo/bsp_csp/develop/Latest/deploy/TARGET_PSOC6.zip" -o "$OUT_DIR/TARGET_PSOC6.zip"
  
  echo "Downloading PDL Mbed integration asset"
  fetch_psoc6pdl_mbed
  echo "Removing existing asset directories"
  
  echo "Extracting BSP/CSP Mbed integration asset"
  unzip -qbo "$OUT_DIR/TARGET_PSOC6.zip" -d "$ROOT_PATH/targets/TARGET_Cypress"
  echo "Extracting PDL Mbed integration asset"
  rm -rf "$OUT_DIR/psoc6pdl/.timestamp"
  cp -r --copy-contents $OUT_DIR/psoc6pdl/* "$ROOT_PATH/targets/TARGET_Cypress/TARGET_PSOC6/" 
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
