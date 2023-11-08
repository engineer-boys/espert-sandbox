# Sandbox

## Build

In order to build espert several development tools are needed. Make sure they are installed and reachable from console.

Required packages:
- gcc and g++ or clang-17 and clang++-17
- make
- cmake
- git
- python3

All code dependecies are managed within project. To download all external modules run in git repository:
```
git submodule update --init --recursive
```

To run project use:
```
python3 scripts/espert-sandbox.py run
```
The project will be configured and build automatically. You can configure project explicitly by runing:

```
python3 scripts/espert-sandbox.py configure
```
and build by running:
```
python3 scripts/espert-sandbox.py build
```
Running configure before build is not necessary as the script will take care of it. \
By default the project doesn't build Vulkan validation layers. If you want to run espert in debug mode and haven't installed Vulkan SDK add --vvl flag to build validation layers. Keep in mind that this process does take a WHILE. Please wait patiently and do not abort the build. Once you have build vvl one you won't have to do it again.

## Formatting

Required dependencies for code formatting are:
- clang-format-17
- clang-tidy-17

To format code use:
```
python3 scripts/code-format.py
```
By default it will run both clang-format and clang-tidy. Keep in mind that clang-tidy works only if you use clang and clang++ as project compilers. To rebuild project with clang run
```
python3 scripts/espert-core.py -c --clang build
```
By default it runs in src/ directory. You can change the directory by adding -p flag.\
To fix coding style errors inplace add -i flag. To fix suggestions add -x and to fix all errors add -e.\
To learn more about code formatting options run
```
python3 scripts/code-format.py --help
```