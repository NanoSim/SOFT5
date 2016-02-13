/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * SWIG interface file for the python bindings.
 *
 * We use numpy for interfacing arrays.  See
 * http://docs.scipy.org/doc/numpy-1.10.0/reference/swig.interface-file.html
 * for how to use numpy.i.
 *
 * The numpy.i file itself is downloaded from
 * https://github.com/numpy/numpy/blame/master/tools/swig/numpy.i
 */

%define DOCSTRING
"""Python bindings to SOFT5.

softpy provides bindings to most of the SOFT C API using SWIG.


Convinience classes
-------------------
For convenience softpy also implements a set of Python classes
exposing the softc interface in a more pythonic way.  This includes:

  * Storage     : connecting to storage


The entity() factory function
-----------------------------
Finally softpy also defines the entity() factory function, that
creates an entity class based on a metadata description.  See the
entity() docstring for more infomation.


User-defined entities
---------------------
You can also convert your own classes to entities that you can work
with through the layer of convenience classes described above, by
ensuring they have the following methods:

    get_id()
        Returns the uuid of the current instance.  A new uuid can
        be generated with softpy.uuidgen().

    get_meta_type()
        ???

    get_meta_name()
        Returns the entity name.

    get_meta_version()
        Returns the entity version.

    get_meta_namespace()
        Returns the entity namespace.

    get_dimensions()
        Returns a list with dimension labels.

    get_dimension_size(label)
        Returns the size of dimension `label` for this instance.

    store(datamodel)
        Stores all property values to `datamodel` by using the
        softpy.datamodel_append_*() functions.  It is important that
        this is consistent with the load() method.

    load(datamodel)
        Sets all property values from `datamodel` by using the
        softpy.datamodel_get_*() functions.  It is important that this
        is consistent with the store() method.

The convinient classes also allows that you prefix these methods with
an underscore.  The reason for this is that the entities created with the
entity() factory prefix all methods with an underscore in order to avoid
conflicts with property names.
"""
%enddef

%module(docstring=DOCSTRING) softpy



/**********************************************
 ** C code included in the wrapper
 **********************************************/
%{
#include <stdint.h>
#include <stdbool.h>

#include "softc.h"

#define SWIG_FILE_WITH_INIT  // tell numpy that we initialize it in %init

  /* Globals */
  softc_t *softc = NULL;     // a global pointer to a softc instance
  //PyObject *SoftPyError = PyErr_NewException("_softpy.SoftPyError", NULL, NULL);

  /* Wrapper for softc_cleanup() */
  void cleanup(void) {
    softc_cleanup(softc);
    softc = NULL;
  }

  /* Initialize softc calling softc_init() with argc and argv obtained from
   * the sys module */
  void init(void) {
    int i, argc;
    char **argv;
    PyObject *sys, *sys_dict, *argv_obj, **items;
    sys = PyImport_ImportModule("sys");  // new ref
    sys_dict = PyModule_GetDict(sys);    // borrowed ref
    argv_obj = PyMapping_GetItemString(sys_dict, "argv");  // new ref
    argc = PySequence_Length(argv_obj);
    argv = malloc(argc * sizeof(char *));
    items = calloc(argc, sizeof(PyObject *));
    for (i=0; i<argc; i++) {
      items[i] = PySequence_GetItem(argv_obj, i);  // new ref
  #if PY_MAJOR_VERSION <= 2
      argv[i] = PyBytes_AsString(items[i]);  // borrowed memory
  #else
      argv[i] = PyUnicode_AsUTF8(items[i]);  // borrowed memory
  #endif
    }
    softc = softc_init(argc, argv);
    // Cleanup, can the strings pointed to by argv be free()ed?
    //for (i=0; i<argc; i++) if (items[i]) Py_DECREF(items[i]);
    free(items);
    free(argv);
    Py_DECREF(argv_obj);
    Py_DECREF(sys);
  }

%}


/**********************************************
 ** Module initialisation
 **********************************************/
%init %{
  /* Initialize numpy */
  import_array();

  /* Initialize softc */
  init();
%}


/**********************************************
 ** Generic typemaps
 **********************************************/
%include <typemaps.i>
%include <exception.i>
%include <numpy.i>  // slightly changed to fit out needs, search for "XXX"


