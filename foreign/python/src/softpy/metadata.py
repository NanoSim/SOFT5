# -*- coding: utf-8 -*-
from __future__ import absolute_import

import sys
import os
import json
import warnings
import re
import fnmatch
from glob import glob

from . import softpy
from .softpy import asStr, asBytes
from .errors import SoftError


class SoftMissingMetadataError(SoftError):
    """Raised when metadata cannot be found in the database."""
    pass


class Metadata(dict):
    """A class representing SOFT metadata.

    Parameters
    ----------
    name : str | Metadata | Entity | Entity instance | file-like | dict | tuple
        If `version` and `namespace` are given, this is the metadata name.

        Otherwise, this is a full description of the metadata in one of
        following forms:
          - Metadata object or dict
          - SOFT entity or entity instance
          - file-like object with a read() method with the metadata in json-
            format.
          - string with the metadata in json-format
          - tuple with (name, version, namespace). The full description is
            looked up in the metadata database.
    version : None | str
        Metadata version or None if `name` provides full description
        of the metadata.
    namespace : None | str
        Metadata namespace or None if `name` provides full description
        of the metadata."""
    def __init__(self, name, version=None, namespace=None):
        if version is None or namespace is None:
            if hasattr(name, 'soft_metadata'):
                d = name.soft_metadata
            elif hasattr(name, 'read'):
                d = json.load(name)
            elif isinstance(name, str):
                d = json.loads(name)
            elif sys.version_info.major == 2 and isinstance(name, unicode):
                d = json.loads(name.decode('utf8'))
            elif isinstance(name, dict):
                d = name
            elif isinstance(name, tuple):
                name, version, namespace = name
                d = find_metadata(name, version, namespace)
            else:
                raise TypeError(
                    'Cannot convert %s to metadata' % (type(name),))
        else:
            d = find_metadata(name, version, namespace)
        self.update(d)
        # A kind of hack, that automatically adds all metadata to the
        # metadata database
        if not self in metaDB:
            dict.__setitem__(metaDB, self.get_uuid(), self)

    def __str__(self):
        return self.get_json()

    def __repr__(self):
        return 'Metadata(%s)' % self.get_json()

    def __hash__(self):
        return hash(self.mtype)

    def __eq__(self, other):
        return self.mtype == other.mtype

    name = property(lambda self: asStr(self['name']),
                    doc='Metadata name.')
    version = property(lambda self: asStr(self['version']),
                       doc='Metadata version.')
    namespace = property(lambda self: asStr(self['namespace']),
                         doc='Metadata namespace.')
    description = property(lambda self: asStr(self['description']),
                           doc='Description of this metadata.')
    mtype = property(lambda self: (self.name, self.version, self.namespace),
                     doc='A (name, version, namespace)-tuple uniquely '
                         'identifying the metadata.')
    dimensions = property(lambda self: [
        str(asStr(d['name'])) for d in self['dimensions']],
                          doc='List of dimension labels.')
    property_names = property(lambda self: [
            asStr(p['name']) for p in self['properties']],
                              doc='List of property names.')

    def get_uuid(self):
        """Returns an UUID generated from a MD5 hash of the metadata name,
        version and namespace."""
        return softpy.uuid_from_entity(self.name, self.version, self.namespace)

    def get_json(self, indent=2, sort_keys=True):
        """Returns a json string representing this metadata."""
        return json.dumps(self, indent=indent, sort_keys=sort_keys)



#
# Metadata database interface
# ===========================

# FIXME: functionality should be implemented in C++?
#
# FIXME2: improve the design
#         A better design would be to create a build-in entity for metadata
#         (i.e. an entity that encode the entity schema).  Then we could
#         save the metadata using any of the already supported storage
#         backends.


