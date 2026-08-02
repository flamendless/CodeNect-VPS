#!/usr/bin/env bash
#
# Build the TinyCC (libtcc) static libraries for macOS (Apple Silicon or Intel).
#
# CodeNect statically links against libtcc to transpile and run the visual
# program in-memory. The prebuilt libraries shipped in the repo only cover
# Windows and Linux, so macOS users have to build a matching libtcc once.
#
# The script clones TinyCC, compiles it for the host architecture and copies
# the resulting "libtcc.a" and "libtcc1.a" into "lib/macos/" where CMake looks
# for them.
#
# Usage:
#   ./scripts/build_tcc_macos.sh [tcc_git_ref]
#
# Notes:
#   * macOS support in TinyCC (Mach-O backend) is still experimental, more so
#     on Apple Silicon (arm64). Pin "tcc_git_ref" to a known-good commit/tag if
#     the default branch regresses.
#   * Requires the Xcode Command Line Tools (clang, make, git).

set -euo pipefail

TCC_REPO="${TCC_REPO:-https://github.com/TinyCC/tinycc.git}"
TCC_REF="${1:-mob}"

# resolve important paths relative to this script so it can be run from anywhere
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DEST_DIR="${ROOT_DIR}/lib/macos"
WORK_DIR="${ROOT_DIR}/build/tcc-macos"

if [[ "$(uname -s)" != "Darwin" ]]; then
	echo "error: this script is meant to be run on macOS." >&2
	exit 1
fi

ARCH="$(uname -m)"
echo "==> Building TinyCC for macOS (${ARCH}) from ${TCC_REPO}@${TCC_REF}"

for tool in git make clang; do
	if ! command -v "${tool}" >/dev/null 2>&1; then
		echo "error: '${tool}' not found. Install the Xcode Command Line Tools:" >&2
		echo "       xcode-select --install" >&2
		exit 1
	fi
done

mkdir -p "${WORK_DIR}"

if [[ -d "${WORK_DIR}/tinycc/.git" ]]; then
	echo "==> Updating existing TinyCC checkout"
	git -C "${WORK_DIR}/tinycc" fetch --depth 1 origin "${TCC_REF}"
	git -C "${WORK_DIR}/tinycc" checkout FETCH_HEAD
else
	echo "==> Cloning TinyCC"
	rm -rf "${WORK_DIR}/tinycc"
	git clone --depth 1 --branch "${TCC_REF}" "${TCC_REPO}" "${WORK_DIR}/tinycc" \
		|| git clone "${TCC_REPO}" "${WORK_DIR}/tinycc"
fi

cd "${WORK_DIR}/tinycc"

echo "==> Configuring"
# a local prefix keeps the build self-contained; we only consume the .a files
./configure \
	--cc=clang \
	--prefix="${WORK_DIR}/install" \
	--extra-cflags="-arch ${ARCH}" \
	--extra-ldflags="-arch ${ARCH}"

echo "==> Compiling libtcc and the runtime support library"
make clean >/dev/null 2>&1 || true
make libtcc.a
# libtcc1.a is the compiler runtime that generated programs link against
make libtcc1.a || make

echo "==> Installing libraries into ${DEST_DIR}"
mkdir -p "${DEST_DIR}"
cp -v libtcc.a "${DEST_DIR}/libtcc.a"

# libtcc1.a lands either at the repo root or under a per-arch subdirectory
if [[ -f libtcc1.a ]]; then
	cp -v libtcc1.a "${DEST_DIR}/libtcc1.a"
else
	found_libtcc1="$(find . -name 'libtcc1.a' -print -quit || true)"
	if [[ -n "${found_libtcc1}" ]]; then
		cp -v "${found_libtcc1}" "${DEST_DIR}/libtcc1.a"
	else
		echo "warning: libtcc1.a was not produced; runtime linking may fail." >&2
	fi
fi

echo ""
echo "Done. libtcc for macOS is installed in ${DEST_DIR}:"
ls -la "${DEST_DIR}"
echo ""
echo "You can now build CodeNect:"
echo "  ./build.sh rebuild && ./build.sh compile && ./build.sh run"
