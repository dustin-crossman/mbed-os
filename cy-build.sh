##

git clone git@git-ore.aus.cypress.com:devops/devops_scripts.git

echo 'devops_scripts/*' >> .mbedignore

_arm_gnu=""
[[ $1 = mac ]]     && _arm_gnu=OSX
[[ $1 = ubuntu ]]  && _arm_gnu=Linux64
[[ $1 = win ]]     && _arm_gnu=Win32
_arm_compiler=""
[[ $1 = ubuntu ]]  && _arm_compiler=Linux64
[[ $1 = win ]]     && _arm_compiler=Win64

# install ARM_GNU
if [[ ${_arm_gnu} != "" ]]; then
  devops_scripts/install_tool.sh git@git-ore.aus.cypress.com:devops/tools/ARM_GNU.git gcc_631 ${_arm_gnu}
  export MBED_GCC_ARM_PATH=$(pwd)/ARM_GNU/${_arm_gnu}/bin
  echo 'ARM_GNU/*' >> .mbedignore
fi

# install ARM_Compiler
if [[ ${_arm_compiler} != "" ]]; then
  devops_scripts/install_tool.sh git@git-ore.aus.cypress.com:devops/tools/ARM_Compiler.git arm_compiler_611 ${_arm_compiler}
  export MBED_ARMC6_PATH=$(pwd)/ARM_Compiler/${_arm_compiler}/bin
  export ARMLMD_LICENSE_FILE=8224@armlmd-aus-prod-wan.licenses.cypress.com
  echo 'ARM_Compiler/*' >> .mbedignore
fi

# install virtualenv
[[ $1 = mac ]]     && virtualenv env
[[ $1 = ubuntu ]]  && python3 -m virtualenv env
[[ $1 = win ]]     && virtualenv env
[[ $1 = mac ]]     && source env/bin/activate
[[ $1 = ubuntu ]]  && source env/bin/activate
[[ $1 = win ]]     && env/Scripts/activate.bat
echo 'env/*' >> .mbedignore

# install mbed-cli
pip install mbed-cli --no-cache-dir
pip install -r requirements.txt --no-cache-dir
mbed --version
wd=`pwd`

windows=$(bash --version | grep msys | wc -l)

# Create a new map drive to work around max-path...
if [[ ${windows} -ne 0 ]]; then
  bash -c "subst A: //D" || :
  subst A: .
  cd A:
fi

# Compile single test case just to check linkage issues
mbed config ROOT .
mbed test --compile --toolchain $2 -n tests-usb_device-basic --target CY8CKIT_062_WIFI_BT -vv
mbed test --compile --toolchain $2 -n tests-usb_device-basic --target CY8CPROTO_062_4343W -vv
#mbed test --compile --toolchain $2 -n tests-usb_device-basic --target CY8CKIT_062_BLE -vv

# Delete the mapped drive...
if [[ ${windows} -ne 0 ]]; then
  subst A: //D
fi

cd ${wd}

# uninstall virtualenv
[[ $1 = mac ]]     && deactivate
[[ $1 = ubuntu ]]  && deactivate
[[ $1 = win ]]     && env/Scripts/deactivate.bat

#
echo "... done"
