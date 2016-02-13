#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import sys
import softpy



e = softpy.entity_t(get_meta_name='MyStructure',
                    get_meta_version='0.1.1',
                    get_meta_namespace='http://sintef.no/meta/soft',
                    #get_dimensions=lambda self: ['N'],
                    get_dimensions=['N'],
                    get_dimension_size=[5],
                    #get_dimension_size=lambda self, label : 5,
                    load=lambda self, model: None,
                    store=lambda self, model: None)

#assert softpy.entity_get_meta_name(e) == 'MyStructure'
#assert softpy.entity_get_meta_version(e) == '0.1.1'
#assert softpy.entity_get_meta_namespace(e) == 'http://sintef.no/meta/soft'

assert e.name == 'MyStructure'
assert e.version == '0.1.1'
assert e.namespace == 'http://sintef.no/meta/soft'
assert softpy.entity_get_dimensions(e) == ['N']
assert softpy.entity_get_dimension_size(e, 'N') == 5


#del e
