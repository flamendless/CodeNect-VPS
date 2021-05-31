@set app_name=CodeNectTest
@set dir_build=build_win_test
@set dir_install=bin_win_test

cmake -G "MinGW Makefiles" -B%dir_build% -H. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%dir_install% -DCMAKE_C_COMPILER=clang.exe -DCMAKE_CXX_COMPILER=clang++.exe
cmake --build %dir_build% --target install
cd %dir_install%
%app_name%.exe
cd ../
