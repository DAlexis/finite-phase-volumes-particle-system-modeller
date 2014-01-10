import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'model-output.h'),
                                'templates/model-output.h.template',
                                config['output'],
                                generatedFileHeadComment)
