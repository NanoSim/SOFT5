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
jack = Person(name='Jack', age=42, skills=[])
alda = Person(name='Alda', age=39, skills=['foo'])
fred = Person(name='Fred', age=12, skills=['bar'])
coll.register_entity('Jack', jack)
coll.register_entity('Alda', alda)
coll.register_entity('Fred', fred)
assert coll.get_num_entities() == 3

coll.add_relation('Jack', 'loves', 'Alda')
coll.add_relation('Jack', 'loves', 'Fred')
coll.add_relation('Jack', 'husband_of', 'Alda')
coll.add_relation('Jack', 'father_of', 'Fred')

assert coll.find_relations('Jack', 'loves') == {'Fred', 'Alda'}
assert coll.find_relations('Alda', '^loves') == {'Jack'}
assert coll.find_relations('Jack', 'husband_of') == {'Alda'}
assert coll.find_relations('Fred', '^father_of') == {'Jack'}

assert coll.get_labels() == {'Alda', 'Fred', 'Jack'}
assert coll.get_uuid('Fred') == fred.soft_get_id()
assert coll.get_name('Fred') == 'Person'
assert coll.get_version('Fred') == '0.2.4'
assert coll.get_namespace('Fred') == 'http://sintef.no/meta/soft'


# Create and register metadata dababase and insert Person
jsondb = softpy.JSONMetaDB('metadb.json')
softpy.register_metadb(jsondb)
jsondb.clear()
jsondb.insert(Person)


#with softpy.Storage('hdf5', 'softpy-test-collection.h5') as s:
#    s.save(jack)
#    s.save(fred)
#    s.save(alda)

#e = coll.get_entity('Fred', 'hdf5', 'softpy-test-collection.h5')
