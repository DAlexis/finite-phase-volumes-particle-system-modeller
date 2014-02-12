import code_utils

import os

def generate(destinationDir, config, generatedFileHeadComment):
    modDef_h_filename = os.path.join(destinationDir, 'model-defines.h')
    modDef_h = open(modDef_h_filename, 'w')
    modDef_h.write(generatedFileHeadComment)
    modDef_h.write(code_utils.headerGuardTop("model-defines.h"))

    modDef_h.write(code_utils.splitterComment + "\n// General\n")
    
    #
    # Fractions enum
    #
    enumElements = []
    for fractionId in config['model']['fractions']:
        enumElements.append(config['model']['fractions'][fractionId]['fractions_enum_element'])
    enumElements.append('FRACTIONS_COUNT')
    modDef_h.write(code_utils.createEnum('Fraction', enumElements) + "\n")
    
    #
    # Space axis enum
    #
    enumElements = []
    for spaceDimId in config['model']['cordinate_space_grid']:
        enumElements.append(config['model']['cordinate_space_grid'][spaceDimId]['space_dimension_enum_element'])

    enumElements.append('SPACE_COORDS_COUNT')
    modDef_h.write(code_utils.createEnum('SpaceCoordinate', enumElements) + "\n")
    
    #
    # Enums for each fraction
    #
    for fractionId in config['model']['fractions']:
        currentFraction = config['model']['fractions'][fractionId]
        modDef_h.write(code_utils.splitterComment + '\n')
        modDef_h.write('// ' + currentFraction['name'] + ' defines\n')
        
        #
        # Fraction's axis enum
        #
        enumElements = []
        if ('fraction_space_grid' in currentFraction):
            if currentFraction['fraction_space_grid']:
                for fractDimensionId in currentFraction['fraction_space_grid']:
                    enumElements.append(currentFraction['fraction_space_grid'][fractDimensionId]['fraction_coordinate_enum_element'])
        enumElements.append(currentFraction['coordinates_enum_prefix'] + 'COUNT')
        modDef_h.write(code_utils.createEnum(currentFraction['fraction_coordinate_enum'], enumElements) + "\n")
        
        #
        # Fraction's quantities enum
        #
        enumElements = []
        enumElements.append(currentFraction['quantities']['particles_count']['fraction_quantity_enum_element'])
        for quantityId in currentFraction['quantities']:
            if not quantityId == 'particles_count':
                enumElements.append(currentFraction['quantities'][quantityId]['fraction_quantity_enum_element'])
        enumElements.append(currentFraction['quantities_enum_prefix'] + 'COUNT')
        modDef_h.write(code_utils.createEnum(currentFraction['fraction_quantity_enum'], enumElements) + "\n");
        
        #
        # Fraction's secondary quantities enum
        #
        enumElements = []
        if 'secondary_quantities' in currentFraction:
            if currentFraction['secondary_quantities']:
                for seqQuantityId in currentFraction['secondary_quantities']:
                    enumElements.append(currentFraction['secondary_quantities'][seqQuantityId]['fraction_secondary_quantity_enum_element'])
        
        enumElements.append(currentFraction['secondary_quantities_enum_prefix'] + 'COUNT')
        modDef_h.write(code_utils.createEnum(currentFraction['fraction_secondary_quantity_enum'], enumElements) + "\n");
        
    modDef_h.write(code_utils.headerGuardBottom("model-defines.h"))

    modDef_h.close()
