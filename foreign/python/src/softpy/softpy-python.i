# -*- python -*-


%pythoncode %{

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



#def create_softc_entity(obj, name, version, namespace, dimensions, 
#                        dimension_size, store, load):
#    """Returns a new C-level entity.
#
#    The main purpose of fhis function is to turn normal Python-objects into
#    entities.  This is done by adding a __softc_entity__ attribute to you
#    object set to the return value of this function.  Hence
#
#        obj.__softc_entity__ = create_softc_entity(obj, name=...)
#
#    will turn `obj` into an entity.
#    """
#    e = softpy.entity_t(name, version, namespace, dimensions,
#                        dimension_size, store, load)
#    e.set_self(obj)
#    return e


#class Entity(object):
#    """Class representing a C entity at Python level.
#    """
#    def __init__(self, name, version, namespace, dimensions, dimension_size, 
#                 load, store):
#        self.this = softpy.entity_t(name, version, namespace, dimensions,
#                                    dimension_size, load, store)
#        self.this.set_self(self)
#
#    id = property(
#        lambda self: softpy.entity_get_id(self.this),
#        doc='Entity id.')
#    meta_name = property(
#        lambda self: softpy.entity_get_meta_name(self.this),
#        doc='Entity name.')
#    meta_version = property(
#        lambda self: softpy.entity_get_meta_version(self.this),
#        doc='Entity version.')
#    meta_namespace = property(
#        lambda self: softpy.entity_get_meta_namespace(self.this),
#        doc='Entity namespace.')
#    dimensions = property(
#        lambda self: softpy.entity_get_dimensions(self.this),
#        doc='List of dimension labels.')
#    dimension_sizes = property(
#        lambda self: [softpy.entity_get_dimension_size(self.this, label)
#                      for label in softpy.entity_get_dimensions(self.this)],
#        doc='List of dimension sizes.')
#    
#    def get_dimension_size(self, label):
#        """Returns the size of dimension with this label."""
#        return softpy.entity_get_dimension_size(self.this, label)
#
#    def store(self, datamodel):
#        """Stores data into datamodel."""
#        softpy.entity_store(self.this, datamodel)
#
#    def load(self, datamodel):
#        """Reads data from datamodel."""
#        softpy.entity_load(self.this, datamodel)
#


    
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
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asBytes(entity._id))
        datamodel_set_meta_name(datamodel, asBytes(entity._name))
        datamodel_set_meta_version(datamodel, asBytes(entity._version))
        datamodel_set_meta_namespace(datamodel, asBytes(entity._namespace))
        entity._store(datamodel)   # Ask the entity to fill out the datamodel
        storage_strategy_store(self.strategy, datamodel)

    def load(self, entity):
        """Fills `entity` with data loaded from this storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asBytes(entity._id))
        datamodel_set_meta_name(datamodel, asBytes(entity._name))
        datamodel_set_meta_version(datamodel, asBytes(entity._version))
        datamodel_set_meta_namespace(datamodel, asBytes(entity._namespace))
        storage_strategy_start_retrieve(self.strategy, datamodel)
        entity._load(datamodel)   # Ask the entity to fill out itself
        storage_strategy_end_retrieve(self.strategy, datamodel)

    def close(self):
        """Closes current storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
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
                 options=None):
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
            Dimensions if entity is kept uninitialised (i.e. `uri` is None).
            Can be given either as ``[3, 5]`` or ``{'I'=3, 'J'=5}`` provided
            that the entity has dimensions I and J.
            If None, it will be attempted derived from 
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
        """
        self._instancedata = {}
        self._instancedata['id'] = uuidgen() if uuid is None else uuid

        if dimensions:
            if len(dimensions) != len(self._meta['dimensions']):
                raise TypeError('Entity "%s" has %d dimensions, %d given' % (
                    self._name, len(self._dimensions), len(dimensions)))
            if hasattr(dimensions, 'keys'):
                self._instancedata['dimensions'] = [
                    dimensions[label] for label in self._dimensions]
            else:
                self._instancedata['dimensions'] = [int(d) for d in dimensions]
            for key in self._keys():
                setattr(self, key, Uninitialized)

        if uri:
            if uuid is None:
                raise TypeError('`uuid` must be provided when initialising '
                                'an entity from storage')
            if driver is None:
                raise TypeError('`driver` must be provided when initialising '
                                'an entity from storage')
            with Storage(driver, uri, options) as s:
                s.load(self)
            if dimensions is None:
                dims = {}
                for label in self._get_dimensions():
                    for key in self._keys():
                        value = getattr(self, key)
                        for i, name in enumerate(self._get_property_dims(key)):
                            if name == label:
                                if isinstance(value, np.ndarray):
                                    dims[label] = value.shape[i]
                                elif i == 0:
                                    dims[label] = len(value)
                            break
                    if not label in dims:
                        raise SoftMissingDimensionsError(
                            'cannot determine dimension with label "%s"' % 
                            label)
                self._instancedata['dimensions'] = [
                    dims[label] for label in self._get_dimensions()]

        assert (len(self._instancedata['dimensions']) == len(
            self._get_dimensions()))


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
    _dimension_sizes = property(lambda self: self._instancedata['dimensions'],
                                doc='Dict with dimension lengths.')
    _id = property(lambda cls: cls._instancedata['id'],
                   doc='UUID of Entity instance.')

    @classmethod
    def _keys(cls):
        return [p['name'] for p in cls._meta['properties']]

    @classmethod
    def _get_property_unit(cls, name):
        """Returns unit for property `name`, or and empty string if
        `property_name` has no unit."""
        return _get_prop_data(cls, asBytes(name), 'unit', '')

    @classmethod
    def _get_property_type(cls, name):
        """Returns the type of property `name`."""
        return _get_prop_data(cls, asBytes(name), 'type')
    
    @classmethod
    def _get_property_description(cls, name):
        """Returns description of property `name`."""
        return _get_prop_data(cls, asBytes(name), 'description', '')

    @classmethod
    def _get_property_dims(cls, name):
        """Returns a list with the dimensions of property `name`."""
        return _get_prop_data(cls, asBytes(name), 'dims', [])

    def _get_property_dim_sizes(self, name):
        """Returns a list with the dimensions of property `name` evaluated
        to integers."""
        sizes = zip(self._get_dimensions(), self._instancedata['dimensions'])
        dims = []
        for label in _get_prop_data(self, asBytes(name), 'dims', []):
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
        i = self._get_dimensions().index(label)
        return self._instancedata['dimensions'][i]

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
            setter(datamodel, asBytes(key), value)

    def _load(self, datamodel):
        """Loads property values from `datamodel` into self.  Normally you
        would not call this function directly, but instead through
        Storage.load()."""
        for key in self._keys():
            vtype = self._get_property_type(key)
            dims = self._get_property_dims(key)
            if not dims:
                getter = getattr(_softpy, 'datamodel_get_' + vtype)
            elif vtype == 'string':
                assert len(dims) == 1
                getter = getattr(_softpy, 'datamodel_get_string_list')
            elif len(dims) == 1:
                getter = getattr(_softpy, 'datamodel_get_array_' + vtype)
            else:
                getter = getattr(_softpy, 'datamodel_get_array_%s_%dd' % (
                    vtype, len(dims)))
            value = getter(datamodel, asBytes(key))
            setattr(self, key, value)


def entity(metadata):
    """Factory fuction for creating an Entity class object for `metadata`.

    Here `metadata` is a metadata description (in json format) of the
    entity to create.  It should either be a string or a file-like
    object."""
    if hasattr(metadata, 'read'):
        meta = json.load(metadata)
    else:
        meta = json.loads(metadata)
    instancedata = {
        'id': None,  # is set in __init__()
        'dimensions': [0] * len(meta['dimensions']),
    }
    attr = dict(_meta=meta, _instancedata=None)
    return type(str(meta['name']), (BaseFactoryEntity,), attr)


    
%}
