import code_utils

import string
import re

particlesCountQuantityId = 'particles_count'
everyFractionCountQuantityIndex = 'EVERY_FRACTION_COUNT_QUANTITY_INDEX'

axisUniformConfigTemplate = code_utils.readTemplate("fragments/axis-uniform-config.template")
axisSpecificConfigTemplate = code_utils.readTemplate("fragments/axis-specific-config.template")
fractionInitCodeTemplate = code_utils.readTemplate("fragments/fraction-init-code.template")
outputInstanceInitCodeTemplate = code_utils.readTemplate("fragments/output-instance-config.template")

def resolveSymbolsInFractionCode(code, configTree, thisFraction):
    result = code
    # Replacing simple words
    result = re.sub(r'\bmodel\b', 'static_cast<Model*>(getModel())', result)
    
    # Replacing fractions id to its adresses
    for fractionId in configTree['model']['fractions']:
        fraction = configTree['model']['fractions'][fractionId]
        fractionFullName = 'getSpaceCell()->fractions[' + fraction['fractions_enum_element'] + ']'
        result = re.sub(r'\b' + fractionId + r'\b', fractionFullName, result)
    
    # Replacing this fraction's coords
    if 'fraction_space_grid' in fraction:
        if fraction['fraction_space_grid']:
            for coordId in thisFraction['fraction_space_grid']:
                coordDerFullName = 'fractionCoordsDerivatives[' + thisFraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] + ']'
                result = re.sub(r'\b' + coordId + r'[\s]*.[\s]*DER\b', coordDerFullName, result)
                coordFullName = 'coordinates[' + thisFraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] + ']'
                result = re.sub(r'\b' + coordId + r'\b', coordFullName, result)
    
    # Replacing this fraction's quantities
    for quantityId in thisFraction['quantities']:
        nextStepQuantityFullName = 'nextStepQuantities[' + thisFraction['quantities'][quantityId]['fraction_quantity_enum_element'] + ']'
        result = re.sub(r'\b' + quantityId + r'[\s]*.[\s]*NEXT\b', nextStepQuantityFullName, result)
        quantityFullName = 'quantities[' + thisFraction['quantities'][quantityId]['fraction_quantity_enum_element'] + ']'
        result = re.sub(r'\b' + quantityId + r'\b', quantityFullName, result)
    
    # Replacing space coords and its derivatives
    for coordId in configTree['model']['cordinate_space_grid']:
        coordDerFullName = 'spaceCoordsDerivatives[' + configTree['model']['cordinate_space_grid'][coordId]['space_dimension_enum_element'] + ']'
        result = re.sub(r'\b' + coordId + r'[\s]*.[\s]*DER\b', coordDerFullName, result)
        coordFullName = 'getSpaceCell()->coordinates[' + configTree['model']['cordinate_space_grid'][coordId]['space_dimension_enum_element'] + ']'
        result = re.sub(r'\b' + coordId + r'\b', coordFullName, result)
    
    return result


def generateAxisConfig(asixDescriptionSubtree, axisId, axisIndex, axisType):
    descriptionClassName = "fractionGridDescription"
    if axisType=='space':
        descriptionClassName = "spaceGridDescription"
    if asixDescriptionSubtree['division']['mode'] == 'uniform':
        return axisUniformConfigTemplate.substitute(
            axis_id     = axisId,
            axis_index  = axisIndex,
            description_class = descriptionClassName,
            axis_name   = asixDescriptionSubtree['name'],
            axis_min    = asixDescriptionSubtree['division']['min'],
            axis_max    = asixDescriptionSubtree['division']['max'],
            axis_segments_count = asixDescriptionSubtree['division']['segments_count']
            )
    else:
        return axisSpecificConfigTemplate.substitute(
            axis_id     = axisId,
            axis_index  = axisIndex,
            description_class = descriptionClassName,
            axis_name   = asixDescriptionSubtree['name'],
            filename    = asixDescriptionSubtree['division']['filename'],
            )

