#!/bin/bash

# Precondition: this file is always in the game engine's root directory
# Ensure the script always is work from the game engine root
cd $(dirname $(realpath $0 ))

INSTALL_DIR=conan-env

function exitIfError() {
  if [[ $1 -ne 0 ]]; then
    echo "ERROR: $2"
    exit $1
  fi
}

conan install cmake/3.19.3@ -g=virtualenv --install-folder $INSTALL_DIR -pr windows-profile
exitIfError $? "environment installation has failed"

GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo -e "${GREEN}Development environment have been installed succesfully.${NC}"
echo "All platforms: \"source $INSTALL_DIR/activate.sh\" and \"source $INSTALL_DIR/deactivate.sh\""
echo "   Powershell: \"$INSTALL_DIR\\activate.ps1\" and \"$INSTALL_DIR\\deactivate.ps1\""
echo "      Windows: \"$INSTALL_DIR\\activate.bat\" and \"$INSTALL_DIR\\deactivate.bat\""

# To use this env:
# source activate.sh
# Windows: activate.bat without the source
# more info in https://docs.conan.io/en/latest/mastering/virtualenv.html
