This directory contains tools and scripts for generating keys, 
preparing provisioning packets and execution of provisioning.

Do this BEFORE first compilation:

* In case of compilation for CY8CPROTO_064_SB:

	1. Generate default keys, by executing the following comands:
		python.exe keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem
	2. Create provisioning packets:
		Execute from ./prepare folder:		
		python.exe provisioning_packet.py --policy policy_1stage_CM4.json  --out ../packet --cyboot ../prebuild/CyBootloader_Debug/CypressBootloader_CM0p.jwt --ckey USERAPP_CM4_KEY.json	
	
* In case of compilation for CY8CPROTO_064_SB_M0_PSA and CY8CPROTO_064_SB_PSA:
	1. Generate default keys, by executing the following comands:
		python.exe keygen.py -k 6 --jwk MCUBOOT_CM0P_KEY.json --pem-priv MCUBOOT_CM0P_KEY_PRIV.pem
		python.exe keygen.py -k 8 --jwk USERAPP_CM4_KEY.json --pem-priv USERAPP_CM4_KEY_PRIV.pem
	2. Create provisioning packets:
	Execute from ./prepare folder:
		python.exe provisioning_packet.py --policy policy_2stage_CM0p.json  --out ../packet --cyboot ../prebuild/CyBootloader_Debug/CypressBootloader_CM0p.jwt --ckey ../keys/MCUBOOT_CM0P_KEY.json --ckey ../keys/USERAPP_CM4_KEY.json

Prebuild folder contains CyBootloader_Debug and CyBootloader_Release with corresponding hex and jwt files.
* Debug enables logs print to terminal.
* Release does not print to terminal and boots up silently.

CypressBootloader_CM0p.jwt and CypressBootloader_CM0p.hex must be used in pair from the same directory
while creation of provisioning packet (.packets/prov_cmd.jwt) and performing of provisioning.