// Converts Python sequence of strings to (char **, size_t)
%typemap(in,numinputs=1) (char **STRING_LIST, size_t LEN) {
  if (PySequence_Check($input)) {
    $2 = PySequence_Length($input);
    int i = 0;
    $1 = (char **) malloc(($2)*sizeof(char *));
    for (i = 0; i < $2; i++) {
      PyObject *o = PySequence_GetItem($input, i);
      if (PyString_Check(o))
	$1[i] = PyString_AsString(PySequence_GetItem($input, i));
      else {
	PyErr_SetString(PyExc_TypeError,"sequence must contain strings");
	free($1);
	return NULL;
      }
    }
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}
%typemap(freearg) (char **STRING_LIST, size_t LEN) {
  free((char *) $1);
}

// Converts (char **) return value to a python list of strings
%typemap(out) char ** {
  char **p;
  if (!$1) SWIG_fail;
  $result = PyList_New(0);
  for (p=$1; *p; p++) {
    PyList_Append($result, PyString_FromString(*p));
    free(*p);
  }
  free($1);
}

// Converts (char ***, size_t) to sqeuence of strings
%typemap(in,numinputs=0) (char ***OUT_STRING_LIST, size_t *LEN) (char **s, size_t len) {
  $1 = &s;
  $2 = &len;
}
%typemap(argout) (char ***OUT_STRING_LIST, size_t *LEN) {
  int i;
  $result = PyList_New(0);
  for (i=0; i<*$2; i++) {
    PyList_Append($result, PyString_FromString((*$1)[i]));
    free((*$1)[i]);
  }
  free(*$1);
}

// Convert false return value to RuntimeError exception
// Consider to replace a general bool with a ``typedef bool status_t;``
%typemap(out) bool %{
  if (!$1) SWIG_exception(SWIG_RuntimeError,
			  "false return value in softc_$symname()");
  $result = Py_None;
  Py_INCREF(Py_None); // Py_None is a singleton so increment its reference if used.
  //if (!$1) fprintf(stderr, "Warning: False return value in softc_$symname()\n");
  $result = PyBool_FromLong($1);
%}



/**********************************************
 ** Declare functions to wrap
 **********************************************/

/* Remove the softc_ prefix from the python bindings */
%feature("autodoc","1");
%feature("keyword");
%rename("%(strip:[softc_])s") "";
%include "stdint.i"


/* 
 * softc
 */
void   init();  // called automatically in module initialisation...
void   cleanup();

%feature("docstring", "\
Returns the number of available storage drivers.") softc_storage_driver_count;
int    softc_storage_driver_count();

%feature("docstring", "\
Returns a list with storage driver names.") softc_get_storage_drivers;
char **softc_get_storage_drivers();

%feature("docstring", "Returns a new unique uuid.") softc_uuidgen;
char  *softc_uuidgen();


/*
 * datamodel
 */
%typemap(argout,
         fragment="NumPy_Backward_Compatibility,NumPy_Utilities")
  (DATA_TYPE** ARGOUTVIEWM_ARRAY1, DIM_TYPE* DIM1)
{
  npy_intp dims[1] = { *$2 };
  PyObject* obj = PyArray_SimpleNewFromData(1, dims, DATA_TYPECODE, (void*)(*$1));
  PyArrayObject* array = (PyArrayObject*) obj;

  if (!array) SWIG_fail;

%#ifdef SWIGPY_USE_CAPSULE
    PyObject* cap = PyCapsule_New((void*)(*$1), SWIGPY_CAPSULE_NAME, free_cap);
%#else
    PyObject* cap = PyCObject_FromVoidPtr((void*)(*$1), free);
%#endif

%#if NPY_API_VERSION < 0x00000007
  PyArray_BASE(array) = cap;
%#else
  PyArray_SetBaseObject(array,cap);
%#endif

  //$result = SWIG_Python_AppendOutput($result,obj);
  $result = obj;
}



// Create numpy typemaps
%numpy_typemaps(unsigned char, NPY_UBYTE,  size_t)
%numpy_typemaps(int32_t,       NPY_INT32,  size_t)
%numpy_typemaps(double,        NPY_DOUBLE, size_t)

// Wrap softc_datamodel_append_*()
%apply (char       **STRING_LIST, size_t LEN)                            {(const char    **value, size_t n_elements)};
%apply (unsigned char *IN_ARRAY1, size_t DIM1)                           {(unsigned char  *value, size_t length)};
%apply (int32_t       *IN_ARRAY1, size_t DIM1)                           {(const int32_t  *value, size_t size)};
%apply (double        *IN_ARRAY1, size_t DIM1)                           {(const double   *value, size_t size)};
//%apply (double        *IN_ARRAY2, size_t DIM1, size_t DIM2)              {(const double  **value, size_t size_i, size_t size_j)};
//%apply (double        *IN_ARRAY3, size_t DIM1, size_t DIM2, size_t DIM3) {(const double ***value, size_t size_i, size_t size_j, size_t size_k)};
bool softc_datamodel_append_string         (softc_datamodel_t *model, const char *key, const char *value);
bool softc_datamodel_append_int8           (softc_datamodel_t *model, const char *key, int8_t value);
bool softc_datamodel_append_uint8          (softc_datamodel_t *model, const char *key, uint8_t value);
bool softc_datamodel_append_int16          (softc_datamodel_t *model, const char *key, int16_t value);
bool softc_datamodel_append_uint16         (softc_datamodel_t *model, const char *key, uint16_t value);
bool softc_datamodel_append_int32          (softc_datamodel_t *model, const char *key, int32_t value);
bool softc_datamodel_append_uint32         (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_int64          (softc_datamodel_t *model, const char *key, int64_t value);
bool softc_datamodel_append_uint64         (softc_datamodel_t *model, const char *key, uint64_t value);
bool softc_datamodel_append_float          (softc_datamodel_t *model, const char *key, float value);
bool softc_datamodel_append_double         (softc_datamodel_t *model, const char *key, double value);
bool softc_datamodel_append_bool           (softc_datamodel_t *model, const char *key, bool value);
bool softc_datamodel_append_blob           (softc_datamodel_t *model, const char *key, unsigned char  *value, size_t length);
bool softc_datamodel_append_string_list    (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements);
bool softc_datamodel_append_array_int32    (softc_datamodel_t *model, const char *key, const int32_t  *value, size_t size);
bool softc_datamodel_append_array_double   (softc_datamodel_t *model, const char *key, const double   *value, size_t size);
//bool softc_datamodel_append_array_double_2d(softc_datamodel_t *model, const char *key, const double  **value, size_t size_i, size_t size_j);
//bool softc_datamodel_append_array_double_3d(softc_datamodel_t *model, const char *key, const double ***value, size_t size_i, size_t size_j, size_t size_k);
%clear (char         **value, size_t n_elements);
%clear (unsigned char *value, size_t length);
%clear (int32_t       *value, size_t size);
%clear (double        *value, size_t size);
//%clear (double       **value, size_t size_i, size_t size_j);
//%clear (double      ***value, size_t size_i, size_t size_j, size_t size_k);


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


%apply (char            ***OUT_STRING_LIST, size_t *LEN)  {(char         ***value, size_t *n_elements)};
%apply (unsigned char **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(unsigned char **value, size_t *size)};
%apply (int32_t       **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(int32_t       **value, size_t *size)};
%apply (double        **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(double        **value, size_t *size)};
//%apply (double        **ARGOUTVIEWM_ARRAY2, size_t *DIM1, size_t *DIM2) {(double       ***value, size_t *size_i, size_t *size_j)};
//%apply (double        **ARGOUTVIEWM_ARRAY3, size_t *DIM1, size_t *DIM2, size_t *DIM3) {(double        ****value, size_t *size_i, size_t *size_j, size_t *size_k)};
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
bool softc_datamodel_get_string_list     (const softc_datamodel_t *model, const char *key, char ***value, size_t *n_elements);
bool softc_datamodel_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size);
bool softc_datamodel_get_array_double    (const softc_datamodel_t *model, const char *key, double **value, size_t *size);
//bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j);
//bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k);
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
%clear (char ***value, size_t *n_elements);
%clear (unsigned char **value, size_t *length);
%clear (int32_t **value, size_t *size);
%clear (double **value, size_t *size);
//%clear (double ***value, size_t *size_i, size_t *size_j);
//%clear (double ****value, size_t *size_i, size_t *size_j, size_t *size_k);



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
void                softc_storage_strategy_start_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void                softc_storage_strategy_end_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);


