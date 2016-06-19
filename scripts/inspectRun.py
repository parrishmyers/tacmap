#!/usr/bin/env python
import sys
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.animation as animation
from graphviz import Digraph

class Points:
    index = []
    addr = []
    x = []
    y = []
    z = []
    def add(self,data):
        self.index.append(data['index'])
        self.addr.append(data['data']['addr'])
        self.x.append(data['data']['location'][0])
        self.y.append(data['data']['location'][1])
        self.z.append(data['data']['location'][2])
    def find(self,addr):
        for i in range(0,len(self.addr)):
            if addr == self.addr[i]:
                return (self.x[i],self.y[i])
        return None

class Triangle:
    addr = None
    valid = False
    point_addr = []
    x = []
    y = []
    z = []
    def __init__(self,data):
        self.addr = data['addr']
        v = data['valid']
        if 1 == v:
            self.valid = True
        tri = data['triangle']
        for t in tri:
            self.point_addr.append(t['addr'])
            l = t['location']
            self.x.append(l[0])
            self.y.append(l[1])
            self.z.append(l[2])
        self.x.append(self.x[0])
        self.y.append(self.y[0])
        self.z.append(self.z[0])
        self.point_addr.append(self.point_addr[0])

class DivideStep:
    type = None
    orig = []
    new = []
    def __init__(self,data):
        self.type = data['type']
        for t in data['orig']:
            if None == t:
                orig.append(None)
            else:
                orig.append(Triangle(t))
        for t in data['new']:
            if None == t:
                new.append(None)
            else:
                new.append(Triangle(t))

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
    plt.scatter(points.x, points.y, c='blue', s=100,
                alpha=0.3, edgecolors='none')

    if None != current_point:
        plt.scatter(current_point[0], current_point[1],
                    c='red', s=100, edgecolors='none')

    for triangle in current_tree:
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


def processData(dta):
    points = Points()
    loop = -1
    current_tree = None
    current_point = None
    current_dag_name = None
    for s in dta:
        if s['step'] == 'point':
            points.add(s)
        elif s['step'] == 'top':
            plt.clf()
            loop = s['loop']
            current_point = points.find(s['point'])
            fig.canvas.set_window_title('STEP: {}'.format(loop))
        elif s['step'] == 'dag':
            current_tree = s['tree']
            current_dag_name = 'dag_{}_{:04d}'.format(s['name'], loop)
            genDag(current_dag_name, current_tree)
            plotFrames(current_dag_name,
                       points,
                       current_point,
                       current_tree)
            yield True
        elif s['step'] == 'divide':
            raw_input('press to step')
            divide = DivideStep(data['data'])
            for n in divide.new:
                if None != n:
                    plt.plot(n.x, n.y, '-', linewidth=2)
            yield True

def processLine(runfh):
    for line in runfh:
        l = line.strip()
        if 0 < len(l):
            step = eval(line)
            yield step

if __name__ == '__main__':
    runfile = 'run1.txt'

    data = []
    with open(runfile,'r') as runfh:
        for l in processLine(runfh):
            data.append(l)
        runfh.close()

        fig, ax = plt.subplot()
        ani = animation.FuncAnimation(fig, run, data_gen, blit)