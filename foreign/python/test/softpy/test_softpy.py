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
            'hdf5', 'file://teststorage.json')
        strategy = softpy.storage_get_storage_strategy(storage)
        datamodel = softpy.storage_strategy_get_datamodel(strategy)
        softpy.storage.write


if __name__ == "__main__":
    unittest.main()
