#!/usr/bin/env python

import json

with open('../DerivedData/tacmap/Build/Products/Debug/points.txt','r') as jsonfh:
    points = json.load(jsonfh)
    for p in points:
        print p
    jsonfh.close()

with open('../DerivedData/tacmap/Build/Products/Debug/loop_0.txt','r') as jsonfh:
    dag = json.load(jsonfh)



    jsonfh.close()