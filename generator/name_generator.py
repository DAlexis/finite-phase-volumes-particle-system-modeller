import code_utils

import string

axisUniformConfigTemplate = string.Template("""Axis& ${axis_id} = ${description_class}.axis[${axis_index}];
    ${axis_id}.uniformInit(${axis_min}, ${axis_max}, ${axis_segments_count});
    ${axis_id}.setName("${axis_name}");
    
    """)

fractionInitCodeTemplate = string.Template("fractions[${fractions_enum_element}] = new ${fraction_space_classname}(this);\n    ")

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

def completeConfig(configTree):
    allFractionHeadersInclude = ""
    fractionsInitCode = ""
    fractionSourcesList = ""
    for fractionId in configTree['model']['fractions']:
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
        fraction['header_name'] = fractionId.lower() + '.h'
        allFractionHeadersInclude = allFractionHeadersInclude + '#include "' + fraction['header_name'] + '"\n'
        fraction['cpp_name'] = fractionId.lower() + '.cpp'
        fractionSourcesList = fractionSourcesList + fraction['cpp_name'] + ' '
        fraction['header_guard'] = code_utils.formHeaderGuard(fraction ['header_name'])
        
        fractionsInitCode = fractionsInitCode + fractionInitCodeTemplate.substitute(fraction)
        
        axisConfig = ""
        
        for dimensionId in fraction['fraction_space_grid']:
            dimension = fraction['fraction_space_grid'][dimensionId]
            dimension['fraction_coordinate_enum_element'] = fraction['coordinates_enum_prefix'] + dimensionId.upper()
            axisConfig = axisConfig + generateAxisConfig(dimension, dimensionId, dimension['fraction_coordinate_enum_element'], 'fraction')
           
        fraction['axis_configuration'] = axisConfig
        
        if fraction['quantities']:
            for quantityId in fraction['quantities']:
                currentQuantity = fraction['quantities'][quantityId]
                currentQuantity['fraction_quantity_enum_element'] = fraction['quantities_enum_prefix'] + quantityId
    
    configTree['model']['all_fraction_headers'] = allFractionHeadersInclude
    configTree['model']['fraction_sources_list'] = fractionSourcesList
    configTree['model']['fractions_init_code'] = fractionsInitCode
    
    
    spaceAxisConfiguration = ""
    
    for dimensionId in configTree['model']['cordinate_space_grid']:
        dimension = configTree['model']['cordinate_space_grid'][dimensionId]
        dimension ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimensionId.upper()
        spaceAxisConfiguration = spaceAxisConfiguration + generateAxisConfig(dimension, dimensionId, dimension ['space_dimension_enum_element'], 'space')
    configTree['model']['space_axis_configuration'] = spaceAxisConfiguration
