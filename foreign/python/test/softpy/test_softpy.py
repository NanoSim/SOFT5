#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import sys
import unittest

import softpy


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
        
if __name__ == "__main__":
    unittest.main()
