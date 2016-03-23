# -*- python -*-

%pythoncode %{

import sys
import json
import ast
import operator

import numpy as np


class SoftError(Exception):
    """Base class for SOFT exceptions."""
    pass

class SoftUninitializedError(SoftError):
    """Raised when working with uninitialized data."""
    pass

class SoftStorageClosedError(SoftError):
    """Raised when working on a closed storage."""
    pass

class SoftMissingDimensionsError(SoftError):
    """Raised if dimensions are missing or cannot be derived."""
    pass

class Uninitialized(object):
    """Class representing uninitialized values. Not intended to be
    instanciated..."""
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
    """
    def __init__(self, driver, uri, options=None):
        self.driver = driver
        self.uri = uri
        self.options = options
        self.storage = storage_create(driver, uri, options)
        self.strategy = storage_get_storage_strategy(self.storage)
        self._closed = False

    def save(self, entity):
        """Saves entity in this storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        e = get_c_entity(entity)
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asStr(e.id))
        datamodel_set_meta_name(datamodel, asStr(e.name))
        datamodel_set_meta_version(datamodel, asStr(e.version))
        datamodel_set_meta_namespace(datamodel, asStr(e.namespace))
        entity_store(e, datamodel)   # Ask the entity to fill out the datamodel
        storage_strategy_store(self.strategy, datamodel)
        storage_strategy_free_datamodel(datamodel)

    def load(self, entity):
        """Fills `entity` with data loaded from this storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        e = get_c_entity(entity)
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asStr(e.id))
        datamodel_set_meta_name(datamodel, asStr(e.name))
        datamodel_set_meta_version(datamodel, asStr(e.version))
        datamodel_set_meta_namespace(datamodel, asStr(e.namespace))
        storage_strategy_start_retrieve(self.strategy, datamodel)
        entity_load(e, datamodel)  # Ask the entity to fill out itself
        storage_strategy_end_retrieve(self.strategy, datamodel)
        storage_strategy_free_datamodel(datamodel)

    def close(self):
        """Closes current storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        storage_free_storage_strategy(self.strategy)
        storage_free(self.storage)
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


class Collection(object):
    """A collection of entities."""
    def __init__(self, id=None):
        self.__soft_entity__ = collection_create(id)

    def __delete__(self):
        collection_free(self.__softpy_entity__)
        object.__del__(self)

    def __len__(self):
        return collection_num_entities(self.__soft_entity__)

    def __setitem__(self, label, value):
        if isinstance(value, entity_t):
            e = value
        elif hasattr(value, '__soft_entity__'):
            e = value.__soft_entity__
        else:
            raise TypeError('Value must be an softpy entity')
        collection_register_entity(self.__soft_entity__, label, e)

    def __getitem__(self, label):
        raise NotImplementedError

    #def __setattr__(self, label, value):
    #    self.__setitem__(label, value)

    #def __getattr__(self, label):
    #    return self.__getitem__(label)

    def register_entity(self, label, value):
        self.__setitem__(label, value)

    def add_dim(self, label, description=''):
        collection_add_dim(self.__soft_entity__, label, description)

    def add_connection(self, subject, predicate, object_):
        collection_connection(self.__soft_entity__, subject,
                              predicate, object_)

    def get_num_entities(self):
         return len(self)

    def get_num_dims(self):
         return collection_num_dims(self.__soft_entity__)

    def get_num_relations(self):
         return collection_num_relations(self.__soft_entity__)

    def get_num_dim_maps(self):
         return collection_num_dim_maps(self.__soft_entity__)

    def get_dimensions(self):
         return collection_get_dimensions(self.__soft_entity__)

    
def get_c_entity(entity):
    """Returns a reference to the underlying C-level entity_t."""
    if hasattr(entity, '__soft_entity__'):
        e = entity.__soft_entity__
    else:
        e = entity
    if not isinstance(e, entity_t):
        raise TypeError('Not a proper entity')
    return e


def arithmetic_eval(s):
    """Returns the result of arithmetic evaluation of s.  This function is
    much safer to use than eval() and should be preferred for simple
    arithmetic evaluation.

    It is based on the answer at
    http://bingbots.com/questions/148864/valueerror-malformed-string-when-using-ast-literal-eval
    """
    binOps = {
        ast.Add: operator.add,
        ast.Sub: operator.sub,
        ast.Mult: operator.mul,
        ast.Div: operator.div,
        ast.Mod: operator.mod,
        ast.Pow: operator.pow,
    }
    node = ast.parse(s, mode='eval')

    def _eval(node):
        if isinstance(node, ast.Expression):
            return _eval(node.body)
        elif isinstance(node, ast.Str):
            return node.s
        elif isinstance(node, ast.Num):
            return node.n
        elif isinstance(node, ast.BinOp):
            return binOps[type(node.op)](_eval(node.left), _eval(node.right))
        else:
            raise Exception('Unsupported type %s in evaluation of "%s"' % (
                node, s))

    return _eval(node.body)


