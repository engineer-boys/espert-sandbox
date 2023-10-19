from argparse import Action
from enum import Enum
from pathlib import Path

ROOT = Path(__file__).parent.parent.absolute()
BUILD_DIR = ROOT / 'build'
SOURCE_DIR = ROOT

class EnumAction(Action):
    def __init__(self, **kwargs):
        enum_type = kwargs.pop("type", None)

        if enum_type is None:
            raise ValueError("type must be assigned an Enum when using EnumAction")
        if not issubclass(enum_type, Enum):
            raise TypeError("type must be an Enum when using EnumAction")

        kwargs.setdefault("choices", tuple(e.value for e in enum_type))

        super(EnumAction, self).__init__(**kwargs)

        self._enum = enum_type

    def __call__(self, parser, namespace, values, option_string=None):
        value = self._enum(values)
        setattr(namespace, self.dest, value)
