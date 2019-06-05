#
# Copyright (c) 2017-2018 Future Electronics
# Copyright (c) 2018-2019 Cypress Semiconductor Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import os
import sys

import platform
import subprocess
import errno
from array import array
from struct import (pack, unpack)
from shutil import copy2
from intelhex import IntelHex, hex2bin, bin2hex
from intelhex.compat import asbytes

from ..config import ConfigException

from pathlib import Path, PurePath
import json

# The size of the program data in Cypress HEX files is limited to 0x80000000
# Higher addresses contain additional metadata (chip protection, eFuse data, etc..)
CY_PROGRAM_SIZE = 0x80000000

# The starting address of the program data checksum section
CY_CHECKSUM_ADDR = 0x90300000

# The starting address of the .cymeta section (12 bytes)
# Additional metadata include silicon revision, Silicon/JTAG ID, etc.
CY_META_ADDR = 0x90500000

# The address of the silicon ID (4 bytes)
CY_META_SILICON_ID_ADDR = 0x90500002

# The address of the metadata checksum (4 bytes)
CY_META_CHECKSUM_ADDR = 0x90500008


# Patch Cypress hex file:
# - update checksum
# - update metadata
# - align regions to page (256 bytes) boundary
def patch(message_func, ihex, hexf, align=256):
    update_checksum = False
    update_metadata = False

    # calculate checksum of the program section, detect metadata
    checksum = 0
    for start, end in ihex.segments():
        if start == CY_CHECKSUM_ADDR:
            # checksum section found in the original hex
            update_checksum = True
        if start == CY_META_ADDR:
            # metadata section found in the original hex
            update_metadata = True
        if start >= CY_PROGRAM_SIZE:
            continue
        segment = ihex.tobinarray(start, end)
        checksum += sum(segment)

    # only update checksum if it was found in the original hex
    if update_checksum:
        lowchecksum = checksum & 0x0FFFF
        message_func("Calculated checksum for %s is 0x%04x" % (hexf, lowchecksum))

        checksum_str = pack('>H', lowchecksum)
        ihex.frombytes(array('B', checksum_str), offset=CY_CHECKSUM_ADDR)

    # only update metadata if it was found in the original hex
    if update_metadata:
        signature = unpack('>L', ihex.tobinstr(start=CY_META_SILICON_ID_ADDR, size=4))[0]
        sigcheck = pack('>L', (checksum + signature) & 0x0FFFF)
        ihex.frombytes(array('B', sigcheck), offset=CY_META_CHECKSUM_ADDR)

    # align flash segments
    align_mask = align - 1
    alignments = IntelHex()
    for start, end in ihex.segments():
        if start >= CY_PROGRAM_SIZE:
            continue
        aligned_start = start & ~align_mask
        if start != aligned_start:
            message_func("Aligning start from 0x%x to 0x%x" % (start, aligned_start))
            alignments.frombytes(ihex.tobinarray(aligned_start, start - 1), aligned_start)
        aligned_end = end & ~align_mask
        if end != aligned_end:
            aligned_end += align
            message_func("Aligning end from 0x%x to 0x%x" % (end, aligned_end))
            alignments.frombytes(ihex.tobinarray(end, aligned_end - 1), end)
    ihex.merge(alignments, 'ignore')


def merge_images(hexf0, hexf1=None):
    ihex = IntelHex()
    ihex.padding = 0x00
    ihex.loadfile(hexf0, "hex")
    if hexf1 is not None:
        # Merge the CM0+ image
        ihex1 = IntelHex(hexf1)
        ihex.merge(ihex1, 'ignore')
    return ihex


def complete_func(message_func, elf0, hexf0, hexf1=None, dest=None):
    message_func("Postprocessing %s -> %s" % (elf0, hexf0))
    ihex = merge_images(hexf0, hexf1)
    patch(message_func, ihex, hexf0)
    ihex.write_hex_file(dest if dest else hexf0, write_start_addr=False, byte_count=16)