def asBytes(s):
    """Returns `s` as bytes using the default encoding - works for Python
    2.7 and 3.x."""
    if hasattr(s, 'encode'):
        return s.encode()
    return s

def asStr(s):
    """Returns `s` as string using the default encoding - works for Python
    2.7 and 3.x."""
    if isinstance(s, str):
        return s
    if sys.version_info.major == 2:
        if hasattr(s, 'encode'):
            return s.encode()
    else:
        if hasattr(s, 'decode'):
            return s.decode()
    raise TypeError('Cannot convert %r to string' % (s, ))


def _get_entity_method(entity, name):
    """Returns a reference to method `name` in `entity`.
    """
    if hasattr(entity, name):
        attr = getattr(entity, name)
        if hasattr(attr, '__call__'):
            method = attr
    method = getattr(entity, '_' + name)
    assert hasattr(method, '__call__')
    return method

def _get_prop_data(cls, property_name, dataname, default=None):
    """Help function used by MetaFactoryEntity."""
    for p in cls._meta['properties']:
        if p['name'] == property_name:
            return p.get(dataname, default)
    raise NameError('%s has no property named "%s"' % (
        cls._name, dataname))


class MetaFactoryEntity(type):
    """Metaclass for BaseFactoryEntity providing some functionality to
    Entity classes that is not inherited by their instances."""
    def __str__(self):
        return json.dumps(self._meta, indent=2, sort_keys=True)

    def __repr__(self):
        return '<class %s version=%r, namespace=%r)>' % (
            self._meta['name'], self._meta['version'], self._meta['namespace'])

class classproperty(property):
    """For defining class properties."""
    def __get__(self, cls, owner):
        return self.fget.__get__(None, owner)()

class EntityIter(object):
    """For iterating over property names of Entity instances."""
    def __init__(self, entity):
        self.n = 0
        self.entity = entity
    def __next__(self):
        if self.n >= len(self.entity):
            raise StopIteration
        self.n += 1
        return self.entity._meta['properties'][self.n - 1]['name']
    next = __next__  # for Python 2


