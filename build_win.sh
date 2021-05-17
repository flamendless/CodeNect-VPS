app_name=CodeNect
dir_build=build_win
dir_install=bin_win

function debug()
{
	cd ${dir_install} && lldb ${app_name}
}

function rebuild()
{
	cmake -G "MinGW Makefile" -B${dir_build} -H. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PWD/${dir_install}
}

function compile()
{
	cmake --build ${dir_build} --target install
	ctags -R src
}

function run()
{
	cd ${dir_install} && ./${app_name}
}

function clean()
{
	rm -rf ${dir_build}/*
}

if [ $# -eq 0 ]; then
	echo "Must pass command: rebuild, compile, run, clean"
else
	"$@"
fi
