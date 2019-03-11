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
from shutil import copyfile
from intelhex import IntelHex
from intelhex.compat import asbytes

from ..config import ConfigException

from pathlib import Path
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
        if start  >= CY_PROGRAM_SIZE:
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
        ihex.frombytes(array('B',sigcheck), offset=CY_META_CHECKSUM_ADDR)

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
    if  hexf1 is not None:
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

# Make a dictionary with arguments for signing image
def collect_args(image_slot):

    prj_config_file = Path("targets/TARGET_Cypress/TARGET_PSOC6/"
                           "TARGET_CY8CPROTO_064_SB_M0_PSA/secure_image_parameters.json")

    with open(prj_config_file.absolute()) as f:
        json_str = f.read()
        prj_config = json.loads(json_str)

        # Secure MCU definitions
        # sys.path.insert(0, str(Path(prj_config.get("sdk_path")).absolute()))  # 'mbed-os/targets/TARGET_Cypress/TARGET_PSOC6/secureboot-tools-1.0/')

        # print("+++ we are in collect_arg func")
        # print(prj_config["sdk_path"])

        args_for_signature = {
            "sdk_path":         Path(prj_config.get("sdk_path")).absolute(),
            "priv_key":         str(Path(prj_config.get("sdk_path") + "MCUBOOT_CM0P_KEY_PRIV.pem").absolute()),
            "imgtool":          str(Path(prj_config.get("sdk_path") + "imgtool/imgtool.py").absolute()),
            "version":          str(prj_config["cm0p"][image_slot]["VERSION"]),
            "id":               str(prj_config["cm0p"][image_slot]["IMAGE_ID"]),
            "rollback_counter": str(prj_config["cm0p"][image_slot]["ROLLBACK_COUNTER"]),
            "slot_size":        str(prj_config["cm0p"][image_slot]["SLOT_SIZE"]),
            "header_size":      "0x400",
            "align":            "8",
            "pad":              prj_config["cm0p"][image_slot]["PAD"]
        }

    return args_for_signature

# Sign binary image with Secureboot SDK tools
def sign_image(toolchain, resources, elf0, binf, hexf1=None):

    mbed_elf_path = Path(elf0).resolve()
    mbed_bin_path = Path(elf0[:-4] + ".bin").resolve()
    mbed_hex_path = Path(binf).resolve()

    print(binf)

    # create binary file from mbed elf for the following processing
    subprocess.Popen(["arm-none-eabi-objcopy.exe", str(mbed_elf_path),
                      "-O", "binary", str(mbed_bin_path)])

    # preserve original hex file from mbed-os build
    mbed_hex_path.replace((str(mbed_hex_path)[:-4] + "_unsigned.hex"))

    binf = binf[:-4] + ".bin"
    binf_signed = binf[:-4] + "_signed.bin"

    # gather arguments for signature command invoking
    sign_args = collect_args(image_slot="boot1")

    # call imgtool for signature
    process = subprocess.Popen(["python.exe", sign_args.get("imgtool"), "sign", "--key", sign_args.get("priv_key"),
                               "--header-size", sign_args.get("header_size"), "--pad-header", "--align",
                               sign_args.get("align"), "--version", sign_args.get("version"), "--image-id",
                               sign_args.get("id"), "--rollback_counter", sign_args.get("rollback_counter"),
                               "--slot-size", sign_args.get("slot_size"), "--overwrite-only", binf, binf_signed],
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr, stdout outputs
    stderr = process.communicate()
    if stderr:
        print("[PSOC6.sign_image hook message] - " + str(stderr))

    # convert signed image binary back to hex format
    subprocess.Popen(["arm-none-eabi-objcopy.exe", "--image-base", "0x10600400",
                      "-I", "binary", "-O", "ihex", binf_signed, str(mbed_hex_path)])

def complete(toolchain, elf0, hexf0, hexf1=None):
    complete_func(toolchain.notify.debug, elf0, hexf0, hexf1)
