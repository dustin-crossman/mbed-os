This directory contains tools and scripts for generating keys, preparing provisioning packets and execution of provisioning.
These files are relevant to CY8CPROTO_064_SB or CY8CPROTO_064_SB_M0_PSA, CY8CPROTO_064_SB_PSA targets.

# DEVICE PROVISIONING

## 1.   Generate new keys by executing the following commands from ./keys:

    *Create keys for image signing:*

    python.exe keygen.py -k 6 --jwk MCUBOOT_CM0P_KEY.json --pem-priv MCUBOOT_CM0P_KEY_PRIV.pem
    python.exe keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem

    *Create key for image encription:*

    python.exe keygen.py --aes aes.key

        
**_NOTE_:** DO NOT COMMIT any new keys to repository. ---

## 2.   Create provisioning packets:
Execute from ./prepare folder:

* To create packet for CY8CPROTO_064_SB target using single-stage policy (CM4 only):
    
        python.exe provisioning_packet.py --policy policy_single_stage_CM4.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/USERAPP_CM4_KEY.json

* To create packet for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA targets using dual-stage policy (CM0 and CM4):
    
        python.exe provisioning_packet.py --policy policy_dual_stage_CM0p_CM4.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json
        
* To use external memory (via SMIF) as staging(upgrade) area (slot_1) of NSPE (CM4) image use policy file with corresponding name:

        python.exe provisioning_packet.py --policy policy_dual_stage_CM0p_CM4_smif.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json
        
* To enable SMIF make sure targets.json has following names defined: SMIF MCUBOOT_USE_SMIF_STAGE, MCUBOOT_USE_SMIF_XIP. This applies for AUGUST_CYW43012_M0_PSA and CY8CPROTO_064_SB_M0_PSA targets. smif_id = 0 if SMIF disabled or N/A; smif_id = 1 for CY8CPROTO_064_SB; smif_id = 2 for AUGUST_CYW43012.
        
Prebuild folder contains CyBootloader_WithLogs and CyBootloader_Release with corresponding hex and jwt files.
  * WithLogs enables logs print to terminal.
  * Release does not print to terminal and boots up silently.

**_NOTE:_** CypressBootloader_CM0p.jwt and CypressBootloader_CM0p.hex must be used in pair from the same directory while creation of provisioning packet (.packets/prov_cmd.jwt) and performing of provisioning. ---

## 3. Run entrance exam

        python.exe entrance_exam_runner.py

## 4.   Perform provisioning:

Execute *provision_device_runner.py*.
If arguments for the script are not specified it will run with the default arguments.

Default arguments can be overridden with a custom:

    --prov-jwt FILENAME     Path to provisioning JWT file (packet which contains all data necessary for provisioning, including policy, authorization packets and keys)
    --hex FILENAME          Path to Cypress Bootloader HEX binary file
    --pubkey-json FILENAME  File where to save public key in JSON format
    --pubkey-pem FILENAME   File where to save public key in PEM format

*Example:*
    
    python.exe provision_device_runner.py --prov-jwt packet/prov_cmd.jwt --hex prebuild/CyBootloader_Release/CypressBootloader_CM0p.hex --pubkey-json keys/dev_pub_key.json --pubkey-pem keys/dev_pub_key.pem
    
**_NOTE:_** PSoC 6 supply voltage must be 2.5 V to perform provisioning.

# TESTS
        
1.  Build and run tests for PSA targets with these commands (valid for mbed-os starting from 5.12.2 and later):

        Run commands:
        mbed test --compile --build OUT/CY8CPROTO_064_SB_M0_PSA -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n components-*psa* -v
        cd mbed-os/OUT and create file witn name .mbedignore and with contents " * " (only asterix) 
        mbed test --compile --build OUT/CY8CPROTO_064_SB_PSA -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n components-*psa* -v --run
         
        Run commands:
        mbed test --compile --build OUT/CY8CPROTO_064_SB_M0_PSA -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n *spm_client* -DUSE_PSA_TEST_PARTITIONS -DUSE_CLIENT_TESTS_PART1 -v
        mbed test --compile --build OUT/CY8CPROTO_064_SB_PSA -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *spm_client* -DUSE_PSA_TEST_PARTITIONS -DUSE_CLIENT_TESTS_PART1 -v --run
         
        Run commands:
        mbed test --compile --build OUT/CY8CPROTO_064_SB_M0_PSA -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n *spm_server -DUSE_PSA_TEST_PARTITIONS -DUSE_SERVER_TESTS_PART1 -DUSE_SERVER_TESTS_PART2 -v
        mbed test --compile --build OUT/CY8CPROTO_064_SB_PSA -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *spm_server -DUSE_PSA_TEST_PARTITIONS -DUSE_SERVER_TESTS_PART1 -DUSE_SERVER_TESTS_PART2 -v --run
         
        Run commands:
        mbed test --compile --build OUT/CY8CPROTO_064_SB_M0_PSA -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n *spm_smoke -DUSE_PSA_TEST_PARTITIONS -DUSE_SMOKE_TESTS_PART1 -v
        mbed test --compile --build OUT/CY8CPROTO_064_SB_PSA -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *spm_smoke -DUSE_PSA_TEST_PARTITIONS -DUSE_SMOKE_TESTS_PART1 -v --run
         
        Run commands:
        mbed test --compile --build OUT/CY8CPROTO_064_SB_M0_PSA -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n tests-psa-crypto_access* -DUSE_PSA_TEST_PARTITIONS -DUSE_CRYPTO_ACL_TEST -v
        mbed test --compile --build OUT/CY8CPROTO_064_SB_PSA -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n tests-psa-crypto_access* -DUSE_PSA_TEST_PARTITIONS -DUSE_CRYPTO_ACL_TEST -v --run
         
        Run commands:
        mbed test --build OUT/CY8CPROTO_064_SB_M0_PSA --compile -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n tests-psa-* -v
        mbed test --build OUT/CY8CPROTO_064_SB_PSA --compile -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n tests-psa-* -v
        mbedgt -i tests-psa-spm*,tests-psa-crypto_access* -v

**_NOTE:_** In case of using non Windows platform flag --build OUT/@TARGET_NAME@ can be ommited.

# TROUBLESHOOTING:

1. In case of messages like "unable to find device" execute "mbedls -m 1907:CY8CPROTO_064_SB", then check with "mbedls" if device is detected as CY8CPROTO_064_SB with code 1907.
2. Keys, from ./keys folder is used for signing images by default, these keys should be used for provisioning.
3. Use CyBootloader from CyBootloader_WithLogs folder. It produces logs, which is useful to understand whether CyBootloader works correctly.
