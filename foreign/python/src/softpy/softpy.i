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
//%typemap(in) Int {$1= SWIG_macro(int);}
 //%typemap(in,numinputs=0) char *value {$1= SWIG_macro(int);}

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
// converts false return values to RuntimeError exception
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
const char * softc_datamodel_get_id              (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model);

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
void *        softc_entity_new (const char *uri);
const char *  softc_entity_get_id (const void *self);
const char *  softc_entity_get_meta_type(const void *self);
const char *  softc_entity_get_meta_name(const void *self);
const char *  softc_entity_get_meta_namespace(const void *self);
const char *  softc_entity_get_meta_version(const void *self);  
const char ** softc_entity_get_dimensions (const void *self, size_t *size);
int           softc_entity_get_dimension_size (const void *self, const char *label);
void          softc_entity_store  (const void *self, softc_datamodel_t *data_model);
void          softc_entity_load   (void *self, const softc_datamodel_t *data_model);



/*
 * Entity
 */
/*
%inline %{

typedef struct {
  %immutable;
  char *_id;
  char *_meta_name;
  char *_meta_version;
  char *_meta_namespace;
  %mutable;
} Entity;
 
%}

%extend Entity {
  Entity(const char *id, const char *meta_name,
	 const char *meta_version, const char *meta_namespace) {
    Entity *e = calloc(1, sizeof(Entity));
    e->_id = strdup(id);
    e->_meta_name = strdup(id);
    e->_meta_version = strdup(meta_version);
    e->_meta_namespace = strdup(meta_namespace);
    return e;
  }
  ~Entity() {
    free($self->id);
    free($self->meta_name);
    free($self->meta_version);
    free($self->meta_namespace);
    free($self);
  }
};
*/


/*
%pythoncode %{
def entity(id, meta_name, meta_version, meta_namespace):
    """Factory fuction for creating an Entity.
    """
%}
*/

