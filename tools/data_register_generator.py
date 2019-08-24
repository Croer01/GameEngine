import os
import glob
import re
from pathlib import Path

# global vars
dataFolder = "data"
prototypeRegisters = []
sceneRegisters = []

for filename in glob.iglob("data/**/*.yaml", recursive=True):
    prototypeName = Path(filename).stem
    prototypeName = prototypeName[:1].upper() + prototypeName[1:]

    if os.name == 'nt':
        filename = filename.replace('\\', "/")

    if not "Scene" in prototypeName:
        prototypeRegisters.append("\tenv->addPrototype(\"" + prototypeName + "\", \"" + filename + "\");")
    else:
        prototypeName = prototypeName.replace("Scene", "")
        sceneRegisters.append("\tenv->addScene(\"" + prototypeName + "\", \"" + filename + "\");")

print("#ifndef DATA_REGISTERS_GENERATED_HPP")
print("#define DATA_REGISTERS_GENERATED_HPP")
print("#include <game-engine/geEnvironment.hpp>")
print("")
print("/*")
print(" * This file has been generated")
print(" */")
print("void RegisterData(GameEngine::geEnvironment *env) {")
print(*prototypeRegisters, sep="\n")
print(*sceneRegisters, sep="\n")
print("};")
print("#endif")