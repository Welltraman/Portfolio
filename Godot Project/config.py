def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_path():
    return "docs"

def get_doc_classes():
    return [
        "ToolsAdapter",
        "ResourceManager",
        "TPTProcess",
        "TPTHandler",

        "InputHandler",
        "InputHandlerEvent",

        "ConfigResource",
        "JSONResource",
        "TextFileResource",
        "TPTResource",

        "TPTMethodBase",
        "TPTMDebug",
        "TPTMDialogue",
        "TPTMIf",
        "TPTMNext",
        "TPTMRepeat",
        "TPTMWait"
    ]
