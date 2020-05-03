import os
import glob
import re
from pathlib import Path

# global vars
dataFolder = "data"
prototypeRegisters = []
sceneRegisters = []

# get all the supported data files recursively
dataFolderPath = Path(dataFolder)
dataFiles = []
dataFiles.extend(dataFolderPath.rglob("*.scene"))
dataFiles.extend(dataFolderPath.rglob("*.prototype"))

for filename in dataFiles:
    dataObjectName = filename.stem

    if ".scene" == filename.suffix:
        sceneRegisters.append("\tenv->addScene(\"" + dataObjectName + "\", \"" + filename.as_posix() + "\");")
    elif ".prototype" == filename.suffix:
        prototypeRegisters.append("\tenv->addPrototype(\"" + dataObjectName + "\", \"" + filename.as_posix() + "\");")
    else:
        raise ValueError("unkown file format " + filename.suffix)

print("#ifndef DATA_REGISTERS_GENERATED_HPP")
print("#define DATA_REGISTERS_GENERATED_HPP")
print("#include <game-engine/geEnvironment.hpp>")
print("")
print("/*")
print(" * This file has been generated based in the content of data folder")
print(" * data folder: " + str(dataFolderPath))
print(" * cwd: " + str(os.getcwd()))
print(" */")
print("void inline RegisterData(const GameEngine::geEnvironmentRef &env) {")
print(*prototypeRegisters, sep="\n")
print(*sceneRegisters, sep="\n")
print("};")
print("#endif")
