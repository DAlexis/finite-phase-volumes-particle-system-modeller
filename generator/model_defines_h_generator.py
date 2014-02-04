import code_utils
import name_generator

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
    fractionsEnumItems = []
    for fractionId in config['model']['fractions']:
        fractionsEnumItems.append(config['model']['fractions'][fractionId]['fractions_enum_element'])
    fractionsEnumItems.append('FRACTIONS_COUNT')
    modDef_h.write(code_utils.createEnum('Fraction', fractionsEnumItems) + "\n")
    
    #
    # Space axis enum
    #
    fractionsEnumItems = []
    for spaceDimId in config['model']['cordinate_space_grid']:
        fractionsEnumItems.append(config['model']['cordinate_space_grid'][spaceDimId]['space_dimension_enum_element'])

    fractionsEnumItems.append('SPACE_COORDS_COUNT')
    modDef_h.write(code_utils.createEnum('SpaceCoordinate', fractionsEnumItems) + "\n")
    
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
        fractionsEnumItems = []
        print ""
        print currentFraction
        print ""
        if ('fraction_space_grid' in currentFraction):
            if currentFraction['fraction_space_grid']:
                for fractDimensionId in currentFraction['fraction_space_grid']:
                    fractionsEnumItems.append(currentFraction['fraction_space_grid'][fractDimensionId]['fraction_coordinate_enum_element'])
        fractionsEnumItems.append(currentFraction['coordinates_enum_prefix'] + 'COUNT')
        modDef_h.write(code_utils.createEnum(currentFraction['fraction_coordinate_enum'], fractionsEnumItems) + "\n")
        
        #
        # Fraction's Quantities enum
        #
        fractionsEnumItems = []
        fractionsEnumItems.append(currentFraction['quantities']['particles_count']['fraction_quantity_enum_element'])
        for quantityId in currentFraction['quantities']:
            if not quantityId == 'particles_count':
                fractionsEnumItems.append(currentFraction['quantities'][quantityId]['fraction_quantity_enum_element'])
        fractionsEnumItems.append(currentFraction['quantities_enum_prefix'] + 'COUNT' + '\n')
        modDef_h.write(code_utils.createEnum(currentFraction['fraction_quantity_enum'], fractionsEnumItems) + "\n");

    modDef_h.write(code_utils.headerGuardBottom("model-defines.h"))

    modDef_h.close()
