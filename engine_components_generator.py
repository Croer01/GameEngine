import os
import glob
import re

# global vars
includes = []
registers = []

for filename in glob.iglob("./**/*.hpp", recursive=True):
    for line in open(filename, "r"):
        match = re.search("^\s*COMPONENT\(([^\)]+)\)", line)
        if match:
            includes.append("#include \"" + os.path.basename(filename) + "\"")
            registers.append("\tObjectManager::GetInstance().registerComponentBuilder(\"" + match.group(1) + "\", new ComponentTBuilder<" + match.group(1) + ">());")
            break

print("#include \"src/ObjectManager.hpp\"")
print(*includes, sep="\n")
print("")
print("/*")
print(" * This file has been generated. To register a component use macro COMPONENT(NameOfComponentClass)")
print(" * in the definition of the class")
print(" */")
print("void RegisterComponents() {")
print(*registers, sep="\n")
print("}")