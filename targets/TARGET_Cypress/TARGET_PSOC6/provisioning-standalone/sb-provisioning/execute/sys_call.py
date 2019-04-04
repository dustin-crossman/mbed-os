import os
from time import sleep
from gen_data_from_json import *
from p6_reg import *

PROVISION_KEYS_AND_POLICIES_OPCODE = 0x33
GET_OPCODE = 0x37


def get_prov_det_noprint_syscall(tool, key_id):
    # Acquire IPC structure
    tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
    print(hex(CYREG_IPC2_STRUCT_ACQUIRE), hex(tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)))
    ipc_acquire = 0
    while (ipc_acquire & 0x80000000) == 0:
        ipc_acquire = tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)

    # Set RAM address and Opcode
    op_code = GET_OPCODE << 24
    tool.write32(CYREG_IPC2_STRUCT_DATA, ENTRANCE_EXAM_SRAM_ADDR)  # IPC_STRUCT.DATA
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR, op_code)  # SRAM_SCRATCH
    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x08
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, scratch_addr)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, key_id)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x0C, 0x0)

    # IPC_STRUCT[ipc_id].IPC_NOTIFY -
    tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)

    # Wait on response
    print('Wait on response')  # TODO: remove
    response = 0x80000000
    while (response & 0x80000000) != 0:
        response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)

    response = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
    print(f'response={hex(response)}')  # TODO: remove

    print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)))  # Expected MSB=0xA0
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x04), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x08), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)))

    if (response & 0xFF000000) == 0xa0000000:
        scratch_addr = tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)
        print(f'scratch_addr={hex(scratch_addr)}')
        read_hash_size = tool.read32(scratch_addr + 0x00)
        print(f'read_hash_size={hex(read_hash_size)}')
        read_hash_addr = tool.read32(scratch_addr + 0x04)
        print(f'read_hash_addr={hex(read_hash_addr)}')
        response = ''

        i = 0
        while i < read_hash_size:
            # Save data in string format
            hash_byte_chr = chr(tool.read8(read_hash_addr + i))
            response += hash_byte_chr
            i += 1

        response = response.strip()
        is_exam_pass = True
        return is_exam_pass, response
    else:
        print(hex(CYREG_IPC2_STRUCT_DATA), tool.read32(CYREG_IPC2_STRUCT_DATA))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR), tool.read32(ENTRANCE_EXAM_SRAM_ADDR))
        response = None
        return False, response


def provision_keys_and_policies_debug(tool, blow_secure_efuse, filename):
    file_size = os.path.getsize(filename)
    if file_size > ENTRANCE_EXAM_SRAM_SIZE:
        print('JWT packet too long')
        return

    if blow_secure_efuse:
        enable_blowing_secure = 0
        print('Chip will be converted to SECURE mode')
    else:
        enable_blowing_secure = 1
        print('Chip will NOT be converted to SECURE mode')

    print(f'JWT packet size: {file_size}')

    with open(filename, 'r+') as f:
        f.seek(0)
        content = f.read()
    jwt_chars = list(content)

    # Acquires IPC structure.
    tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
    print(hex(CYREG_IPC2_STRUCT_ACQUIRE), hex(tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)))
    ipc_acquire = 0
    while (ipc_acquire & 0x80000000) == 0:
        ipc_acquire = tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)
        print(f'ipc_acquire={ipc_acquire}')

    # Set RAM address and Opcode
    print("Set RAM address and Opcode")
    tool.write32(CYREG_IPC2_STRUCT_DATA, ENTRANCE_EXAM_SRAM_ADDR)
    print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR, (PROVISION_KEYS_AND_POLICIES_OPCODE << 24)
                 + (enable_blowing_secure << 16) + 1)
    print(hex(ENTRANCE_EXAM_SRAM_ADDR), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)))

    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x08
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, scratch_addr)
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x04), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)))
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, file_size + 0x04)
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x08), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)))
    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x0C

    for ch in jwt_chars:
        tool.write8(scratch_addr, ord(ch))
        print(hex(tool.read8(scratch_addr)))
        scratch_addr += 1

    # IPC_STRUCT[ipc_id].IPC_NOTIFY -
    print("IPC_STRUCT[ipc_id].IPC_NOTIFY -")
    tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)
    print(hex(CYREG_IPC2_STRUCT_NOTIFY), hex(tool.read32(CYREG_IPC2_STRUCT_NOTIFY)))
    # Wait on response
    print("Wait on response")
    response = 0x80000000
    while (response & 0x80000000) != 0:
        response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)
        print(hex(response))
        sleep(0.1)

    # Read response for test
    print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)))

    response = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
    if (response & 0xFF000000) == 0xa0000000:
        print('Provisioning complete')
        return True
    else:
        print(f'Provisioning Error response: {hex(response)}')
        return False