class MetaDB(dict):
    """Generic metadata database interface.

    It is implemented as a dict mapping metadata uuid's to Metadata objects.

    Keys may be provided either as (name, version, namespace)-tuples or
    as metadata uuid's.
    """
    def __setitem__(self, key, meta):
         dict.__setitem__(self, self.touuid(key), Metadata(meta))

    def __getitem__(self, key):
         return dict.__getitem__(self, self.touuid(key))

    def __contains__(self, key):
         return dict.__contains__(self, self.touuid(key))

    @staticmethod
    def touuid(key):
        """Returns `key` converted to a metadata uuid. `key` may be either a
        (name, version, namespace)-tuple or an uuid."""
        if isinstance(key, tuple):
            name, version, namespace = key
            return softpy.uuid_from_entity(name, version, namespace)
        elif isinstance(key, str) and re.match(
                '[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
                '[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$', key):
            return key
        elif isinstance(key, Metadata):
            return key.get_uuid()
        raise TypeError(
            'key must be an uuid or (name, version, namespace)-tuple: %r' % (
                key, ))

    def find(self, name='*', version='*', namespace='*'):
        """Returns a list of Metadata objects matching the given `name`,
        `version` and `namespace` shell patterns."""
        values = self.values()
        if name is not '*':
            rname = re.compile(fnmatch.translate(name))
            values = [v for v in values if rname.match(v)]
        if version is not '*':
            rversion = re.compile(fnmatch.translate(version))
            values = [v for v in values if rversion.match(v)]
        if namespace is not '*':
            rnamespace = re.compile(fnmatch.translate(namespace))
            values = [v for v in values if rnamespace.match(v)]
        return list(values)

    def add(self, meta):
        """Add Metadata object `meta` to database."""
        meta = Metadata(meta)
        dict.__setitem__(self, meta.get_uuid(), meta)

    def mtypes(self):
        """Returns a list of (name, version, namespace)-tuples for all
        registered metadata."""
        return [meta.mtype for meta in self.values()]

    def get_json(self, indent=2, soft_keys=True):
        """Returns a JSON representation of the database."""
        return json.dumps(self, indent=indent, sort_keys=soft_keys)

#
# Old deprecated metadatabases
# ----------------------------
# FIXME: remove
#class BaseMetaDB(object):
#    """A base class for metadata databases."""
#    def __init__(self, **kwargs):
#        """Connects to the database."""
#        raise NotImplementedError
#
#    def find(self, name, version, namespace):
#        """Returns a Metadata object for the given name, version and
#        namespace.
#
#        Should raise SoftMissingMetadataError if not metadata can be found.
#        """
#        raise NotImplementedError
#
#    def find_uuid(self, uuid):
#        """Returns a Metadata object with given uuid."""
#        for mtype in self.mtypes():
#            if softpy.uuid_from_entity(*mtype) == uuid:
#                return Metadata(*mtype)
#
#    def insert(self, metadata):
#        """Inserts `metadata` into the database.
#
#        May not be implemented by all subclasses."""
#        raise NotImplementedError
#
#    def mtypes(self):
#        """Returns a list of (name, version, namespace)-tuples for all
#        registered metadata.
#
#        May not be implemented by all subclasses."""
#        raise NotImplementedError
#
#    def close(self):
#        """Closes the connection to the database."""
#        raise NotImplementedError
#
#    def has(self, name, version, namespace):
#        """Returns whether metadata with the given name, version and
#        namespace exists in the database."""
#        try:
#            self.find(name, version, namespace)
#        except SoftError:
#            return False
#        return True
#
#
#class JSONMetaDB(BaseMetaDB):
#    """A simple metadata database using a json file.
#
#    The `fname` argument should either be a file name or an open
#    file-like object with an array of metadata definitions.
#
#    Note, if `fname` is a file-like object, it will not be closed
#    when the close() method is called on the returned database object.
#    """
#    def __init__(self, fname):
#        self.fname = fname
#        if hasattr(fname, 'read'):
#            data = json.load(fname)
#        elif not os.path.exists(fname):
#            data = []
#        else:
#            with open(fname) as f:
#                data = json.load(f)
#        self.data = [Metadata(d) for d in data]
#        self.changed = False
#        self.closed = False
#
#    def find(self, name, version, namespace):
#        """Returns a Metadata object for the given name, version and
#        namespace.
#
#        SoftMissingMetadataError is raised if not metadata can be found."""
#        return self.find_uuid(softpy.uuid_from_entity(name, version, namespace))
#
#    def find_uuid(self, uuid):
#        """Returns a Metadata object with given uuid."""
#        for meta in self.data:
#            if meta.get_uuid() == uuid:
#                return meta
#        raise SoftMissingMetadataError('Cannot find metadata: %s' % uuid)
#
#    def insert(self, metadata):
#        """Inserts `metadata` into the database."""
#        meta = Metadata(metadata)
#        try:
#            self.find(meta.name, meta.version, meta.namespace)
#        except SoftMissingMetadataError:
#            self.data.append(meta)
#            self.changed = True
#        else:
#            raise SoftError('Metadata %s/%s-%s already in the database' % (
#                meta.namespace, meta.name, meta.version))
#
#    def mtypes(self):
#        """Returns a list of (name, version, namespace)-tuples for all
#        registered metadata."""
#        return [meta.mtype for meta in self.data]
#
#    def flush(self):
#        """Flushes the database to file."""
#        if self.changed:
#            if hasattr(self.fname, 'read'):
#                json.dump(self.data, fname, indent=2, sort_keys=True)
#            else:
#                with open(self.fname, 'w') as f:
#                    json.dump(self.data, f, indent=2, sort_keys=True)
#
#    def clear(self):
#        """Removes all metadata in the database.  This is probably only
#        useful for testing."""
#        if self.data:
#            self.changed = True
#        del self.data[:]
#
#    def close(self):
#        """Closes the connection to the database."""
#        self.flush()
#        self.closed = True
#
#
#class JSONDirMetaDB(JSONMetaDB):
#    """A metadata database using a directory with json files.
#
#    The `path` argument is the path to the directory containing the json
#    files.  Only files with a .json extension will be read.
#    """
#    def __init__(self, path):
#        self.path = path
#        self.data = []
#        self.filenames = {}
#        for filename in glob(os.path.join(path, '*.json')):
#            with open(filename) as f:
#                meta = Metadata(f)
#            self.filenames[meta.mtype] = filename
#            self.data.append(meta)
#        self.changed = False
#        self.closed = False
#
#    def flush(self):
#        """Flushes the database to file."""
#        if self.changed:
#            for meta in data:
#                if not meta.mtype in self.filenames:
#                    filename = os.path.join(self.path, '%s-%s.json' % (
#                        meta.name, meta.version))
#                if os.path.exists(filename):
#                    warnings.warn(
#                        'will not overwrite existing metadata: %s' + filename)
#                else:
#                    with open(filename, 'w') as f:
#                        f.write(meta.json())
#
#
#class MongoMetaDB(BaseMetaDB):
#    """A simple metadata database for mongodb.
#
#    Parameters
#    ----------
#    uri : string
#        URI of database to connect to.
#    dbname : string
#        Name of the mongodb database.
#    collection_name : string
#        Name of mongodb collection to connect to.
#
#    Note: this requires that you have pymongo installed.
#    """
#    def __init__(self, uri, dbname, collection_name):
#        import pymongo
#        self.client = pymongo.MongoClient(uri)
#        self.db = self.client[dbname]
#        self.coll = self.db[collection_name]
#
#    def find(self, name, version, namespace):
#        """Returns a Metadata object for the given name, version and
#        namespace.
#
#        SoftMissingMetadataError is raised if not metadata can be found."""
#        meta = self.coll.find_one(
#            {'name': name, 'version': version, 'namespace': namespace})
#        if meta:
#            return meta
#        raise SoftMissingMetadataError('Cannot find metadata %s/%s-%s' % (
#            namespace, name, version))
#
#    def insert(self, metadata):
#        """Inserts `metadata` into the database."""
#        meta = Metadata(metadata)
#        try:
#            self.find(meta.name, meta.version, meta.namespace)
#        except SoftMissingMetadataError:
#            self.coll.insert_one(meta)
#            self.changed = True
#        else:
#            raise SoftError('Metadata %s/%s-%s already in the database' % (
#                meta.namespace, meta.name, meta.version))
#
#    def mtypes(self):
#        """Returns a list of (name, version, namespace)-tuples for all
#        registered metadata."""
#        return [(post['name'], post['version'], post['namespace'])
#                for post in self.coll.find()]
#
#    def clear(self):
#        """Removes all metadata in the database.  This is probably only
#        useful for testing."""
#        self.coll.delete_many({})
#
#    def close(self):
#        """Closes the connection to the database."""
#        self.client.close()


