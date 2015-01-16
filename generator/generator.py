#!/usr/bin/python2.7
import os
import code_utils
import config_completer
import model_defines_h_generator

import yaml
from  optparse import OptionParser
import shutil
import time

# Executed script location
generatorDir = os.path.dirname(os.path.abspath(__file__))

# Parsing command line
cmdLineParser = OptionParser()
cmdLineParser.add_option("-d", "--directory", dest="directory", default="./test",
                  help="Directory whewre to put generated program", metavar="DIRECTORY")
cmdLineParser.add_option("-c", "--config", dest="config", default="configs/1d.yml",
                  help="Configuration file", metavar="CONFIG")

(cmdLineOptions, cmdLineArgs) = cmdLineParser.parse_args()
configFileName = cmdLineOptions.config
destinationDir = cmdLineOptions.directory

# Beginning work
print ""
print "Code generation started"


config = yaml.load(open(configFileName))
sourceLibraryDir = os.path.join(generatorDir, 'indiferent/')

generatedFileHeadComment = """ /*
 * This file is generated by script using """ + configFileName + ', ' + time.strftime("%H:%M:%S %d/%m/%Y") + """
 */
"""
generatedFileHeadCommentForMake = """#
# This file is generated by script using """ + configFileName + ', ' + time.strftime("%H:%M:%S %d/%m/%Y") + """
#
"""

#
# Copying files that should not be modified
#

print "Creating directories..."

destinationLibraryDir = os.path.join(destinationDir, "fvm")
if not os.path.exists(destinationLibraryDir):
    os.makedirs(destinationLibraryDir)

print ""
print "Copying FVM library files"
filesAsIs = [
            'axis.cpp',
            'axis.h',
            'threads-pool.h',
            'fraction-base.h',
            'fraction-space-base.h',
            'fraction-cell-base.h',
            'fractions-pool-base.h',
            'model-base.h',
            'space-base.h',
            'interfaces.h',
            'global-defines.h',
            'grid-template.h',
            'descriptions.h',
            'output.h',
            'output.cpp'
            ]

for fileName in filesAsIs:
    target = os.path.join(sourceLibraryDir, fileName)
    print "Copying " + target + " to " + destinationLibraryDir
    shutil.copy(target, destinationLibraryDir)

config_completer.completeConfig(config)

#
# Generating code
#
print ""
print "Generating model-defines.h"
model_defines_h_generator.generate(destinationDir, config, generatedFileHeadComment)

print "Generating fraction-related headers and sources:"
for fractionId in config['model']['fractions']:
    fraction = config['model']['fractions'][fractionId]
    print "   " + fractionId + " header..."
    code_utils.genFileByTemplate(os.path.join(destinationDir, fraction['header_name']),
                                os.path.join(generatorDir, 'templates/fraction-header.h.template'),
                                fraction,
                                generatedFileHeadComment)
    print "   " + fractionId + " source..."
    code_utils.genFileByTemplate(os.path.join(destinationDir, fraction['cpp_name']),
                                os.path.join(generatorDir, 'templates/fraction.cpp.template'),
                                fraction,
                                generatedFileHeadComment)

print "Generating constants.h"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'constants.h'),
                                os.path.join(generatorDir, 'templates/constants.h.template'),
                                config['model'],
                                generatedFileHeadComment)


print "Generating model.h"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'model.h'),
                                os.path.join(generatorDir, 'templates/model.h.template'),
                                config['model'],
                                generatedFileHeadComment)

print "Generating fractions-pool.cpp"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'fractions-pool.cpp'),
                                os.path.join(generatorDir, 'templates/fractions-pool.cpp.template'),
                                config['model'],
                                generatedFileHeadComment)

print "Generating space.cpp"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'space.cpp'),
                                os.path.join(generatorDir, 'templates/space.cpp.template'),
                                config['model'],
                                generatedFileHeadComment)


print "Generating model.cpp"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'model.cpp'),
                                os.path.join(generatorDir, 'templates/model.cpp.template'),
                                config['model'],
                                generatedFileHeadComment)

print "Generating main.cpp"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'main.cpp'),
                                os.path.join(generatorDir, 'templates/main.cpp.template'),
                                config['run_options'],
                                generatedFileHeadComment)

print "Generating Makefile"
code_utils.genFileByTemplate(os.path.join(destinationDir, 'Makefile'),
                                os.path.join(generatorDir, 'templates/Makefile.template'),
                                config['model'],
                                generatedFileHeadCommentForMake)


print ""
print "Code generation done."

