import code_utils

headerName = "space.h"

class SpaceHGenerator:
    __configTree = []
    string = ""
    
    def __add(self, what):
        self.string = self.string + what
    
    def __init__(self, configTree):
        self.__configTree = configTree
        
    def generate(self):
        self.__add(code_utils.headerGuardTop(headerName))
        self.__add("\n")
        self.__add(code_utils.include("fractions-pool.h"))
        self.__add(code_utils.include("grid-template.h"))
        self.__add("\n")
        
        self.__add(code_utils.headerGuardBottom(headerName))
