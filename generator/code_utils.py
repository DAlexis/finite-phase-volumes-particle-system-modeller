import string
import re

splitterComment = '//////////////////////////'

def formHeaderGuard(headerName):
    return headerName.upper().replace(".", "_").replace("-", "_") + '_INCLUDED'

def headerGuardTop(key):
    guard = formHeaderGuard(key)
    return "#ifndef "+guard+"_INCLUDED\n#define "+guard+"\n\n"

def headerGuardBottom(key):
    guard = formHeaderGuard(key)
    return "#endif // "+guard+"\n"

def include(header):
    return '#include "' + header + '"\n'

def includeStd(header):
    return '#include <' + header + '>\n'

def indentCode(code, indentation):
    result = re.sub(r'\n', '\n' + indentation, code)
    result = indentation + result;
    return result

def createEnum(name, items):
    result = "enum " + name + "\n{\n"
    isFirst = True
    for item in items:
        result = result + "    " + item;
        if isFirst:
            result = result + " = 0"
            isFirst = False
        result = result + ",\n"
    result = result + "};\n"
    return result

def createSwitch(variable, cases):
    result = ""
    if cases:
        result = "switch(" + variable + ") {\n"
        for caseName in cases:
            result = result + "    case " + caseName + ": {\n"
            result = result + indentCode(cases[caseName], "        ")
            result = result + "\n    } break;\n"
        result = result + "}"
    return result

def genFileByTemplate(result, template, dictionary, headComment):
    """
    Generate file with name result from a template stored it template,
    substituting using dictionary. Result file will be also beginned 
    with test from headComment.
    """
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
    """
    Simply read template's text fromfile and return this template
    """
    inpFile = open(fileName, "r")
    text = inpFile.read()
    inpFile.close()
    return string.Template(text)
