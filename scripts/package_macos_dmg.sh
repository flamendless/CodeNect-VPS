#!/usr/bin/env bash
# Stage bin/CodeNect.app and build a compressed .dmg for distribution (e.g. itch.io).
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
APP_NAME="CodeNect"
APP_SRC="${ROOT_DIR}/bin/${APP_NAME}.app"
EXE="${APP_SRC}/Contents/MacOS/${APP_NAME}"
DIST_DIR="${ROOT_DIR}/dist"
STAGE_DIR="${DIST_DIR}/dmg-staging"

usage() {
	echo "Usage: $0 [--no-sign]" >&2
	echo "  Builds dist/${APP_NAME}-<version>-macOS-<arch>.dmg from bin/${APP_NAME}.app." >&2
	echo "  Run ./build.sh compile first." >&2
	echo "  Optional: CODENECT_CODESIGN_IDENTITY=\"Developer ID Application: …\" to codesign." >&2
}

sign_app=1
for arg in "$@"; do
	case "$arg" in
		-h | --help) usage; exit 0 ;;
		--no-sign) sign_app=0 ;;
		*) echo "Unknown option: $arg" >&2; usage; exit 1 ;;
	esac
done

if [ "$(uname -s)" != "Darwin" ]; then
	echo "error: package_macos_dmg.sh requires macOS." >&2
	exit 1
fi

if [ ! -x "${EXE}" ]; then
	echo "error: ${EXE} not found. Run: ./build.sh rebuild && ./build.sh compile" >&2
	exit 1
fi

version="${CODENECT_VERSION:-}"
if [ -z "${version}" ] && [ -f "${ROOT_DIR}/config.ini" ]; then
	version="$(sed -n 's/^version[[:space:]]*=[[:space:]]*//p' "${ROOT_DIR}/config.ini" | head -1 | tr -d '\r')"
fi
version="${version:-0.0.0}"

arch="$(uname -m)"
dmg_name="${APP_NAME}-${version}-macOS-${arch}.dmg"
dmg_path="${DIST_DIR}/${dmg_name}"

# Copy non-system dylibs next to the binary and fix load paths (Homebrew GLFW, etc.).
bundle_third_party_dylibs() {
	local binary="$1"
	local macos_dir
	macos_dir="$(dirname "${binary}")"
	local fw_dir="${macos_dir}/../Frameworks"
	mkdir -p "${fw_dir}"

	otool -L "${binary}" | awk '/^\t\// {print $1}' | while IFS= read -r lib; do
		case "${lib}" in
			/usr/lib/* | /System/*) continue ;;
		esac
		if [ ! -f "${lib}" ]; then
			echo "error: missing dependency ${lib} (rebuild or install the library)." >&2
			exit 1
		fi
		local base
		base="$(basename "${lib}")"
		local dest="${fw_dir}/${base}"
		if [ ! -f "${dest}" ]; then
			cp -f "${lib}" "${dest}"
			chmod 755 "${dest}"
		fi
		install_name_tool -id "@executable_path/../Frameworks/${base}" "${dest}"
		install_name_tool -change "${lib}" "@executable_path/../Frameworks/${base}" "${binary}"
	done
}

codesign_app_if_requested() {
	local app="$1"
	if [ "${sign_app}" -eq 0 ]; then
		return 0
	fi
	local identity="${CODENECT_CODESIGN_IDENTITY:-}"
	if [ -z "${identity}" ]; then
		return 0
	fi
	echo "Codesigning with identity: ${identity}"
	codesign --force --sign "${identity}" --timestamp --options runtime \
		"${app}/Contents/MacOS/"*.dylib 2>/dev/null || true
	if [ -d "${app}/Contents/Frameworks" ]; then
		codesign --force --sign "${identity}" --timestamp --options runtime \
			"${app}/Contents/Frameworks/"*.dylib 2>/dev/null || true
	fi
	codesign --force --deep --sign "${identity}" --timestamp --options runtime "${app}"
}

rm -rf "${STAGE_DIR}"
mkdir -p "${STAGE_DIR}" "${DIST_DIR}"

echo "Staging ${APP_NAME}.app …"
ditto "${APP_SRC}" "${STAGE_DIR}/${APP_NAME}.app"

STAGED_EXE="${STAGE_DIR}/${APP_NAME}.app/Contents/MacOS/${APP_NAME}"
bundle_third_party_dylibs "${STAGED_EXE}"
codesign_app_if_requested "${STAGE_DIR}/${APP_NAME}.app"

ln -sf /Applications "${STAGE_DIR}/Applications"

rm -f "${dmg_path}"
echo "Creating ${dmg_path} …"
hdiutil create \
	-volname "${APP_NAME}" \
	-srcfolder "${STAGE_DIR}" \
	-ov \
	-format UDZO \
	"${dmg_path}"

if [ -n "${CODENECT_CODESIGN_IDENTITY:-}" ] && [ "${sign_app}" -eq 1 ]; then
	codesign --force --sign "${CODENECT_CODESIGN_IDENTITY}" --timestamp "${dmg_path}" 2>/dev/null || true
fi

echo "Done: ${dmg_path}"
if [ -z "${CODENECT_CODESIGN_IDENTITY:-}" ] || [ "${sign_app}" -eq 0 ]; then
	echo "Note: unsigned builds may require Right-click → Open on first launch (Gatekeeper)."
fi
