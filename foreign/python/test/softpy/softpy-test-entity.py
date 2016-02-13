#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import softpy



e = softpy.entity_t(get_meta_name=lambda : 'MyStructure',
                    get_meta_version=lambda : '0.1.1',
                    get_meta_namespace=lambda : 'http://sintef.no/meta/soft',
                    get_dimensions=lambda self: ['N'],
                    get_dimension_size=lambda self, label : 5,
                    load=lambda self, model: None,
                    store=lambda self, model: None)
e.set_self(e)

#assert softpy.entity_get_meta_name(e) == 'MyStructure'
#assert softpy.entity_get_meta_version(e) == '0.1.1'
#assert softpy.entity_get_meta_namespace(e) == 'http://sintef.no/meta/soft'

print(softpy.entity_get_meta_name(e))

print(softpy.entity_get_dimensions(e))
print(softpy.entity_get_dimension_size(e, 'N'))

del e