# Exposed instance of MetaDB currently caching all metadata
metaDB = MetaDB()

# List of instances of deprecated metadata databases
_metadbs = []

def register_metadb(metadb):
    """Registers metadata database `metadb`."""
    _metadbs.append(metadb)

def register_jsondir(path):
    """Registers all the entities defined in json-files in the directory
    with the given path."""
    filenames = glob(os.path.join(path, '*.json'))
    for filename in filenames:
        with open(filename) as f:
            Metadata(f)  # this automatically registers the metadata...

def find_metadata(name, version, namespace):
    """Search through all registered metadata databases and return
    a Metadata object corresponding to `name`, `version`, `namespace`.
    """
    return find_metadata_uuid(softpy.uuid_from_entity(
        asStr(name), asStr(version), asStr(namespace)))

def find_metadata_uuid(uuid):
    """Search through all registered metadata databases and return
    a Metadata object corresponding to `name`, `version`, `namespace`.
    """
    if uuid in metaDB:
        return metaDB[uuid]
    for db in _metadbs:
        try:
            meta = db.find_uuid(uuid)
        except SoftMissingMetadataError:
            pass
        else:
            metaDB[uuid] = meta
            return meta
    raise SoftMissingMetadataError(
        'Cannot find metadata with uuid: ' + uuid)
