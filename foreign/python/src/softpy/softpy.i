/*  -*- Mode: C -*- */

%define DOCSTRING
"Python bindings to SOFT5."
%enddef

%module(docstring=DOCSTRING) softpy



/**********************************************
 ** C code included in the wrapper
 **********************************************/
%{
 #include "softc.h"
 #include <stdint.h>
 #include <stdbool.h>
%}


/**********************************************
 ** Module initialisation
 **********************************************/
%init %{
  /* Call softc_init() with argc and argv obtained from the sys module */
  int i;
  PyObject *sys = PyImport_ImportModule("sys");  // new ref
  PyObject *sys_dict = PyModule_GetDict(sys);    // borrowed ref
  PyObject *argv_obj = PyMapping_GetItemString(sys_dict, "argv");  // new ref
  int argc = PySequence_Length(argv_obj);
  char **argv = malloc(argc * sizeof(char *));
  PyObject **items = calloc(argc, sizeof(PyObject *));
  for (i=0; i<argc; i++) {
    items[i] = PySequence_GetItem(argv_obj, i);  // new ref
#if PY_MAJOR_VERSION <= 2
    argv[i] = PyBytes_AsString(items[i]);  // borrowed memory
#else
    argv[i] = PyUnicode_AsUTF8(items[i]);  // borrowed memory
#endif
  }
  softc_init(argc, argv);
  // Cleanup, can the strings pointed to by argv be free'ed?
  //for (i=0; i<argc; i++) if (items[i]) Py_DECREF(items[i]);
  free(items);
  free(argv);
  Py_DECREF(argv_obj);
  Py_DECREF(sys);
%}


/**********************************************
 ** Typemaps
 **********************************************/
%include <typemaps.i>
%include <exception.i>


 /**********************************************
 ** Declare functions to wrap
 **********************************************/

/* Remove the softc_ prefix from the python bindings */
%feature("autodoc","1");
//%feature("keyword");
%rename("%(strip:[softc_])s") "";
%include "stdint.i"

/* 
 * softc
 */
%typemap(out) char ** {
  char **p;
  $result = PyList_New(0);
  for (p=$1; *p; p++) {
    PyList_Append($result, PyString_FromString(*p));
    free(*p);
  }
  free($1);
}

/* softc_init() is called automatically in module initialisation */
//softc_t * softc_init(int argc, char *argv[]);
int       softc_storage_driver_count();
char   ** softc_get_storage_drivers();
char    * softc_uuidgen();
void      softc_cleanup(softc_t *);

/*
 * datamodel
 */
/*
// Convert false return values to RuntimeError exception
%typemap(out) bool %{
    if(!$1) SWIG_exception(SWIG_RuntimeError,"false return value");
    $result = Py_None;
    Py_INCREF(Py_None); // Py_None is a singleton so increment its reference if used.
%}
*/
bool softc_datamodel_append_string       (softc_datamodel_t *model, const char *key, const char *value);
bool softc_datamodel_append_int8         (softc_datamodel_t *model, const char *key, int8_t value);
bool softc_datamodel_append_uint8        (softc_datamodel_t *model, const char *key, uint8_t value);
bool softc_datamodel_append_int16        (softc_datamodel_t *model, const char *key, int16_t value);
bool softc_datamodel_append_uint16       (softc_datamodel_t *model, const char *key, uint16_t value);
bool softc_datamodel_append_int32        (softc_datamodel_t *model, const char *key, int32_t value);
bool softc_datamodel_append_uint32       (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_int64        (softc_datamodel_t *model, const char *key, int64_t value);
bool softc_datamodel_append_uint64       (softc_datamodel_t *model, const char *key, uint64_t value);
bool softc_datamodel_append_float        (softc_datamodel_t *model, const char *key, float value);
bool softc_datamodel_append_double       (softc_datamodel_t *model, const char *key, double value);
bool softc_datamodel_append_bool         (softc_datamodel_t *model, const char *key, bool value);
bool softc_datamodel_append_blob         (softc_datamodel_t *model, const char *key, unsigned char *value, size_t length);
bool softc_datamodel_append_string_list  (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements);
//bool softc_datamodel_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size);
bool softc_datamodel_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size);

%typemap(in,numinputs=0) char **value (char *temp)        { $1 = &temp; };
%typemap(argout)         char **value                     { $result = PyString_FromString(*$1); };

