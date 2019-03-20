from pyocd.core.exceptions import TransferFaultError


class ExtendedTransferFaultError(TransferFaultError):
    def __str__(self):
        desc = super().__str__()
        desc += ' Make sure the address does not point to a register.'
        return desc
