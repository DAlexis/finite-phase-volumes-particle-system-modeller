#!/usr/bin/python2.7
import code_utils
import space_h_generator
import name_generator

import yaml
import argparse
import os
import shutil

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
templatesDir = 'templates/'

#
# Copying files that should not be modified
#

filesAsIs = [
            'fraction-cell-template.h',
            'fraction-space-template.h',
            'fractions-pool-template.h',
            'grid-template.h',
            'space-template.h'
            ]

for fileName in filesAsIs:
    print os.path.join(templatesDir, fileName)
    shutil.copy(os.path.join(templatesDir, fileName), destinationDir)

#
# Initialising name generators
#
fractionNameGen = name_generator.FractionRelatedNamesGenerator(config)

#spaceGen = space_h_generator.SpaceHGenerator(config)
#spaceGen.generate()
#print spaceGen.string

print "Code generation done."
