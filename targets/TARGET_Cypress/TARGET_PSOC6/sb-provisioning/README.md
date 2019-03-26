This directory contains tools and scripts for generating keys, 
preparing provisioning packets and execution of provisioning.

Do this BEFORE first compilation for CY8CPROTO_064_SB or CY8CPROTO_064_SB_M0_PSA, CY8CPROTO_064_SB_PSA:

*	Generate default keys, by executing the following comands:

		python.exe keygen.py -k 6 --jwk MCUBOOT_CM0P_KEY.json --pem-priv MCUBOOT_CM0P_KEY_PRIV.pem
		python.exe keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem

**  Create provisioning packets:
		Execute from ./prepare folder:	
		In case of compilation for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA:		
		python.exe provisioning_packet.py --policy policy_1stage_CM4.json  --out ../packet --cyboot ../prebuild/CyBootloader_Debug/CypressBootloader_CM0p.jwt --ckey USERAPP_CM4_KEY.json	

* Create provisioning packets:
		In case of compilation for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA:	
		Execute from ./prepare folder:
		python.exe provisioning_packet.py --policy policy_2stage_CM0p.json  --out ../packet --cyboot ../prebuild/CyBootloader_Debug/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json

		Prebuild folder contains CyBootloader_Debug and CyBootloader_Release with corresponding hex and jwt files.
		* Debug enables logs print to terminal.
		* Release does not print to terminal and boots up silently.

		CypressBootloader_CM0p.jwt and CypressBootloader_CM0p.hex must be used in pair from the same directory
		while creation of provisioning packet (.packets/prov_cmd.jwt) and performing of provisioning.

* Perform provisioning, using generated .packets/prov_cmd.jwt (with OpenOCD until PyOCD implemented)

* Build tests for PSA targets with these commands:

		mbed test --compile -m CY8CPROTO_064_SB_M0_PSA -t GCC_ARM -n *psa* -v
		mbed test --compile -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *psa* -v
		
* Run tests with command:

		mbed test --run -m CY8CPROTO_064_SB_PSA -t GCC_ARM -n *psa* -v
		

TROUBLESHOOTING:

1. mbedls -m 1907:CY8CPROTO_064_SB
2. Keys, from ./keys folder is used for signing images by default, these keys should be used for provisioning.
3. Use CyBootloader from CyBootloader_Release folder, as it does not produce logs, which interferes with Greentea framework.
