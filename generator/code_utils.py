import string

splitterComment = '//////////////////////////'

def formHeaderGuard(headerName):
    return headerName.upper().replace(".", "_").replace("-", "_") + '_INCLUDED'

def headerGuardTop(key):
    guard = formHeaderGuard(key)
    return "#ifndef "+guard+"_INCLUDED\n#define "+guard+"_INCLUDED\n\n"

def headerGuardBottom(key):
    guard = formHeaderGuard(key)
    return "#endif // "+guard+"_INCLUDED\n"

def include(header):
    return '#include "' + header + '"\n'

def includeStd(header):
    return '#include <' + header + '>\n'

def genFileByTemplate(result, template, dictionary, headComment):
    outputFile = open(result, 'w')
    inputFile = open(template, 'r')
    outputFile.write(headComment)
    lines = inputFile.readlines()
    for line in lines:
        lineTemplate = string.Template(line)
        outputFile.write(lineTemplate.substitute(dictionary))
    inputFile.close()
    outputFile.close()

def readTemplate(fileName):
    inpFile = open(fileName, "r")
    text = inpFile.read()
    inpFile.close()
    return string.Template(text)
