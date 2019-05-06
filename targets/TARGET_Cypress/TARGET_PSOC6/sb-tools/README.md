This directory contains tools and scripts for generating keys, preparing provisioning packets and execution of provisioning.
These files are relevant to CY8CPROTO_064_SB or CY8CPROTO_064_SB_M0_PSA, CY8CPROTO_064_SB_PSA targets.

*	Build tests for PSA targets with these commands:

		mbed test --compile -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n *psa* -v
		mbed test --compile -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *psa* -v

*	Folder ./keys contains default keys. DO NOT COMMIT any new keys to repository.
	You can generate new keys pairs by executing the following comands from ./keys:

		python.exe keygen.py -k 6 --jwk MCUBOOT_CM0P_KEY.json --pem-priv MCUBOOT_CM0P_KEY_PRIV.pem
		python.exe keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem

*	Create provisioning packets:
		Execute from ./prepare folder:	
		
		** In case of compilation for CY8CPROTO_064_SB (CM4 only):		
		python.exe provisioning_packet.py --policy policy_1stage_CM4.json  --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey USERAPP_CM4_KEY.json	

		** In case of compilation for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA:
		python.exe provisioning_packet.py --policy policy_2stage_CM0p.json  --out ../packet --cyboot ../prebuild/CyBootloader_Release/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json

		Prebuild folder contains CyBootloader_WithLogs and CyBootloader_Release with corresponding hex and jwt files.
		* WithLogs enables logs print to terminal.
		* Release does not print to terminal and boots up silently.

		CypressBootloader_CM0p.jwt and CypressBootloader_CM0p.hex must be used in pair from the same directory
		while creation of provisioning packet (.packets/prov_cmd.jwt) and performing of provisioning.

*	Run entrance exam:
		python.exe entrance_exam_runner.py

*	Perform provisioning:
		Execute provision_device_runner.py
		If arguments for the script are not specified it will run with the default arguments.
		Default arguments can be overridden with a custom:
			--prov-jwt FILENAME     Path to provisioning JWT file (packet which contains all
									data necessary for provisioning, including policy, authorization packets and keys)
			--hex FILENAME          Path to Cypress Bootloader HEX binary file
			--pubkey-json FILENAME  File where to save public key in JSON format
			--pubkey-pem FILENAME   File where to save public key in PEM format

		Example:
			python.exe provision_device_runner.py --prov-jwt packet/prov_cmd.jwt --hex prebuild/CyBootloader_Release/CypressBootloader_CM0p.hex --pubkey-json keys/dev_pub_key.json --pubkey-pem keys/dev_pub_key.pem

		Pay attention to mode in which device left after provisioning.

*	Run tests with command:

		mbed test --run -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *psa* -v
		

TROUBLESHOOTING:

1. In case of messeges like "unable to find device" execute "mbedls -m 1907:CY8CPROTO_064_SB", then check with "mbedls" if device is detected as CY8CPROTO_064_SB with code 1907.
2. Keys, from ./keys folder is used for signing images by default, these keys should be used for provisioning.
3. Use CyBootloader from CyBootloader_WithLogs folder. It produces logs, which is useful to undestand if CyBootloader works correctly.
