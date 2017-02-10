#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import softpy

s = softpy.string_create('Hello world!')
assert softpy.from_softc_string(s) == 'Hello world!'

#assert softpy.string_compare(s, 'Hello world!') == 0
#assert softpy.string_compare(s, 'Another value...') != 0

#softpy.string_assign(s, 'A new value...')
#assert softpy.from_softc_string(s) == 'A new value...'

#assert softpy.string_compare(s, 'A new value...') == 0
#assert softpy.string_compare(s, 'Another value...') != 0

#s2 = softpy.string_create('A new value...')
#assert softpy.string_isequal(s, s2) == False
#
#s3 = softpy.string_create('Hello world!')
#assert softpy.string_isequal(s, s3) == True

#softpy.string_assign(s, 'A different value...')
#assert softpy.string_isequal(s, s2) == False

softpy.string_destroy(s)
#softpy.string_destroy(s2)
#softpy.string_destroy(s3)
