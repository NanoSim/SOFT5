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
%include "numpy.i"  // slightly changed to fit out needs, search for "XXX"

%{
/* typedef bool status_t; */
typedef char const_char;
%}


// Converts Python sequence of strings to (char **, size_t)
%typemap(in,numinputs=1) (char **STRING_LIST, size_t LEN) {
  // typemap(in,numinputs=1) (char **STRING_LIST, size_t LEN)
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
  // typemap(out) char **
  char **p;
  if (!$1) SWIG_fail;
  $result = PyList_New(0);
  for (p=$1; *p; p++) {
    PyList_Append($result, PyString_FromString(*p));
    free(*p);
  }
  free($1);
}

// Converts (const char **) return value to a python list of strings
%typemap(out) const_char ** {
  // typemap(out) const_char **
  char **p;
  if (!$1) SWIG_fail;
  $result = PyList_New(0);
  for (p=$1; *p; p++) {
    PyList_Append($result, PyString_FromString(*p));
  }
}

// Converts (char ***, size_t) to sqeuence of strings
%typemap(in,numinputs=0) (char ***OUT_STRING_LIST, size_t *LEN) (char **s, size_t len) {
  // typemap(in,numinputs=0) (char ***OUT_STRING_LIST, size_t *LEN)
  $1 = &s;
  $2 = &len;
}
%typemap(argout) (char ***OUT_STRING_LIST, size_t *LEN) {
  // typemap(argout) (char ***OUT_STRING_LIST, size_t *LEN)
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
%typemap(out) bool {
  // typemap(out) bool
  if (!$1) SWIG_exception(SWIG_RuntimeError,
			  "false return value in softc_$symname()");
  $result = Py_None;
  Py_INCREF(Py_None); // Py_None is a singleton so increment its reference if used.
}



/**********************************************
 ** Declare functions to wrap
 **********************************************/

/* Remove the softc_ prefix from the python bindings */
%feature("autodoc","1");
%feature("keyword");
%rename("%(strip:[softc_])s") "";
%include <stdint.i>


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
 * entity
 */
%include "softpy-entity.i"


/*
 * datamodel
 */
%include "softpy-datamodel.i"


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
%include "softpy-python.i"
