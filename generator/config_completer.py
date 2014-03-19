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
    result = re.sub(r'\bspace_volume\b', 'getSpaceCell()->volume', result)
    
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
    
    # Replacing this fraction's extensiveQuantities
    for quantityId in thisFraction['extensive_quantities']:
        nextStepQuantityFullName = 'extensiveQuantitiesDelta[' + thisFraction['extensive_quantities'][quantityId]['fraction_quantity_enum_element'] + ']'
        result = re.sub(r'\b' + quantityId + r'[\s]*.[\s]*DELTA\b', nextStepQuantityFullName, result)
        quantityFullName = 'extensiveQuantities[' + thisFraction['extensive_quantities'][quantityId]['fraction_quantity_enum_element'] + ']'
        result = re.sub(r'\b' + quantityId + r'\b', quantityFullName, result)
    
    # Replacing this fraction's secondary quantities
    if 'intensive_quantities' in thisFraction:
        if thisFraction['intensive_quantities']:
            for secQuantityId in thisFraction['intensive_quantities']:
                secondaryQuantityFullName = 'intensiveQuantities[' + thisFraction['intensive_quantities'][secQuantityId]['fraction_secondary_quantity_enum_element'] + ']'
                result = re.sub(r'\b' + secQuantityId + r'\b', secondaryQuantityFullName, result)
        
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
    configTree['model']['space_axis_configuration'] = code_utils.indentCode(spaceAxisConfiguration, "    ")
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
        fraction['extensive_quantities_enum_prefix'] = fractionId.upper() + '_EXTENSIVE_QUANTITIES_'
        fraction['intensive_quantities_enum_prefix'] = fractionId.upper() + '_INTENSIVE_QUANTITIES_'
        
        fraction['fraction_coordinate_enum'] = fractionId.title() + 'Coordinate'
        fraction['fraction_quantity_enum'] = fractionId.title() + 'ExtensiveQuantity'
        fraction['fraction_secondary_quantity_enum'] = fractionId.title() + 'IntensiveQuantity'
        
        fraction['fraction_cell_classname'] = fractionId.title() + 'Cell'
        fraction['fraction_cell_base_classname'] = fractionId.title() + 'CellBase'
        fraction['fraction_space_classname'] = fractionId.title() + 'Space'
        fraction['fraction_space_base_classname'] = fractionId.title() + 'SpaceBase'
        fraction['fractions_quantities_count_enum_element'] = fraction['extensive_quantities_enum_prefix'] + 'COUNT'
        fraction['header_name'] = fractionId.lower() + '.h'
        allFractionHeadersInclude = allFractionHeadersInclude + '#include "' + fraction['header_name'] + '"\n'
        fraction['cpp_name'] = fractionId.lower() + '.cpp'
        fractionSourcesList = fractionSourcesList + fraction['cpp_name'] + ' '
        fraction['header_guard'] = code_utils.formHeaderGuard(fraction ['header_name'])
        fractionsInitCode = fractionsInitCode + code_utils.indentCode(fractionInitCodeTemplate.substitute(fraction), "    ")
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
        if not 'extensive_quantities' in fraction:
            fraction['extensive_quantities'] = {}
        
        if not particlesCountQuantityId in fraction['extensive_quantities']:
            fraction['extensive_quantities'][particlesCountQuantityId] = {}
            fraction['extensive_quantities'][particlesCountQuantityId]['name'] = 'Particles count of ' + fraction['name']
        
        for quantityId in fraction['extensive_quantities']:
            currentQuantity = fraction['extensive_quantities'][quantityId]
            currentQuantity['fraction_quantity_enum_element'] = fraction['extensive_quantities_enum_prefix'] + quantityId.upper()
        #
        # Intensive quantities
        #
        secQuantitiesCountingCode = ""
        if 'intensive_quantities' in fraction:
            if fraction['intensive_quantities']:
                for secQuantityId in fraction['intensive_quantities']:
                    currentSecQuantity = fraction['intensive_quantities'][secQuantityId]
                    currentSecQuantity['fraction_secondary_quantity_enum_element'] = fraction['intensive_quantities_enum_prefix'] + secQuantityId.upper()
                    secQuantitiesCountingCode = secQuantitiesCountingCode + currentSecQuantity['counting'] + '\n'
        fraction['intensive_quantities_counting_code'] = secQuantitiesCountingCode
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
                                    + '] = BT_OPEN;\n'
                        if 'bottom' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['bottom'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'spaceBottomBorderType[' \
                                    + configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element'] \
                                    + '] = BT_OPEN;\n'
                    else:
                        # Fraction axis borders
                        if 'top' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['top'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'fractionTopBorderType[' \
                                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                                    + '] = BT_OPEN;\n'
                        if 'bottom' in fraction['boundary_conditions'][axisId]:
                            if fraction['boundary_conditions'][axisId]['bottom'] == 'open':
                                boundaryConditionsInitCode = boundaryConditionsInitCode + 'fractionBottomBorderType[' \
                                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                                    + '] = BT_OPEN;\n'
        fraction['boundary_conditions_config'] = code_utils.indentCode(boundaryConditionsInitCode, "    ")
        #
        # Diffusion coefficient counting
        #
        diffCountingCode = ""
        if 'diffusion' in fraction:
            if fraction['diffusion']:
                axisSwitchCases = {}
                for axisId in fraction['diffusion']:
                    axisSwitchCases[ configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element'] ] = \
                        fraction['diffusion'][axisId] # Adding code to cases dictionary
                diffCountingCode = code_utils.createSwitch("axisIndex", axisSwitchCases)
        fraction['diffusion_coefficient_counting_code'] = code_utils.indentCode(diffCountingCode, "    ")
        #
        # Quantities initialisation code
        #
        if not 'init_quantities' in fraction:
            fraction['init_quantities'] = ""
        else:
            if not fraction['init_quantities']:
                fraction['init_quantities'] = ""
        #
        # Filling some fields if not specified in config file
        #
        if not 'fraction_coords_derivatives' in fraction:
            fraction['fraction_coords_derivatives'] = ""
    
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
        if instance['quantity'] in fraction['extensive_quantities']:
            instance['quantity_index'] = fraction['extensive_quantities'][instance['quantity']]['fraction_quantity_enum_element']
            instance['quantity_type'] = "OCT_EXTENSIVE_QUANTITY"
        else:
            instance['quantity_index'] = fraction['intensive_quantities'][instance['quantity']]['fraction_secondary_quantity_enum_element']
            instance['quantity_type'] = "OCT_INTENSIVE_QUANTITY"
        
        # Space point config
        spacePointInitCode = ""
        if instance['space_point']:
            for coordId in instance['space_point']:
                spacePointInitCode = spacePointInitCode \
                    + instance['instance_id'] + "->getSpacePoint()[" \
                    + configTree['model']['cordinate_space_grid'][coordId]['space_dimension_enum_element'] \
                    + "] = " + str(instance['space_point'][coordId]) + ";\n"
        instance['space_point_init'] = spacePointInitCode
        
        # Fraction point config
        fractionPointInitCode = ""
        if 'fraction_point' in instance:
            for coordId in instance['fraction_point']:
                fractionPointInitCode = fractionPointInitCode \
                    + instance['instance_id'] + "->getFractionPoint()[" \
                    + fraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] \
                    + "] = " + str(instance['fraction_point'][coordId]) + ";\n"
        instance['fraction_point_init'] = fractionPointInitCode
        
        # Output axis config
        axisAddingCode = ""
        for axisId in instance['output_axis']:
            #if ('fraction_space_grid' in fraction) and (axisId in fraction['fraction_space_grid']):
            if (axisId in configTree['model']['cordinate_space_grid']):
                axisAddingCode = axisAddingCode + "->addOutputAxis(OAT_SPACE_COORDINATE, " \
                    + str(instance['output_axis'][axisId]['points_count']) + ", " \
                    + configTree['model']['cordinate_space_grid'][axisId]['space_dimension_enum_element']\
                    + ")\n"
            else:
                axisAddingCode = axisAddingCode + "->addOutputAxis(OAT_FRACTION_COORDINATE, " \
                    + str(instance['output_axis'][axisId]['points_count']) + ", " \
                    + fraction['fraction_space_grid'][axisId]['fraction_coordinate_enum_element'] \
                    + ")\n"
        
        instance['output_axis_configuration'] = code_utils.indentCode(axisAddingCode, "    ")
        
        # Convolution configuration
        convilutionAddingCode = ""
        if 'convolution' in instance:
            if instance['convolution'] == "all":
                convilutionAddingCode = "->useAllFractionSpaceConvolution(" \
                    + fraction['fractions_quantities_count_enum_element'] + ")\n"
            else:
                for fractionAxis in instance['convolution']:
                    convilutionAddingCode = convilutionAddingCode \
                        + "->useConvolutionByFractionAxis(" \
                        + fraction['fraction_space_grid'][fractionAxis]['fraction_coordinate_enum_element'] \
                        + ")\n"
        instance['convolution_configureation'] = code_utils.indentCode(convilutionAddingCode, "    ")
        outputsInitialisationCode = outputsInitialisationCode + \
            code_utils.indentCode(outputInstanceInitCodeTemplate.substitute(instance), "    ") + "\n"
    
    configTree['model']['outputs_init_code'] = outputsInitialisationCode
    
    #
    # Resolving macro symbols in code fragments
    #
    for fractionId in configTree['model']['fractions']:
        fraction = configTree['model']['fractions'][fractionId]
        # Resolving id's in code
        fraction['sources'] = code_utils.indentCode(resolveSymbolsInFractionCode(fraction['sources'], configTree, fraction), "    ")
        fraction['space_coords_derivatives'] = code_utils.indentCode(resolveSymbolsInFractionCode(fraction['space_coords_derivatives'], configTree, fraction), "    ")
        fraction['fraction_coords_derivatives'] = code_utils.indentCode(resolveSymbolsInFractionCode(fraction['fraction_coords_derivatives'], configTree, fraction), "    ")
        fraction['intensive_quantities_counting_code'] = code_utils.indentCode(resolveSymbolsInFractionCode(fraction['intensive_quantities_counting_code'], configTree, fraction), "    ")
        fraction['init_quantities'] = code_utils.indentCode(resolveSymbolsInFractionCode(fraction['init_quantities'], configTree, fraction), "    ")
