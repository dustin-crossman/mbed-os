
This directory tree contains Secure images released under Permissive Binary License.

Build by mbed-cli using GNU Arm Embedded - version 6.3.1

These images were compiled by the following command:

```
mbed compile -m CY8CKIT_062_WIFI_BT_M0_PSA -t GCC_ARM --profile release -N psa_release_1.0
mbed compile -m CY8CKIT_062_WIFI_BT_M0_PSA -t GCC_ARM --profile debug -N psa_debug_1.0
```

There are also prebuilt images for PSA tests.

Those images can be found in the test folder under a `CY8CKIT_062_WIFI_BT_PSA` directory

These images were compiled by the following command:

```
mbed test --compile -m CY8CKIT_062_WIFI_BT_M0_PSA -t GCC_ARM --profile debug -n *psa-*
```
<<<<<<< HEAD

To update the prebuilt binaries run the previous commands and then run ```python export_binaries.py```
=======
>>>>>>> 8f70d75d706af3d398e604b7c94b82a6b8c608cd