# Find Cortex M0 image.
def find_cm0_image(toolchain, resources, elf, hexf, hex_filename):
    # Locate user-specified image
    from tools.resources import FileType
    hex_files = resources.get_file_paths(FileType.HEX)
    m0hexf = next((f for f in hex_files if os.path.basename(f) == hex_filename), None)
    if toolchain.target.is_PSA_non_secure_target:
        m0hexf = next((f for f in hex_files if os.path.basename(f) == os.path.basename(hexf)), m0hexf)

    if m0hexf:
        toolchain.notify.info("M0 core image file found: %s." % m0hexf)
    else:
        toolchain.notify.info("M0 core hex image file %s not found. Aborting." % hex_filename)
        raise ConfigException("Required M0 core hex image not found.")

    return m0hexf


# check if policy parameters are consistent
def check_slots_integrity(toolchain, fw_cyb, target_data, fw_spe=None, fw_nspe=None):

    print("+++++++++++++ from check_args_integrity function")

    print(fw_cyb)
    print(fw_spe)
    print(fw_nspe)
    print(target_data)

    if fw_spe is None:
        img_id = fw_nspe["id"]

        # check single stage scheme
        if not (fw_cyb["launch"] == img_id):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: ID of build image"
                                   " does not correspond launch ID in CyBootloader!")
        # check slots addresses and sizes if upgrade is set to True
        if fw_nspe["upgrade"] and True:

            for slot in fw_nspe["resources"]:
                if slot["type"] == "BOOT":
                    slot0 = slot
                    print(slot0)
                elif slot["type"] == "UPGRADE":
                    slot1 = slot
                    print(slot1)
                else:
                    toolchain.notify.debug("[PSOC6.sign_image] ERROR: BOOT or UPGRADE sections can't be found in policy file ")
                    raise Exception("imgtool finished execution with errors!")

    else:
        # check if PSA targets flash map correspond to slots addresses and sizes in policy
        if not (int(target_data["overrides"]["secure-rom-start"], 16) - 1024) ==\
                int(fw_spe["resources"][0]["address"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: SPE start address "
                                   "does not correspond BOOT slot start address defined in policy. "
                                   "Check if MCUboot header offset 0x400 is included in SPE flash start")
            # raise Exception("imgtool finished execution with errors!")

        if not (int(target_data["overrides"]["non-secure-rom-start"], 16) - 1024) ==\
                int(fw_nspe["resources"][0]["address"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: NSPE start address "
                                   "does not correspond BOOT slot start address defined in policy. "
                                   "Check if MCUboot header offset 0x400 is included in NSPE flash start")
            # raise Exception("imgtool finished execution with errors!")

        if (int(target_data["overrides"]["secure-rom-size"], 16) + 1024) > int(fw_spe["resources"][0]["size"]):
            toolchain.notify.debug("[PSOC6.sign_image] WARNING: SPE flash size "
                                   "does not fit in BOOT slot size defined in policy.")
            # raise Exception("imgtool finished execution with errors!")

        if (int(target_data["overrides"]["non-secure-rom-size"], 16) + 1024) > int(fw_nspe["resources"][0]["size"]):

            print(int(target_data["overrides"]["non-secure-rom-size"], 16))
            print(int(fw_nspe["resources"][0]["size"]))

            toolchain.notify.debug("[PSOC6.sign_image] WARNING: NSPE flash size "
                                   "does not fit in BOOT slot size defined in policy.")
            # raise Exception("imgtool finished execution with errors!")

        img_id = fw_spe["id"]
        # check dual stage scheme
        if img_id != 1:
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: Image ID of SPE image"
                                   " is not equal to 1!")
            raise Exception("imgtool finished execution with errors!")

        if not (fw_cyb["launch"] == img_id):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: ID of build image"
                                   " does not correspond launch ID in CyBootloader!")
            raise Exception("imgtool finished execution with errors!")

        if not (fw_spe["launch"] == fw_nspe["id"]):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: ID of NSPE image"
                                   " does not correspond launch ID in SPE part!")
            raise Exception("imgtool finished execution with errors!")

        # check slots addresses and sizes if upgrade is set to True
        if fw_spe["upgrade"] and True:

            for slot in fw_spe["resources"]:
                if slot["type"] == "BOOT":
                    slot0 = slot
                elif slot["type"] == "UPGRADE":
                    slot1 = slot
                else:
                    toolchain.notify.debug("[PSOC6.sign_image] ERROR: BOOT or UPGRADE sections can't be found in policy file ")
                    raise Exception("imgtool finished execution with errors!")

    # bigger or equal to 0x18000000 hex
    if slot1["address"] >= 402653184:
        toolchain.notify.debug("[PSOC6.sign_image] INFO: UPGRADE slot will be resided in external flash")

    # TODO: implement slots do not overlap check?

    if slot0["size"] != slot1["size"]:
        toolchain.notify.debug("[PSOC6.sign_image] WARNING: BOOT and UPGRADE slots sizes are not equal")
        # raise Exception("imgtool finished execution with errors!")

    return [slot0, slot1, img_id]


