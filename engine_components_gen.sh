#!/usr/bin/env bash
components=($(grep -RE --include=\*.hpp '^\s*COMPONENT\(' | sed -r 's/([^:]+):.+\(([^\)]+).+/\1;\2/g'))

NEWLINE=$'\n'
INCLUDES=""
REGISTERS=""

OIFS=$IFS
IFS=';'
for var in "${components[@]}"
do
read -ra ADDR <<< "$var"
INCLUDES="$INCLUDES#include \"${ADDR[0]}\"$NEWLINE"
REGISTERS="$REGISTERS ObjectManager::GetInstance().registerComponentBuilder(\"${ADDR[1]}\", new ComponentTBuilder<${ADDR[1]}>());$NEWLINE"
done
IFS=$OIFS

echo "#include \"src/ObjectManager.hpp\""
echo $INCLUDES
echo ""
echo "/*"
echo " * This file has been generated. To register a component use macro COMPONENT(NameOfComponentClass)" 
echo " * in the definition of the class" 
echo " */" 
echo "void RegisterComponents() {"
echo      $REGISTERS
echo "}"


