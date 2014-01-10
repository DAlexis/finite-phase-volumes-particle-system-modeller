import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment, fractionId):
    fraction = config['model']['fractions'][fractionId]
    code_utils.genFileByTemplate(os.path.join(destinationDir, fraction['cpp_name']),
                                'templates/fraction.cpp.template',
                                fraction,
                                generatedFileHeadComment)