# Resolve Secure Boot policy sections considering target
def process_target(toolchain, target, binf):

    targets_json = Path("targets/targets.json")
    cy_targets = Path("targets/TARGET_Cypress/TARGET_PSOC6/")
    sb_params_file_name = Path("secure_image_parameters.json")
    # assume root dir is ./mbed-os
    root_dir = Path(os.getcwd())

    mbed_os_targets = root_dir / targets_json

    if not os.path.isfile(str(mbed_os_targets)):
        # try location for tests
        mbed_os_targets = root_dir / 'mbed-os' / targets_json
        root_dir = root_dir / 'mbed-os'
        if not os.path.isfile(str(mbed_os_targets)):
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: targets.json not found!")
            raise Exception("imgtool finished execution with errors!")

    # print("+++++++++++++")
    # print(mbed_os_targets)

    with open(str(mbed_os_targets)) as j:
        json_str = j.read()
        all_targets = json.loads(json_str)
        j.close()

    processing_target = all_targets[target]

    # print("+++++++++++++")
    # print(processing_target)

    sb_params_file_path = root_dir / cy_targets / Path("TARGET_" + str(target)) / sb_params_file_name

    if os.path.isfile(str(sb_params_file_path)):
        with open(str(sb_params_file_path)) as f:
            json_str = f.read()
            sb_config = json.loads(json_str)
            f.close()
    else:
        toolchain.notify.debug("[PSOC6.sign_image] ERROR: secure_image_parametest.json not found!")
        raise Exception("imgtool finished execution with errors!")

    print("+++++++++++++")
    print(sb_config)

    sdk_path = root_dir / sb_config["sdk_path"]

    priv_key_path = sdk_path / Path(sb_config["priv_key_file"])

    if not os.path.isfile(str(priv_key_path)):
        toolchain.notify.debug("[PSOC6.sign_image] ERROR: Private key file not found in " + priv_key_path)
        raise Exception("imgtool finished execution with errors!")

    if "_PSA" in target:
        # assume dual stage bootloading scheme
        with open(sdk_path / "prepare/policy_dual_stage_CM0p_CM4.json") as p:
            policy_str = p.read()
            policy_file = json.loads(policy_str)
            p.close()

            firmware_list = policy_file["boot_upgrade"]["firmware"]

            firmware_cyb_cm0p = firmware_list[0]
        if "_M0_" in target:
            firmware_spe_cm0p = firmware_list[1]
            firmware_nspe_cm4 = firmware_list[2]
            print("+++++++++_________________===========")
            print(firmware_spe_cm0p)

            slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p, fw_spe=firmware_spe_cm0p,
                                          fw_nspe=firmware_nspe_cm4, target_data=processing_target)
        else:
            firmware_nspe_cm4 = firmware_list[2]
            slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p, fw_nspe=firmware_nspe_cm4,
                                          target_data=processing_target)

    else:
        # consider single stage bootloading scheme
        with open(sdk_path / "prepare/policy_single_stage_CM4.json") as p:
            policy_str = p.read()
            policy_file = json.loads(policy_str)
            p.close()

        firmware_list = policy_file["boot_upgrade"]["firmware"]
        firmware_cyb_cm0p = firmware_list[0]
        firmware_nspe_cm4 = firmware_list[1]
        slots = check_slots_integrity(toolchain, fw_cyb=firmware_cyb_cm0p,
                                      fw_nspe=firmware_nspe_cm4, target_data=processing_target)


    print("++++++ slots info")
    print(slots)

    target_sig_data = [{"img_data": sb_config["boot0"], "slot_data": slots[0],
                        "key_file": sb_config["priv_key_file"], "sdk_path": sdk_path, "id": slots[2]},
                       {"img_data": sb_config["boot1"], "slot_data": slots[1],
                        "key_file": sb_config["priv_key_file"], "sdk_path": sdk_path, "id": slots[2]}]

    return target_sig_data


