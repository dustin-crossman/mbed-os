from execute.programmer.pyocd_wrapper import Pyocd


tools = {
    'pyocd': Pyocd
}


class ProgrammingTool:
    @staticmethod
    def create(name):
        tool_type = tools[name]
        tool = tool_type()
        return tool
