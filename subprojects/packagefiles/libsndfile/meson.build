project('libsndfile', 'cpp', version: '1.1.0')

fs = import('fs')

if fs.exists('bin' / 'sndfile.dll')
  message('directory exists')
else
  error('directory not exists')
endif

#TODO: check this https://github.com/mesonbuild/meson/issues/1546
compiler = meson.get_compiler('cpp')
libsndfile_lib = compiler.find_library(
  'sndfile',
  dirs : [
    meson.current_source_dir() / 'lib',
    meson.current_source_dir() / 'bin'
  ],
  required: true
)

devenv = environment()
devenv.append('PATH', meson.current_source_dir() / 'bin')
meson.add_devenv(devenv)

libsndfile_dep = declare_dependency(
  dependencies : libsndfile_lib,
  include_directories : include_directories('include')
)

install_data(
  files([ 'bin' / 'sndfile.dll']),
  install_dir: 'bin'
)