import sys
import prepare_device

if __name__ == "__main__":
    # Get path to OpenOCD binary file
    try:
        openocd_root = sys.argv[1]
    except IndexError: 
        print('Path to OpenOCD binary file is not specified!')
    
    test_status = False
    
    # Program flash boot and toc2
    test_status = prepare_device.program_flashboot_and_toc2(openocd_root)
    
    if test_status:
        print("-"*80)
        print("Everything is fine!")
        print("-"*80)
    