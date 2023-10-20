#!/usr/bin/python3

###########################################################################
#                                                                         #
#    Code formatting manager for espert-sandbox                           #
#                                                                         #
#    Allows easy running of clang-format and and inplace fixing.          #
#    Run python3 code-analysis.py --help for further instructions.        #
#                                                                         #
###########################################################################

from argparse import ArgumentParser, Namespace
from common import ROOT, SOURCE_DIR
from pathlib import Path
import subprocess
import sys

CLANG_FORMAT = 'clang-format-17'


def is_program_avaliable(prog: str) -> bool:
    CMD = f'{prog} --help'
    proc = subprocess.Popen(CMD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, shell=True)
    proc.wait()
    return proc.returncode == 0


def get_format_command(args: Namespace) -> str:
    CMD = f'{CLANG_FORMAT}'

    if args.inplace:
        CMD += ' -i'
    else:
        CMD += ' --dry-run -Werror'

    return CMD


def run_analysis(command: str):
    all_files = 0
    correct_files = 0

    if args.path.is_file():
        entries = [args.path]
    else:
        entries = [entry for entry in args.path.rglob('*.h')] + \
        [entry for entry in args.path.rglob('*.hh')] + \
        [entry for entry in args.path.rglob('*.c')] + \
        [entry for entry in args.path.rglob('*.cc')]

    for entry in entries:
        if entry.is_dir(): continue
        print(entry)
        proc = subprocess.Popen(f'{command} {entry}', cwd=ROOT, shell=True)
        proc.wait()
        all_files += 1
        if proc.returncode == 0:
            correct_files += 1

    return (correct_files, all_files)


def run_format(args: Namespace) -> None:
    correct_files, all_files =  run_analysis(get_format_command(args))
    print(f'{CLANG_FORMAT}: {correct_files}/{all_files}')


def get_parsed_args() -> Namespace:
    main_parser = ArgumentParser(prog='Code format',
                            description='Run clang format.')

    main_parser.add_argument('-i',
                             '--inplace',
                             required=False,
                             default=False,
                             action='store_true',
                             help='Fix coding style errors inplace.')
    main_parser.add_argument('-p',
                             '--path',
                             required=False,
                             type=Path,
                             default=SOURCE_DIR,
                             help='Path to file or directory to run analysis on. (defaults to src)')
    
    return main_parser.parse_args()


if __name__ == '__main__':
    args = get_parsed_args()

    if not is_program_avaliable(CLANG_FORMAT):
        print(f'{CLANG_FORMAT} is not avaliable')
        sys.exit(1)

    run_format(args)
