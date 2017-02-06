#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)


coll = softpy.Collection()
assert coll.get_num_entities() == 0
assert coll.get_num_relations() == 0

coll.name = 'foo'
assert coll.name == 'foo'

coll.version = '1.0'
assert coll.version == '1.0'


Person = softpy.entity(open(os.path.join(thisdir, 'person.json')))
jack = Person(name='Jack', age=42)
alda = Person(name='Alda', age=39)
fred = Person(name='Fred', age=12)
coll.register_entity('Jack', jack)
coll.register_entity('Alda', alda)
coll.register_entity('Fred', fred)
assert coll.get_num_entities() == 3

coll.add_relation('Jack', 'loves', 'Alda')
coll.add_relation('Jack', 'loves', 'Fred')
coll.add_relation('Jack', 'husband_of', 'Alda')
coll.add_relation('Jack', 'father_of', 'Fred')
#assert coll.get_num_relations() == 2  # ??? what is this supposed to return?

lst = coll.find_relations('Jack', 'loves')
assert lst == set(['Fred', 'Alda'])
