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
both_libraries(project_name, include_directories: 'include', sources: sources)



