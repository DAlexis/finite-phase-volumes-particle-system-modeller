
class NamesGenerator:
    fractions = {}
    sapceDimensions = {}
    
    def __init__(self, configTree):
        for fraction in configTree['model']['fractions']:
            namesList = {}
            namesList ['id'] = fraction['id']
            namesList ['name'] = fraction['name']
            namesList ['fractions_enum_element'] = 'FRACTION_' + fraction['id'].upper()
            namesList ['enum_prefix'] = fraction['id'].upper() + '_'
            namesList ['fraction_cell_classname'] = fraction['id'].title() + 'Cell'
            namesList ['fraction_space_classname'] = fraction['id'].title() + 'Space'
            namesList ['fraction_coordinate_enum'] = fraction['id'].title() + 'Coordinate'
            namesList ['fraction_quantity_enum'] = fraction['id'].title() + 'Quantity'
            self.fractions[namesList ['id']] = namesList
            #print ''
            #print self.names
        for dimension in configTree['model']['cordinate_space_grid']:
            namesList = {}
            namesList ['id'] = dimension['id']
            namesList ['name'] = dimension['name']
            namesList ['space_dimension_enum_element'] = 'SPACE_COORDS_' + dimension['id'].upper()
            self.sapceDimensions[namesList ['id']] = namesList