def completeConfig(configTree):
    #
    # Space axis configuration
    #
    spaceAxisConfiguration = ""
    for dimensionId in configTree['model']['cordinate_space_grid']:
        dimension = configTree['model']['cordinate_space_grid'][dimensionId]
        dimension ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimensionId.upper()
        spaceAxisConfiguration = spaceAxisConfiguration + generateAxisConfig(dimension, dimensionId, dimension ['space_dimension_enum_element'], 'space')
    configTree['model']['space_axis_configuration'] = spaceAxisConfiguration
    #
    # Configuring fractions
    #
    allFractionHeadersInclude = ""
    fractionsInitCode = ""
    fractionSourcesList = ""
    for fractionId in configTree['model']['fractions']:
        #
        # Fraction-related strings
        #
        fraction = configTree['model']['fractions'][fractionId]
        fraction['fractions_enum_element'] = 'FRACTION_' + fractionId.upper()
        fraction['coordinates_enum_prefix'] = fractionId.upper() + '_COORDS_'
        fraction['quantities_enum_prefix'] = fractionId.upper() + '_QUANTITIES_'
        fraction['fraction_coordinate_enum'] = fractionId.title() + 'Coordinate'
        fraction['fraction_quantity_enum'] = fractionId.title() + 'Quantity'
        fraction['fraction_cell_classname'] = fractionId.title() + 'Cell'
        fraction['fraction_cell_base_classname'] = fractionId.title() + 'CellBase'
        fraction['fraction_space_classname'] = fractionId.title() + 'Space'
        fraction['fraction_space_base_classname'] = fractionId.title() + 'SpaceBase'
        fraction['fractions_quantities_count_enum_element'] = fraction['quantities_enum_prefix'] + 'COUNT'
        fraction['header_name'] = fractionId.lower() + '.h'
        allFractionHeadersInclude = allFractionHeadersInclude + '#include "' + fraction['header_name'] + '"\n'
        fraction['cpp_name'] = fractionId.lower() + '.cpp'
        fractionSourcesList = fractionSourcesList + fraction['cpp_name'] + ' '
        fraction['header_guard'] = code_utils.formHeaderGuard(fraction ['header_name'])
        fractionsInitCode = fractionsInitCode + fractionInitCodeTemplate.substitute(fraction)
        axisConfig = ""
        #
        # Fraction space configuration
        #
        if 'fraction_space_grid' in fraction:
            if fraction['fraction_space_grid']:
                for dimensionId in fraction['fraction_space_grid']:
                    dimension = fraction['fraction_space_grid'][dimensionId]
                    dimension['fraction_coordinate_enum_element'] = fraction['coordinates_enum_prefix'] + dimensionId.upper()
                    axisConfig = axisConfig + generateAxisConfig(dimension, dimensionId, dimension['fraction_coordinate_enum_element'], 'fraction')
        fraction['axis_configuration'] = axisConfig
        #
        # Fraction's quantities configuration
        #
        # Adding particles_count quantity declared implicitly
        if not 'quantities' in fraction:
            fraction['quantities'] = {}
        
        if not particlesCountQuantityId in fraction['quantities']:
            fraction['quantities'][particlesCountQuantityId] = {}
            fraction['quantities'][particlesCountQuantityId]['name'] = 'Particles count of ' + fraction['name']
        
        for quantityId in fraction['quantities']:
            currentQuantity = fraction['quantities'][quantityId]
            currentQuantity['fraction_quantity_enum_element'] = fraction['quantities_enum_prefix'] + quantityId.upper()
        #
        # Boundary conditions configuration
        #
        boundaryConditionsInitCode = ""
        if 'boundary_conditions' in fraction:
            if fraction['boundary_conditions']:
                
                for axisId in fraction['boundary_conditions']:
                    if axisId in configTree['model']['cordinate_space_grid']:
                        # Space axis borders
                        if 'top' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['top'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'spaceTopBorderType[' \
                                    + configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element'] \
                                    + '] = BT_OPEN;\n    '
                        if 'bottom' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['bottom'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'spaceBottomBorderType[' \
                                    + configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element'] \
                                    + '] = BT_OPEN;\n    '
                    else:
                        # Fraction axis borders
                        if 'top' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['top'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'fractionTopBorderType[' \
                                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                                    + '] = BT_OPEN;\n    '
                        if 'bottom' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['bottom'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'fractionBottomBorderType[' \
                                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                                    + '] = BT_OPEN;\n    '
        fraction['boundary_conditions_config'] = boundaryConditionsInitCode
        #
        # Diffusion coefficient counting
        #
        diffCountingCode = ""
        if 'diffusion' in fraction:
            if fraction['diffusion']:
                quantitiesSwitchCases = {}
                for quantityId in fraction['diffusion']:
                    currentQuantityDescription = fraction['quantities'][quantityId]
                    axisSwitchCases = {}
                    if fraction['diffusion'][quantityId]:
                        for axisId in fraction['diffusion'][quantityId]:
                            axisSwitchCases[ configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element'] ] = \
                                fraction['diffusion'][quantityId][axisId] # Adding code to cases dictionary
                    quantitiesSwitchCases[currentQuantityDescription['fraction_quantity_enum_element']] = \
                        code_utils.createSwitch("axisIndex", axisSwitchCases)
                diffCountingCode = code_utils.createSwitch("quantity", quantitiesSwitchCases)
        fraction['diffusion_coefficient_counting_code'] = code_utils.indentCode(diffCountingCode, "    ")
        
    configTree['model']['all_fraction_headers'] = allFractionHeadersInclude
    configTree['model']['fraction_sources_list'] = fractionSourcesList
    configTree['model']['fractions_init_code'] = fractionsInitCode
    
    #
    # Output initialisation code generation
    #
    outputsInitialisationCode = ""
    for instanceId in configTree['output']:
        instance = configTree['output'][instanceId]
        instance['instance_id'] = instanceId
        fraction = configTree['model']['fractions'][instance['fraction']]
        instance['fraction_index'] = fraction['fractions_enum_element']
        instance['quantity_index'] = fraction['quantities'][instance['quantity']]['fraction_quantity_enum_element']
        
        # Space point config
        spacePointInitCode = ""
        if instance['space_point']:
            for coordId in instance['space_point']:
                spacePointInitCode = spacePointInitCode \
                    + instance['instance_id'] + "->getSpacePoint()[" \
                    + configTree['model']['cordinate_space_grid'][coordId]['space_dimension_enum_element'] \
                    + "] = " + str(instance['space_point'][coordId]) + ";\n    "
        instance['space_point_init'] = spacePointInitCode
        
        # Fraction point config
        fractionPointInitCode = ""
        if 'fraction_point' in instance:
            for coordId in instance['fraction_point']:
                fractionPointInitCode = fractionPointInitCode \
                    + instance['instance_id'] + "->getFractionPoint()[" \
                    + fraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] \
                    + "] = " + str(instance['fraction_point'][coordId]) + ";\n    "
        instance['fraction_point_init'] = fractionPointInitCode
        
        # Output axis config
        axisAddingCode = ""
        for axisId in instance['output_axis']:
            #if ('fraction_space_grid' in fraction) and (axisId in fraction['fraction_space_grid']):
            if (axisId in configTree['model']['cordinate_space_grid']):
                axisAddingCode = axisAddingCode + "->addOutputAxis(OAT_SPACE_COORDINATE, " \
                    + str(instance['output_axis'][axisId]['points_count']) + ", " \
                    + configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element']\
                    + ")\n        "
            else:
                axisAddingCode = axisAddingCode + "->addOutputAxis(OAT_FRACTION_COORDINATE, " \
                    + str(instance['output_axis'][axisId]['points_count']) + ", " \
                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                    + ")\n        "
        
        instance['output_axis_configuration'] = axisAddingCode
        
        # Convolution configuration
        convilutionAddingCode = ""
        if 'convolution' in instance:
            if instance['convolution'] == "all":
                convilutionAddingCode = "->useAllFractionSpaceConvolution(" \
                    + fraction['fractions_quantities_count_enum_element'] + ")\n        "
            else:
                for fractionAxis in instance['convolution']:
                    convilutionAddingCode = convilutionAddingCode \
                        + "->useConvolutionByFractionAxis(" \
                        + fraction['fraction_space_grid'][fractionAxis]['fraction_coordinate_enum_element'] \
                        + ")\n        "
        instance['convolution_configureation'] = convilutionAddingCode
        outputsInitialisationCode = outputsInitialisationCode + outputInstanceInitCodeTemplate.substitute(instance)
    
    configTree['model']['outputs_init_code'] = outputsInitialisationCode
    
    #
    # Resolving macro symbols in code fragments
    #
    for fractionId in configTree['model']['fractions']:
        fraction = configTree['model']['fractions'][fractionId]
        # Resolving id's in code
        fraction['sources'] = resolveSymbolsInFractionCode(fraction['sources'], configTree, fraction)
        fraction['space_coords_derivatives'] = resolveSymbolsInFractionCode(fraction['space_coords_derivatives'], configTree, fraction)
        fraction['fraction_coords_derivatives'] = resolveSymbolsInFractionCode(fraction['fraction_coords_derivatives'], configTree, fraction)
