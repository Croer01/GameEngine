project('GameEngine', 'cpp',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++17',
    # set here the default_library options because some dependencies (sdl2 my case) ignore it if
    # this option is set by dependency(...). Workaround from https://github.com/mesonbuild/meson/issues/6728#issuecomment-595749400
    'box2d:default_library=shared',
    'openal:default_library=shared',
    'sdl2_image:default_library=static',
    'sdl2:default_library=static',
    'yamlcpp:default_library=static',
  ])


host_system = host_machine.system()
is_windows = host_system == 'windows'
is_debug = get_option('buildtype') == 'debug'

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = []

if is_windows
  lib_args += ['/DGAMEENGINE_EXPORTS']
  if is_debug
    lib_args += ['/DDEBUG']
  endif
else
  lib_args += ['-DGAMEENGINE_EXPORTS']
  if is_debug
    lib_args += ['-DDEBUG']
  endif
endif



if is_windows
  libsndfile_dep = dependency('libsndfile-win64', version : '1.1.0', default_options: ['default_library=shared'])
else
  libsndfile_dep = dependency('sndfile', default_options: ['default_library=shared'])
endif
thread_dep = dependency('threads')
box2d_dep = dependency('box2d', version : '2.4.1')
freetype2_dep = dependency('freetype2', version : '24.2.18')# aka v2.12.0
glew_dep = dependency('glew', version : '2.1.0')
glm_dep = dependency('glm', version : '0.9.9.8')
libpng_dep = dependency('libpng', version : '1.6.37')
libjpeg_dep = dependency('libjpeg', version : '2.1.2')
if is_windows
  openal_dep = dependency('openal', version : '1.21.0', default_options: ['default_library=shared'])
else
  openal_dep = dependency('openal', default_options: ['default_library=shared'])
endif
sdl2_image_dep = dependency('sdl2_image', version : '2.0.5', default_options: ['default_library=static'])
sdl2_dep = dependency('sdl2', version : '2.0.18', default_options: ['default_library=static'], static: true)
utfcpp_dep = dependency('utfcpp', version : '2.3.5', fallback : ['utfcpp' , 'utfcpp_dep'])
yaml_dep = dependency('yamlcpp', version : '0.6.3', default_options: ['default_library=static'])
zlib_dep = dependency('zlib', version : '1.2.11')

deps = [
  thread_dep,
  box2d_dep,
  freetype2_dep,
  glew_dep,
  glm_dep,
  libpng_dep,
  libjpeg_dep,
  openal_dep,
  sdl2_image_dep,
  sdl2_dep,
  utfcpp_dep,
  yaml_dep,
  zlib_dep,
  libsndfile_dep
]

gtest_proj = subproject('gtest', version : '1.8.1')
gtest_dep = [
  # gtest with main
  gtest_proj.get_variable('gtest_main_dep'),
  gtest_proj.get_variable('gmock_dep')
]

subdir('engine')

# Make this library usable as a Meson subproject.
gameengine_dep = declare_dependency(
  include_directories: gameengine_inc,
  dependencies: deps,
  link_with : gameengine_lib)

subdir('test')

subdir('editorV2')

# Make this library usable from the system's
# package manager.
#install_headers('gameengine.hpp', subdir : 'GameEngine')

#pkg_mod = import('pkgconfig')
#pkg_mod.generate(
#  name : 'GameEngine',
#  filebase : 'GameEngine',
#  description : 'Game Engine by Adrià.',
#  subdirs : 'GameEngine',
#  libraries : gameengine_lib,
#  version : '0.0.1',
#)
