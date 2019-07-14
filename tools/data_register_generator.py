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
    prototypeName = prototypeName.capitalize().replace(' ', '')
    if os.name == 'nt':
        filename = filename.replace('\\', "/")

    if not "scene" in prototypeName.lower():
        prototypeRegisters.append("\tenv->addPrototype(\"" + prototypeName + "\", \"" + filename + "\");")
    else:
        prototypeName = prototypeName.replace("scene", "")
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