# Sign binary image with Secure Boot SDK tools
def sign_image(toolchain, elf0, binf, hexf1=None):
    target = {"name": "UNDEFINED", "core": "UNDEFINED"}
    img_start_addr = 0

    # preserve original hex file from mbed-os build
    binf_orig = str(binf)[:-4] + "_unsigned.hex"
    copy2(str(binf), binf_orig)

    # find target name and type before processing
    for part in PurePath(binf).parts:
        if "CY" in part:
            target_sig_data = process_target(toolchain=toolchain, target=part, binf=binf)
        # else:
        #     toolchain.notify.tool_error("[PSOC6.sign_image] ERROR: Target not found!")
        #     exit(1)

    for slot in target_sig_data:
        # sign_args = collect_args(toolchain, slot)

        if slot["slot_data"]["type"] == "UPGRADE":
            out_bin_name = str(binf)[:-4] + "_upgrade.hex"
        else:
            out_bin_name = binf

        print(str(hex(slot["slot_data"]["size"])))

        # call imgtool for signature
        process = subprocess.Popen([sys.executable, str(slot["sdk_path"] / "imgtool/imgtool.py"),
                                    "sign", "--key", str(slot["sdk_path"] / slot["key_file"]),
                                    "--header-size", "0x400", "--pad-header", "--align", "8",
                                    "--version", str(slot["img_data"]["VERSION"]), "--image-id",
                                    str(slot["id"]), "--rollback_counter", str(slot["img_data"]["ROLLBACK_COUNTER"]),
                                    "--slot-size", str(hex(slot["slot_data"]["size"])), "--overwrite-only", "--pad",
                                    binf_orig, out_bin_name],
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        print("IMAGE TOOL ------------------------------------------------")
        print(str(slot["sdk_path"] / "imgtool/imgtool.py") +
                                    " sign " + "--key " + str(slot["sdk_path"] / slot["key_file"]) +
                                    " --header-size " + "0x400 " + " --pad-header " + "--align " + "8 " +
                                    "--version " + str(slot["img_data"]["VERSION"]) + " --image-id",
                                    str(slot["id"]) + " --rollback_counter " +str(slot["img_data"]["ROLLBACK_COUNTER"]) +
                                    " --slot-size " + str(hex(slot["slot_data"]["size"])) + "--overwrite-only " + "--pad " +
              str(binf_orig) + " " + str(out_bin_name))


        # catch stderr outputs
        stderr = process.communicate()
        rc = process.wait()

        if rc != 0:
            toolchain.notify.debug("[PSOC6.sign_image] ERROR: Signature is not added!")
            toolchain.notify.debug("[PSOC6.sign_image] Message from imgtool: " + stderr[1].decode("utf-8"))
            raise Exception("imgtool finished execution with errors!")
        else:
            toolchain.notify.info("[PSOC6.sign_image] SUCCESS: Image for slot " +
                                  slot["slot_data"]["type"] + " is signed with no errors!")

        if slot["slot_data"]["type"] == "UPGRADE":
            print(str(hex(slot["slot_data"]["address"])))
            #bin2hex(out_bin_name, out_bin_name, offset=0x18000000) #str(hex(slot["slot_data"]["address"]))
            subprocess.Popen(["arm-none-eabi-objcopy", "--change-address", "0x8000000",
                               "-I", "ihex", "-O", "ihex", out_bin_name, out_bin_name])


def complete(toolchain, elf0, hexf0, hexf1=None):
    if os.path.isfile(str(hexf0)) and os.path.isfile(str(hexf1)):
        complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)
            
def sign_complete(toolchain, elf0, binf, m0hex):
    sign_image(toolchain, elf0, binf, None)
    complete_func(toolchain.notify.debug, None, binf, m0hex)