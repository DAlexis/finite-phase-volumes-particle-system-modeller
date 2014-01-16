import code_utils
import name_generator

import os

def generate(destinationDir, config, generatedFileHeadComment):
    modDef_h_filename = os.path.join(destinationDir, 'model-defines.h')
    modDef_h = open(modDef_h_filename, 'w')
    modDef_h.write(generatedFileHeadComment)
    modDef_h.write(code_utils.headerGuardTop("model-defines.h"))

    modDef_h.write(code_utils.splitterComment + "\n// General\nenum Fraction\n{\n")

    isFirst = True
    for fractionId in config['model']['fractions']:
        modDef_h.write('    ' + config['model']['fractions'][fractionId]['fractions_enum_element'])
        if isFirst:
            modDef_h.write(' = 0')
        modDef_h.write(',\n')

    modDef_h.write('    ' + 'FRACTIONS_COUNT\n')
    modDef_h.write("};\n\n" + code_utils.splitterComment + "\n// Space defines\nenum SpaceCoordinate\n{\n")

    isFirst = True
    for spaceDimId in config['model']['cordinate_space_grid']:
        modDef_h.write('    ' + config['model']['cordinate_space_grid'][spaceDimId]['space_dimension_enum_element'])
        if isFirst:
            modDef_h.write(' = 0')
            isFirst = False
        modDef_h.write(',\n')

    modDef_h.write('    ' + 'SPACE_COORDS_COUNT\n')
    modDef_h.write("};\n\n")

    for fractionId in config['model']['fractions']:
        currentFraction = config['model']['fractions'][fractionId]
        modDef_h.write(code_utils.splitterComment + '\n')
        modDef_h.write('// ' + currentFraction['name'] + ' defines\n')
        
        # Coordinates enum
        modDef_h.write('enum ' + currentFraction['fraction_coordinate_enum'] + '\n{\n')
        isFirst = True
        if currentFraction['fraction_space_grid']:
            for fractDimensionId in currentFraction['fraction_space_grid']:
                currentFractionDimension = currentFraction['fraction_space_grid'][fractDimensionId]
                modDef_h.write('    ' + currentFractionDimension['fraction_coordinate_enum_element'])
                if isFirst:
                    modDef_h.write(' = 0')
                    isFirst = False
                modDef_h.write(',\n')
        modDef_h.write('    ' + currentFraction['coordinates_enum_prefix'] + 'COUNT')
        if isFirst:
            modDef_h.write(' = 0')
        modDef_h.write('\n};\n\n');
        
        # Quantities enum
        modDef_h.write('enum ' + currentFraction['fraction_quantity_enum'] + '\n{\n')
        modDef_h.write('    ' + currentFraction['quantities_enum_prefix'] + 'PARTICLES_COUNT' + ' = 0,\n')
        if currentFraction['quantities']:
            for quantityId in currentFraction['quantities']:
                currentQuantity = currentFraction['quantities'][quantityId]
                modDef_h.write('    ' + currentQuantity['fraction_quantity_enum_element'] + ',\n')
        modDef_h.write('    ' + currentFraction['quantities_enum_prefix'] + 'COUNT' + '\n')
        
        modDef_h.write('};\n\n');

    modDef_h.write(code_utils.headerGuardBottom("model-defines.h"))

    modDef_h.close()
