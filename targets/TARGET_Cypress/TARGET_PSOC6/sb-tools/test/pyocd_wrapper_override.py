import sys
import os.path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(os.path.dirname('../execute'))
from execute.programmer.base import ProgrammerBase, Interface, ResetType
from pyocd.core.helpers import ConnectHelper
from pyocd.core import exceptions
from pyocd.flash import loader
from pyocd.flash.loader import FlashEraser
from pyocd import coresight
from execute.programmer.exceptions import ExtendedTransferFaultError
from execute.programmer.pyocd_wrapper import Pyocd as PyocdWrapper


class PyocdOverride(PyocdWrapper):
    def __init__(self):
        super(PyocdWrapper, self).__init__()
        self.session = None
        self.board = None
        self.target = None
        self.probe = None

    def connect(self, target_name=None, interface=None, probe_id=None):
        """
        Connects to target using default debug interface.
        :param target_name: The target name.
        :param interface: Debug interface.
        :param probe_id: Probe serial number.
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
            self.session = ConnectHelper.session_with_chosen_probe(blocking=True, options=options, board_id=probe_id, unique_id=probe_id)
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

            # Write infinite loop into RAM and start core execution
            self.halt()
            # B662 - CPSIE I - Enable IRQ by clearing PRIMASK
            # E7FE - B - Jump to address (argument is an offset)
            self.write32(0x08000000, 0xE7FEB662)
            self.write_reg('pc', 0x08000000)
            self.write_reg('sp', 0x08001000)
            self.write_reg('xpsr', 0x01000000)
            self.resume()

            return True
