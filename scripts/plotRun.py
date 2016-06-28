#!/usr/bin/env python

import fnmatch
import json
import os
import os.path
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.animation as animation
import numpy as np
from graphviz import Digraph

PATH = '../DerivedData/tacmap/Build/Products/Debug'

class Points:
    def __init__(self):
        self.addr = []
        self.x = None
        self.y = None
        self.z = None

    def parse(self, points):
        self.x = np.zeros(len(points))
        self.y = np.zeros(len(points))
        self.z = np.zeros(len(points))
        i = 0
        for p in points:
            self.addr.append(p['addr'])
            self.x[i] = p['point'][0]
            self.y[i] = p['point'][1]
            self.z[i] = p['point'][2]
            i += 1

    def find(self,addr):
        for i in range(0,len(self.addr)):
            if addr == self.addr[i]:
                return self.x[i], self.y[i], self.z[i]
        return None

POINTS = Points()
STEP = {}
LOOP = {}

def genDag(name, tree):
    dot = Digraph(comment='Dag')

    i = 0
    for triangle in tree:
        addr = triangle['addr']
        valid = triangle["valid"]
        dot.node(addr,'{}({})'.format(addr, valid))
        i += 1

    for triangle in tree:
        addr = triangle['addr']
        children = triangle['children']
        for c in children:
            if None != c:
                dot.edge(addr, c)

    dot.format = 'png'
    dot.render(name)
    return name

def plotFrames(name, points, current_point, current_tree):
    plt.subplot(2,1,2)
    img = mpimg.imread(name + '.png')
    plt.imshow(img)

    plt.subplot(2,1,1)
    plt.title(name)
    plt.scatter(points.x, points.y, c='blue', s=100,
                alpha=0.3, edgecolors='none')

    if None != current_point:
        plt.scatter(current_point[0], current_point[1],
                    c='red', s=100, edgecolors='none')

    for triangle in current_tree:
        if True == triangle['valid']:
            x = []
            y = []
            pts = triangle['triangle']
            for p in pts:
                xyz = POINTS.find(p)
                x.append(xyz[0])
                y.append(xyz[1])
            x.append(x[0])
            y.append(y[0])
            plt.plot(x, y, '-', linewidth=1)
    plt.grid(True)
    plt.show()

def plotError(name, points, current_point, current_tree):
    plt.subplot(1,1,1)
    plt.title(name)
    plt.scatter(points.x, points.y, c='blue', s=100,
                alpha=0.3, edgecolors='none')

    if None != current_point:
        plt.scatter(current_point[0], current_point[1],
                    c='red', s=100, edgecolors='none')

    for triangle in current_tree:
        if True == triangle['valid']:
            x = []
            y = []
            pts = triangle['triangle']
            for p in pts:
                xyz = POINTS.find(p)
                x.append(xyz[0])
                y.append(xyz[1])
            x.append(x[0])
            y.append(y[0])
            plt.plot(x, y, '-', linewidth=1)
    plt.grid(True)
    plt.show()

for file in os.listdir(PATH):
    if fnmatch.fnmatch(file, '*.json'):
        parts = file.replace('.json','')
        parts = parts.split('_')
        if parts[0] == 'points':
            fh = open(os.path.join(PATH,file),'r')
            POINTS.parse( json.load(fh) )
            fh.close()
        if parts[0] == 'loop':
            fh = open(os.path.join(PATH,file),'r')
            LOOP[parts[1]] = json.load(fh)
            fh.close()
        if parts[0] == 'step':
            fh = open(os.path.join(PATH, file), 'r')
            if (parts[2] == 'error'):
                error = json.load(fh)
                plotError("error",POINTS,POINTS.find(error['point']['addr']), error['error'])
                fh.close()
                break;
            else:
                STEP[parts[1]] = json.load(fh)
                fh.close()
        if parts[0] == 'solution':
            fh = open(os.path.join(PATH, file), 'r')
            dag = json.load(fh)
            fh.close()
            genDag("triangulation", dag)
            plotFrames("triangulation",POINTS,None,dag)

STEP_KEYS = STEP.keys()
STEP_KEYS.sort()

for k in STEP_KEYS:
    step = STEP[k]
    current_point = POINTS.find(step[0]['point']['addr'])

    for s in step:
        name = s['name']
        type = s['type']
        name1 = 'step_{}_{}_{}'.format(k,name,type)
        genDag(name1, s['dag'])
        plotFrames(name1,POINTS,current_point, s['dag'])
