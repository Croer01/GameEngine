workspace(name = "game_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# get windows cmake to use in cygwin
_all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""
http_archive(
    name = "cmake_windows",
    build_file_content = _all_content,
    strip_prefix = "cmake-3.16.1-win64-x64",
    urls = [
        "https://github.com/Kitware/CMake/releases/download/v3.16.1/cmake-3.16.1-win64-x64.zip",
    ],
    sha256 = "dd00f6e7d7ee7dc5d7f17b915181c6df8026946c82d7e03b0082f10a09f7d44a",
)

# configure cmake support from https://github.com/bazelbuild/rules_foreign_cc
# TODO: change to the official repo until fix the persmission issues with bash's ln command
git_repository(
    name = "rules_foreign_cc",
#    remote = "https://github.com/schoppmp/rules_foreign_cc.git",
    remote = "https://github.com/bazelbuild/rules_foreign_cc.git",
    branch = "master",
#    commit = "ce9a2d085f8dde45e5dae9f17da33c3a2d5a6682",
#    shallow_since = "1571848616 +0200"
)
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies([
    "@openal-soft//:built_windows_cmake_toolchain",
    "@openal-soft//:built_windows_ninja_toolchain"
])

http_archive(
    name = "SDL2",
    url = "https://www.libsdl.org/release/SDL2-2.0.9.zip",
    sha256 = "7d21dd8226dec33365775144e21884016154af27dc4adbc117504e6cee63a0d7",
    build_file = "@//:bazel_ports/BUILD.sdl2",
    strip_prefix = "SDL2-2.0.9"
)

http_archive(
    name = "SDL2_image",
    url = "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.2.zip",
    sha256 = "7c3158ca1bf84ef041bcf55ea237306684caf6b70f4303ee7e1f0777bfba55a2",
    build_file = "@//:bazel_ports/BUILD.sdl_image",
    strip_prefix = "SDL2_image-2.0.2"
)

#TODO: choose the correct zip by platform
http_archive(
    name = "glew",
    url = "https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.zip",
    sha256 = "2700383d4de2455f06114fbaf872684f15529d4bdc5cdea69b5fb0e9aa7763f1",
    build_file = "@//:bazel_ports/BUILD.glew",
    strip_prefix = "glew-2.1.0"
)

http_archive(
    name = "glm",
    url = "https://github.com/g-truc/glm/archive/0.9.9.0.tar.gz",
    sha256 = "514dea9ac0099dc389cf293cf1ab3d97aff080abad55bf79d4ab7ff6895ee69c",
    strip_prefix = "glm-0.9.9.0",
    build_file = "@//:bazel_ports/BUILD.glm"
)


# TODO: set the correct version. Currently use one commit from master because
# there aren't any release has the Bazel Build yet.
git_repository(
    name = "yaml-cpp",
    remote = "https://github.com/jbeder/yaml-cpp.git",
    commit = "587b24e2eedea1afa21d79419008ca5f7bda3bf4",
    # config suggested by bazel
    shallow_since = "1575068541 -0600"
)


# version 2.3.1-374664b-1
new_git_repository(
    name = "box2d",
    remote = "https://github.com/erincatto/Box2D.git",
    commit = "374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc",
    # config suggested by bazel
    build_file = "@//:bazel_ports/BUILD.box2d",
    strip_prefix = "Box2D"
)

# version v3.1
new_git_repository(
    name = "utf8-cpp",
    remote = "https://github.com/nemtrif/utfcpp.git",
    commit = "82c674b7a7aff3f2845e8eb17d0d9f790b629799",
    shallow_since = "1556315357 -0400",
    # config suggested by bazel
    build_file = "@//:bazel_ports/BUILD.utf8cpp"
)


# version 1.19.1
new_git_repository(
    name = "openal-soft",
    remote = "https://github.com/kcat/openal-soft.git",
    commit = "6761218e51699f46bf25c377e65b3e9ea5e434b9",
    shallow_since = "1539295531 -0700",
    build_file = "@//:bazel_ports/BUILD.openal"
)
