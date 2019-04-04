from sys_call import get_prov_det_noprint_syscall, provision_keys_and_policies_debug
from gen_data_from_json import *
from p6_reg import *
from enum import Enum
from time import sleep


def provision_execution(tool, prov_cmd_jwt, cy_bootloader_hex):
    # Use CM0, CM4, or SYS access port
    use_access_port = DebugCore.debug_cm0_ap

    # Indicates if will be converted to SECURE mode or not
    blow_secure_fuse = True

    # Acquire without RESET
    enable_acquire = 0

    if use_access_port == DebugCore.debug_cm0_ap:
        ENABLE_CM4 = 0
        ENABLE_CM0 = 1
        #tool.set_core(0)
        print('Selected Access Port: CM0_AP')
    elif use_access_port == DebugCore.debug_cm4_ap:
        ENABLE_CM4 = 1
        ENABLE_CM0 = 0
        #tool.set_core(1)
        print('Selected Access Port: CM4_AP')
    elif use_access_port == DebugCore.debug_sys_ap:
        ENABLE_CM4 = 0
        ENABLE_CM0 = 0
        #tool.set_core(0)
        print('Selected Access Port: SYS_AP')
    else:
        tool.disconnect()
        raise ValueError(f"FAIL: 'use_access_port' value is invalid: {use_access_port}")

    tool.set_frequency(200)

    sleep(0.1)

    # psoc6 sflash_restrictions 2    !!!!!!!!!!!!!!! no command

    # Prepare file with example status
    filename = 'tcl_prov_res.txt'
    with open(filename, 'w') as result_file:
        result_file.write('PROVISIONING: FAIL\n')

    print("\nCPUSS.PROTECTION state: '0': UNKNOWN. '1': VIRGIN. '2': NORMAL. '3': SECURE. '4': DEAD.")
    print(hex(tool.read32(CYREG_CPUSS_PROTECTION)))

    tool.reset()
    sleep(0.1)

    rc, key = get_prov_det_noprint_syscall(tool, 1)
    if rc:
        print('Device public key has been read successfully.')
    else:
        print('FAIL: Cannot read device public key.')

    print(key)
    with open('dev_pub_key.json', 'w') as json_file:
        json_file.write(key)

    if tool.read32(CYREG_CPUSS_PROTECTION) != 3:
        print(f'FAIL: Device is not in NORMAL mode, error code: {hex(tool.read32(CYREG_IPC2_STRUCT_DATA))}')
        print('\nRead Secure Hash from eFUSEs:\n')  # 00 expected on virgin device
        got_factory_hash = ''
        i = 0
        while i < 24:
            hash_byte_val = hex(tool.read8(0x402C0814 + i))
            got_factory_hash += hash_byte_val
            i += 1
        print(f"Received SECURE_HASH: '{hex(got_factory_hash)}'")
        return False

    print('\nErase main flash and TOC3:')
    tool.erase(0x10000000, 0x000e0000)

    tool.reset()
    sleep(0.1)

    print('\nRead FB Firmware status:\n')
    fb_firmware_status = tool.read32(ENTRANCE_EXAM_FW_STATUS_REG)
    print(f'FB Firmware status = {hex(fb_firmware_status)}')
    # Print Expected and received LIFECYCLE_STAGE values
    print(f'Received FB_FW_STATUS = {hex(fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK)}')
    print(f'Expected FB_FW_STATUS = {hex(ENTRANCE_EXAM_FW_STATUS_VAL)}')
    # Verify if received value is the same as expected
    if (fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK) == ENTRANCE_EXAM_FW_STATUS_VAL:
        print('PASS: FB Firmware status is as expected')
        is_exam_pass = True
    else:
        print('FAIL: FB Firmware status is not as expected')
        is_exam_pass = False

    if is_exam_pass:
        print('\nPROGRAMMING APP HEX:')
        # poll off !!!!!!! no command
        tool.program(cy_bootloader_hex)
        # reset_config srst_only
        # adapter_nsrst_delay 2000
        tool.reset()
        sleep(0.1)

    print('Reading FB Status code:')
    print(hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))

    print('\nRead FB Firmware status:(expected MSB=0xfx)')
    print(hex(tool.read32(ENTRANCE_EXAM_FW_STATUS_REG)))

    if is_exam_pass:
        print('\nRun provisioning syscall')
        syscall_passed = provision_keys_and_policies_debug(tool, blow_secure_fuse, prov_cmd_jwt)
        i = 0
        print(hex(NVSTORE_AREA_1_ADDRESS), ":\n")
        if syscall_passed:
            while i < 8 * 4:
                print(hex(tool.read32(NVSTORE_AREA_1_ADDRESS + i)))
                i += 4
        else:
            print('FAIL: Unexpected provision_keys_and_policies_debug syscall response')
            is_exam_pass = False

    if is_exam_pass:
        # Prepare file with example status
        print('\n*****************************************')
        print("       PROVISIONING PASSED         ")
        print("*****************************************")
        with open(filename, 'w') as result_file:
            result_file.write('PROVISIONING: PASS\n')


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
