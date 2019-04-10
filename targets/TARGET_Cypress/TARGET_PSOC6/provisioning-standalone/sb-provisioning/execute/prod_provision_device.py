"""
Provides high level support for provisioning device with the specified programming tool.
"""
import os
import sys
from enum import Enum
from provision_device import provision_execution
from programmer.programmer import ProgrammingTool
from prepare.provisioning_lib.cyprov_pem import PemKey


class DebugCore(Enum):
    """
    CM0 access port.
    """
    debug_cm0_ap = 0

    """
    CM4 access port.
    """
    debug_cm4_ap = 1

    """
    SYS access port.
    """
    debug_sys_ap = 2


TOOL_NAME = 'pyocd'  # Programming/debugging tool used for communication with device
ACCESS_PORT = DebugCore.debug_cm4_ap  # Access port used for device provisioning


def main(argv):
    """
    Parses command line arguments and starts device provisioning.
    :param argv: Command line arguments.
    :return: Exit code.
    """
    path = os.path.dirname(argv[0])
    dev_key_file = 'dev_pub_key.json'

    try:
        dev_key_out = argv[1]
    except IndexError:
        dev_key_out = os.path.join(path, '../')

    try:
        cy_bootloader_hex = argv[2]
    except IndexError:
        print('Path to Cypress Bootloader HEX binary file is not specified!')
        sys.exit(1)

    try:
        jwt_path = argv[3]
    except IndexError:
        print('Path to JWT provisioning packets is not specified!')
        sys.exit(1)

    if ACCESS_PORT == DebugCore.debug_cm0_ap:
        target = 'cy8c64xx_cm0'
    elif ACCESS_PORT == DebugCore.debug_cm4_ap:
        target = 'cy8c64xx_cm4'
    elif ACCESS_PORT == DebugCore.debug_sys_ap:
        target = 'cy8c64xx_nosysap'
    else:
        print('Invalid access port.')
        sys.exit(1)

    # Execute secure_blinky.tcl script
    test_status = False
    tool = ProgrammingTool.create(TOOL_NAME)
    if tool.connect(target):
        test_status = provision_execution(tool, jwt_path, cy_bootloader_hex)
        tool.disconnect()

    if test_status:
        # Read device response file and take device key from it
        dev_key_file = os.path.join(jwt_path, dev_key_file)
        if os.path.exists(dev_key_file):
            pem = PemKey(dev_key_file)
            key_path = os.path.join(dev_key_out, 'dev_pub_key.pem')
            pem.save(key_path, private_key=False)
    else:
        sys.exit(3)


if __name__ == "__main__":
    main(sys.argv)
