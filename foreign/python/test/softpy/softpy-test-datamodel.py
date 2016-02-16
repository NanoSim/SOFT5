#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import sys
import unittest

import numpy as np

import softpy


class MyEntity(object):
    pass

m = MyEntity()
m._id = softpy.uuidgen()
m._name = 'mydata'
m._version = '0.1'
m._namespace = 'sintef.no/TR/metadata/test'

m.int32 = 42
m.double = 3.14
m.string = 'Hello world!'
m.int32_array = range(5)
m.double_array = np.arange(6.0)
m.string_list = ['this', 'is', 'a', 'test', 'string-list']

# Store
storage = softpy.storage_create('hdf5', 'softpy-test-datamodel.h5')
strategy = softpy.storage_get_storage_strategy(storage)
model = softpy.storage_strategy_get_datamodel(strategy)

softpy.datamodel_set_id(model, m._id)
softpy.datamodel_set_meta_name(model, m._name)
softpy.datamodel_set_meta_version(model, m._version)
softpy.datamodel_set_meta_namespace(model, m._namespace)

softpy.datamodel_append_int32(model, 'int32', m.int32)
softpy.datamodel_append_double(model, 'double', m.double)
softpy.datamodel_append_string(model, 'string', m.string)
softpy.datamodel_append_array_int32(
    model, 'int32_array', m.int32_array)
softpy.datamodel_append_array_double(
    model, 'double_array', m.double_array)
softpy.datamodel_append_string_list(
    model, 'string_list', m.string_list)

softpy.storage_strategy_store(strategy, model)
softpy.storage_free(storage)


# Load
storage = softpy.storage_create('hdf5', 'softpy-test-datamodel.h5')
strategy = softpy.storage_get_storage_strategy(storage)
model = softpy.storage_strategy_get_datamodel(strategy)

softpy.datamodel_set_id(model, m._id)
softpy.datamodel_set_meta_name(model, m._name)
softpy.datamodel_set_meta_version(model, m._version)
softpy.datamodel_set_meta_namespace(model, m._namespace)

softpy.storage_strategy_start_retrieve(strategy, model)

int32 = softpy.datamodel_get_int32(model, 'int32')
assert(int32 == m.int32)

double = softpy.datamodel_get_double(model, 'double')
assert(double == m.double)

string = softpy.datamodel_get_string(model, 'string')
assert(string == m.string)

int32_array = softpy.datamodel_get_array_int32(model, 'int32_array')
assert(np.all(int32_array == m.int32_array))

double_array = softpy.datamodel_get_array_double(model, 'double_array')
assert(np.all(double_array == m.double_array))

string_list = softpy.datamodel_get_string_list(model, 'string_list')
assert(string_list == m.string_list)

softpy.storage_strategy_end_retrieve(strategy, model)
softpy.storage_free(storage)
