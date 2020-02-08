import os
import sys
import re

# global vars
includes = []
componentRegisters = []

# pre-condition: all the arguments from command are files
for filename in sys.argv[1:]:
    for line in open(filename, "r"):
        include = None
        # register a component
        match = re.search("^\s*COMPONENT\(([^\)]+)\)", line)
        if match:
            include = "#include \"" + os.path.basename(filename) + "\""
            componentRegisters.append("\tenv->registerComponent(\"" + match.group(1) + "\", new GameEngine::ComponentTBuilder<" + match.group(1) + ">());")

        if include is not None:
            includes.append(include)

print("#ifndef COMPONENT_REGISTERS_GENERATED_HPP")
print("#define COMPONENT_REGISTERS_GENERATED_HPP")
print("#include <game-engine/geEnvironment.hpp>")
print(*includes, sep="\n")
print("")
print("/*")
print(" * This file has been generated. To register a component use macro COMPONENT(NameOfComponentClass)")
print(" * in the definition of the class")
print(" */")
print("void RegisterComponents(GameEngine::geEnvironment *env) {")
print(*componentRegisters, sep="\n")
print("};")
print("#endif")
