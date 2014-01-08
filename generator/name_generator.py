import code_utils

import string

axisUniformConfigTemplate = string.Template("""Axis& ${axis_id} = fractionGridDescription.axis[${axis_index}];
    ${axis_id}.uniformInit(${axis_min}, ${axis_max}, ${axis_segments_count});
    ${axis_id}.setName("${axis_name}");
    
    """)

def completeConfig(configTree):
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
        fraction['cpp_name'] = fractionId.lower() + '.cpp'
        fraction['header_guard'] = code_utils.formHeaderGuard(fraction ['header_name'])
        
        axisConfig = ""
        
        for dimensionId in fraction['fraction_space_grid']:
            dimension = fraction['fraction_space_grid'][dimensionId]
            dimension['fraction_coordinate_enum_element'] = fraction['coordinates_enum_prefix'] + dimensionId.upper()
            if dimension['division']['mode'] == 'uniform':
                axisConfig = axisConfig + axisUniformConfigTemplate.substitute(
                    axis_id     = dimensionId,
                    axis_index  = dimension['fraction_coordinate_enum_element'],
                    axis_name   = dimension['name'],
                    axis_min    = dimension['division']['min'],
                    axis_max    = dimension['division']['max'],
                    axis_segments_count = dimension['division']['segments_count']
                    )
        fraction['axis_configuration'] = axisConfig
        
        if fraction['quantities']:
            for quantityId in fraction['quantities']:
                currentQuantity = fraction['quantities'][quantityId]
                currentQuantity['fraction_quantity_enum_element'] = fraction['quantities_enum_prefix'] + quantityId
    
    for dimensionId in configTree['model']['cordinate_space_grid']:
        dimension = configTree['model']['cordinate_space_grid'][dimensionId]
        dimension ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimensionId.upper()
