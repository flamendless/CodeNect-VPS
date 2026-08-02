app_name=CodeNect
dir_build=build
dir_install=bin

function debug()
{
	cd ${dir_install} && lldb ${app_name}
}

function rebuild()
{
	cmake -G Ninja -B${dir_build} -H. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/${dir_install}
}

function compile()
{
	cmake --build ${dir_build} --target install
	ctags -R src
	cp ${dir_build}/compile_commands.json .
}

function run()
{
	cd ${dir_install} && ./${app_name}
}

function package_linux()
{
	zip -9r codenect_linux.zip assets build.sh build_win.bat cmake CMakeLists.txt config.ini includes lib LICENSE src
}

function package_macos()
{
	zip -9r codenect_macos.zip assets build.sh build_win.bat cmake CMakeLists.txt config.ini includes lib LICENSE scripts src
}

function deps_macos()
{
	# installs the toolchain and libraries needed to build CodeNect on macOS
	# (Apple Silicon or Intel) via Homebrew
	brew install cmake ninja pkg-config glfw
	# TinyCC static libs are architecture specific and are not shipped in the
	# repo, so build them locally into lib/macos/
	./scripts/build_tcc_macos.sh
}

function clean()
{
	rm -rf ${dir_build}/*
}

if [ $# -eq 0 ]; then
	echo "Must pass command: rebuild, compile, run, clean, package_linux, package_macos, deps_macos"
else
	"$@"
fi
