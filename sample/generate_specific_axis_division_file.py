#!/usr/bin/python2.7

import math

result = ""
last = -10.0
for i in range(1, 50):
    result = result + ' ' + str(last)
    last = last + 0.801 + 0.8*math.sin(i/10.0*3.14)
print result
