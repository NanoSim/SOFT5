#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import softpy

strlist = softpy.string_list_create()
softpy.string_list_append_cstr(strlist, 'first')
softpy.string_list_append_cstr(strlist, 'second')
softpy.string_list_append_cstr(strlist, 'third')

assert softpy.string_list_count(strlist) == 3
assert softpy.from_softc_string(softpy.string_list_first(strlist)) == 'first'
assert softpy.from_softc_string(softpy.string_list_last(strlist)) == 'third'
assert softpy.from_softc_string(softpy.string_at(strlist, 0)) == 'first'
assert softpy.from_softc_string(softpy.string_at(strlist, 1)) == 'second'
assert softpy.from_softc_string(softpy.string_at(strlist, 2)) == 'third'
