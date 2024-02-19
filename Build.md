0. pip install conan
Run in VS console 'x64 Native Tools Command Prompt for VS 2022'
1. conan install . --build=missing --settings=build_type=Debug
2. cd build
3. cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=generators\conan_toolchain.cmake
4. cmake --build . --config Debug
Open in VS code
