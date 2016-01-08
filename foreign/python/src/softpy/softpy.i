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
  for (i=0; i<argc; i++) {
    PyObject *s = PySequence_GetItem(argv_obj, i);  // new ref
    argv[i] = PyString_AsString(s);
    Py_DECREF(s);
  }
  softc_init(argc, argv);
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
/* softc_init() is called automatically in module initialisation */
//softc_t * softc_init(int argc, char *argv[]);
int       softc_storage_driver_count();
char   ** softc_get_storage_drivers();
char    * softc_uuidgen();
void      softc_cleanup();

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


    
def _get_prop_data(cls, property_name, dataname, default=None):
    for p in cls._meta['properties']:
        if p['name'] == property_name:
            return p.get(dataname, default)
    raise NameError('%s has no property named "%s"' % (
        cls._name, dataname))

    
class MetaEntity(type):
    _id = property(lambda self: self._instanceid)
    _name = property(lambda self: self._meta['name'])
    _version = property(lambda self: self._meta['version'])
    _namespace = property(lambda self: self._meta['namespace'])
    _description = property(lambda self: self._meta['description'])
    _dimensions = property(lambda self: self._meta['dimensions'])
    
    def _keys(self):
        return [p['name'] for p in self._meta['properties']]
    
    def _get_unit(self, property_name):
        """Returns unit `property_name`, or None if `property_name` has no 
        unit."""
        return _get_prop_data(self, property_name, 'unit')
    
    def _get_type(self, property_name):
        """Returns the type of `property_name`."""
        return _get_prop_data(self, property_name, 'type')
    
    def _get_description(self, property_name):
        """Returns description of `property_name`."""
        return _get_prop_data(self, property_name, 'description', '')
    
    def _get_dims(self, property_name):
        """Return a list with the dimensions of `property_name`."""
        return _get_prop_data(self, property_name, 'dims', [1])
    
    def _save(self, driver, uri, options=None):
        """Stores this entity to `uri` using `driver`."""
        storage = storage_create(driver, uri, options)
        strategy = storage_get_storage_strategy(storage)
        datamodel = storage_strategy_get_datamodel(strategy)
        # Hmm, shouldn't we always create a new id when storing?
        datamodel_set_id(datamodel, uuidgen())
        datamodel_set_meta_name(datamodel, str(self._name))
        datamodel_set_meta_version(datamodel, str(self._version))
        datamodel_set_meta_namespace(datamodel, str(self._namespace))
        #datamodel_set_meta_description(datamodel, self._description)
        for key in self._keys():
            value = getattr(self, key)
            type = self._get_type(key)
            if value is Uninitialized:
                raise SoftUninitializedError(
                    'Uninitialized data for "%s" in %s' % (
                        key, self.__class__.__name__))
            if type == 'string':
                value = str(value)
            setter = getattr(_softpy, 'datamodel_append_' + type)
            setter(datamodel, str(key), value)
        storage_strategy_store(strategy, datamodel)

    def _load(self, driver, uri, options=None):
        """Loads data into self."""
        storage = softpy.storage_create(driver, uri, options)
        strategy = softpy.storage_get_storage_strategy(storage)
        datamodel = softpy.storage_strategy_get_datamodel(strategy)
        softpy.storage_strategy_retrieve(strategy, datamodel)
        self._instanceid = softc_datamodel_get_id(datamodel)
        self._meta['name'] = softc_datamodel_get_name(datamodel)
        self._meta['version'] = softc_datamodel_get_version(datamodel)
        self._meta['namespace'] = softc_datamodel_get_namespace(datamodel)
        #self._meta['description'] = softc_datamodel_get_description(datamodel)
        for key in self._keys():
            getter = getattr(_softpy, 'datamodel_get_' + self._get_type(key))
            setattr(self, key, getter(datamodel, str(key)))

  
def entity(metadata, default=None):
    """Factory fuction for creating an Entity.

    Parameters
    ----------
    metadata : string | file_like
        The metadata description of the entity to create in json format. 
    default : ???
    """
    if hasattr(metadata, 'read'):
        meta = json.load(metadata)
    else:
        meta = json.loads(metadata)
    attr = dict(__metaclass__=MetaEntity, _meta=meta, _instanceid=None)
    for p in meta['properties']:
        attr[p['name']] = Uninitialized
    return MetaEntity(str(meta['name']), (), attr)
    
    
%}


