import code_utils
import name_generator

import string
import os

def generate(destinationDir, fractionNameGen, generatedFileHeadComment, fractionId):
    fraction = fractionNameGen.fractions[fractionId]
    outputFile = open(os.path.join(destinationDir, fraction['header_name']), 'w')
    inputFile = open('templates/fraction-header.h.template', 'r')
    
    lines = inputFile.readlines()
    
    for line in lines:
        lineTemplate = string.Template(line)
        outputFile.write(lineTemplate.substitute(fraction))
    
    inputFile.close()
    outputFile.close()
