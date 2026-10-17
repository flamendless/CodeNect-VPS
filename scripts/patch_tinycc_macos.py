#!/usr/bin/env python3
"""Apply TinyCC source tweaks needed to build libtcc1.a with clang on macOS."""

from __future__ import annotations

import pathlib
import sys


def patch_stdatomic(path: pathlib.Path) -> None:
    text = path.read_text()
    if "CODENECT_MACOS_STDATOMIC" in text:
        return
    old = (
        "#ifndef __TINYC__\n"
        "bool __atomic_is_lock_free(unsigned long size, const volatile void *ptr) "
        '__attribute__((alias("__tcc_atomic_is_lock_free")));\n'
        "#endif\n"
    )
    new = (
        "#ifndef __TINYC__\n"
        "/* CODENECT_MACOS_STDATOMIC */\n"
        "#if !defined(__APPLE__)\n"
        "bool __atomic_is_lock_free(unsigned long size, const volatile void *ptr) "
        '__attribute__((alias("__tcc_atomic_is_lock_free")));\n'
        "#endif\n"
        "#endif\n"
    )
    if old not in text:
        raise SystemExit(f"{path}: stdatomic.c layout changed; update patch_tinycc_macos.py")
    path.write_text(text.replace(old, new, 1))


def patch_builtin(path: pathlib.Path) -> None:
    text = path.read_text()
    if "CODENECT_MACOS_BUILTIN" in text:
        return
    pairs = [
        (
            "int BUILTIN(ffsl) (long x) __attribute__((alias(BUILTINN(ffs))));",
            "int BUILTIN(ffsl) (long x) { return BUILTIN(ffs)((int)x); }",
        ),
        (
            "int BUILTIN(ffsl) (long x) __attribute__((alias(BUILTINN(ffsll))));",
            "int BUILTIN(ffsl) (long x) { return BUILTIN(ffsll)(x); }",
        ),
        (
            "int BUILTIN(clzl) (unsigned long x) __attribute__((alias(BUILTINN(clz))));",
            "int BUILTIN(clzl) (unsigned long x) { return BUILTIN(clz)((unsigned int)x); }",
        ),
        (
            "int BUILTIN(clzl) (unsigned long x) __attribute__((alias(BUILTINN(clzll))));",
            "int BUILTIN(clzl) (unsigned long x) { return BUILTIN(clzll)(x); }",
        ),
        (
            "int BUILTIN(ctzl) (unsigned long x) __attribute__((alias(BUILTINN(ctz))));",
            "int BUILTIN(ctzl) (unsigned long x) { return BUILTIN(ctz)((unsigned int)x); }",
        ),
        (
            "int BUILTIN(ctzl) (unsigned long x) __attribute__((alias(BUILTINN(ctzll))));",
            "int BUILTIN(ctzl) (unsigned long x) { return BUILTIN(ctzll)(x); }",
        ),
        (
            "int BUILTIN(clrsbl) (long x) __attribute__((alias(BUILTINN(clrsb))));",
            "int BUILTIN(clrsbl) (long x) { return BUILTIN(clrsb)((int)x); }",
        ),
        (
            "int BUILTIN(clrsbl) (long x) __attribute__((alias(BUILTINN(clrsbll))));",
            "int BUILTIN(clrsbl) (long x) { return BUILTIN(clrsbll)(x); }",
        ),
        (
            "int BUILTIN(popcountl) (unsigned long x) __attribute__((alias(BUILTINN(popcount))));",
            "int BUILTIN(popcountl) (unsigned long x) { return BUILTIN(popcount)((unsigned int)x); }",
        ),
        (
            "int BUILTIN(popcountl ) (unsigned long x) __attribute__((alias(BUILTINN(popcountll))));",
            "int BUILTIN(popcountl ) (unsigned long x) { return BUILTIN(popcountll)(x); }",
        ),
        (
            "int BUILTIN(parityl) (unsigned long x) __attribute__((alias(BUILTINN(parity))));",
            "int BUILTIN(parityl) (unsigned long x) { return BUILTIN(parity)((unsigned int)x); }",
        ),
        (
            "int BUILTIN(parityl) (unsigned long x) __attribute__((alias(BUILTINN(parityll))));",
            "int BUILTIN(parityl) (unsigned long x) { return BUILTIN(parityll)(x); }",
        ),
    ]
    for old, new in pairs:
        if old in text:
            text = text.replace(old, new, 1)
    start = text.find("#ifndef __TINYC__\n")
    if start < 0:
        raise SystemExit(f"{path}: builtin.c layout changed; update patch_tinycc_macos.py")
    text = (
        text[:start]
        + "/* CODENECT_MACOS_BUILTIN: omit gcc/clang __builtin_* aliases on macOS */\n"
    )
    path.write_text(text)


def main() -> None:
    root = pathlib.Path(sys.argv[1])
    patch_stdatomic(root / "lib" / "stdatomic.c")
    patch_builtin(root / "lib" / "builtin.c")


if __name__ == "__main__":
    main()
