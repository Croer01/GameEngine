fs = import('fs')

data_files = [
    'main.scene',
    'mainUI.prototype',
    'fonts/OpenSans/OpenSans-Regular.ttf',
    'fonts/OpenSans/LICENSE.txt',
]

cp = find_program('cp', files('C:/cygwin64/bin/cp.exe'), required : true)

foreach file : data_files
  output_file = meson.current_build_dir() + '/' + file
  output_dir = fs.parent(output_file)
  if not fs.is_samepath(meson.current_build_dir(), output_dir)
    if is_windows
      # Disable check on Windows because it returns error if directory already exists
      # and there is not an option like POSIX to avoid this.
      run_command(['cmd', '/C', 'mkdir', output_dir], check: false)
    else
      run_command(['mkdir', '-p', output_dir], check: true)
    endif

  endif
  game_object_editor_srcs += custom_target('copy-editor-data_' + fs.name(file),
    command: [
      cp,
      '@INPUT@',
      output_file
    ],
    input: meson.current_source_dir() / file,
    # the out file is just to have a dummy file to allow meson check file's timestamp
    output: fs.name(file) + '.out'
  )
endforeach

install_headers(data_files, preserve_path: true)
