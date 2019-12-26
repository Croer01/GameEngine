#Ensure Bazel use cmake and ninja from VS tools instead of Cygwin
BAZEL_VS_WINDOWS=$(cygpath -w "$BAZEL_VS")
PATH="${BAZEL_VS_WINDOWS}\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin:${BAZEL_VS_WINDOWS}\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\Ninja:$PATH"

# trick to execute any command with VS tools loaded
# based on https://stackoverflow.com/a/374411/6952678
TMP_FILE="$TMP/tmp$$.bat"
echo "@echo off" > $TMP_FILE
echo "call \"$BAZEL_VS_WINDOWS\\VC\\Auxiliary\\Build\\vcvars64.bat\" >NUL:" >> $TMP_FILE
echo "bash -c ${@//\\//}" >> $TMP_FILE
cmd /c `cygpath -m "$TMP_FILE"`
status=$?
rm -f $TMP_FILE
exit $status
