#!/usr/bin/python2.7
import code_utils
import name_generator
import model_defines_h_generator
import fraction_header_generator

import yaml
import argparse
import os
import shutil
import time

# Parsing command line
cmdLineParser = argparse.ArgumentParser(description='Input and output configuration:')
cmdLineParser.add_argument('--config', help='XML configuration file to be parsed to generate code')
cmdLineParser.add_argument('--output-dir', help='Directory where program should be created')
cmdLineArgs = cmdLineParser.parse_args()

# Beginning work
print ""
print "Code generation started"

configFileName = 'configs/1d.yml'
destinationDir = '../test/'
config = yaml.load(open(configFileName))
#print config
indiferentDir = 'indiferent/'

generatedFileHeadComment = """ /*
 * This file is generated by script using """ + configFileName + ', ' + time.strftime("%H:%M:%S %d/%m/%Y") + """
 */
"""

#
# Copying files that should not be modified
#
print ""
print "Copying model-indiferent files"
filesAsIs = [
            'axis.cpp',
            'axis.h',
            'fraction-base.h',
            'fraction-cell-interface.h',
            'fraction-space-interface.h',
            'fractions-pool-base.cpp',
            'fractions-pool-base.h',
            'global-defines.h',
            'grid-template.h',
            'output.cpp',
            'output.h',
            'space.cpp',
            'space.h',
            ]

for fileName in filesAsIs:
    target = os.path.join(indiferentDir, fileName)
    print "Copying " + target + " to " + destinationDir
    shutil.copy(target, destinationDir)

# Initialising name generator
fractionNameGen = name_generator.NamesGenerator(config)

#
# Generating model-defines.h
#
print ""
print "Generating model-defines.h"
model_defines_h_generator.generate(destinationDir, fractionNameGen, generatedFileHeadComment)

print ""
print "Generating fraction headers"
for fractionId in fractionNameGen.fractions:
    fraction_header_generator.generate(destinationDir, fractionNameGen, generatedFileHeadComment, fractionId)


print ""
print "Code generation done."