class BaseFactoryEntity(object):
    """Base class for entities created with the entity() factory function.
    """
    __metaclass__ = MetaFactoryEntity

    def __init__(self, dimensions=None, uuid=None, driver=None, uri=None, 
                 options=None, **kwargs):
        """Initialize a new entity.  

        If only `dimensions` is given, an new empty entity will be created.

        If `uuid`, `driver` and `uri` are given, initial property
        values of the new entity will be loaded from dataset `uuid` is
        storage `uri`.  In `dimensions` is not given, it will be
        derived from the loaded data (if possible, otherwise a
        SoftMissingDimensionsError is raised).

        Parameters
        ----------
        dimensions : None | sequence | dict
            Dimensions of the returned entity.  Can be given either as
            ``[3, 5]`` or ``{'I'=3, 'J'=5}`` provided that the entity
            has dimensions I and J.  If None, it will be attempted
            derived from the data loaded from `uri`.
        uuid : None | string
            If given, it should be an unique uuid for the new instance.
            The default is to generate it.
        driver : None | "json" | "hdf5" | "mongo" | ...
            The driver to use for loading initial values.
        uri : None | string
            Where the initial values are stored.  If None, all property values
            are set to softpy.Uninitialized.
        options : None | string
            Additional options passed to the driver.
        kwargs : 
            Initial property label-value pairs.  These will overwrite
            values initial values read from `uri`.
        """
        meta = self._meta
        dims = [str(d['name']) for d in meta['dimensions']]

        if dimensions:
            if len(dimensions) != len(dims):
                raise TypeError('Entity "%s" has %d dimensions, %d given' % (
                    self._name, len(dims), len(dimensions)))
            if hasattr(dimensions, 'keys'):
                dim_sizes = [dimensions[label] for label in dims]
            else:
                dim_sizes = [int(size) for size in dimensions]
            for key in self._keys():
                setattr(self, key, Uninitialized)

        if uri:
            if uuid is None:
                raise TypeError('`uuid` must be provided when initialising '
                                'an entity from storage')
            if driver is None:
                raise TypeError('`driver` must be provided when initialising '
                                'an entity from storage')
            # We should be able to queue the storage about the size of 
            # each dimensions...
            # A workaround is to create a temporary entity and try to extract
            # the dimension sizes from the loaded data...
            def get_dimension_size(e, label):
                raise RuntimeError('dimension sizes are not available...')
            self.__soft_entity__ = entity_t(
                get_meta_name=meta['name'],
                get_meta_version=meta['version'],
                get_meta_namespace=meta['namespace'],
                get_dimensions=dims,
                get_dimension_size=get_dimension_size,
                load=self._load,
                store=self._store,
                id=uuid,
                user_data=self)
            with Storage(driver, uri, options) as s:
                s.load(self)
            if dimensions is None:
                sizes = {}
                for label in self._get_dimensions():
                    for key in self._keys():
                        value = getattr(self, key)
                        for i, name in enumerate(self._get_property_dims(key)):
                            if name == label:
                                if isinstance(value, np.ndarray):
                                    sizes[label] = value.shape[i]
                                elif i == 0:
                                    sizes[label] = len(value)
                            break
                    if not label in sizes:
                        raise SoftMissingDimensionsError(
                            'cannot determine dimension with label "%s"' % 
                            label)
                dim_sizes = [sizes[label] for label in dims]
        elif dimensions is None:
            if len(dims) == 0:
                dim_sizes = []
            else:
                raise SoftMissingDimensionsError(
                    'Dimension size(s) must be provided with the '
                    '`dimentions` argument: ' + ', '.join(dims))

        assert len(dim_sizes) == len(dims)

        self.__soft_entity__ = entity_t(
            get_meta_name=meta['name'],
            get_meta_version=meta['version'],
            get_meta_namespace=meta['namespace'],
            get_dimensions=dims,
            get_dimension_size=dim_sizes,
            load=lambda e, model: self._load(model),
            store=lambda e, model: self._store(model),
            id=uuid,
            user_data=self)

        for label, value in kwargs.items():
            self[label] = value

    def __str__(self):
        s = []
        s.append('Entity %s' % self._name)
        s.append('  id: %s' % self._id)
        s.append('  version: %s' % self._version)
        s.append('  namespace: %s' % self._namespace)
        s.append('  dimensions: %s' % ', '.join(
            '%s=%d' % (d, self._get_dimension_size(d)) for d in
                self._get_dimensions()))
        s.append('  description: %s' % self._description)
        s.append('  properties:')
        for key in self._keys():
             s.append('    %s: %r %s' % (
                 key, self[key], self._get_property_unit(key)))
        return '\n'.join(s)
    
    def __getitem__(self, name):
        return self.__dict__[name]

    def __setitem__(self, name, value):
        if name not in self.__dict__:
            raise KeyError('No property named "%s"' % (name, ))
        self.__dict__[name] = value

    def __len__(self):
        return len(self._meta['properties'])

    def __contains__(self, name):
        return name in self._keys()

    def __iter__(self):
        return EntityIter(self)
    
    _name = classproperty(
        classmethod(lambda cls: str(cls._meta['name'])),
        doc='Entity name.')
    _version = classproperty(
        classmethod(lambda cls: str(cls._meta['version'])),
        doc='Entity version string.')
    _namespace = classproperty(
        classmethod(lambda cls: str(cls._meta['namespace'])),
        doc='Entity namespace.')
    _description = classproperty(
        classmethod(lambda cls: cls._meta['description']),
        doc='Entity description.')
    _dimensions = classproperty(
        classmethod(lambda cls: cls._get_dimensions()),
                    doc='List with dimension names.')
    _dimension_sizes = property(
        lambda self: self.__soft_entity__.dimension_sizes,
        doc='List with dimension sizes.')
    _id = property(
        lambda self: self.__soft_entity__.id,
        doc='UUID of Entity instance.')

    @classmethod
    def _keys(cls):
        return [p['name'] for p in cls._meta['properties']]

    @classmethod
    def _get_property_unit(cls, name):
        """Returns unit for property `name`, or and empty string if
        `property_name` has no unit."""
        return _get_prop_data(cls, asStr(name), 'unit', '')

    @classmethod
    def _get_property_type(cls, name):
        """Returns the type of property `name`."""
        return _get_prop_data(cls, asStr(name), 'type')
    
    @classmethod
    def _get_property_description(cls, name):
        """Returns description of property `name`."""
        return _get_prop_data(cls, asStr(name), 'description', '')

    @classmethod
    def _get_property_dims(cls, name):
        """Returns a list with the dimensions of property `name`."""
        return _get_prop_data(cls, asStr(name), 'dims', [])

    def _get_property_dim_sizes(self, name):
        """Returns a list with the dimensions of property `name` evaluated
        to integers."""
        e = self.__soft_entity__
        sizes = zip(e.dimensions, e.dimension_sizes)
        dims = []
        for label in _get_prop_data(self, name, 'dims', []):
            for dname, dsize in sizes:
                label = label.replace(dname, str(dsize))
            dims.append(arithmetic_eval(label))
        return dims

    def _initialized(self):
        """Returns true if all properties are initialized. False is returned
        otherwise."""
        return all(self[key] is not Uninitialized for key in self._keys())

    # ------------ methods modelled after the softc-entity API -----------

    #@classmethod
    #def _get_meta_type(cls):
    #    """Returns """
    #    pass

    @classmethod
    def _get_meta_name(cls):
        return self._meta['name']

    @classmethod
    def _get_meta_version(cls):
        return self._meta['version']

    @classmethod
    def _get_meta_namespace(cls):
        return self._meta['namespace']

    @classmethod
    def _get_dimensions(cls):
        """Returns a list with dimension labels."""
        return [str(d['name']) for d in cls._meta['dimensions']]

    def _get_dimension_size(self, label):
        """Returns the length of dimension `label`."""
        return entity_get_dimension_size(self.__soft_entity__, label)

    def _store(self, datamodel):
        """Stores property values to `datamodel`, raising SoftUnitializedError
        if any property is uninitialized.

        Normally you would not call this function directly, but
        instead through Storage.save()."""
        for key in self._keys():
            value = getattr(self, key)
            vtype = self._get_property_type(key)
            if value is Uninitialized:
                raise SoftUninitializedError(
                    'Uninitialized data for "%s" in %s' % (
                        key, self.__class__.__name__))
            dims = self._get_property_dims(key)
            if not dims:
                setter = getattr(_softpy, 'datamodel_append_' + vtype)
            elif vtype == 'string':
                assert len(dims) == 1
                setter = getattr(_softpy, 'datamodel_append_string_list')
            elif len(dims) == 1:
                setter = getattr(_softpy, 'datamodel_append_array_' + vtype)
            else:
                setter = getattr(_softpy, 'datamodel_append_array_%s_%dd' % (
                    vtype, len(dims)))
            setter(datamodel, asStr(key), value)

    def _load(self, e, datamodel):
        """Loads property values from `datamodel` into self.  Normally you
        would not call this function directly, but instead through
        Storage.load()."""
        for key in self._keys():
            vtype = self._get_property_type(key)
            dims = self._get_property_dims(key)
            if vtype.startswith('array_'):
                vtype = vtype[6:]
            if vtype.endswith('_2d') or vtype.endswith('_3d'):
                vtype = vtype[:-3]
            if not dims:
                getter = getattr(_softpy, 'datamodel_get_' + vtype)
            elif vtype == 'string_list' or vtype == 'string':
                assert len(dims) == 1
                getter = getattr(_softpy, 'datamodel_get_string_list')
            elif len(dims) == 1:
                getter = getattr(_softpy, 'datamodel_get_array_' + vtype)
            else:
                getter = getattr(_softpy, 'datamodel_get_array_%s_%dd' % (
                    vtype, len(dims)))
            value = getter(datamodel, str(key))
            setattr(self, asStr(key), value)


