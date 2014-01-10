import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'model.cpp'),
                                'templates/model.cpp.template',
                                config['model'],
                                generatedFileHeadComment)
