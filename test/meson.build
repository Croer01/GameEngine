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
        dependencies: [gtest_dep, gameengine_dep],
    )
    test(
        t,
        test_exe,
        protocol: 'gtest',
        workdir: meson.current_build_dir()
    )
endforeach

subdir('data')