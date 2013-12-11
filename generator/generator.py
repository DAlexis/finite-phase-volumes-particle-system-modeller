#!/usr/bin/python2.7

import argparse
import yaml
import code_utils

import space_h_generator

# Parsing command line
cmdLineParser = argparse.ArgumentParser(description='Input and output configuration:')
cmdLineParser.add_argument('--config', help='XML configuration file to be parsed to generate code')
cmdLineParser.add_argument('--output-dir', help='Directory where program should be created')
cmdLineArgs = cmdLineParser.parse_args()

# Beginning work
print ""
print "Code generation started"
config = yaml.load(open('configs/sample.yml'))

spaceGen = space_h_generator.SpaceHGenerator(config)
spaceGen.generate()

print spaceGen.string

print "Code generation done."