%typemap(in,numinputs=0) int8_t *value (int8_t temp)      { $1 = &temp; };
%typemap(argout)         int8_t *value                    { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint8_t *value (uint8_t temp)    { $1 = &temp; };
%typemap(argout)         uint8_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int16_t *value (int16_t temp)    { $1 = &temp; };
%typemap(argout)         int16_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint16_t *value (uint16_t temp)  { $1 = &temp; };
%typemap(argout)         uint16_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int32_t *value (int32_t temp)    { $1 = &temp; };
%typemap(argout)         int32_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint32_t *value (uint32_t temp)  { $1 = &temp; };
%typemap(argout)         uint32_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int64_t *value (int64_t temp)    { $1 = &temp; };
%typemap(argout)         int64_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint64_t *value (uint64_t temp)  { $1 = &temp; };
%typemap(argout)         uint64_t *value                  { $result = PyLong_FromUnsignedLong(*$1); };

%typemap(in,numinputs=0) float *value (float temp)        { $1 = &temp; };
%typemap(argout)         float *value                     { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) double *value (double temp)      { $1 = &temp; };
%typemap(argout)         double *value                    { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) bool *value (bool temp)          { $1 = &temp; };
%typemap(argout)         bool *value                      { $result = PyBool_FromLong(*$1); };

// XXX Add typemaps for get_blob, get_array_*

bool softc_datamodel_get_string          (const softc_datamodel_t *model, const char *key, char **value);
bool softc_datamodel_get_int8            (const softc_datamodel_t *model, const char *key, int8_t *value);
bool softc_datamodel_get_uint8           (const softc_datamodel_t *model, const char *key, uint8_t *value);
bool softc_datamodel_get_int16           (const softc_datamodel_t *model, const char *key, int16_t *value);
bool softc_datamodel_get_uint16          (const softc_datamodel_t *model, const char *key, uint16_t *value);
bool softc_datamodel_get_int32           (const softc_datamodel_t *model, const char *key, int32_t *value);
bool softc_datamodel_get_uint32          (const softc_datamodel_t *model, const char *key, uint32_t *value);
bool softc_datamodel_get_int64           (const softc_datamodel_t *model, const char *key, int64_t *value);
bool softc_datamodel_get_uint64          (const softc_datamodel_t *model, const char *key, uint64_t *value);
bool softc_datamodel_get_float           (const softc_datamodel_t *model, const char *key, float *value);
bool softc_datamodel_get_double          (const softc_datamodel_t *model, const char *key, double *value);
bool softc_datamodel_get_bool            (const softc_datamodel_t *model, const char *key, bool *value);
bool softc_datamodel_get_blob            (const softc_datamodel_t *model, const char *key, unsigned char **value, size_t *length);
bool softc_datamodel_get_array_string    (const softc_datamodel_t *model, const char *key, char ***value, size_t *n_elements);
bool softc_datamodel_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size);
bool softc_datamodel_get_array_double    (const softc_datamodel_t *model, const char *key, double **value, size_t *size);

%clear char **value;
%clear int8_t *value;
%clear uint8_t *value;
%clear int16_t *value;
%clear uint16_t *value;
%clear int32_t *value;
%clear uint32_t *value;
%clear int64_t *value;
%clear uint64_t *value;
%clear float *value;
%clear double *value;
%clear bool *value;
%clear unsigned char **value, size_t *length;
%clear char ***value, size_t *n_elements;
%clear int32_t **value, size_t *size;
%clear double **value, size_t *size;

bool softc_datamodel_set_id              (softc_datamodel_t* model, const char *id);
bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name);
bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version);
bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace);
//bool softc_datamodel_set_meta_description(softc_datamodel_t* model, const char *meta_description);
const char * softc_datamodel_get_id              (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model);
//const char * softc_datamodel_get_meta_description(const softc_datamodel_t* model);

/*
 * storage
 */
softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options=NULL);
void              softc_storage_free (softc_storage_t *);
void              softc_storage_load (softc_storage_t *, void *entity);
void              softc_storage_save (softc_storage_t *, void *entity);
softc_storage_strategy_t * softc_storage_get_storage_strategy(softc_storage_t *);

/*
 * storage strategy
 */
softc_datamodel_t * softc_storage_strategy_get_datamodel(softc_storage_strategy_t *);
void                softc_storage_strategy_store(softc_storage_strategy_t *, const softc_datamodel_t *);
void                softc_storage_strategy_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);


