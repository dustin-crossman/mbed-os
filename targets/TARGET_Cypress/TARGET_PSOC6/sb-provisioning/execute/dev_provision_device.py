import os
import re
import sys
import provision_device

sys.path.insert(0, './Packets')
from cyprov_pem import PemKey

def provision_execution(openocd_root_path):
    entrance_exam_pass_msg = 'PROVISION PASSED'
    script = 'dev_provisioning.tcl'
    execution_status, cmd_output = provision_device.run_tcl_script(openocd_root_path, script)

    if execution_status == 0:
        print("PASS: Provisioning passed")
    else:
        print("FAIL: Provisioning returns invalid error code: %d" % execution_status)
        return False
    return True

if __name__ == "__main__":
    
    path = os.path.dirname(sys.argv[0])
    
    try:
        dev_rsp_file = sys.argv[1]
    except IndexError: 
        dev_rsp_file = os.path.join(path, 'test_response.json')
    
    try:
        keys_path = sys.argv[2]
    except IndexError:
        keys_path = os.path.join(path, '../../')
        
    try:
        openocd_root = sys.argv[3]
    except IndexError: 
        print('Path to OpenOCD binary file is not specified!')
    
    test_status = False
    
    # Exucute secure_blinky.tcl script
    test_status = provision_execution(openocd_root)
    
    # Read device responce file and take device key from it    
    if os.path.exists(dev_rsp_file):
        pem = PemKey(dev_rsp_file)
        key_path = os.path.join(keys_path, 'dev_pub_key.pem')
        pem.save(key_path, private_key=False)
    
    
