#!/usr/bin/env bash
# Build assets/CodeNect.icns from assets/logo.png (macOS app / Dock icon).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC="${ROOT}/assets/logo.png"
ICONSET="${ROOT}/build/CodeNect.iconset"
OUT="${ROOT}/assets/CodeNect.icns"

if [[ "$(uname -s)" != "Darwin" ]]; then
	echo "error: make_macos_icns.sh requires macOS (iconutil)." >&2
	exit 1
fi

if [[ ! -f "${SRC}" ]]; then
	echo "error: missing ${SRC}" >&2
	exit 1
fi

rm -rf "${ICONSET}"
mkdir -p "${ICONSET}"

for size in 16 32 128 256 512; do
	sips -z "${size}" "${size}" "${SRC}" --out "${ICONSET}/icon_${size}x${size}.png" >/dev/null
	double=$((size * 2))
	sips -z "${double}" "${double}" "${SRC}" --out "${ICONSET}/icon_${size}x${size}@2x.png" >/dev/null
done

xattr -cr "${ICONSET}" 2>/dev/null || true
iconutil -c icns "${ICONSET}" -o "${OUT}"
rm -rf "${ICONSET}"