/*
 * entity
 */
//void *        softc_entity_new (const char *uri);
//const char *  softc_entity_get_id (const void *self);
//const char *  softc_entity_get_meta_type(const void *self);
//const char *  softc_entity_get_meta_name(const void *self);
//const char *  softc_entity_get_meta_namespace(const void *self);
//const char *  softc_entity_get_meta_version(const void *self);  
//const char ** softc_entity_get_dimensions (const void *self, size_t *size);
//int           softc_entity_get_dimension_size (const void *self, const char *label);
//void          softc_entity_store  (const void *self, softc_datamodel_t *data_model);
//void          softc_entity_load   (void *self, const softc_datamodel_t *data_model);



 /**********************************************
 ** Python code
 **********************************************/
%pythoncode %{

import json

class SoftError(Exception):
    """Base class for SOFT exceptions."""
    pass

class SoftUninitializedError(SoftError):
    """Raised when working with uninitialized data."""
    pass

class Uninitialized(object):
    """Class for marking uninitialized values. Not intended to be
    instanciated..."""
    pass

class classproperty(property):
    def __get__(self, cls, owner):
        return self.fget.__get__(None, owner)()
    
def _get_prop_data(cls, property_name, dataname, default=None):
    """Help function used by MetaEntity."""
    for p in cls._meta['properties']:
        if p['name'] == property_name:
            return p.get(dataname, default)
    raise NameError('%s has no property named "%s"' % (
        cls._name, dataname))
    
    
def save_entity(entity, driver, uri, options=None):
    """Stores `entity` to `uri` using `driver`."""
    storage = storage_create(driver, uri, options)
    strategy = storage_get_storage_strategy(storage)
    datamodel = storage_strategy_get_datamodel(strategy)
    # Hmm, shouldn't we always create a new id when storing?
    datamodel_set_id(datamodel, uuidgen())
    datamodel_set_meta_name(datamodel, str(entity._name))
    datamodel_set_meta_version(datamodel, str(entity._version))
    datamodel_set_meta_namespace(datamodel, str(entity._namespace))
    #datamodel_set_meta_description(datamodel, entity._description)
    #datamodel_set_meta_dimensions(datamodel, entity._dimensions)
    for key in entity._keys():
        value = getattr(entity, key)
        type = entity._get_property_type(key)
        if value is Uninitialized:
            raise SoftUninitializedError(
                'Uninitialized data for "%s" in %s' % (
                    key, entity.__class__.__name__))
        if type == 'string':
            value = str(value)
        setter = getattr(_softpy, 'datamodel_append_' + type)
        setter(datamodel, str(key), value)
    storage_strategy_store(strategy, datamodel)

def load_entity_data(entity, driver, uri, options=None):
    """Loads data into `entity` from `uri` using `driver`."""
    storage = softpy.storage_create(driver, uri, options)
    strategy = softpy.storage_get_storage_strategy(storage)
    datamodel = softpy.storage_strategy_get_datamodel(strategy)
    softpy.storage_strategy_retrieve(strategy, datamodel)
    entity._instanceid = softc_datamodel_get_id(datamodel)
    entity._meta['name'] = softc_datamodel_get_name(datamodel)
    entity._meta['version'] = softc_datamodel_get_version(datamodel)
    entity._meta['namespace'] = softc_datamodel_get_namespace(datamodel)
    #entity._meta['description'] = softc_datamodel_get_description(datamodel)
    #entity._meta['dimensions'] = softc_datamodel_get_dimensions(datamodel)
    for key in entity._keys():
        getter = getattr(
            _softpy, 'datamodel_get_' + entity._get_property_type(key))
        setattr(entity, key, getter(datamodel, str(key)))


def _init_entity(entity, driver='json', uri=None, options=None, dimensions=()):
    """Initiates the entity.

    Parameters
    ----------
    driver : "json" | "hdf5" | "mongo"...
        The driver to use for loading initial values.
    uri : None | string
        Where the initial values are stored.  If None, all property values
        are set to softpy.Uninitialized.
    options : string
        Additional options passed to the driver.
    dimensions : sequence | dict
        Dimensions if entity is kept uninitialised (i.e. `uri` is None).
        Can be given either as "[3, 5]" or "{'I=3, 'J'=5'}" provided that
        the entity has dimensions I and J.
    """
    for p in entity._meta['properties']:
        setattr(entity, p['name'], Uninitialized)
    if uri:
        load_entity_data(entity, driver, uri, options)
    else:
        if len(dimensions) != len(entity._dimension_names):
            raise TypeError('Length of `dimensions` must be %d for %s objects'
                            % (len(entity._dimension_names), entity._name))
        if hasattr(dimensions, 'keys'):
            entity._instancedata['dimensions'] = [
                dimensions[dname] for dname in entity._dimension_names]
        else:
            entity._instancedata['dimensions'] = [int(d) for d in dimensions]

class EntityIter(object):
    def __init__(self, entity):
        self.n = 0
        self.entity = entity
    def __next__(self):
        if self.n >= len(self.entity):
            raise StopIteration
        self.n += 1
        return self.entity._meta['properties'][self.n - 1]['name']
    next = __next__  # for Python 2

class BaseEntity(object):
    """Base class for all entities.  Intented to be used through the
    entity() factory function.
    """
    def __str__(self):
        s = []
        s.append('Entity %s' % self._name)
        s.append('  id: %s' % self._id)
        s.append('  version: %s' % self._version)
        s.append('  namespace: %s' % self._namespace)
        s.append('  dimensions: %s' % ', '.join(
            '%s=%d' % (d, n) for d, n in zip(
                self._dimension_names, self._dimensions)))
        s.append('  description: %s' % self._description)
        s.append('  properties:')
        for key in self._keys():
             s.append('    %s: %r %s' % (
                 key, self[key], self._get_property_unit(key)))
        return '\n'.join(s)
    
    def __getitem__(self, name):
        return self.__dict__[name]

    def __len__(self):
        return len(self._meta['properties'])

    def __contains__(self, name):
        return name in self._keys()

    def __iter__(self):
        return EntityIter(self)
    
    _name = classproperty(
        classmethod(lambda cls: cls._meta['name']))
    _version = classproperty(
        classmethod(lambda cls: cls._meta['version']))
    _namespace = classproperty(
        classmethod(lambda cls: cls._meta['namespace']))
    _description = classproperty(
        classmethod(lambda cls: cls._meta['description']))
    _dimension_names = classproperty(
        classmethod(lambda cls:
                    [str(d['name']) for d in cls._meta['dimensions']]))

    # The instance is mutable - should it really have an id?
    # Wouldn't it be more natural to assign a new id to the stored instance?
    _id = property(lambda cls: cls._instancedata['id'])
    _dimensions = property(lambda self: self._instancedata['dimensions'])
    
    @classmethod
    def _keys(cls):
        return [p['name'] for p in cls._meta['properties']]

    @classmethod
    def _get_property_unit(cls, name):
        """Returns unit for property `name`, or and empty string if
        `property_name` has no unit."""
        return _get_prop_data(cls, name, 'unit', '')

    @classmethod
    def _get_property_type(cls, name):
        """Returns the type of property `name`."""
        return _get_prop_data(cls, name, 'type')
    
    @classmethod
    def _get_property_description(cls, name):
        """Returns description of property `name`."""
        return _get_prop_data(cls, property_name, 'description', '')
    
    @classmethod
    def _get_property_dims(cls, name):
        """Return a list with the dimensions of property `name`."""
        return _get_prop_data(cls, property_name, 'dims', [1])

    def _save(self, driver, uri, options=None):
        """Stores this entity to `uri` using `driver`."""
        save_entity(self, driver, uri, options=None)

    def _load(self, driver, uri, options=None):
        """Loads data from `uri` into self using `driver`."""
        load_entity_data(self, driver, uri, options=None)

    def _initialized(self):
        """Returns true if all properties are initialized. False is returned
        otherwise."""
        return all(self[p['name']] is not Uninitialized
                   for p in self._meta['properties'])
  
def entity(metadata):
    """Factory fuction for creating an Entity.

    The `metadata` argument is a metadata description (in json format)
    of the entity to create.  It should either be a string or a
    file-like object."""
    if hasattr(metadata, 'read'):
        meta = json.load(metadata)
    else:
        meta = json.loads(metadata)
    instancedata = {
        'id': None,  # XXX
        'dimensions': [0] * len(meta['dimensions']),
    }
    attr = dict(__init__=_init_entity, _meta=meta, _instancedata=instancedata)
    #for p in meta['properties']:
    #    attr[p['name']] = Uninitialized
    return type(str(meta['name']), (BaseEntity,), attr)
    
    
%}


