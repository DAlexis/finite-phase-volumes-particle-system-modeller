
class NamesGenerator:
    fractions = {}
    sapceDimensions = {}
    
    def __init__(self, configTree):
        for fraction in configTree['model']['fractions']:
            namesDict = {}
            namesDict ['id'] = fraction['id']
            namesDict ['name'] = fraction['name']
            namesDict ['fractions_enum_element'] = 'FRACTION_' + fraction['id'].upper()
            namesDict ['coordinates_enum_prefix'] = fraction['id'].upper() + '_COORDS_'
            namesDict ['quantities_enum_prefix'] = fraction['id'].upper() + '_QUANTITIES_'
            namesDict ['fraction_cell_classname'] = fraction['id'].title() + 'Cell'
            namesDict ['fraction_space_classname'] = fraction['id'].title() + 'Space'
            namesDict ['fraction_coordinate_enum'] = fraction['id'].title() + 'Coordinate'
            namesDict ['fraction_quantity_enum'] = fraction['id'].title() + 'Quantity'
            dimensionsDict = {}
            for dimension in fraction['fraction_space_grid']:
                dimensionDescription = {}
                dimensionDescription['id'] = dimension['id']
                dimensionDescription['name'] = dimension['name']
                dimensionDescription['fraction_coordinate_enum_element'] = namesDict ['coordinates_enum_prefix'] + dimension['id'].upper()
                dimensionsDict[ dimensionDescription['id'] ] = dimensionDescription
            namesDict['dimensions'] = dimensionsDict
            
            quantitiesDict = {}
            if fraction['quantities']:
                for quantity in fraction['quantities']:
                    quantityDescription = {}
                    quantityDescription['id'] = quantity['id']
                    quantityDescription['name'] = quantity['name']
                    quantityDescription['fraction_quantity_enum_element'] = namesDict ['quantities_enum_prefix'] + quantity['id'].upper()
                    quantitiesDict[quantityDescription['id']] = quantityDescription
            namesDict['quantities'] = quantitiesDict
            
            self.fractions[namesDict ['id']] = namesDict
            
        for dimension in configTree['model']['cordinate_space_grid']:
            namesDict = {}
            namesDict ['id'] = dimension['id']
            namesDict ['name'] = dimension['name']
            namesDict ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimension['id'].upper()
            self.sapceDimensions[namesDict ['id']] = namesDict
        print self.fractions
