#!/usr/bin/env python

import fnmatch
import json
import os
import os.path

PATH = '../DerivedData/tacmap/Build/Products/Debug'

POINTS = []
STEP_PRE = {}
STEP_POST = {}
LOOP = {}

for file in os.listdir(PATH):
    if fnmatch.fnmatch(file, '*.json'):
        parts = file.replace('.json','')
        parts = parts.split('_')
        if parts[0] == 'points':
            fh = open(os.path.join(PATH,file),'r')
            POINTS = json.load(fh)
            fh.close()
        if parts[0] == 'loop':
            fh = open(os.path.join(PATH,file),'r')
            LOOP[parts[1]] = json.load(fh)
            fh.close()
        if parts[0] == 'step':
            fh = open(os.path.join(PATH, file), 'r')
            if 'pre' == parts[2]:
                STEP_PRE[parts[1]] = json.load(fh)
            else:
                STEP_POST[parts[1]] = json.load(fh)
            fh.close()

KEYS_PRE = STEP_PRE.keys()
KEYS_PRE.sort()

KEYS_POST = STEP_POST.keys()
KEYS_POST.sort()

for k,l in zip(KEYS_PRE,KEYS_POST):
    data_pre = STEP_PRE[k]
    data_post = STEP_POST[l]

    print data_pre
    print data_post
    print ''