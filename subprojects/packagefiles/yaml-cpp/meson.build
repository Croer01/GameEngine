# DISCLAIMER: this meson.build is very specific for this project
# and not for a general porpuse
project('yaml-cpp', 'cpp', version: '0.6.3',
        default_options: ['cpp_std=c++17'])

yamlcpp_inc = include_directories('include')

all_sources = []

subdir('src')

yamlcpp_lib = library('yaml-cpp',
  all_sources,
  include_directories: yamlcpp_inc,
  cpp_args: '-DYAML_CPP_NO_CONTRIB'
)

yamlcpp_dep = declare_dependency(
  include_directories: yamlcpp_inc,
  link_with: yamlcpp_lib
)
