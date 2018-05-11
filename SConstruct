import os
import glob

debug = ARGUMENTS.get('debug',0)
compiler = ARGUMENTS.get('compiler','g++')

linkflags = ''
if compiler=='g++':
    cflags = ' -Wfatal-errors '
    if int(debug):
        cflags += ' -Og -g -pg '
        linkflags = ' -g -pg '
    else:
        cflags += ' -O3 '
elif compiler=='clang++':
    cflags = '-Weverything Wno-unused-variable -Werror -ferror-limit=1 -Wno-error=padded'
    if int(debug):
        cflags += ' -O0 -g -pg'
        linkflags = ' -g -pg'
    else:
        cflags += ' -O3 -march=native'
else:
    raise NameError('unsupported compiler')

build_dir = 'build/'+compiler
if int(debug):
    build_dir += '/debug'
else:
    build_dir += '/release'

env = Environment(ENV = os.environ,
                  CXX=compiler,
                  CPPPATH=[os.environ['RICH_ROOT']+'/source',
                           os.environ['RICH_ROOT']],
                  LIBPATH=[os.environ['RICH_ROOT']+'/'+build_dir,
			'.',os.environ['HDF5_LIB_PATH']],
                  LIBS=['rich','hdf5','hdf5_cpp'],
                  LINKFLAGS=linkflags,
                  CXXFLAGS=cflags)
env.VariantDir(build_dir,'source')                  
env.Program(build_dir+'/rich',
            Glob('source/*.cpp'))
