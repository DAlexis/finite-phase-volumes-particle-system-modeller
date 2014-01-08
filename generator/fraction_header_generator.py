import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment, fractionId):
    fraction = config['model']['fractions'][fractionId]
    outputFile = open(os.path.join(destinationDir, fraction['header_name']), 'w')
    inputFile = open('templates/fraction-header.h.template', 'r')
    
    outputFile.write(generatedFileHeadComment)
    
    lines = inputFile.readlines()
    for line in lines:
        lineTemplate = string.Template(line)
        outputFile.write(lineTemplate.substitute(fraction))
    
    inputFile.close()
    outputFile.close()
