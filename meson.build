project('stdc++2', 'cpp')
project_name = 'stdc++2'

files_ls = run_command('ls', 'src', check: false).stdout().split()
sources = []
foreach file : files_ls
  sources += ['src/' + file]
endforeach
message(sources)

debug()
add_global_link_arguments('-nostdinc++', '-nostdlib++', '-Wno-unused-function', language: 'cpp')
libraries = both_libraries(project_name, include_directories: 'include', sources: sources)

tests_ls = run_command('ls', 'tests', check: false).stdout().split()
message(tests_ls)
foreach test_file : tests_ls
  test_name = test_file.split('.cpp')[0]
  test_out = executable(test_name, include_directories: 'include', sources: ['tests/' + test_file, './test.cpp'], link_with: libraries)
  test(test_name + ' test', test_out)
endforeach

binaries_ls = run_command('ls', 'bin', check: false).stdout().split()
foreach binary : binaries_ls
  executable(binary.split('.cpp')[0], include_directories: 'include', sources: ['bin/' + binary ], link_with: libraries, install_rpath: 'bin')
endforeach
