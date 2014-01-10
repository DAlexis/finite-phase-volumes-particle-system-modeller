import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'space.cpp'),
                                'templates/space.cpp.template',
                                config['model'],
                                generatedFileHeadComment)
