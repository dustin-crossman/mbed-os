import os
import sys
from provision_device import provision_execution
from programmer.programmer import ProgrammingTool
from ..prepare.provisioning_lib.cyprov_pem import PemKey


TOOL = 'pyocd'
TARGET = 'cy8c64xx_nosysap'

if __name__ == "__main__":
    path = os.path.dirname(sys.argv[0])
    dev_key_file = 'dev_pub_key.json'

    try:
        dev_key_out = sys.argv[1]
    except IndexError:
        dev_key_out = os.path.join(path, '../packet')

    try:
        cy_bootloader_hex = sys.argv[2]
    except IndexError:
        print('Path to Cypress Bootloader HEX binary file is not specified!')
        exit(1)
    if not os.path.isfile(cy_bootloader_hex):
        print('Specified Cypress Bootloader HEX binary file is does not exists!')
        exit(1)

    try:
        jwt_path = sys.argv[3]
    except IndexError:
        print('Path to jwt provisioning packets is not specified!')
        exit(1)

    tool = ProgrammingTool.create(TOOL)
    if tool.connect(TARGET):
        test_status = True if provision_execution(tool, jwt_path, cy_bootloader_hex) else False
    tool.disconnect()
    # Read device response file and take device key from it
    dev_key_file = os.path.join(jwt_path, dev_key_file)
    if os.path.exists(dev_key_file):
        pem = PemKey(dev_key_file)
        key_path = os.path.join(dev_key_out, 'dev_pub_key.pem')
        pem.save(key_path, private_key=False)
