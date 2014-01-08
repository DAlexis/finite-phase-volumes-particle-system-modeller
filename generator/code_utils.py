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
