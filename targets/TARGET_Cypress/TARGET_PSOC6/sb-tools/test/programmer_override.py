from pyocd_wrapper_override import PyocdOverride


tools = {
    'pyocd': PyocdOverride
}


class ProgrammingTool:
    @staticmethod
    def create(name):
        tool_type = tools[name]
        tool = tool_type()
        return tool
