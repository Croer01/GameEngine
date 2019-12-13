workspace(name = "game_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "SDL2",
    url = "https://www.libsdl.org/release/SDL2-2.0.9.zip",
    sha256 = "7d21dd8226dec33365775144e21884016154af27dc4adbc117504e6cee63a0d7",
    build_file = "@//:bazel-ports/BUILD-SDL2.bazel"
)

http_archive(
    name = "SDL2_image",
    url = "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.2.zip",
    sha256 = "7c3158ca1bf84ef041bcf55ea237306684caf6b70f4303ee7e1f0777bfba55a2",
    build_file = "@//:bazel-ports/BUILD-SDL2_Image.bazel"
)

# version 2.1.0
new_git_repository(
    name = "glew",
    remote = "https://github.com/nigels-com/glew.git",
    commit = "3a8eff77da3658c13fbd3634c943d5251d76322c",
    # config suggested by bazel
    shallow_since = "1501499511 +1000",
    init_submodules = 0,
    build_file = "@//:bazel-ports/BUILD-glew.bazel"
)

http_archive(
    name = "glm",
    url = "https://github.com/g-truc/glm/archive/0.9.9.0.tar.gz",
    sha256 = "514dea9ac0099dc389cf293cf1ab3d97aff080abad55bf79d4ab7ff6895ee69c",
    strip_prefix = "glm-0.9.9.0",
    build_file = "@//:bazel-ports/BUILD-glm.bazel"
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
    build_file = "@//:bazel-ports/BUILD-box2d.bazel"
)

# version v3.1
new_git_repository(
    name = "utf8-cpp",
    remote = "https://github.com/nemtrif/utfcpp.git",
    commit = "82c674b7a7aff3f2845e8eb17d0d9f790b629799",
    shallow_since = "1556315357 -0400",
    # config suggested by bazel
    build_file = "@//:bazel-ports/BUILD-utf8-cpp.bazel"
)
