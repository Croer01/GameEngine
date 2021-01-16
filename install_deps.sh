#!/bin/bash

# Precondition: this file is always in the game engine's root directory
# Ensure the script always is work from the game engine root
cd $(dirname $(realpath $0 ))

BUILD_DIR=build
CONAN_USER=adria
CONAN_CHANNEL=stable

function exitIfError() {
  if [[ $1 -ne 0 ]]; then
    echo "ERROR: $2"
    exit $1
  fi
}

# Prepare the game engine custom recipies
conan export conan-deps/tinyfiledialogs $CONAN_USER/$CONAN_CHANNEL
exitIfError $? "conan export has failed"
conan export conan-deps/sdl2-image $CONAN_USER/$CONAN_CHANNEL
exitIfError $? "conan export has failed"

# Install the dependencies to use in CMake and build project
conan install . --install-folder $BUILD_DIR -s build_type=Debug --build=missing -pr windows-profile
exitIfError $? "conan depencies installation has failed"


GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo -e "${GREEN}Conan packages have been installed succesfully.${NC}"
