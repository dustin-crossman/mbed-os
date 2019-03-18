from cyprov_protocols import cyprov
from datetime import datetime
from cyprov_auth import authorization
from cyprov_pem import PemKey
import json
import sys
import os

hsm_name= "hsm"
image_id = 0
flashboot_image_file ="../flashboot_psoc62_0C.hex"
image_toc3_addr = int("0x100DFE00", 0)
exp= datetime( 2019 , 12 , 31 )
disti_pub_key= None
cy_pub_key= None
auth= {}
prod_id= "my_thing"

path = os.path.dirname(sys.argv[0])

image_jwt_file = os.path.join(path, "../CypressBootloader_CM0p.jwt")

try:
    policy_file = sys.argv[1]
except IndexError:
    policy_file = os.path.join(path, '../policy_1stage_CM4.json')

try:
    image_file = sys.argv[2]
except IndexError:    
    image_file = os.path.join(path, "../CypressBootloader_CM0p.hex")

try:
    keys_path = sys.argv[3]
except IndexError:
    keys_path = os.path.join(path, '../../')

print('Policy file:', policy_file)
print('Cypress Bootlader image:', image_file)

with open(policy_file) as f:
    json_str = f.read()
    blob = json.loads(json_str)
    blob['prod_id'] = prod_id

cnt= 100

dev_name= "my_device"
dev_id="dev_id"
die_id= authorization.create_die_id( fab=5, lot=26, wafer=40, ypos=321, xpos=456 )
     

cyprov.protocol_create_entities(exp, image_id, image_file, image_toc3_addr, image_jwt_file, flashboot_image_file)

cyprov.protocol_disti_creates_hsm( hsm_name,exp )

cyprov.protocol_cypress_authorizes_hsm( hsm_name,disti_pub_key,exp,auth,cy_pub_key )

cyprov.protocol_oem_signing_key( hsm_name,prod_id )

cyprov.protocol_oem_request( hsm_name,blob,cnt,exp )

cyprov.protocol_manufacture_device(dev_name,die_id,dev_id)

cyprov.protocol_provision_device(hsm_name,dev_name,prod_id,image_jwt_file)

pem = PemKey('oem_state.json', 'oem_priv_key')
key_path = os.path.join(keys_path, 'oem_pub_key.pem')
pem.save(key_path, private_key=False)
key_path = os.path.join(keys_path, 'oem_priv_key.pem')
pem.save(key_path, private_key=True)
