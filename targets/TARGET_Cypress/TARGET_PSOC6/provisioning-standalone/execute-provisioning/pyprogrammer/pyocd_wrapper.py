from base import ProgrammerBase, Interface, ResetType
from pyocd.core.helpers import ConnectHelper
from pyocd.core import exceptions
from exceptions import ExtendedTransferFaultError


class Pyocd(ProgrammerBase):
    def __init__(self):
        super(Pyocd, self).__init__()
        self.session = None
        self.board = None
        self.target = None
        self.probe = None

    def connect(self, target_name=None, interface=None):
        """
        Connects to target using default debug interface.
        :param target_name: The target name.
        :param interface:
        :return: True if connected successfully, otherwise False.
        """
        if interface:
            raise NotImplementedError
        else:
            if target_name:
                options = {
                    'target_override': target_name
                }
            else:
                options = {}
            self.session = ConnectHelper.session_with_chosen_probe(options=options)
            if self.session is None:
                return False
            self.board = self.session.board
            try:
                self.session.open()
            except exceptions.TransferFaultError as e:
                if not self.board.target.is_locked():
                    print(f"Transfer fault while initializing board: {e}")
                    return False
            except Exception as e:
                print(f"Exception while initializing board: {e}")
                return False

            self.target = self.board.target
            self.probe = self.session.probe
            return True

    def disconnect(self):
        """
        Closes active connection.
        """
        self.session.close()

    def set_frequency(self, value_khz):
        pass

    def halt(self):
        pass

    def reset(self, reset_type=ResetType.SW):
        pass

    def set_core(self, core_name):
        pass

    def read8(self, address):
        try:
            data = self.target.read_memory(address, transfer_size=8)
        except exceptions.TransferFaultError as e:
            raise ExtendedTransferFaultError(e.fault_address, e.fault_length)
        return data

    def read16(self, address):
        if (address & 0x01) == 0:
            try:
                data = self.target.read_memory(address, transfer_size=16)
            except exceptions.TransferFaultError as e:
                raise ExtendedTransferFaultError(e.fault_address, e.fault_length)
            return data
        else:
            raise ValueError('Address not aligned.')

    def read32(self, address):
        if (address & 0x03) == 0:
            try:
                data = self.target.read_memory(address, transfer_size=32)
            except exceptions.TransferFaultError as e:
                raise ExtendedTransferFaultError(e.fault_address, e.fault_length)
            return data
        else:
            raise ValueError('Address not aligned.')

    def write8(self, address, value):
        pass

    def write16(self, address, value):
        pass

    def write32(self, address, value):
        pass

    def erase(self, address, size):
        pass

    def program(self, filename):
        pass
