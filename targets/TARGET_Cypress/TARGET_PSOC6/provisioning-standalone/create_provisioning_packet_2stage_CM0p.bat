:: Change file name of key MCUBOOT_CM0P_KEY.json to your CM0 application key and USERAPP_CM4_KEY.json to your CM4 application key

echo Creating provisioning packet for Single Stage Bootloading of CM4 image

python.exe provisioning_packet.py --policy policy_2stage_CM0p.json  --out ./provisioning-packet --cyboot CypressBootloader_CM0p.jwt --ckey MCUBOOT_CM0P_KEY.json --ckey USERAPP_CM4_KEY.json
