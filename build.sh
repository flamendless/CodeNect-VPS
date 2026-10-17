app_name=CodeNect
dir_build=build
dir_install=bin
_repo_root="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"

# Older macOS installs left bin/CodeNect and build/CodeNect next to CodeNect.app.
function remove_stale_macos_flat_binary()
{
	local f
	for f in "${_repo_root}/bin/CodeNect" "${_repo_root}/build/CodeNect"; do
		if [ -f "${f}" ]; then
			rm -f -- "${f}"
		fi
	done
}

function debug()
{
	if [ "$(uname -s)" = "Darwin" ]; then
		cd "${dir_install}/CodeNect.app/Contents/MacOS" && lldb ./${app_name}
	else
		cd ${dir_install} && lldb ${app_name}
	fi
}

function rebuild()
{
	cmake -G Ninja -B${dir_build} -H. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/${dir_install}
}

function compile()
{
	cmake --build ${dir_build} --target install
	if [ "$(uname -s)" = "Darwin" ]; then
		remove_stale_macos_flat_binary
	fi
	ctags -R src
	cp ${dir_build}/compile_commands.json .
}

function run()
{
	if [ "$(uname -s)" = "Darwin" ]; then
		cd "${dir_install}/CodeNect.app/Contents/MacOS" && ./${app_name}
	else
		cd ${dir_install} && ./${app_name}
	fi
}

function package_linux()
{
	zip -9r codenect_linux.zip assets build.sh build_win.bat cmake CMakeLists.txt config.ini includes lib LICENSE src
}

function package_macos()
{
	zip -9r codenect_macos.zip assets build.sh build_win.bat cmake CMakeLists.txt config.ini includes lib LICENSE scripts src
}

function dmg()
{
	if [ "$(uname -s)" != "Darwin" ]; then
		echo "dmg: macOS only" >&2
		return 1
	fi
	if [ ! -d "${dir_install}/CodeNect.app" ]; then
		echo "dmg: run ./build.sh compile first (need ${dir_install}/CodeNect.app)" >&2
		return 1
	fi
	"${_repo_root}/scripts/package_macos_dmg.sh" "$@"
}

function package_macos_app()
{
	if [ "$(uname -s)" != "Darwin" ]; then
		echo "package_macos_app: macOS only" >&2
		return 1
	fi
	if [ ! -d "${dir_install}/CodeNect.app" ]; then
		echo "package_macos_app: run ./build.sh compile first" >&2
		return 1
	fi
	local version arch zip_name
	version="$(sed -n 's/^version[[:space:]]*=[[:space:]]*//p' config.ini 2>/dev/null | head -1 | tr -d '\r')"
	version="${version:-0.0.0}"
	arch="$(uname -m)"
	zip_name="dist/CodeNect-${version}-macOS-${arch}.zip"
	mkdir -p dist
	rm -f "${zip_name}"
	( cd "${dir_install}" && zip -9ry "../${zip_name}" CodeNect.app )
	echo "Wrote ${zip_name}"
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
	echo "Must pass command: rebuild, compile, run, clean, package_linux, package_macos, package_macos_app, dmg, deps_macos"
else
	"$@"
fi
