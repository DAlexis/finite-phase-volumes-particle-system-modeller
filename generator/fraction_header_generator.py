import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment, fractionId):
    fraction = config['model']['fractions'][fractionId]
    code_utils.genFileByTemplate(os.path.join(destinationDir, fraction['header_name']),
                                'templates/fraction-header.h.template',
                                fraction,
                                generatedFileHeadComment)
