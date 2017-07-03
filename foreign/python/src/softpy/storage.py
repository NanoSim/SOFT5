# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

import os
import json

from . import softpy
from .softpy import asStr, asBytes
from .errors import SoftError


class SoftStorageClosedError(SoftError):
    """Raised when working on a closed storage."""
    pass


class Storage(object):
    """Class for connecting to a storage.

    Parameters
    ----------
    driver : "json" | "hdf5" | "mongo"...
        The storage driver to use.  Available drivers can be obtained from
        softpy.get_storage_drivers().
    uri : string
        Storage uri.  For json or hdf5 this might be a file name, but might
        be a url for database connections.
    options : None | string
        Additional options passed to the driver.

    Notes
    -----
    Options for the default drivers:

    hdf5 : "append=yes"  - append to the hdf5 file, otherwise truncate.
    """
    def __init__(self, driver, uri, options=None):
        self.driver = driver
        self.uri = uri
        self.options = options
        self.storage = softpy.storage_create(driver, uri, options)
        self.strategy = softpy.storage_get_storage_strategy(self.storage)
        self._closed = False

    def save(self, instance, uuid=None):
        """Saves instance in this storage.  By default `instance` is stored
        under its own id, but by providing `uuid` you can store it under this
        id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)

        # FIXME - hack for json storage, remove then it is implemented in core
        if self.driver == 'json':
            return json_hack_save(self, instance, uuid)

        e = softpy.get_c_entity(instance)
        if isinstance(e, softpy.collection_s):
            softpy.storage_save(self.storage, e)
        else:
            datamodel = softpy.storage_strategy_get_datamodel(self.strategy)
            softpy.datamodel_set_id(
                datamodel, asStr(uuid if uuid else e.id))
            softpy.datamodel_set_meta_name(
                datamodel, asStr(softpy.get_meta_name(e)))
            softpy.datamodel_set_meta_version(
                datamodel, asStr(softpy.get_meta_version(e)))
            softpy.datamodel_set_meta_namespace(
                datamodel, asStr(softpy.get_meta_namespace(e)))
            softpy.entity_store(e, datamodel)   # Ask the instance to fill out
                                                # the datamodel
            softpy.storage_strategy_store(self.strategy, datamodel)
            softpy.storage_strategy_free_datamodel(datamodel)

    def load(self, instance, uuid=None):
        """Fills `instance` with data loaded from this storage.  By default
        `instance` is loaded from its own id, but by providing `uuid`
        you can load it from this id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)

        # FIXME - hack for json storage, remove then it is implemented in core
        if self.driver == 'json':
            return json_hack_load(self, instance, uuid)

        e = softpy.get_c_entity(instance)
        if isinstance(e, softpy.collection_s):
            softpy.collection_load(self.storage, e)
        else:
            datamodel = softpy.storage_strategy_get_datamodel(self.strategy)
            softpy.datamodel_set_id(
                datamodel, asStr(uuid if uuid else e.id))
            softpy.datamodel_set_meta_name(
                datamodel, asStr(softpy.get_meta_name(e)))
            softpy.datamodel_set_meta_version(
                datamodel, asStr(softpy.get_meta_version(e)))
            softpy.datamodel_set_meta_namespace(
                datamodel, asStr(softpy.get_meta_namespace(e)))
            softpy.storage_strategy_start_retrieve(self.strategy, datamodel)
            softpy.entity_load(e, datamodel)  # Ask instance to fill out itself
            softpy.storage_strategy_end_retrieve(self.strategy, datamodel)
            softpy.storage_strategy_free_datamodel(datamodel)

    def close(self):
        """Closes current storage."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)
        softpy.storage_free_storage_strategy(self.strategy)
        softpy.storage_free(self.storage)
        self._closed = True

    closed = property(lambda self: self._closed,
                      doc='Whether current storage is closed.')

    def __delete__(self):
        if not self.closed:
            self.close()
        object.__del__(self)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def __repr__(self):
        if self.options:
            return "Storage(%r, %r, %r)" % (
                self.driver, self.uri, self.options)
        else:
            return "Storage(%r, %r)" % (self.driver, self.uri)



#=======================================================================
# json hacks
#=======================================================================

def json_hack_save(storage, instance, uuid=None):
    """Stores `instance` to `storage`.

    If `uuid` is provided, the instance is stored with this uuid, otherwise
    it is stored with its own uuid.

    Recognised options are:
      append : whether to append to storage instead of overwriting the file [yes]
      indent : non-negative integer, level of indentation [2]
      sort : whether to sort the output by key [no]
    """
    assert storage.driver == 'json'
    optdict = _parse_options(storage.options)
    if istrue(optdict.get('append', 'yes')) and os.path.exists(storage.uri):
        with open(storage.uri, 'r') as f:
            d = json.load(f)
    else:
        d = {}

    if isinstance(instance, Collection):
        uuid = uuid if uuid else instance.uuid
        d[uuid] = instance.soft_as_dict()
        for label in instance.get_labels():
            ent = instance.get_instance(label)
            d[instance.get_uuid(label)] = ent.soft_as_dict()
    elif isinstance(instance, BaseEntity):
        uuid = uuid if uuid else instance.soft_get_id()
        d[uuid] = instance.soft_as_dict()
    elif isinstance(instance, Metadata):
        uuid = uuid if uuid else instance.get_uuid()
        dd = dict(instance)
        dd['meta'] = {'name': 'entity_schema', 'version': '0.3',
                      'namespace': 'org.sintef.soft'}
        d[uuid] = dd
    else:
        raise TypeError('`instance` must be an Entity, Collection or Metadata')

    with open(storage.uri, 'w') as f:
        json.dump(d, f, indent=int(optdict.get('indent', 4)),
                  sort_keys=istrue(optdict.get('sort', 'no')))


def json_hack_load(storage, instance, uuid=None):
    """Populates `instance` from `storage`.

    If `uuid` is provided, the instance is loaded from this uuid, otherwise
    it is loaded from its own uuid.
    """
    assert storage.driver == 'json'
    with open(storage.uri) as f:
        d = json.load(f)

    e = softpy.get_c_entity(instance)
    if not uuid:
        uuid = softpy.get_id(e)

    name = softpy.get_meta_name(e)
    version = softpy.get_meta_version(e)
    namespace = softpy.get_meta_namespace(e)
    meta = d[uuid]['meta']
    if (    meta['name'] != name or
            meta['version'] != version or
            meta['namespace'] != namespace):
        raise SoftError('Cannot load %s/%s-%s into a %s/%s-%s' % (
            meta['name'], meta['version'], meta['namespace'],
            name, version, namespace))

    if isinstance(e, softpy.collection_s):
        instance.soft_from_dict(d[uuid], storage.driver, storage.uri, storage.options)
    elif isinstance(e, softpy.entity_t):
        instance.soft_from_dict(d[uuid])
    else:
        raise TypeError(
            '`instance` must be an Entity or Collection, got %r' % type(instance))

# FIXME - get rid to these cyclic imports
from .collection import Collection
#from .entity import BaseEntity
from .entity import entity


def _parse_options(options):
    """Parses options string and returns is as a dict."""
    d = {}
    if options:
        for opt in options.split('&'):
            k, v = opt.split('=', 1)
            d[k] = v
    return d


def istrue(value):
    """Returns false if the string `value` corresponds to false, otherwise
    true is returned.  The following values are considered false:
    "false", "no", "0", "off", ".false.".
    """
    if value.lower() in ('false', 'no', '0', 'off', '.false.'):
        return False
    else:
        return True
