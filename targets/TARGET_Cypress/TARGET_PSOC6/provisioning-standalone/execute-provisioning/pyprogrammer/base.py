from enum import Enum
from abc import ABCMeta, abstractmethod


class ResetType(Enum):
    SW = 1,
    HW = 2


class Interface(Enum):
    SWD = 1
    JTAG = 2


class ProgrammerBase(metaclass=ABCMeta):
    def __init__(self):
        pass

    @abstractmethod
    def connect(self, target_name=None, interface=None):
        """
        Connects to target.
        :param target_name: The target name.
        :param interface: Debug interface.
        :return: True if connected successfully, otherwise False.
        """
        raise NotImplementedError()

    @abstractmethod
    def disconnect(self):
        """
        Disconnects from target.
        """
        raise NotImplementedError()

    @abstractmethod
    def set_frequency(self, value_khz):
        """
        Sets probe frequency.
        :param value_khz: Frequency in kHz.
        """
        raise NotImplementedError()

    @abstractmethod
    def halt(self):
        """
        Halts the target.
        """
        raise NotImplementedError()

    @abstractmethod
    def reset(self, reset_type=ResetType.SW):
        """
        Resets the target.
        :param reset_type: The reset type.
        """
        raise NotImplementedError()

    @abstractmethod
    def reset_and_halt(self, reset_type=ResetType.SW):
        """
        Resets the target and halts the CPU immediately after reset.
        :param reset_type: The reset type.
        """
        raise NotImplementedError()

    @abstractmethod
    def set_core(self, core_number):
        """
        Selects CPU core by number.
        :param core_number: The core number.
        """
        raise NotImplementedError()

    @abstractmethod
    def read8(self, address):
        """
        Reads 8-bit value from specified memory location.
        :param address: The memory address to read.
        :return: The read value.
        """
        raise NotImplementedError()

    @abstractmethod
    def read16(self, address):
        """
        Reads 16-bit value from specified memory location.
        :param address: The memory address to read.
        :return: The read value.
        """
        raise NotImplementedError()

    @abstractmethod
    def read32(self, address):
        """
        Reads 32-bit value from specified memory location.
        :param address: The memory address to read.
        :return: The read value.
        """
        raise NotImplementedError()

    @abstractmethod
    def write8(self, address, value):
        """
        Writes 8-bit value by specified memory location.
        :param address: The memory address to write.
        :param value: The 8-bit value to write.
        """
        raise NotImplementedError()

    @abstractmethod
    def write16(self, address, value):
        """
        Writes 16-bit value by specified memory location.
        :param address: The memory address to write.
        :param value: The 16-bit value to write.
        """
        raise NotImplementedError()

    @abstractmethod
    def write32(self, address, value):
        """
        Writes 32-bit value by specified memory location.
        :param address: The memory address to write.
        :param value: The 32-bit value to write.
        """
        raise NotImplementedError()

    @abstractmethod
    def erase(self, address, size):
        """
        Erases entire device flash or specified sectors.
        :param address: The memory location.
        :param size: The memory size.
        """
        raise NotImplementedError()

    @abstractmethod
    def program(self, filename, file_format=None, address=None):
        """
        Programs a file into flash.
        :param filename: Path to a file.
        :param file_format: File format. Default is to use the file's extension.
        :param address: Base address used for the address where to flash a binary.
        :return: True if programmed successfully, otherwise False.
        """
        raise NotImplementedError()
