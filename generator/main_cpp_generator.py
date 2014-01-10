import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'main.cpp'),
                                'templates/main.cpp.template',
                                config['run_options'],
                                generatedFileHeadComment)
