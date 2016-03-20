#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import softpy

import numpy as np


metadata = """
{
    "name": "Structure",
    "version": "0.1",

    "namespace": "http://sintef.no/meta/soft",
    "description": "Incomplete description of an atomic structure",
    "dimensions": [
        {
            "name": "N",
            "description": "Number of atoms"
        }
    ],
    "properties": [
        {
            "name": "structure",
            "type": "string",
            "description": "Name of structure"
        },
        {
            "name": "symbols",
            "type": "string",
            "dims": [
                "N"
            ],
            "description": "List of atomic symbols"
        },
        {
            "name": "spacegroup",
            "type": "int32",
            "description": "Spacegroup number in International Tables"
        },
        {
            "name": "total_energy",
            "type": "double",
            "unit": "eV",
            "description": "Total energy of the system"
        },
        {
            "name": "masses",
            "type": "double",
            "unit": "u",
            "dims": [
                "N"
            ],
            "description": "Mass of each atom"
        }
    ]
}
"""

# Create new Structure entity based on the metadata
Structure = softpy.entity(metadata)

# Create a empty Structure instance
s = Structure(dimensions=(3, ))
s.structure = 'water'
s.symbols = ['H', 'H', 'O']
s.spacegroup = 1
s.total_energy = 446.32
s.masses = np.array([1., 1., 16.])

# Write the structure to file
with softpy.Storage('hdf5', 'softpy-test-factory.h5') as storage:
    storage.save(s)


# Create new Structure instance initiated from file
s2 = Structure(uuid=s._id, driver='hdf5', uri='softpy-test-factory.h5')
#
#
## Check that the entities are equals
#for name in '_id', '_name', '_version', '_namespace':
#    assert getattr(s, name) == getattr(s2, name)
#    
#assert s._keys() == s2._keys()
#
#for key in s._keys():
#    if isinstance(s[key], np.ndarray):
#        assert s[key].all() == s2[key].all()
#    else:
#        assert s[key] == s2[key]
#

