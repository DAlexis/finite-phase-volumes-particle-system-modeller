import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'model-output.cpp'),
                                'templates/model-output.cpp.template',
                                config['output'],
                                generatedFileHeadComment)
