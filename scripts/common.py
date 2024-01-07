from abc import ABC
from argparse import Action
from enum import Enum
import multiprocessing
from pathlib import Path
import sys

ROOT = Path(__file__).parent.parent.absolute()
BUILD_DIR = ROOT / "build"
SOURCE_DIR = ROOT / "src"


def get_cpu_count() -> int:
    return multiprocessing.cpu_count()


def get_optimal_job_count() -> int:
    return max(1, get_cpu_count() - 2)


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


class CmakeParameter:
    def __init__(self, name: str, value: str) -> None:
        self._name = name
        self._value = value

    def __str__(self) -> str:
        return f"-D{self._name}={self._value}"


class CmakeCommand(ABC):
    def __init__(self) -> None:
        super().__init__()
        self._command = "cmake"

    def __str__(self) -> str:
        return self._command

    def add_parameter(self, param: CmakeParameter):
        self._command += f" {param}"
        return self


class CmakeConfigureCommand(CmakeCommand):
    def __init__(self, build_dir: str = "build", source_dir: str = ".") -> None:
        super().__init__()
        self._command += f" -B {build_dir}"
        self._command += f" -S {source_dir}"


class CmakeBuildCommand(CmakeCommand):
    def __init__(
        self, build_dir: str = "build", jobs: int = get_optimal_job_count()
    ) -> None:
        super().__init__()
        self._command += f" --build {build_dir} -j {jobs}"


def is_platform_linux() -> bool:
    return sys.platform.startswith("linux")


def is_platform_windows() -> bool:
    return sys.platform.startswith("win32")
