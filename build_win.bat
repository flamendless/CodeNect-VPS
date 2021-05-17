set app_name=CodeNect
set dir_build=build_win
set dir_install=bin_win

cmake -G "MinGW Makefiles" -B%dir_build% -H. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%dir_install% -DCMAKE_C_COMPILER=clang.exe -DCMAKE_CXX_COMPILER=clang++.exe
cmake --build %dir_build% --target install