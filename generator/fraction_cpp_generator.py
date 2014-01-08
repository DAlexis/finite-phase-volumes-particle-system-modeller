import code_utils
import name_generator

import string
import os

def generate(destinationDir, fractionNameGen, generatedFileHeadComment, fractionId, config):
    fraction = fractionNameGen.fractions[fractionId]
    outputFile = open(os.path.join(destinationDir, fraction['cpp_name']), 'w')
    inputFile = open('templates/fraction.cpp.template', 'r')
    
    lines = inputFile.readlines()
    replacementDict = {}
    replacementDict['fraction_space_classname'] = fraction['fraction_space_classname']
    replacementDict['fraction_space_base_classname'] = fraction['fraction_space_base_classname']
    replacementDict['fraction_cell_classname'] = fraction['fraction_cell_classname']
    
    replacementDict['space_coords_derivatives'] = fraction['config']['space_coords_derivatives']
    replacementDict['fraction_coords_derivatives'] = fraction['config']['fraction_coords_derivatives']
    replacementDict['sources'] = fraction['config']['sources']
    
    axisUniformConfigTemplate = string.Template("""
        Axis& ${axis_id} = fractionGridDescription.axis[${axis_index}];
        ${axis_id}.uniformInit(-10.0, 10.0, 50);
        ${axis_id}.setName(${axis_name});
    """)
    
    for line in lines:
        lineTemplate = string.Template(line)
        outputFile.write(lineTemplate.substitute(replacementDict))
    
    inputFile.close()
    outputFile.close()
