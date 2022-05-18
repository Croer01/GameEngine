from conans import ConanFile, CMake, tools


# conan export . adria/testing
class GameEngineConan(ConanFile):
    name = "game-engine"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    generators = "cmake"
    exports_sources = "engine*", "editor*", "test*", "tools*", "CMakeLists.txt"
    requires = [
            "boost/1.69.0",
            "box2d/2.3.1",
            "freetype/2.10.1",
            "glew/2.1.0",
            "glm/0.9.9.8",
            "imgui/1.74",
            "libsndfile/1.0.29",
            "openal/1.19.1",
            "sdl2_image/2.0.4@adria/stable",
            "sdl2/2.0.12@bincrafters/stable",
            "tinyfiledialogs/3.3.7@adria/stable",
            "utfcpp/3.1",
            "yaml-cpp/0.6.3",
            "zlib/1.2.11",
            "libpng/1.6.37"
            ]
    build_requires =[
            "cmake/3.19.3",
            "gtest/1.8.1",
    ]
    default_options = {
            "shared": False,
            "fPIC": True,
            "boost:without_atomic": True,
            "boost:without_chrono": True,
            "boost:without_container": True,
            "boost:without_context": True,
            "boost:without_contract": True,
            "boost:without_coroutine": True,
            "boost:without_date_time": True,
            "boost:without_exception": True,
            "boost:without_fiber": True,
            "boost:without_filesystem": False,
            "boost:without_graph": True,
            "boost:without_graph_parallel": True,
            "boost:without_iostreams": True,
            "boost:without_locale": True,
            "boost:without_log": True,
            "boost:without_math": True,
            "boost:without_mpi": True,
            "boost:without_program_options": True,
            "boost:without_python": True,
            "boost:without_random": True,
            "boost:without_regex": True,
            "boost:without_serialization": True,
            "boost:without_stacktrace": True,
            "boost:without_system": False,
            "boost:without_test": True,
            "boost:without_thread": True,
            "boost:without_timer": True,
            "boost:without_type_erasure": True,
            "boost:without_wave": True,
            "gtest:build_gmock": False
            }

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin


    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp", dst="include", src="engine/include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("CMakeLists.txt", dst="tools", src="tools")
        self.copy("*.py", dst="tools", src="tools")

    def package_info(self):
        self.cpp_info.libs = ["GameEngine"]
