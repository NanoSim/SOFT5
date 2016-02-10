#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import sys
import unittest

import numpy as np

import softpy


# Wrap softpy-test-datamodel in a TestCase
def test_datamodel():
    execfile(os.path.join(os.path.dirname(__file__), 
                          'softpy-test-datamodel.py'))
TestDatamodel = unittest.FunctionTestCase(test_datamodel)



class TestSoftpy(unittest.TestCase):

    def test_softpy(self):
        storage = softpy.storage_create(
            'hdf5', 'teststorage.h5')
        strategy = softpy.storage_get_storage_strategy(storage)
        datamodel = softpy.storage_strategy_get_datamodel(strategy)
        softpy.datamodel_set_id(datamodel, softpy.uuidgen())
        softpy.datamodel_set_meta_name(datamodel, 'pytest')
        softpy.datamodel_set_meta_version(datamodel, '0.1-SNAPSHOT-1')
        softpy.datamodel_set_meta_namespace(
            datamodel, 'http://sintef.no/meta/soft');
        softpy.datamodel_append_int32(datamodel, 'foo', 42)
        softpy.datamodel_append_double(datamodel, 'food', 3.14)
        softpy.storage_strategy_store(strategy, datamodel)

        self.assertEqual(softpy.datamodel_get_meta_name(datamodel), 'pytest')
        self.assertEqual(softpy.datamodel_get_meta_version(datamodel),
                         '0.1-SNAPSHOT-1')
        self.assertEqual(softpy.datamodel_get_meta_namespace(datamodel),
                         'http://sintef.no/meta/soft')
        #v = softpy.datamodel_get_double(datamodel, 'food')
        #print('*** food =', v)

    def test_manual_initialization(self):
        metafile = os.path.join(
            os.path.dirname(__file__), 'eu.nanosim.vasp.extraction.json')

        Extraction = softpy.entity(open(metafile))
        e = Extraction(dimensions={'nAtoms': 5})

        nAtoms = e._get_dimension_size('nAtoms')
        assert(nAtoms == 5)

        e.surface_name = '111'
        e.atoms = "CH4"
        e.atom_species = 'H C'
        e.state = 'surface'
        e.site_name = 'on_top'
        e.total_energy = 543.2
        e.frequencies = 0.0  # XXX
        e.cell = 4.05 * np.eye(3).flatten()
        e.symbols = ['H', 'H', 'H', 'H', 'C']
        e.positions = np.random.rand(nAtoms, 3).flatten()
        self.assertFalse(e._initialized())
        e.info = ''
        self.assertTrue(e._initialized())
        self.assertTrue('cell' in e)
        self.assertFalse('cellx' in e)

        with softpy.Storage('hdf5', 'extraction.h5') as s:
            s.save(e)


        
if __name__ == "__main__":
    unittest.main()