def entity(metadata):
    """Factory fuction for creating an Entity class object for `metadata`.

    Here `metadata` is a metadata description (in json format) of the
    entity to create.  It should either be a string or a file-like
    object."""
    if hasattr(metadata, 'read'):
        meta = json.load(metadata)
    else:
        meta = json.loads(metadata)
    attr = dict(_meta=meta)
    return type(str(meta['name']), (BaseFactoryEntity,), attr)


# Convinience functions for returning entity info
# -----------------------------------------------
def _get_entity_info(e, field, *args):
    """Help function for returning info about entities."""
    if isinstance(e, entity_t):
        return globals()['entity_get_' + field](e, *args)
    elif hasattr(e, '__soft_entity__'):
        return globals()['entity_get_' + field](e.__soft_entity__, *args)
    else:
        raise TypeError('not an entity: %r' % e)

def get_id(e):
    """Returns the id of entity `e`."""
    return _get_entity_info(e, 'id')

def get_meta_name(e):
    """Returns the name of entity `e`."""
    return _get_entity_info(e, 'meta_name')

def get_meta_version(e):
    """Returns the version of entity `e`."""
    return _get_entity_info(e, 'version')

def get_meta_namespace(e):
    """Returns the namespace of entity `e`."""
    return _get_entity_info(e, 'meta_namespace')

def get_dimensions(e):
    """Returns list of dimension lables for entity `e`."""
    return _get_entity_info(e, 'dimensions')

def get_dimension_size(e, label):
    """Returns size of dimension `label` for entity `e`.  If `label` is
    not a valid dimension label -1 is returned."""
    return _get_entity_info(e, 'dimension_size', label)

%}
