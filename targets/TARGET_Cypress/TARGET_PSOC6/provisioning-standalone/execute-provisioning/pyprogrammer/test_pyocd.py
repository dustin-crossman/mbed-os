from unittest import TestCase
from programmer import ProgrammingTool
from exceptions import ExtendedTransferFaultError


TOOL = 'pyocd'
TARGET = 'CY8C6xx7'

# PSoC6 BLE Memory Regions
RAM_ADDR = 0x08000000
MAIN_ADDR = 0x10000000
WORK_ADDR = 0x14000000
SFLASH_ADDR = 0x16000000

# PSoC6 BLE Register Addresses
CYREG_IPC2_STRUCT_ACQUIRE = 0x40230040
CYREG_IPC2_STRUCT_DATA = 0x4023004c
CYREG_IPC2_STRUCT_NOTIFY = 0x40230048
ENTRANCE_EXAM_SRAM_ADDR = 0x0802E000


class TestPyocd(TestCase):
    def setUp(self):
        self.tool = ProgrammingTool.create(TOOL)
        self.assertTrue(self.tool.connect(TARGET))

    def tearDown(self):
        self.tool.disconnect()

    def test_read8_ram(self):
        data = self.tool.read8(RAM_ADDR)
        self.assertGreaterEqual(data, 0)

    def test_read8_flash_main_region(self):
        data = self.tool.read8(MAIN_ADDR)
        self.assertEqual(data, 0x00)

    def test_read8_flash_work_region(self):
        data = self.tool.read8(WORK_ADDR)
        self.assertEqual(data, 0x00)

    def test_read8_sflash(self):
        data = self.tool.read8(SFLASH_ADDR)
        self.assertEqual(data, 0x00)

    def test_read8_register32_negative(self):
        with self.assertRaises(ExtendedTransferFaultError) as context:
            self.tool.read8(CYREG_IPC2_STRUCT_DATA)
        self.assertTrue('If address points to a register it should be aligned with the register size'
                        in str(context.exception))

    def test_read16_ram(self):
        data = self.tool.read16(RAM_ADDR)
        self.assertGreater(data, 0)

    def test_read16_flash_main_region(self):
        data = self.tool.read16(MAIN_ADDR)
        self.assertEqual(data, 0x00)

    def test_read16_flash_work_region(self):
        data = self.tool.read16(WORK_ADDR)
        self.assertEqual(data, 0x00)

    def test_read16_register32_negative(self):
        with self.assertRaises(ExtendedTransferFaultError) as context:
            self.tool.read16(CYREG_IPC2_STRUCT_DATA)
        self.assertTrue('If address points to a register it should be aligned with the register size'
                        in str(context.exception))

    def test_read32_ram(self):
        data = self.tool.read32(RAM_ADDR)
        self.assertGreater(data, 0x00)

    def test_read32_flash_main_region(self):
        data = self.tool.read32(MAIN_ADDR)
        self.assertEqual(data, 0x00)

    def test_read32_flash_work_region(self):
        data = self.tool.read32(WORK_ADDR)
        self.assertEqual(data, 0x00)

    def test_read32_register32(self):
        data = self.tool.read32(CYREG_IPC2_STRUCT_DATA)
        self.assertGreater(data, 0x00000000)

    def test_write8_ram(self):
        self.tool.write8(RAM_ADDR, 0xCE)
        data = self.tool.read8(RAM_ADDR)
        self.assertEqual(0xCE, data)

    def test_write16_ram(self):
        self.tool.write16(RAM_ADDR, 0xAC21)
        data = self.tool.read16(RAM_ADDR)
        self.assertEqual(0xAC21, data)

    def test_write32_ram(self):
        self.tool.write32(RAM_ADDR, 0xDEADBEAF)
        data = self.tool.read32(RAM_ADDR)
        self.assertEqual(data, 0xDEADBEAF)

    def test_write32_CYREG_IPC2_STRUCT_ACQUIRE(self):
        self.tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
        data = self.tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)
        byte = self.decomposite32(data)[3]
        self.assertEqual(byte, 0x80)

    def test_write32_CYREG_IPC2_STRUCT_DATA(self):
        self.tool.write32(CYREG_IPC2_STRUCT_DATA, 0xAFECAB91)
        data = self.tool.read32(CYREG_IPC2_STRUCT_DATA)
        self.assertEqual(data, 0xAFECAB91)

    def test_write32_CYREG_IPC2_STRUCT_NOTIFY(self):
        self.tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)
        data = self.tool.read32(CYREG_IPC2_STRUCT_NOTIFY)
        byte = self.decomposite32(data)[3]
        self.assertEqual(byte, 0x00)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR, 0xD1A2B3C4)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
        self.assertEqual(data, 0xD1A2B3C4)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR_offset4(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, 0xABCABCFA)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)
        self.assertEqual(data, 0xABCABCFA)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR_offset8(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, 0x12890735)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)
        self.assertEqual(data, 0x12890735)

    def test_erase(self):
        # self.tool.program('512.bin', 'bin', MAIN_ADDR)
        data = self.tool.read32(MAIN_ADDR)
        self.tool.erase(0x10000000, 0x00000800)
        data = self.tool.read32(MAIN_ADDR)

    def test_program(self):
        self.tool.program('512.bin', 'bin', MAIN_ADDR)
        data = self.tool.read8(MAIN_ADDR)
        data = self.tool.read8(MAIN_ADDR+1)
        data = self.tool.read8(MAIN_ADDR+2)
        data = self.tool.read8(MAIN_ADDR+3)
        data = self.tool.read8(MAIN_ADDR+4)
        self.assertGreater(data, 0xBABBCCDD)

    @staticmethod
    def decomposite32(value):
        """
        Decomposites 32-bit value into byte array.
        :param value: 32-bit value.
        :return: Array of bytes.
        """
        byte = value.to_bytes(4, 'little')
        return byte
