import os
import sys
import click
sys.path.insert(0, './provisioning_lib')
from provisioning_lib.cyprov_oem import OemEntity
from provisioning_lib.cyprov_hsm import HsmEntity

PROD_NAME = 'my_thing'
OEM_AUDIT_NAME = 'oem_log.json'
HSM_AUDIT_NAME = 'hsm_log.json'


@click.command()
@click.option('--oem', 'oem_state_path',
              default='../prebuild/oem_state.json',
              help='OEM key file.')
@click.option('--hsm', 'hsm_state_path',
              default='../prebuild/hsm_state.json',
              help='HSM key file.')
@click.option('--out', 'output_path',
              default='../packet',
              help='Output directory.')
def main(oem_state_path, hsm_state_path, output_path):
    oem_audit_path = os.path.join(output_path, OEM_AUDIT_NAME)
    hsm_audit_path = os.path.join(output_path, HSM_AUDIT_NAME)

    oem = OemEntity(oem_state_path, oem_audit_path)
    hsm = HsmEntity(hsm_state_path, hsm_audit_path)

    rot_auth_pkg = oem.pack_rot_auth(PROD_NAME, hsm.state["hsm_pub_key"])

    if not os.path.exists(output_path):
        os.makedirs(output_path)

    jwt_file = os.path.join(output_path, 'rot_auth.jwt')
    with open(jwt_file, 'w') as file:
        file.write(rot_auth_pkg)


if __name__ == "__main__":
    main()
