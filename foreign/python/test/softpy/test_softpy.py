#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import sys
import unittest

import softpy


class TestSoftpy(unittest.TestCase):
    def test_softpy(self):
        softpy.init(sys.argv)
        storage = softpy.storage_create(
        'hdf5', 'teststorage.h5')
        strategy = softpy.storage_get_storage_strategy(storage)
        datamodel = softpy.storage_strategy_get_datamodel(strategy)
        a = 42
        b = 3.14
#        softpy.datamodel_append_int32(datamodel, 'foo', a)
        softpy.datamodel_set_id(datamodel, 'be2b1711-75b3-4923-9b02-c7464a092efb');
        softpy.datamodel_set_meta_name(datamodel, 'pytest')
        softpy.datamodel_set_meta_version(datamodel, '0.1-SNAPSHOT-1')
        softpy.datamodel_set_meta_namespace(datamodel, 'http://sintef.no/meta/soft');
        softpy.datamodel_append_double(datamodel, 'food', b)
        softpy.storage_strategy_store(strategy, datamodel)

if __name__ == "__main__":
    unittest.main()
