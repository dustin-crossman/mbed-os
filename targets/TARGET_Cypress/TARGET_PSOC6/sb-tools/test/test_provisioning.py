import sys
import time
import os.path
import unittest
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(os.path.dirname('../execute/programmer'))
import provision_device_runner
from execute.enums import ProtectionState
from execute.helper import get_target_name
from execute.programmer.programmer import ProgrammingTool
from execute.gen_data_from_json import ENTRANCE_EXAM_SRAM_ADDR
from execute.p6_reg import CYREG_CPUSS_PROTECTION
from execute.p6_reg import CYREG_IPC2_STRUCT_NOTIFY, CYREG_IPC2_STRUCT_LOCK_STATUS

PROBE_ID = '19071301dd0b211502dd0b2100000000000000002e127069'  # DAP-Link


class TestProvisioning(unittest.TestCase):
    def test_provision_device_with_logs(self):
        # Run provisioning
        try:
            self.provision_device('CyBootloader_WithLogs')
        except SystemExit as e:
            self.assertEqual(e.code, 0)

        # Create programming tool object and connect to the target
        connected, tool = self.connect()
        self.assertTrue(connected)

        # Check device protection mode
        self.assertEqual(self.read_device_mode(tool), ProtectionState.virgin)

        # Check device provisioning status
        self.assertTrue(self.read_provisioning_status(tool))

        # Disconnect from the target
        tool.disconnect();

    def test_provision_device_release(self):
        # Run provisioning
        try:
            self.provision_device('CyBootloader_Release')
        except SystemExit as e:
            self.assertEqual(e.code, 0)

        # Create programming tool object and connect to the target
        connected, tool = self.connect()
        self.assertTrue(connected)

        # Check device protection mode
        self.assertEqual(self.read_device_mode(tool), ProtectionState.virgin)

        # Check device provisioning status
        self.assertTrue(self.read_provisioning_status(tool))

        # Disconnect from the target
        tool.disconnect();

    @staticmethod
    def connect():
        """
        Connect to the target.
        :return: Connection status, programming tool.
        """
        target = get_target_name(provision_device_runner.TOOL_NAME, provision_device_runner.ACCESS_PORT)
        tool = ProgrammingTool.create(provision_device_runner.TOOL_NAME)
        connected = tool.connect(target, probe_id=PROBE_ID)
        return connected, tool

    @staticmethod
    def provision_device(cy_bootloader):
        args = [
            '--prov-jwt', '../packet/prov_cmd.jwt',
            '--hex', f'../prebuild/{cy_bootloader}/CypressBootloader_CM0p.hex',
            '--pubkey-json', '../keys/dev_pub_key.json',
            '--pubkey-pem', '../keys/dev_pub_key.pem',
            '--protection-state', '1',
            '--probe-id', PROBE_ID
        ]
        provision_device_runner.main(args)

    @staticmethod
    def read_device_mode(tool):
        """
        Check whether device is still in Virgin mode.
        :param tool: Programming tool.
        :return: Device protection mode.
        """
        return ProtectionState(tool.read32(CYREG_CPUSS_PROTECTION))

    @staticmethod
    def read_provisioning_status(tool):
        """
        Check whether device provisioned successfully.
        :param tool: Programming tool.
        :return: Device provisioning status.
        """
        tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)
        response = 0x80000000
        future = time.time() + 10
        while (response & 0x80000000) != 0:
            response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)
            if time.time() > future:
                raise TimeoutError('Failed to get response while checking provisioning status.')
        value = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
        return value & 0xFF000000 == 0xa0000000


if __name__ == '__main__':
    ret = not unittest.main().wasSuccessful()
    sys.exit(ret)
