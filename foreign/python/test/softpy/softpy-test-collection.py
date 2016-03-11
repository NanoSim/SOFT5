#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import softpy


thisdir = os.path.normpath(os.path.abspath(os.path.dirname(__file__)))

Extraction = softpy.entity(os.path.join(
    thisdir, 'eu.nanosim.vasp.extraction.json'))
e = Extraction()    

c = softpy.collection_create()

assert softpy.collection_num_entities(c) == 0


softpy.collection_register_entity(e)



assert softpy.collection_num_dims(c) == 0
softpy.collection_add_dim(c, 'newdim', 'Description of newdim...')
#assert softpy.collection_num_dims(c) == 1

softpy.collection_free(c)
