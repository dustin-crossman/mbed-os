from pyocd.core.exceptions import TransferFaultError


class ExtendedTransferFaultError(TransferFaultError):
    def __str__(self):
        desc = super().__str__()
        desc += ' If address points to a register it should be aligned with the register size.'
        return desc
