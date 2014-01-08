splitterComment = '//////////////////////////'

def headerGuardTop(key):
    key = key.upper().replace(".", "_")
    return "#ifndef "+key+"_INCLUDED\n#define "+key+"_INCLUDED\n\n"

def headerGuardBottom(key):
    key = key.upper().replace(".", "_")
    return "#endif // "+key+"_INCLUDED\n"

def include(header):
    return '#include "' + header + '"\n'

def includeStd(header):
    return '#include <' + header + '>\n'
