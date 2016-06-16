#!/usr/bin/env python
import sys
from graphviz import Digraph

def extractTop(data):
    return data['loop']

def extractDag(data):
    name = data['name']
    tree = data['tree']
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

    dot.render('{}.gv'.format(name),view=True)
    return name

def extractDivide(data):
    return data['type'], data['orig'], data['new']

def stepThroughRun(runfh):
    inStep = False
    for line in runfh:
        l = line.strip()
        if 0 == len(l):
            inStep = False
        else:
            step = eval(line)
            if step['step'] == 'top':
                print extractTop(step)
            elif step['step'] == 'divide':
                print extractDivide(step)
            elif step['step'] == 'dag':
                print extractDag(step)

if __name__ == '__main__':
    runfile = 'run1.txt'

    with open(runfile,'r') as runfh:
        stepThroughRun(runfh)
        runfh.close()