# ======================================= #
# ============ TicoKit Build ============ #
# ======================================= #

from colorama import just_fix_windows_console, Fore, Style
from io import StringIO
import os
import sys
import subprocess
import ninja_syntax
from pathlib import Path

just_fix_windows_console()

# ================================= #
# ============ Utility ============ #
# ================================= #
REGION = ["USA", "PAL", "JPN", "KOR"]
LETTERS = ['E', 'P', 'J', 'K']

def error(msg: str):
    print(Fore.RED + msg + Style.RESET_ALL)
    sys.exit(1)

def validate_dir(path: Path, create: bool = False) -> Path:
    if os.path.isdir(path):
        return path
    
    if create:
        os.makedirs(path)
        return path
    
    error(f"The folder '{path}' does not exist.")

def validate_region(region: str) -> str:
    if region in REGION:
        return region
    
    if region == "JPN":
        error(f"JPN builds are not currently supported.")
    
    error(f"The region '{region}' is invalid.")

# ================================= #
# =========== Arguments =========== #
# ================================= #
if len(sys.argv) < 3:
    error("Not enough arguments provided!\nUsage:\n  py build.py [Region] [Path_To_Petari] <Output>")

REGION = validate_region(sys.argv[1])
PETARI_PATH = validate_dir(sys.argv[2])
validate_dir(f"{PETARI_PATH}/include")
validate_dir(f"{PETARI_PATH}/libs/RVL_SDK/include")
validate_dir(f"{PETARI_PATH}/libs/MetroTRK/include")
validate_dir(f"{PETARI_PATH}/libs/Runtime/include")
validate_dir(f"{PETARI_PATH}/libs/MSL_C/include")
validate_dir(f"{PETARI_PATH}/libs/RVLFaceLib/include")
validate_dir(f"{PETARI_PATH}/libs/JSystem/include")
validate_dir(f"{PETARI_PATH}/libs/nw4r/include")

DEPS_PATH    = validate_dir("deps/")
INCLUDE_PATH = validate_dir("include/")
LOADER_PATH  = validate_dir("loader/")
SYMBOLS_PATH = validate_dir("symbols/")
PROJECT_PATH = validate_dir("main/")
PROJECT_INCLUDE_PATH = validate_dir(os.path.join(PROJECT_PATH, "include/"))
PROJECT_SOURCE_PATH  = validate_dir(os.path.join(PROJECT_PATH, "source/"))
PROJECT_OUTPUT_PATH  = validate_dir(("disc/CustomCode/" if len(sys.argv) < 4 else sys.argv[3]), True)

# ================================= #
# ============ Options ============ #
# ================================= #
CFLAGS = [
    "-c",
    "-Cpp_exceptions off",
    "-nodefaults",
    "-proc gekko",
    "-fp hard",
    "-lang=c++",
    "-O2",
    "-inline on",
    "-rtti off",
    "-sdata 0",
    "-sdata2 0",
    "-align powerpc",
    "-func_align 4",
    "-str pool",
    "-enum int",
    "-DMTX_USE_PS",
    f"-D{REGION}",
    "-I-", # Allow #include <...>
    f"-i \"$petari_path\"",
    f"-i \"$game_header_path\"",
    f"-i \"$rvl_sdk_path\"",
    f"-i \"$trk_path\"",
    f"-i \"$runtime_path\"",
    f"-i \"$msl_c_path\"",
    f"-i \"$facelib_path\"",
    f"-i \"$jsystem_path\"",
    f"-i \"$nw4r_path\"",
    f"-i \"{INCLUDE_PATH}/\"",
    f"-i \"{PROJECT_INCLUDE_PATH}/\""
]

CPP_IGNORE = []

# ============================== #
# ============ Code ============ #
# ============================== #
class NinjaVar():
    def __init__(self, name: str, value: str):
        self.name = name
        self.value = value
    
    def write_var(self, writer: ninja_syntax.Writer):
        writer.variable(self.name, self.value)

    def replace_value(self, s: str) -> str:
        return s.replace(self.value, f"${self.name}")

NJVAR_CFLAGS = ' '.join(CFLAGS)
NJVAR_LIST = [
    NinjaVar("petari_path", PETARI_PATH),
    NinjaVar("game_header_path", "$petari_path/include"),
    NinjaVar("rvl_sdk_path", "$petari_path/libs/RVL_SDK/include"),
    NinjaVar("trk_path", "$petari_path/libs/MetroTRK/include"),
    NinjaVar("runtime_path", "$petari_path/libs/Runtime/include"),
    NinjaVar("msl_c_path", "$petari_path/libs/MSL_C/include"),
    NinjaVar("facelib_path", "$petari_path/libs/RVLFaceLib/include"),
    NinjaVar("jsystem_path", "$petari_path/libs/JSystem/include"),
    NinjaVar("nw4r_path", "$petari_path/libs/nw4r/include"),
    NinjaVar("mwcc_path", f"{DEPS_PATH}/CodeWarrior/mwcceppc.exe"),
    NinjaVar("kamek_path", f"{DEPS_PATH}/Kamek/Kamek.exe"),
    NinjaVar("cflags", NJVAR_CFLAGS)
]

