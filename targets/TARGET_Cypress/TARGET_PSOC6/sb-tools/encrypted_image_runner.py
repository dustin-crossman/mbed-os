"""
Copyright (c) 2018-2019 Cypress Semiconductor Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import os
import sys
import click
import platform
import subprocess
from pathlib import Path
from execute.helper import get_target_name
from execute.enums import DebugCore

from intelhex import IntelHex, hex2bin, bin2hex
from intelhex.compat import asbytes

HEADER_SIZE = 0x400
AES_HEADER="aes_header.txt"   # near the script file

def check_file_exist(file):
    if not Path(file).exists():
        print("ERROR: File %s not found. Check script arguments."% file)

@click.command()
@click.option('--hex-file', 'hex_file',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--key-priv', 'key_priv',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--key-pub', 'key_pub',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--key-aes', 'key_aes',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--ver', 'version',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--img-id', 'img_id',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--rlb-count', 'rlb_count',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--slot-size', 'slot_size',
              default=None,
              type=click.STRING,
              help='TBD')
@click.option('--pad', 'pad',
              default=None,
              type=click.STRING,
              help='TBD')

def main(hex_file,
            key_priv,
            key_pub,
            key_aes,
            version,
            img_id,
            rlb_count,
            slot_size,
            pad
            ):
    """    TBD    """

    check_file_exist(key_priv)
    check_file_exist(key_pub)
    check_file_exist(key_aes)
    check_file_exist(hex_file)
    
    bin_file        = hex_file[:-4] + ".bin"
    bin_sig         = bin_file[:-4] + "_signed.bin"
    bin_sig_enc     = bin_file[:-4] + "_sig_enc.bin"
    bin_sig_enc_sig = bin_file[:-4] + "_sig_enc_sig.bin"
    
    hex2bin(hex_file, bin_file)

    # $PYTHON $IMGTOOL sign --key $KEY --header-size $HEADER_SIZE --pad-header --align 8 --version $VERSION --image-id $ID --rollback_counter $ROLLBACK_COUNTER --slot-size $SLOT_SIZE --overwrite-only $binFileName $signedFileName is_file_created $signedFileName
    
    # call imgtool for signature
    process = subprocess.Popen([sys.executable, "imgtool/imgtool.py", "sign",
                                "--key", key_priv,
                                "--header-size", str(hex(HEADER_SIZE)),
                                "--pad-header",
                                "--align", "8",
                                "--version", version,
                                "--image-id", img_id,
                                "--rollback_counter", rlb_count,
                                "--slot-size", slot_size,
                                "--overwrite-only",
                                bin_file,
                                bin_sig],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr outputs
    stderr = process.communicate()
    rc = process.wait()
    check_file_exist(bin_sig)
    
    # AES
    # $PYTHON $(dirname "${IMGTOOL}")"/create_aesHeader.py" -k $KEY -p $KEY_DEV --key_to_encrypt "$KEY_AES" $AES_HEADER
    # call aesHeader for crypto header generation
    process = subprocess.Popen([sys.executable, "imgtool/create_aesHeader.py",
                                "-k", key_priv,
                                "-p", key_pub,
                                "--key_to_encrypt", key_aes,
                                AES_HEADER],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr outputs
    stderr = process.communicate()
    rc = process.wait()
    check_file_exist(AES_HEADER)

    # aes_cipher.py script file should be in the same folder as imgtool.py    
    # $PYTHON $(dirname "${IMGTOOL}")"/aes_cipher.py" -k $KEY_AES $signedFileName $aes_encryptedFileName
    # is_file_created $aes_encryptedFileName
    # encrypt signed image
    process = subprocess.Popen([sys.executable, "imgtool/aes_cipher.py",
                                "-k", key_aes,
                                bin_sig,
                                bin_sig_enc],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr outputs
    stderr = process.communicate()
    rc = process.wait()
    check_file_exist(bin_sig_enc)

    # second part - obtain signed image from encrypted file - with padding - for staging area
    # $PYTHON $IMGTOOL sign --key $KEY --header-size $HEADER_SIZE --pad-header --align 8 --version $VERSION --image-id $ID --rollback_counter $ROLLBACK_COUNTER --slot-size $SLOT_SIZE --overwrite-only $PAD -a $AES_HEADER $aes_encryptedFileName $signedEncFileName
    # is_file_created $signedEncFileName
    
    # call imgtool for signature
    process = subprocess.Popen([sys.executable, "imgtool/imgtool.py", "sign",
                                "--key", key_priv,
                                "--header-size", str(hex(HEADER_SIZE)),
                                "--pad-header",
                                "--align", "8",
                                "--version", version,
                                "--image-id", img_id,
                                "--rollback_counter", rlb_count,
                                "--slot-size", slot_size,
                                "--overwrite-only",
                                "--pad",
                                "-a", AES_HEADER,
                                bin_sig_enc,
                                bin_sig_enc_sig],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr outputs
    stderr = process.communicate()
    rc = process.wait()
    check_file_exist(bin_sig_enc_sig)
    
    os.remove(AES_HEADER)
    
if __name__ == "__main__":
    main()
