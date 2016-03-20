#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)


with open(os.path.join(thisdir, 'testentity.json')) as f:
    metadata = f.read()
Extraction = softpy.entity(metadata)

e = Extraction(dimensions=[3])


coll = softpy.Collection()
assert len(coll) == 0
coll['extraction1'] = e
#assert len(coll) == 1
