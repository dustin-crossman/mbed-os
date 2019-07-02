import os
import sys
import json
import click
sys.path.insert(0, './provisioning_lib')
from provisioning_lib.cyprov_oem import OemEntity
from provisioning_lib.cyprov_hsm import HsmEntity

PROD_NAME = 'my_thing'
OEM_AUDIT_NAME = 'oem_log.json'
HSM_AUDIT_NAME = 'hsm_log.json'
CUSTOMER_KEY_N = 5


@click.command()
@click.option('--oem', 'oem_state_path',
              default='../prebuild/oem_state.json',
              help='OEM key file.')
@click.option('--hsm', 'hsm_state_path',
              default='../prebuild/hsm_state.json',
              help='HSM key file.')
@click.option('--policy', 'policy_path',
              default='policy_single_stage_CM4.json',
              help='Policy file.')
@click.option('--out', 'output_path',
              default='../packet',
              help='Output directory.')
@click.option('--ckey', 'cust_key_path',
              default=None,
              multiple=True,
              help='Customer key that will be used for image signing.')
def main(oem_state_path, hsm_state_path, policy_path, output_path, cust_key_path):
    if len(cust_key_path) > CUSTOMER_KEY_N:
        raise Exception('Maximum number of customer keys must be {}!'.format(CUSTOMER_KEY_N))

    oem_audit_path = os.path.join(output_path, OEM_AUDIT_NAME)
    hsm_audit_path = os.path.join(output_path, HSM_AUDIT_NAME)

    if not os.path.exists(output_path):
        os.makedirs(output_path)

    oem = OemEntity(oem_state_path, oem_audit_path)
    hsm = HsmEntity(hsm_state_path, hsm_audit_path)

    with open(policy_path) as f:
        json_str = f.read()
        blob = json.loads(json_str)
        blob['prod_id'] = PROD_NAME

    if cust_key_path != None:
        customer = process_customer_keys(cust_key_path)
        if len(customer) > 0:
            blob['custom_pub_key'] = [key.get_pub_key() for key in customer]

    prov_req = oem.create_provision_request(blob)

    pass


if __name__ == "__main__":
    main()