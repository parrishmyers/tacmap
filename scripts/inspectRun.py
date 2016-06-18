#!/usr/bin/env python
import sys
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from graphviz import Digraph

class Points:
    index = []
    addr = []
    x = []
    y = []
    z = []
    def add(self,i,a,l):
        self.index.append(i)
        self.addr.append(a)
        self.x.append(l[0])
        self.y.append(l[1])
        self.z.append(l[2])
    def find(self,addr):
        for i in range(0,len(addr)):
            if addr == self.addr[i]:
                return (self.x[i],self.y[i])
        return None

class DAGViewer:
    def __init__(self):
        self.POINTS = Points()
        self.index = -1
        self.current_point = None
        self.actions = []
        self.tree_before = None
        self.tree_after = None

    def extractDag(self, name, tree):
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

    def plotStep(self):
        name = 'dag_before_{:04d}'.format(self.index)
        self.extractDag(name,self.tree_before)
        img = mpimg.imread(name + '.png')
        plt.subplot(2, 1, 2)
        plt.imshow(img)

        plt.subplot(2, 1, 1)
        plt.scatter(self.POINTS.x, self.POINTS.y, c='blue', s=100,
                    alpha=0.3, edgecolors='none')

        xy = self.POINTS.find(self.current_point)
        if None != xy:
            plt.scatter(xy[0],xy[1],c='red',s=100,edgecolors='none')

        for triangle in self.tree_before:
            if triangle['valid'] == 1:
                x = []
                y = []
                pts = triangle['triangle']
                for p in pts:
                    xyz = p['location']
                    x.append(xyz[0])
                    y.append(xyz[1])
                x.append(x[0])
                y.append(y[0])
                plt.plot(x, y, '-', linewidth=1)
        plt.grid(True)

        plt.show()


def stepThroughRun(runfh):
    dagv = DAGViewer()
    for line in runfh:
        l = line.strip()
        if 0 < len(l):
            step = eval(line)
            if step['step'] == 'top':
                dagv.index = step['loop']
                dagv.current_point = step['point']
            elif step['step'] == 'bottom':
                dagv.plotStep()
            elif step['step'] == 'divide' or step['step'] == 'flip':
                dagv.actions.append(step['data'])
            elif step['step'] == 'dag':
                name = step['name']
                if 'before' == name:
                    dagv.tree_before = step['tree']
                else:
                    dagv.tree_after = step['tree']
            elif step['step'] == 'points':
                index = int(step['index'])
                point = step['data']
                dagv.POINTS.add(index,point['addr'],point['location'])

if __name__ == '__main__':
    runfile = 'run1.txt'

    with open(runfile,'r') as runfh:
        stepThroughRun(runfh)
        runfh.close()