/*
 * entity
 */
/* Questions:
 *   - What is the `size` argument of get_dimensions() used to? Should
 *     it not be removed?
 */

%{
  /* Mirror the opaque definition in softc-entity.c */
  typedef struct _softpy_entity_t {

    /* Header common to all softc-entity_t, see softc-entity.c */
    struct softc_entity_vtable_ *vtable_;
    const char *id;

    PyObject *this;  // Reference to python object for this entity

    /* Python callables */
    PyObject *get_meta_name;
    PyObject *get_meta_version;
    PyObject *get_meta_namespace;
    PyObject *get_dimensions;
    PyObject *get_dimension_size;
    PyObject *store;
    PyObject *load;

    /* Entity data*/
    const char *name;
    const char *version;
    const char *namespace;
    size_t ndims;
    const char **dims;
    size_t *dim_sizes;
  } softpy_entity_t;
  
  //const char *softpy_get_meta_name(const softc_entity_t *self) {
  //  return self->meta_name;
  //}
  static const char *softpy_get_meta_name() {
    return "XXX";
  }
  static const char **softpy_get_dimensions(const softc_entity_t *ptr, 
					    size_t *size) {
    int i, n=0;
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    PyObject *args=NULL, *list=NULL, *label=NULL;
    char **s = NULL;
    //if (!self->dims) {
    //}
    //*size = self->dim_size;
    if (!PyCallable_Check(self->get_dimensions)) {
      PyErr_SetString(PyExc_TypeError,"object is not callable");
      return NULL;
    }

    if (!(args = Py_BuildValue("(O)", self->this))) goto fail;
    assert(PyTuple_Check(args));
    if (!(list = PyObject_CallObject(self->get_dimensions, args))) goto fail;
    if (!PySequence_Check(list)) {
      PyErr_SetString(PyExc_TypeError, "callable must return a sequence "
		      "of dimension labels");
      goto fail;
    }
    n = PySequence_Length(list);
    s = calloc(n, sizeof(char *));
    for (i=0; i<n; i++) {
      if (!(label = PySequence_GetItem(list, i))) goto fail;
      if (!PyString_Check(label)) {
	PyErr_SetString(PyExc_TypeError,"dimension labels must be strings");
	goto fail;
      }
      s[i] = strdup(PyString_AsString(label));
      Py_DECREF(label);
      label = NULL;
    }
    Py_DECREF(list);
    Py_DECREF(args);
    return (const char **)s;
  fail:
    if (s) {
      for (i=0; i<n; i++) if (s[i]) free(s[i]);
      free(s);
    }
    if (label) Py_DECREF(label);
    if (list) Py_DECREF(list);
    if (args) Py_DECREF(args);
    return NULL;
  }
  static int softpy_get_dimension_size(const softc_entity_t *ptr, 
				       const char *label) {
    return 3;
  }
  static void softpy_store(const softc_entity_t *ptr,
			   softc_datamodel_t *model) {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    assert(self->store);
    if (!PyCallable_Check(self->store)) {
      PyErr_SetString(PyExc_TypeError,"`store` must be callable");
      return NULL;
    }
    
    if (!(list = PyObject_CallObject(self->get_dimensions, args))) goto fail;    
  }
  
%}

