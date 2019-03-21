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

    data = tool.read32(0xE0000000)
    print(f'read32 = {hex(data)}')


if __name__ == "__main__":
        main()
