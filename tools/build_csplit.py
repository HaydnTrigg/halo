#!/usr/bin/env python3

###
# Builds csplit from source.
#
# csplit releases do not ship a binary for every host (v0.0.4 has no
# Windows build), so the build fetches a pinned commit and compiles it.
#
# Usage:
#   python3 tools/build_csplit.py build/tools/csplit.exe \
#       --commit a49a9e038670958f332e54500565b8ecfed60c43
#
# Uses CMake when it is installed. Otherwise it compiles the two C sources
# with the first C compiler found (cc, gcc, clang, or MSVC's cl).
###

import argparse
import os
import re
import shutil
import stat
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import List, Optional

REPO = "https://github.com/punpckhdq/csplit"
SOURCES = ["main.c", "cJSON.c"]
DEFINES = ["CJSON_HIDE_SYMBOLS"]
EXE = ".exe" if os.name == "nt" else ""


def run(cmd: List[str], cwd: Optional[Path] = None) -> None:
    print("+ " + " ".join(cmd), flush=True)
    subprocess.run(cmd, cwd=cwd, check=True)


def remove_tree(path: Path) -> None:
    # git marks pack files read-only, which stops rmtree on Windows.
    def make_writable(func, target, _):
        os.chmod(target, stat.S_IWRITE)
        func(target)

    if sys.version_info >= (3, 12):
        shutil.rmtree(path, onexc=make_writable)
    else:
        shutil.rmtree(path, onerror=make_writable)


def checkout(commit: str, dest: Path) -> None:
    """Fetch exactly one commit by SHA and check it out detached."""
    if shutil.which("git") is None:
        sys.exit("git is required to fetch the csplit source")
    if not re.fullmatch(r"[0-9a-f]{40}", commit):
        sys.exit(f"csplit commit must be a full 40-character SHA: {commit!r}")
    dest.mkdir(parents=True)
    git = ["git", "-c", "advice.detachedHead=false", "-C", str(dest)]
    run([*git, "init", "--quiet"])
    run([*git, "fetch", "--quiet", "--depth", "1", REPO, commit])
    run([*git, "checkout", "--quiet", "FETCH_HEAD"])
    head = subprocess.run([*git, "rev-parse", "HEAD"], check=True,
                          capture_output=True, text=True).stdout.strip()
    if head != commit:
        sys.exit(f"csplit checkout is at {head}, expected {commit}")


def build_with_cmake(src: Path, work: Path) -> Path:
    out = work / "cmake-build"
    run(["cmake", "-S", str(src), "-B", str(out), "-DCMAKE_BUILD_TYPE=Release"])
    run(["cmake", "--build", str(out), "--config", "Release"])
    for candidate in (
        out / "csplit" / "Release" / f"csplit{EXE}",
        out / "csplit" / f"csplit{EXE}",
    ):
        if candidate.is_file():
            return candidate
    sys.exit(f"CMake build finished but csplit{EXE} was not found under {out}")


def build_with_compiler(src: Path, work: Path) -> Path:
    code = src / "csplit"
    sources = [str(code / name) for name in SOURCES]
    output = work / f"csplit{EXE}"
    for compiler in ("cc", "gcc", "clang"):
        if shutil.which(compiler):
            run([compiler, "-O2", *[f"-D{d}" for d in DEFINES], *sources,
                 "-o", str(output), "-lm"])
            return output
    if os.name == "nt" and shutil.which("cl"):
        run(["cl", "/nologo", "/O2", *[f"/D{d}" for d in DEFINES], *sources,
             f"/Fe{output}", f"/Fo{work}\\"], cwd=work)
        return output
    sys.exit(
        "Cannot build csplit: install CMake or a C compiler (cc, gcc, clang, "
        "or MSVC cl from a developer prompt), or pass --csplit to configure.py"
    )


def install(binary: Path, output: Path) -> None:
    """Replace output with binary.

    Windows can refuse to overwrite or delete an executable (for example
    while a scanner holds it) yet still allow renaming it, so a refused
    replace moves the old file aside first.
    """
    staged = output.with_name(output.name + ".new")
    shutil.copyfile(binary, staged)
    try:
        os.replace(staged, output)
        return
    except PermissionError:
        if not output.exists():
            raise
    aside = Path(tempfile.mktemp(prefix=output.name + ".old-", dir=output.parent))
    os.rename(output, aside)
    os.replace(staged, output)
    try:
        aside.unlink()
    except OSError:
        pass  # still locked; build/ is disposable


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("output", type=Path, help="output binary path")
    parser.add_argument("--commit", required=True, help="csplit commit SHA")
    args = parser.parse_args()

    output: Path = args.output
    output.parent.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="csplit-", dir=output.parent))
    try:
        src = work / "src"
        checkout(args.commit, src)
        if shutil.which("cmake"):
            binary = build_with_cmake(src, work)
        else:
            binary = build_with_compiler(src, work)
        install(binary, output)
    finally:
        remove_tree(work)
    st = os.stat(output)
    os.chmod(output, st.st_mode | stat.S_IEXEC)
    print(f"Built csplit {args.commit} at {output}")


if __name__ == "__main__":
    main()
