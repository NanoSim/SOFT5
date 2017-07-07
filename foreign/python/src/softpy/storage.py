# -*- coding: utf-8 -*-
from __future__ import absolute_import

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

    def save(self, entity, uuid=None):
        """Saves entity in this storage.  By default `entity` is stored
        under its own id, but by providing `uuid` you can store it under this
        id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)
        e = softpy.get_c_entity(entity)
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
            softpy.entity_store(e, datamodel)   # Ask the entity to fill out
                                                # the datamodel
            softpy.storage_strategy_store(self.strategy, datamodel)
            softpy.storage_strategy_free_datamodel(datamodel)

    def load(self, entity, uuid=None):
        """Fills `entity` with data loaded from this storage.  By default
        `entity` is loaded from its own id, but by providing `uuid` can load
        it from this id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)
        e = softpy.get_c_entity(entity)
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
            softpy.entity_load(e, datamodel)  # Ask entity to fill out itself
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
