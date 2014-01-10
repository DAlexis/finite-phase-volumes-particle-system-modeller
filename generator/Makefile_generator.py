import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'Makefile'),
                                'templates/Makefile.template',
                                config['model'],
                                generatedFileHeadComment)
