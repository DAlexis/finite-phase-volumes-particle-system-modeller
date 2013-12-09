#!/usr/bin/python2.7

import xml.etree.ElementTree as ET

print "Code generation started"
print "Reading configuration..."

configurationTree = ET.parse('configs/sample.xml')
root = configurationTree.getroot()
print root.tag
print root.attrib

print "Code generation done."
