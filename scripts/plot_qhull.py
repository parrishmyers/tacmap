#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np

fh = open('rbox.txt','r')
header = fh.readline()
num_points = int ( fh.readline() )
data = np.zeros((num_points,2))

i = 0
for l in fh:
    ll = l.strip().split()
    data[i,0] = float(ll[0])
    data[i,1] = float(ll[1])
    i += 1
fh.close()

fh = open('result.txt','r')
num_points = int( fh.readline() )
tri = np.zeros((num_points,3)).astype(int)
i = 0
for l in fh:
    ll = l.strip().split()
    tri[i,0] = int(ll[0])
    tri[i,1] = int(ll[1])
    tri[i,2] = int(ll[2])
    i += 1
fh.close()

print data
print tri

plt.plot(data[:,0],data[:,1], 'r.')
for t in tri:
    x = np.zeros((4,1))
    y = np.zeros((4, 1))

    x[0] = data[t[0], 0]
    x[1] = data[t[1], 0]
    x[2] = data[t[2], 0]
    x[3] = data[t[0], 0]

    y[0] = data[t[0], 1]
    y[1] = data[t[1], 1]
    y[2] = data[t[2], 1]
    y[3] = data[t[0], 1]

    plt.plot(x,y,'b-')
plt.show()