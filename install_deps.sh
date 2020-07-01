#!/usr/bin/env bash

# This script is to use in Cygwin, but it should be easy to port to other Unix platforms

# versions used for this project:
#    - boost-filesystem 1.68.0
#    - box2d            2.3.1-374664b-1
#    - freetype         2.10.1-3
#    - glew             2.1.0-1
#    - glm              0.9.9.3
#    - gtest            1.8.1-1
#    - imgui            1.73-1
#    - libsndfile       1.0.29-6830c42-5
#    - openal-soft      1.19.1
#    - sdl2-image       2.0.2-3
#    - sdl2             2.0.9-1
#    - tinyfiledialogs  3.3.7-1
#    - utfcpp           2.3.5
#    - yaml-cpp         0.6.2-2
#    - zlib             1.2.11-5

function exitIfError() {
  if [[ $1 -ne 0 ]]; then
    echo "ERROR: $2"
    exit $1
  fi
}

# Initialize variables
VCPKG_ROOT=$(cygpath -u "${VCPKG_ROOT:-C:\\vcpkg}")
VCPKG_ROOT_WIN=$(cygpath -w "${VCPKG_ROOT}")

# install vcpkg if not found
if [[ ! -d "$VCPKG_ROOT" ]]; then
  mkdir -p "$VCPKG_ROOT"

  git clone git@github.com:microsoft/vcpkg.git "$VCPKG_ROOT"
  exitIfError $? "vcpkg clone failed"
  pushd "$VCPKG_ROOT" || exit 1
  cmd -C .\bootstrap-vcpkg.bat
  exitIfError $? "vcpkg bootstrap failed"
  popd || exit 1
fi

# checkout some ports directories we need to install correct versions of the dependencies
pushd "$VCPKG_ROOT" || exit 1
git fetch
echo "-- get ports versions --"
git checkout ff9e982d ports/boost* \
                         ports/box2d \
                         ports/glew \
                         ports/glm \
                         ports/gtest \
                         ports/libsndfile \
                         ports/openal-soft \
                         ports/sdl2-image \
                         ports/sdl2 \
                         ports/tinyfiledialogs \
                         ports/utfcpp \
                         ports/yaml-cpp \
                         ports/zlib

git checkout 2019.12 ports/imgui
popd || exit 1

# workaround to force vcpkg use its own cmake. Related issue https://github.com/microsoft/vcpkg/issues/9109
PATH=$(echo "$PATH" | sed -e 's/\/usr\/bin:$//')
cmd /C "vcpkg install --triplet x64-windows glew glm sdl2-image sdl2 yaml-cpp box2d freetype openal-soft libsndfile gtest utfcpp imgui tinyfiledialogs boost-filesystem zlib"
# maybe some packages are already installed so we upgrade it to ensure use the correct version
cmd /C "vcpkg upgrade --no-dry-run"

echo ""
echo "-- All dependencies installed correctly --"
echo "   Use this vcpkg cmake toolchain to be able to use these dependecies = \"${VCPKG_ROOT_WIN}\\scripts\\buildsystems\\vcpkg.cmake\""
echo ""
echo "   e.g.: inside project root"
echo "     mkdir outr && cd outr"
echo "     cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT_WIN}\\scripts\\buildsystems\\vcpkg.cmake"
echo "     cmake --build ."

exit 0
