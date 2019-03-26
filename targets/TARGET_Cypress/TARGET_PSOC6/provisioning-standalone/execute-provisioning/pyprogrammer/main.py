from programmer import ProgrammingTool


def main():
    target = 'CY8C6xx7'
    tool = ProgrammingTool.create('pyocd')

    if tool.connect(target):
        print(f"Connected to {target}")

    data = tool.read8(0x10000000)
    print(f'read8 = {hex(data)}')

    data = tool.read16(0x10000000)
    print(f'read16 = {hex(data)}')

    tool.halt()
    tool.write32(0x4023004c, 0xDEADBEAF)

    data = tool.read32(0x4023004c)
    print(f'read32 = {hex(data)}')

    tool.reset()

    data = tool.read32(0x4023004c)
    print(f'read32 = {hex(data)}')


if __name__ == "__main__":
        main()
