import code_utils
import name_generator

import string
import os

def generate(destinationDir, config, generatedFileHeadComment):
    code_utils.genFileByTemplate(os.path.join(destinationDir, 'fractions-pool.h'),
                                'templates/fractions-pool.h.template',
                                config['model'],
                                generatedFileHeadComment)
