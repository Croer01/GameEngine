tests = [
    'GameObject',
    'Game',
    'Properties',
    'Math',
    'Pathfinding',
]

foreach t : tests
    exe_name = t + 'Tests'
    test_exe = executable(exe_name,
        exe_name +'.cpp',
        dependencies: [gtest_dep, deps],
        include_directories: gameengine_inc,
        link_with: [gameengine_lib],
        cpp_args : lib_args
    )
    test(
        t,
        test_exe,
        env: libsndfile_env,
        protocol: 'gtest',
        workdir: meson.current_build_dir()
    )
endforeach

subdir('data')