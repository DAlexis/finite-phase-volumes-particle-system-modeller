import code_utils

import string
import re

axisUniformConfigTemplate = string.Template("""Axis& ${axis_id} = ${description_class}.axis[${axis_index}];
    ${axis_id}.uniformInit(${axis_min}, ${axis_max}, ${axis_segments_count});
    ${axis_id}.setName("${axis_name}");
    
    """)

outputInitCode = string.Template("outputMaker.addInstance(new ${class_name});\n    ")

outputQvsSCHeaderTemplate = string.Template("""
class ${class_name} : public OutputInstanceBase
{
public:
    ${class_name}();
    virtual ~${class_name}();
    
protected:
    virtual void printToFile(double time);
};
""")

outputQvsSCSourceTemplate = string.Template("""
${class_name}::${class_name}() :
    OutputInstanceBase(0, // This is because really quantity index is not used by OutputInstanceBase
        ${time_step},
        ${points_count},
        "${file_name}")
{
}

${class_name}::~${class_name}()
{
}

void ${class_name}::printToFile(double time)
{
    Space& space = *(m_parent->m_space);
    double maxVal = space.gridDescription->axis[${iterating_coord}].getMaxValue();
    double minVal = space.gridDescription->axis[${iterating_coord}].getMinValue();
    
    double spacePoint[SPACE_COORDS_COUNT];
    ${space_point_init}
    
    for (unsigned int sapceIndex=0; sapceIndex<m_pointsCount; sapceIndex++)
    {
        spacePoint[${iterating_coord}] = minVal + (maxVal-minVal) * sapceIndex / m_pointsCount;
        
        FractionsPool *spaceCell = space.accessElement_d(spacePoint);
        ${fraction_space_classname} *fractionSpace = static_cast<${fraction_space_classname}*> (spaceCell->fractions[${fraction_index}]);
        // Convolution in space cell
        double result = 0;
        for (unsigned int particleIndex=0; particleIndex < fractionSpace->elementsCount; particleIndex++)
        {
            ${fraction_cell_classname}& cell = fractionSpace->elements[particleIndex];
            result += ${output_code}
            ;
        }
        (*m_file) << time << " " << spacePoint[${iterating_coord}] << " " << result << std::endl;
    }
}
""")

fractionInitCodeTemplate = string.Template("fractions[${fractions_enum_element}] = new ${fraction_space_classname}(this);\n    ")

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
    for coordId in thisFraction['fraction_space_grid']:
        coordDerFullName = 'fractionCoordsDerivatives[' + thisFraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] + ']'
        result = re.sub(r'\b' + coordId + r'[\s]*.[\s]*DER\b', coordDerFullName, result)
        coordFullName = 'coordinates[' + thisFraction['fraction_space_grid'][coordId]['fraction_coordinate_enum_element'] + ']'
        result = re.sub(r'\b' + coordId + r'\b', coordFullName, result)
    
    # Replacing this fraction's quantities
    for quantityId in thisFraction['quantities']:
        quantityFullName = 'quantities[' + thisFraction['quantities'][quantityId]['fraction_quantity_enum_element'] + ']'
        result = re.sub(r'\b' + quantityId + r'\b', quantityFullName, result)
    
    result = re.sub(r'\bparticles_count\b', 'quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]', result)
    
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
                currentQuantity['fraction_quantity_enum_element'] = fraction['quantities_enum_prefix'] + quantityId.upper()
        
        
    configTree['model']['all_fraction_headers'] = allFractionHeadersInclude
    configTree['model']['fraction_sources_list'] = fractionSourcesList
    configTree['model']['fractions_init_code'] = fractionsInitCode
    
    spaceAxisConfiguration = ""
    
    for dimensionId in configTree['model']['cordinate_space_grid']:
        dimension = configTree['model']['cordinate_space_grid'][dimensionId]
        dimension ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimensionId.upper()
        spaceAxisConfiguration = spaceAxisConfiguration + generateAxisConfig(dimension, dimensionId, dimension ['space_dimension_enum_element'], 'space')
    
    configTree['model']['space_axis_configuration'] = spaceAxisConfiguration
    
    outputHeaderCode = ""
    outputCppCode = ""
    outputsInitialisationCode = ""
    for instanceId in configTree['output']:
        instance = configTree['output'][instanceId]
        instance['class_name'] = instanceId.title()
        if instance['mode'] == 'quantity_vs_space_coord':
            outputHeaderCode = outputHeaderCode + outputQvsSCHeaderTemplate.substitute(instance)
            # Creating space point initialisation
            spacePointInitStr = ""
            for coordId in instance['space_point']:
                spacePointInitStr += 'spacePoint[' + configTree['model']['cordinate_space_grid'][coordId]['space_dimension_enum_element'] + '] = ' + str(instance['space_point'][coordId]) + ';\n    '
            
            outputCppCode = outputCppCode + outputQvsSCSourceTemplate.substitute(
                class_name          = instance['class_name'],
                iterating_coord     = configTree['model']['cordinate_space_grid'][instance['iterating_coord']]['space_dimension_enum_element'],
                space_point_init    = spacePointInitStr,
                fraction_cell_classname     = configTree['model']['fractions'][instance['fraction']]['fraction_cell_classname'],
                fraction_space_classname    = configTree['model']['fractions'][instance['fraction']]['fraction_space_classname'],
                output_code         = instance['output_code'],
                fraction_index      = configTree['model']['fractions'][instance['fraction']]['fractions_enum_element'],
                file_name       = str(instance['file_name']),
                points_count    = str(instance['points_count']),
                time_step       = str(instance['time_step'])
                )
            thisOutputCode = resolveSymbolsInFractionCode(outputInitCode.substitute(instance), configTree, configTree['model']['fractions'][instance['fraction']])
            outputsInitialisationCode = outputsInitialisationCode + thisOutputCode
    configTree['output']['header_code'] = outputHeaderCode
    configTree['output']['cpp_code'] = outputCppCode
    configTree['model']['outputs_init_code'] = outputsInitialisationCode
    
    for fractionId in configTree['model']['fractions']:
        fraction = configTree['model']['fractions'][fractionId]
        # Resolving id's in code
        fraction['sources'] = resolveSymbolsInFractionCode(fraction['sources'], configTree, fraction)
        fraction['space_coords_derivatives'] = resolveSymbolsInFractionCode(fraction['space_coords_derivatives'], configTree, fraction)
        fraction['fraction_coords_derivatives'] = resolveSymbolsInFractionCode(fraction['fraction_coords_derivatives'], configTree, fraction)
        
