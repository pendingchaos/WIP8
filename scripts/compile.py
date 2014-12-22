import glob
import os
import fnmatch
import shutil
import subprocess
import shlex
import optparse
import sys

parser = optparse.OptionParser()
parser.add_option("-t", "--target", dest="target", default="release",
                  help="The compile target. can be release or debug.")
parser.add_option("-b", "--bits", dest="bits", default="64",
                  help="Whenever to compile a 32 bit or 64 bit program.\
                  Can be 32 or 64.")
parser.add_option("-q", "--quiet", dest="quiet", default="true",
                  help="Don't print the command being run to stdout.")

options = parser.parse_args()[0]

if options.target == "release":
    debug = False
elif options.target == "debug":
    debug = True
else:
    print "Unknown target: \"%s\"" % (options.target)
    sys.exit()

if not options.bits in ["32", "64"]:
    print "Unknown bits: \"%s\"" % (options.bits)
    sys.exit()

bits = int(options.bits)

if not options.quiet in ["true", "false"]:
    print "Quiet argument not valid."
    sys.exit()
    
quiet = True if options.quiet == "true" else False

files = []

for root, dirnames, filenames in os.walk("../src"):
    for filename in fnmatch.filter(filenames, "*.cpp"):
        files.append(os.path.join(root, filename))

defines = {"GL_GLEXT_PROTOTYPES":None}
obj_output = "../obj"

if os.path.isdir(obj_output):
    shutil.rmtree(obj_output)

if bits == 64:
    bin_output = "../bin/WIP8-64"
elif bits == 32:
    bin_output = "../bin/WIP8-32"

if debug:
    bin_output += "-debug"
else:
    bin_output += "-release"

if debug:
    defines["TRACK_MEMORY"] = None
else:
    defines["NDEBUG"] = None

libaries = ["c", "m", "stdc++", "dl", "jsoncpp", "GL", "SOIL", "SDL2"]

def compile_cpp(filename):
    output_file = os.path.abspath(obj_output + filename[2:])
    output_file = os.path.splitext(output_file)[0] + ".o"
    output_dir = os.path.split(output_file)[0]
    
    command = "g++ -std=c++11 -Wall -fexceptions "
    
    for name, value in defines.items():
        if value == None:
            command += "-D%s " % (name)
        else:
            command += "-D%s=%s " % (name, value)
    
    if debug:
        command += "-g "
    else:
        command += "-O2 "
    
    if bits == 64:
        command += "-m64 "
    elif bits == 32:
        command += "-m32 "
    
    command += "-I../include "
    
    command += "-c %s " % (os.path.abspath(filename))
    
    command += "-o %s" % (output_file)

    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)
    
    if not quiet:
        print command
    
    return (subprocess.Popen(shlex.split(command)), output_file)

def link(files):
    command = "g++ "
    
    if bits == 64:
        command += "-m64 "
    elif bits == 32:
        command += "-m32 "
    
    if bits == 64:
        command += "-L/usr/lib64 "
    elif bits == 32:
        command += "-L/usr/lib "
    
    command += "-o %s " % (bin_output)
    
    for file in files:
        command += file + " "
    
    command += "-rdynamic "
    
    for library in libaries:
        command += "-l%s " % (library)
    
    if not quiet:
        print command
    
    os.system(command)

output_files = []
processes = []

for filename in files:
    process, output_file = compile_cpp(filename)
    output_files.append(output_file)
    processes.append(process)

for process in processes:
    process.wait()

link(output_files)