CUSTOM_CODE_BIN = f"{PROJECT_OUTPUT_PATH}/CustomCode_{REGION}.bin"
CUSTOM_CODE_MAP = f"{PROJECT_OUTPUT_PATH}/CustomCode_{REGION}.map"
LOADER_XML = f"{PROJECT_OUTPUT_PATH}/TicoLoader_{REGION}.xml"
LOADER_MAP = f"{PROJECT_OUTPUT_PATH}/TicoLoader_{REGION}.map"
LOADER_CPP = f"{LOADER_PATH}/Loader.cpp"
LOADER_O = f"{LOADER_PATH}/Loader.o"
SYMBOL_MAP = f"{SYMBOLS_PATH}/{REGION}.txt"
SYMBOL_EXT_MAP = f"{SYMBOLS_PATH}/{REGION}.ext.txt"

print(Fore.BLUE + f"[BUILD] Region: {REGION}")
print(f"[BUILD] Petari: {PETARI_PATH}")
print(Style.RESET_ALL)

def main():
    out = StringIO()
    n = ninja_syntax.Writer(out)

    ninja_write_vars(n)
    ninja_write_rules(n)
    ninja_write_builds(n)

    with open("build.ninja", "w", encoding="utf-8") as f:
        f.write(out.getvalue())
    
    out.close()
    result = subprocess.run("ninja")
    
    print()

    if result.returncode != 0:
        error("Compilation failed.")

    print(Fore.GREEN + "Compilation successful." + Style.RESET_ALL)

def ninja_write_vars(n: ninja_syntax.Writer):
    n.comment("Variables")
    for var in NJVAR_LIST:
        var.write_var(n)

    n.newline()

def ninja_write_rules(n: ninja_syntax.Writer):
    extern_command = f"-externals=\"{SYMBOL_MAP}\""

    if (os.path.isfile(SYMBOL_EXT_MAP)):
        extern_command += f" -externals=\"{SYMBOL_EXT_MAP}\""

    n.comment("Rules")
    n.rule(
        name="mwcc",
        command=f"$mwcc_path $cflags -MDfile \"$out.d\" -c \"$in\" -o \"$out\"",
        description="MWCC $out",
        depfile="$out.d",
        deps="gcc"
    )
    n.newline()
    n.rule(
        name="kamek",
        command=f"$kamek_path $in {extern_command} -output-kamek=\"{CUSTOM_CODE_BIN}\" -output-map=\"{CUSTOM_CODE_MAP}\"",
        description="Kamek $out"
    )
    n.newline()
    n.rule(
        name="mwcc_loader",
        command=f"$mwcc_path $cflags -i \"{LOADER_PATH}/\" -c \"$in\" -o \"$out\"",
        description="MWCC $out"
    )
    n.newline()
    n.rule(
        name="kamek_loader",
        command=f"$kamek_path $in -static=0x80001800 {extern_command} -output-riiv=\"{LOADER_XML}\" -output-map=\"{LOADER_MAP}\"",
        description="Kamek $out"
    )
    n.newline()

def ninja_write_builds(n: ninja_syntax.Writer):
    n.comment("Code")
    o_files = list()

    for root, dirs, files in os.walk(PROJECT_SOURCE_PATH):
        for file in files:
            if not file.endswith(".cpp"):
                continue
            
            cpp_path = os.path.join(root, file).replace('\\', '/')
            if cpp_path in CPP_IGNORE:
                continue

            o_path = cpp_path.replace("source", "build").replace(".cpp", ".o")
            o_files.append(o_path)

            os.makedirs(os.path.dirname(o_path), exist_ok=True)

            n.build(
                outputs=o_path,
                rule="mwcc",
                inputs=cpp_path
            )

    n.build(
        outputs=[CUSTOM_CODE_BIN, CUSTOM_CODE_MAP],
        rule="kamek",
        inputs=o_files
    )

    n.newline()
    n.comment("Loader")
    n.build(
        outputs=LOADER_O,
        rule="mwcc_loader",
        inputs=LOADER_CPP
    )
    n.build(
        outputs=LOADER_XML,
        rule="kamek_loader",
        inputs=LOADER_O
    )

if (__name__ == "__main__"):
    main()