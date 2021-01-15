from conans import ConanFile, CMake, tools

class TinyfiledialogsConan(ConanFile):
    name = "tinyfiledialogs"
    version = "3.3.7"
    license = "Zlib"
    author = "Adrià Rodríguez"
    description = "Native inputbox password WIN OSX GTK QT CONSOLE VCPKG C# LUA Fortran"
    topics = ("inputbox", "password", "WIN", "OSX", "GTK", "QT", "CONSOLE", "VCPKG", "C#", "LUA", "Fortran")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"
    exports_sources = "CMakeLists.txt"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        # This recipe is based on vcpkg implementation https://github.com/microsoft/vcpkg/blob/ff9e982d26a5041670d700adb3fd26eb5139cb43/ports/tinyfiledialogs/portfile.cmake
        sha = "03d35a86696859128d41f8b967c1ef3e39c980ce"
        tools.download("https://sourceforge.net/p/tinyfiledialogs/code/ci/%s/tree/tinyfiledialogs.h?format=raw" % sha,
                "tinyfiledialogs.h")
        tools.download("https://sourceforge.net/p/tinyfiledialogs/code/ci/%s/tree/tinyfiledialogs.c?format=raw" % sha,
                "tinyfiledialogs.c")

    def build(self):
        with tools.vcvars(self):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["TinyFileDialogs"]

