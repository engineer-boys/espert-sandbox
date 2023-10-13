if not exist build mkdir build

if %1 == "" set BUILD_TYPE="Debug"
if %1 == "debug" set BUILD_TYPE="Debug"
if %1 == "Debug" set BUILD_TYPE="Debug"
if %1 == "release" set BUILD_TYPE="Release"
if %1 == "Release" set BUILD_TYPE="Release"

cmake -S . -B build -DCCMAKE_BUILD_TYPE=%BUILD_TYPE%