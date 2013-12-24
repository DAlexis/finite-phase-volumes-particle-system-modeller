
class FractionRelatedNamesGenerator:
    names = {}
    
    def __init__(self, configTree):
        for fraction in configTree['model']['fractions']:
            namesList = {}
            namesList ['id'] = fraction['id']
            namesList ['name'] = fraction['name']
            namesList ['enum_prefix'] = fraction['id'].upper() + '_'
            #name[ fraction['id'] ] ['header'] = 
            self.names[namesList ['id']] = namesList
            print ''
            print self.names
