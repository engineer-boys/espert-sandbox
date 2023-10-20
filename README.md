# Sandbox

## Build

In order to build espert several development tools are needed. Make sure they are installed and reachable from console.

Required packages:
- gcc-11 and g++-11 or clang-16 and clang++-16
- make
- cmake
- git
- python3

All code dependecies are managed within project. To download all external modules run in git repository:
```
git submodule update --init --recursive
```

To configure project run:
```
python3 scripts/espert-core.py configure
```

To build project run:
```
python3 scripts/espert-core.py build
```
Running configure before build is not necessary as the script will take care of it.

To run program use:
```
python3 scripts/espert-core.py run
```

## Formatting

To format code use:
```
python3 scripts/code-format.py
```
By default it runs in src directory. You can change the directory by adding -p flag.\
To fix coding style errors inplace add -i flag.