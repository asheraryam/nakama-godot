#!/usr/bin/env python
import os, subprocess

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['debug', 'release']))
# opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', "linux"]))
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'demo/addons/nakama-godot/bin/'))
opts.Add(PathVariable('target_name', 'The library name.', 'libnakamagodot', PathVariable.PathAccept))

# Local dependency paths, adapt them to your setup
godot_headers_path = "godot-cpp/godot_headers/"
cpp_bindings_path = "godot-cpp/"
cpp_library = "libgodot-cpp"

nakama_headers_path = "nakama-cpp-sdk/include"
nakama_library_path = "nakama-cpp-sdk/libs/"

# only support 64 at this time..
bits = 64

# Updates the environment with the option variables.
opts.Update(env)

# Process some arguments
if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['p'] != '':
    env['platform'] = env['p']

if env['platform'] == '':
    print("No valid target platform selected.")
    quit();

# Check our platform specifics
# if env['platform'] == "osx":
#     env['target_path'] += 'osx/'
#     cpp_library += '.osx'
#     if env['target'] in ('debug', 'd'):
#         env.Append(CCFLAGS = ['-g','-O2', '-arch', 'x86_64', '-std=c++17'])
#         env.Append(LINKFLAGS = ['-arch', 'x86_64'])
#     else:
#         env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64', '-std=c++17'])
#         env.Append(LINKFLAGS = ['-arch', 'x86_64'])

# elif env['platform'] in ('x11', 'linux'):
#     env['target_path'] += 'x11/'
#     cpp_library += '.linux'
#     if env['target'] in ('debug', 'd'):
#         env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
#     else:
#         env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])
#     nakama_library_path += "linux/"

if env['platform'] == "windows":
    env['target_path'] += 'win64/'
    cpp_library += '.windows'
    nakama_library_path += 'win64/'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV = os.environ)

    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])

    nakama_library_path += "v142/%s/" % env['target'].capitalize()

    if env['target'] in ('debug', 'd'):
        cpp_library += '.debug'
    else:
        cpp_library += '.release'

    cpp_library += '.' + str(bits)


    nlibs = os.listdir(nakama_library_path)
    nlibs = Glob(nakama_library_path + '/*.a')

    lnlibs = [nakama_library_path + x for x in [
        'crypto.lib',
        'ssl.lib',
        'cpprest.lib',
        'nakama-cpp.lib',
        'libprotobuf.lib',
    ]]

    nlibs = [
        'bcrypt',
        'crypt32',
        'httpapi',
        'winhttp',
        'advapi32',
    ]
elif env['platform'] in ('x11', 'linux'):
    env['target_path'] += 'linux/'
    cpp_library += '.linux'
    nakama_library_path += 'linux/x64/'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV = os.environ)

    if env['target'] in ('debug', 'd'):
        cpp_library += '.debug'
    else:
        cpp_library += '.release'

    cpp_library += '.' + str(bits)

    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-fPIC', '-g3', '-Og', '-std=c++11'])
        #env.Append(CXXFLAGS=['-std=c++11'])
    else:
        env.Append(CCFLAGS=['-fPIC', '-g', '-O3', '-std=c++11'])
        #env.Append(CXXFLAGS=['-std=c++11'])
    nlibs = os.listdir(nakama_library_path)
    nlibs = Glob(nakama_library_path + '/*.a')

    lnlibs = [nakama_library_path + x for x in [
        'libcrypto.a',
        'libssl.a',
        'libcpprest.a',
        'libnakama-cpp.a',
        'libprotobuf.a',
    ]]

    # nlibs = [
    #     'bcrypt',
    #     'crypt32',
    #     'httpapi',
    #     'advapi32',
    # ]
else:
    print("Invalid platform.")
    quit()



# make sure our binding library is properly includes
env.Append(CPPPATH=['.', godot_headers_path, cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/', nakama_headers_path])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/', nakama_library_path])

env.Append(LIBS=nlibs)
env.Append(LIBS=[cpp_library])

env.Append(LINKFLAGS=['-Wl,--whole-archive', lnlibs, '-Wl,--no-whole-archive'])

env.Append(CPPDEFINES="NLOGS_ENABLED");

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')

library = env.SharedLibrary(target=env['target_path'] + env['target_name'] , source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
