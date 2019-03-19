import os
import sys
import json
import getopt

# Default input values and pathes
PROD_NAME = 'my_thing'
CYPROV_LIB_PATH = './ProvisioningLib'
OEM_STATE_NAME = 'oem_state.json'
HSM_STATE_NAME = 'hsm_state.json'
CUSTOM_STATE_NAME = 'customer_state.json'
CY_AUTH_FILE = 'cy_auth.jwt'
POLICY_FILE = 'policy.json'
BOOTLOADER_JWT_FILE = 'cy_bootloader.jwt'
INPUT_PATH = './prebuild'
oem_state_path = os.path.join(INPUT_PATH, OEM_STATE_NAME)
hsm_state_path = os.path.join(INPUT_PATH, HSM_STATE_NAME)
cust_key_path = os.path.join(INPUT_PATH, CUSTOM_STATE_NAME)
boot_jwt_file_path = os.path.join(INPUT_PATH, BOOTLOADER_JWT_FILE)
cy_auth_path = os.path.join(INPUT_PATH, CY_AUTH_FILE)
policy_path = os.path.join(INPUT_PATH, POLICY_FILE)

# Default output values and pathes
OEM_AUDIT_NAME = 'oem_log.json'
HSM_AUDIT_NAME = 'hsm_log.json'
ROT_JWT_FILE = 'rot_cmd.jwt'
PROV_JWT_FILE = 'prov_cmd.jwt'
OEM_PUB_FILE = 'oem_pub_key.pem'
OEM_PRIV_FILE = 'oem_priv_key.pem'
output_path = './prebuild/out'

CUSTOMER_KEY_N = 5

sys.path.insert(0, CYPROV_LIB_PATH)
from cyprov_hsm import HsmEntity
from cyprov_oem import OemEntity
from cyprov_customer import CustomerEntity
from cyprov_crypto import crypto
from cyprov_pem import PemKey

def process_args():
    cmd_list = ['oem=', 'hsm=', 'cyboot=', 'cyauth=', 'policy=', 'out=', 'ckey=']
    try:
        opts, args = getopt.getopt(sys.argv[1:], "", cmd_list)
    except getopt.GetoptError as err:
        print(err)
        help_msg = """
        provision_device.py [--oem oem_state.json --hsm hsm_state.json --cyboot 
        cyboot.jwt --cyauth cyauth.jwt --policy policy.json --out output_path 
        --ckey customer_key.json]
        """
        print(help_msg)
        sys.exit(2)

    oem = oem_state_path
    hsm = hsm_state_path
    cyboot = boot_jwt_file_path
    cyauth = cy_auth_path
    policy = policy_path
    out = output_path
    keys = []
        
    for o, a in opts:
        if o == "--oem":
            oem = a
        elif o == "--hsm":
            hsm = a
        elif o == "--cyboot":
            cyboot = a
        elif o == "--cyauth":
            cyauth = a
        elif o == "--policy":
            policy = a
        elif o == "--out":
            out = a
        elif o == "--ckey":
            if len(keys) < CUSTOMER_KEY_N:
                keys.append(a)
            else:
                assert False, "There are too many customer keys! It must be no more than {}.".format(CUSTOMER_KEY_N)
        else:
            assert False, "Unhandled option!"
        
    return oem, hsm, cyboot, cyauth, policy, out, keys

def process_customer_keys(paths):
    customer_key_n = len(paths)
    customer = []
    if customer_key_n > 0:
        customer_key_id = 6
        for i in range(customer_key_n):
            customer_log = os.path.basename(paths[i])
            customer_log = os.path.splitext(customer_log)[0] + '_log.json'
            customer.append(CustomerEntity(paths[i], customer_log))        
            if customer[i].state_loaded != True:
                customer[i].create_entity(customer_key_id + i)
            customer[i].save_state()
    return customer

def main():
    prod_id = PROD_NAME
    
    input_files = process_args()
    oem_state_path = input_files[0]
    hsm_state_path = input_files[1]
    boot_jwt_file_path = input_files[2]
    cy_auth_path = input_files[3]
    policy_path = input_files[4]
    output_path = input_files[5]
    cust_key_path = input_files[6]
    
    oem_audit_path = os.path.join(output_path, OEM_AUDIT_NAME)
    hsm_audit_path = os.path.join(output_path, HSM_AUDIT_NAME)
    rot_jwt_path = os.path.join(output_path, ROT_JWT_FILE)
    prov_jwt_path = os.path.join(output_path, PROV_JWT_FILE)

    if not os.path.exists(output_path):
        os.makedirs(output_path)
    
    oem = OemEntity(oem_state_path , oem_audit_path)
    hsm = HsmEntity(hsm_state_path , hsm_audit_path)
    
    with open(policy_path) as f:
        json_str = f.read()
        blob = json.loads(json_str)
        blob['prod_id'] = prod_id
        
    customer = process_customer_keys(cust_key_path)
    if len(customer) > 0:
        blob["custom_pub_key"] = [key.get_pub_key() for key in customer]
        for key, path in zip(customer, cust_key_path):
            pem = PemKey()
            pem.load(key.get_priv_key())
            key_name = os.path.basename(path)
            key_name = os.path.splitext(key_name)[0] + '_PUB.pem'
            key_path = os.path.join(output_path, key_name)
            pem.save(key_path, private_key=False)
            key_name = os.path.basename(path)
            key_name = os.path.splitext(key_name)[0] + '_PRIV.pem'
            key_path = os.path.join(output_path, key_name)
            pem.save(key_path, private_key=True)
    
    prov_req = oem.create_provision_request(blob)
    
    rot_auth_pkg = oem.pack_rot_auth(prod_id, hsm.state["hsm_pub_key"])
    
    rot_cmd = hsm.pack_rot_command(prod_id, cy_auth_path, rot_auth_pkg)
    crypto.dump_jwt(rot_cmd, rot_jwt_path)
    
    prov_cmd = hsm.pack_provision_cmd(
            cy_auth = cy_auth_path, 
            image_cert = boot_jwt_file_path,
            prov_req = prov_req)
    crypto.dump_jwt(prov_cmd, prov_jwt_path)

    print('#' * 80)
    print('Provisioning packet is created')
    print('#' * 80)

if __name__ == "__main__":
    main()
    