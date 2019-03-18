:: Change file name of key from USERAPP_CM4_KEY.json to yours

echo Creating provisioning packet for Single Stage Bootloading of CM4 image

python.exe provisioning_packet.py --policy policy_1stage_CM4.json  --out ./provisioning-packet --cyboot CypressBootloader_CM0p.jwt --ckey USERAPP_CM4_KEY.json
