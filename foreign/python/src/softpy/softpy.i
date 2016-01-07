/*  -*- Mode: C -*- */

%define DOCSTRING
"Python bindings to SOFT5."
%enddef

%module(docstring=DOCSTRING) softpy

%feature("autodoc","0");
%feature("keyword");
%include "typemaps.i"


/**********************************************
 ** C code included in the wrapper
 **********************************************/
%{
 #include "softc.h"
 
%}

/*
%init %{
  softc_init();
  %}
*/

/* Remove the softc_ prefix from the python bindings */
%rename("%(strip:[softc_])s") "";

%module argv

%typemap(in) (int argc, char *argv[]) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $1 = PyList_Size($input);
  printf("*** arg1 = %d\n", $1);
  $2 = (char **) malloc(($1+1)*sizeof(char *));
  /*  for (i = 0; i < $1; i++) {
    PyObject *s = PyList_GetItem($input, i);
    printf("*** arg2 = %s\n", PyString_AsString(s));
    if (!PyString_Check(s)) {
        free($2);
        PyErr_SetString(PyExc_ValueError, "List items must be strings");
        return NULL;
    }
    $2[i] = PyString_AsString(s);
    }*/
  $2[0] = "pytest";
  $2[1] = 0;
}

%typemap(freearg) (int argc, char *argv[]) {
   free($2); // If here is uneeded, free(NULL) is legal
}


/**********************************************
 ** softc
 **********************************************/
softc_t * softc_init(int argc, char *argv[]);
int       softc_storage_driver_count();
char   ** softc_get_storage_drivers();
char    * softc_uuidgen();
void      softc_cleanup();



/**********************************************
 ** datamodel
 **********************************************/
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



/**********************************************
 ** storage
 **********************************************/
softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options=NULL);
void              softc_storage_free (softc_storage_t *);
void              softc_storage_load (softc_storage_t *, void *entity);
void              softc_storage_save (softc_storage_t *, void *entity);
softc_storage_strategy_t * softc_storage_get_storage_strategy(softc_storage_t *);


/**********************************************
 ** storage strategy
 **********************************************/
softc_datamodel_t * softc_storage_strategy_get_datamodel(softc_storage_strategy_t *);
void                softc_storage_strategy_store(softc_storage_strategy_t *, const softc_datamodel_t *);
void                softc_storage_strategy_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);

