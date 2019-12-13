workspace(name = "game_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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
