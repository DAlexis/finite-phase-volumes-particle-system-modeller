import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    outputFile = open(os.path.join(destinationDir, 'fractions-pool.h'), 'w')
    inputFile = open('templates/fractions-pool.h.template', 'r')
    
    outputFile.write(generatedFileHeadComment)
    
    lines = inputFile.readlines()
    for line in lines:
        lineTemplate = string.Template(line)
        outputFile.write(lineTemplate.substitute(config['model']))
    
    inputFile.close()
    outputFile.close()