/* Keep softc_entity_t opaque, also to python */
typedef struct {
} softc_entity_t;


%extend softc_entity_t {
  void set_self(PyObject *this) {
    ((softpy_entity_t *)$self)->this = this;
  }
  softc_entity_t(PyObject *get_meta_name,
		 PyObject *get_meta_version,
		 PyObject *get_meta_namespace,
		 PyObject *get_dimensions, 
		 PyObject *get_dimension_size, 
		 PyObject *store, 
		 PyObject *load) {
    softpy_entity_t *self = calloc(1, sizeof(softpy_entity_t));
    self->vtable_ = calloc(1, sizeof(softc_entity_vtable));
    self->vtable_->get_meta_name = softpy_get_meta_name;
    //self->vtable_->get_meta_version = softpy_get_meta_version;
    //self->vtable_->get_meta_namespace = softpy_get_meta_namespace;
    self->vtable_->get_dimensions = softpy_get_dimensions;
    self->vtable_->get_dimension_size = softpy_get_dimension_size;
    //self->vtable_->store = softpy_store;
    //self->vtable_->load = softpy_load;
    self->id = softc_uuidgen();
    Py_INCREF(get_meta_name);
    Py_INCREF(get_meta_version);
    Py_INCREF(get_meta_namespace);
    Py_INCREF(get_dimensions);
    Py_INCREF(get_dimension_size);
    Py_INCREF(store);
    Py_INCREF(load);
    self->get_meta_name = get_meta_name;
    self->get_meta_version = get_meta_version;
    self->get_meta_namespace = get_meta_namespace;
    self->get_dimensions = get_dimensions;
    self->get_dimension_size = get_dimension_size;
    self->store = store;
    self->load = load;
    return (softc_entity_t *)self;
  }
  ~softc_entity_t() {
    softpy_entity_t *e = (softpy_entity_t *)$self;
    free(e->vtable_);
    free((char *)e->id);
    Py_DECREF(e->get_meta_name);
    //Py_DECREF(e->get_meta_version);
    //Py_DECREF(e->get_meta_namespace);
    Py_DECREF(e->get_dimensions);
    Py_DECREF(e->get_dimension_size);
    //Py_DECREF(e->store);
    //Py_DECREF(e->load);
    if (e->this) Py_DECREF(e->this);
    //if (e->dims) {
    //  int i;
    //  for (i=0; i<e->dim_size; i++) free((char *)e->dims[i]);
    //  free(e->dims);
    //}
    //free(e);
  }
};



