This directory contains tools and scripts for generating keys, preparing provisioning packets and execution of provisioning.
These files are relevant to CY8CPROTO_064_SB or CY8CPROTO_064_SB_M0_PSA, CY8CPROTO_064_SB_PSA targets.
Version of Python required is 3.7+

# DEVICE PROVISIONING

## 1.   Generate new keys by executing the following commands from ./keys:

    *Create keys for image signing:*

    python keygen.py -k 6 --jwk MCUBOOT_CM0P_KEY.json --pem-priv MCUBOOT_CM0P_KEY_PRIV.pem
    python keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem

    *Create key for image encryption:*

    python keygen.py --aes aes.key

        
**_NOTE_:** DO NOT COMMIT any new keys to repository. ---

## 2.   Create provisioning packets:
Execute from ./prepare folder:

* To create packet for CY8CPROTO_064_SB target using single-stage policy (CM4 only):
    
        python provisioning_packet.py --policy policy_single_stage_CM4.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/USERAPP_CM4_KEY.json

* To create packet for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA targets using dual-stage policy (CM0 and CM4):
    
        python provisioning_packet.py --policy policy_dual_stage_CM0p_CM4.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json
        
* To use external memory (via SMIF) as staging(upgrade) area (slot_1) of NSPE (CM4) image use policy file with corresponding name:

        python provisioning_packet.py --policy policy_dual_stage_CM0p_CM4_smif.json --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json
        
* To enable SMIF make sure targets.json has following names defined: SMIF MCUBOOT_USE_SMIF_STAGE, MCUBOOT_USE_SMIF_XIP. This applies for AUGUST_CYW43012_M0_PSA and CY8CPROTO_064_SB_M0_PSA targets. smif_id = 0 if SMIF disabled or N/A; smif_id = 1 for CY8CPROTO_064_SB; smif_id = 2 for AUGUST_CYW43012.
        
Prebuild folder contains CyBootloader_WithLogs and CyBootloader_Release with corresponding hex and jwt files.
  * WithLogs enables logs print to terminal.
  * Release does not print to terminal and boots up silently.

**_NOTE:_** CypressBootloader_CM0p.jwt and CypressBootloader_CM0p.hex must be used in pair from the same directory while creation of provisioning packet (.packets/prov_cmd.jwt) and performing of provisioning. ---

## 3. Run entrance exam

        python entrance_exam_runner.py

## 4.   Perform provisioning:
**_ATTENTION:_** If UPGRADE image support is requred proceed to **UPGRADE IMAGES** section first

Execute *provision_device_runner.py*.
If arguments for the script are not specified it will run with the default arguments.

Default arguments can be overridden with a custom:

    --prov-jwt FILENAME     Path to provisioning JWT file (packet which contains all data necessary for provisioning, including policy, authorization packets and keys)
    --hex FILENAME          Path to Cypress Bootloader HEX binary file
    --pubkey-json FILENAME  File where to save public key in JSON format
    --pubkey-pem FILENAME   File where to save public key in PEM format

*Example:*
    
    python provision_device_runner.py --prov-jwt packet/prov_cmd.jwt --hex prebuild/CyBootloader_Release/CypressBootloader_CM0p.hex --pubkey-json keys/dev_pub_key.json --pubkey-pem keys/dev_pub_key.pem
    
**_NOTE:_** PSoC 6 supply voltage must be 2.5 V to perform provisioning.

## UPGRADE IMAGES

Secure Boot enabled targets may support image upgrades, if enabled in policy. There are two types of upgrade images supported:
- signed, non encrypted
- signed, encrypted

Type of upgrade images is determined by the following policy setting which are available for firmware sections:

- **_"smif_id":_** 0, - should be set to 2 if AUGUST_CYW43012 target is used, 1 - if CY8CPROTO_064_SB
- **_"upgrade":_** true/false, - should be set to *true* if UPGRADE supported, *false* - if disabled
- **_"encrypt":_** true/false, - should be set to *true* if encrypted UPGRADE supported, *false* - if disabled
- **_"encrypt_key_id":_** 1, - should remain unchaged, means that Device Key will be used in ECDH/HKDF protocol

Requirements:
- Policy with **_smif.json** from prepare/ folder should be used.
For encrypted image:
- aes.key generated, as described in **DEVICE_PROVISIONING - 1**
- dev_pub_key.pem that was obtained after provisioning is located in keys/ folder (default)
- secure_image_parameters.json file in the target directory must contain valid keys paths

Non encrypted UPGRADE image 
**_Example policy for AUGUST_CYW43012:_**
        
        "smif_id": 2,
        "upgrade": true,
        "encrypt": false,
        "encrypt_key_id": 1,
**_Example policy for CY8CPROTO_064_SB:_**
        
        "smif_id": 1,
        "upgrade": true,
        "encrypt": false,
        "encrypt_key_id": 1,

Encrypted UPGRADE image:

**_Example policy for AUGUST_CYW43012:_**
        
        "smif_id": 2,
        "upgrade": true,
        "encrypt": true,
        "encrypt_key_id": 1,
**_Example policy for CY8CPROTO_064_SB:_**
        
        "smif_id": 1,
        "upgrade": true,
        "encrypt": true,
        "encrypt_key_id": 1,

Modified policy file should be used for provisioning the device, as described in paragraph 4.

Now mbed-os application or test can be built as described in section **TESTS**. Images for UPGRADE are generated at build time, according to policy.

- Non enrypted UPGRADE image file name ends with **_upgrade.hex_**
- Enrypted UPGRADE image file name ends with **_enc_upgrade.hex_**

Upgrade image can be programmed to target board using Daplink. Upgrade procedure is performed after first reset.

**_Encrypt generic image:_**
The generic HEX file (for example one that is produced by mbed-os build system) can be converted into encrypted image using script encrypted_image_runner.py located in sb-tools. Example usage:

        python encrypted_image_runner.py --sdk-path . --hex-file someApplication.hex --key-priv keys/MCUBOOT_CM0P_KEY_PRIV.pem --key-pub keys/dev_pub_key.pem --key-aes keys/aes128_key.key --ver 0.1 --img-id 3 --rlb-count 0 --slot-size 0x50000 --pad 1 --img-offset 402653184
    
- **_--sdk-path_** - Path to Secure Boot tools folder
- **_--key-priv_** - ECC Private key used for image signing and for generating shared secret as per ECDH/HKDF.
- **_--key-pub_** - ECC Public key used for image signing and for generating shared secret as per ECDH/HKDF. Only device Key can be used in current implementation. It is generated by provisioning procedure.
- **_--key-aes_** - AES128 key and IV file raw image will be encrypted with.
- **_--img-id_** - Image ID of encrypted image. Must match one mentioned in policy for UPGRADE image.
- **_--slot-size_** - Slot_1 (UPGRADE) size. Must match one mentioned in policy for UPGRADE image.
- **_--ver_** - Version of image. Make sure it matches one defined in secure_image_parameters.json for a given HEX.
- **_--rlb-count_** - Rollback counter. Make sure it matches one defined in secure_image_parameters.json for a given HEX.
- **_--img-offset_** - Starting address offset for UPGRADE image - passed as integer, as represented in policy

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
4. In case of using _smif.json policy field "smif_id": should be 1 for CY8CPROTO_064_SB or 2 for AUGUST_CYW43012
5. Low frequency quarz oscillator should be soldered to CY8CPROTO_064_SB (abscent in stock)
