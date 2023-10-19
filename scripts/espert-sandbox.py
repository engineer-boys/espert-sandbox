#!/usr/bin/python3

###########################################################################
#                                                                         #
#    Build manager for espert-sandbox                                     #
#                                                                         #
#    Allows easy configuring, building and running of espert-sandbox.     #
#    Run python3 espert-sandbox.py --help for further instructions.       #
#                                                                         #
###########################################################################

from argparse import ArgumentParser, Namespace
from common import EnumAction, ROOT, BUILD_DIR
from enum import Enum
import multiprocessing
import os
from shutil import rmtree
import subprocess
import sys

GET_WSI_COMMAND = "loginctl show-session $(loginctl | grep $(whoami) | awk '{print $1}') -p Type | cut -d'=' -f2"
NAME = 'espert-sandbox'


class Platofrm(Enum):
    LINUX = 'linux'
    MACOS = 'darwin',
    WINDOWS = 'win32'


PLATFORM_SUFFIXES = {
    Platofrm.LINUX: '',
    Platofrm.MACOS: '',
    Platofrm.WINDOWS: '.exe'
}


class WSI(Enum):
    XCM = 'xcm'
    XLIB = 'xlib'
    WAYLAND = 'wayland'
    D2D = 'd2d'


def get_platform() -> Platofrm:
    return Platofrm(sys.platform)

BIN_NAME = f'{NAME}{PLATFORM_SUFFIXES[get_platform()]}'


def get_cpu_count() -> int:
    return multiprocessing.cpu_count()


def get_number_of_jobs() -> int:
    return max(1, get_cpu_count() - 2)


def get_wsi_type() -> str:
    proc = subprocess.Popen(GET_WSI_COMMAND, shell=True, stdout=subprocess.PIPE)
    proc.wait()
    return proc.stdout.read().decode('utf-8').strip()


def get_configure_command(args: Namespace) -> str:
    CMD = 'cmake -S . -B build'

    if args.debug:
        CMD += f' -DCMAKE_BUILD_TYPE=Debug'
    elif args.release:
        CMD += f' -DCMAKE_BUILD_TYPE=Release'

    platform = get_platform()
    if platform == Platofrm.LINUX:
        if args.wsi is None:
            wsi = get_wsi_type()
        else:
            wsi = args.wsi.value
        if wsi == 'x11': wsi = 'xlib'
        CMD += f' -DVKB_WSI_SELECTION={wsi.upper()}'

    if args.export_compile_commands:
        CMD += ' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'

    return CMD


def get_build_command(args) -> str:
    CMD = f'make -j{args.jobs}'

    return CMD


def run_command_detached(command: str, cwd: str) -> None:
    print(command)
    proc = subprocess.Popen(command, cwd=cwd, shell=True)
    proc.wait()
    if proc.returncode != 0:
        sys.exit(proc.returncode)


def run_configure(args: Namespace) -> None:
    if args.clean and os.path.exists(BUILD_DIR):
        rmtree(BUILD_DIR)

    if not args.clean and not os.path.exists(BUILD_DIR):
        os.mkdir(BUILD_DIR)

    configure_command = get_configure_command(args)
    run_command_detached(configure_command, ROOT)


def run_build(args: Namespace) -> None:
    if args.clean or not os.path.exists(BUILD_DIR) or os.listdir(BUILD_DIR) == 0:
        run_configure(args)

    build_command = get_build_command(args)
    run_command_detached(build_command, BUILD_DIR)


def run_espert(args: Namespace) -> None:
    if args.clean or not os.path.exists(BUILD_DIR / BIN_NAME):
        run_build(args)

    run_command = f'./{BIN_NAME}'
    run_command_detached(run_command, BUILD_DIR)


def get_parser() -> ArgumentParser:
    main_parser = ArgumentParser(prog='espert-sandbox',
                                 description='Manage espert-sandbox project.')

    subparsers = main_parser.add_subparsers(dest='cmd')
    configure_parser = subparsers.add_parser('configure',
                                             help='Configure project with cmake.')
    configure_parser.set_defaults(func=run_configure)
    build_parser = subparsers.add_parser('build',
                                         help='Build project.')
    build_parser.set_defaults(func=run_build)
    run_parser = subparsers.add_parser('run',
                                        help='Run project.')
    run_parser.set_defaults(func=run_espert)

    build_type_group = main_parser.add_mutually_exclusive_group()
    build_type_group.add_argument('-d',
                                  '--debug',
                                  default=True,
                                  action='store_true',
                                  help='Use debug configuration.')
    build_type_group.add_argument('-r',
                                  '--release',
                                  default=False,
                                  action='store_true',
                                  help='Use release configuration.')

    main_parser.add_argument('-c',
                             '--clean',
                             required=False,
                             default=False,
                             action='store_true',
                             help='Clean out build directory first.')
    main_parser.add_argument('-e',
                             '--export-compile-commands',
                             required=False,
                             default=False,
                             action='store_true',
                             help='Add CMAKE_EXPORT_COMPILE_COMMANDS to build.')
    main_parser.add_argument('-j',
                             '--jobs',
                             required=False,
                             type=int,
                             default=get_number_of_jobs(),
                             help='Number of paralell jobs during build.')
    main_parser.add_argument('-w',
                             '--wsi',
                             required=False,
                             type=WSI,
                             action=EnumAction,
                             help='Select WSI type for linux systems.')
    
    return main_parser


if __name__ == '__main__':
    parser = get_parser()
    args = parser.parse_args()
    if args.cmd is None:
        print('Command not given.')
        parser.print_usage()
        sys.exit(1)
    args.func(args)