%typemap(in,numinputs=0) (size_t *IGNORED) {};

//softc_entity_t *softc_entity_new(const char *uri);
const char  *softc_entity_get_id(const softc_entity_t *self);
const char  *softc_entity_get_meta_type(const softc_entity_t *self);
const char  *softc_entity_get_meta_name(const softc_entity_t *self);
const char  *softc_entity_get_meta_namespace(const softc_entity_t *self);
const char  *softc_entity_get_meta_version(const softc_entity_t *self);  
const char **softc_entity_get_dimensions(const softc_entity_t *self, size_t *IGNORED);
int          softc_entity_get_dimension_size (const softc_entity_t *self, const char *label);
void         softc_entity_store(const softc_entity_t *self, softc_datamodel_t *model);
void         softc_entity_load(softc_entity_t *self, const softc_datamodel_t *model);


/*
 * collection
 */
/* Questions:
 *   - What is the difference between softc_collection_create()
 *     and softc_collection_create_ext().  What about namespace?
 *
 *   - Is the ***dimensions arg. a pointer to a newly allocated
 *     NULL-terminated array of string pointers?
 *
 *   - How to treat softc_entity_t? We have so far avoided it, but
 *     maybe we need to make a sort of proxy with a C representation
 *     filling out the needed methods working on an entity.
 */
softc_collection_s * softc_collection_create(const char *id);
//softc_collection_s * softc_collection_create_ext(const char *name, const char *version);
void                 softc_collection_free(softc_collection_s *self);
void                 softc_collection_register_entity(softc_collection_s *self, const char *label, const softc_entity_t *entity);
void                 softc_collection_add_dim(softc_collection_s *self, const char *label, const char *description);
void                 softc_collection_connection(softc_collection_s *self, const char *subject, const char *predicate, const char *object);
size_t               softc_collection_num_entities(softc_collection_s *self);
size_t               softc_collection_num_dims(softc_collection_s *self);
size_t               softc_collection_num_relations(softc_collection_s *self);
size_t               softc_collection_num_dim_maps(softc_collection_s *self);
void                 softc_collection_get_dimensions(softc_collection_s *self, char ***dimensions);



/**********************************************
 ** Python code
 **********************************************/
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



def create_softc_entity(obj, name, version, namespace, dimensions, 
                        dimension_size, store, load):
    """Returns a new C-level entity.

    The main purpose of fhis function is to turn normal Python-objects into
    entities.  This is done by adding a __softc_entity__ attribute to you
    object set to the return value of this function.  Hence

        obj.__softc_entity__ = create_softc_entity(obj, name=...)

    will turn `obj` into an entity.
    """
    e = softpy.entity_t(name, version, namespace, dimensions,
                        dimension_size, store, load)
    e.set_self(obj)
    return e


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


