/* -*- C -*-  (not really, but good for syntax highlighting) */

%{
#include "softc-string.h"
%}


/*
 * FIXME - there are still some bugs for unicode strings both under
 *         Python2 and Python3
 */


/* String input typemaps */
%typemap("doc") (softc_string_s value) "String."
%typemap(in,numinputs=1) softc_string_s value {
  char *s;
  if (PyUnicode_Check( $input )) {
    s = PyString_AsString(PyUnicode_AsUTF8String($input));
  } else if (PyBytes_Check( $input )) {
    s = PyBytes_AsString($input);
  } else if (PyString_Check( $input )) {
    s = PyString_AsString($input);
  } else {
    PyErr_SetString(PyExc_ValueError, "Expecting a string");
    return NULL;
  }
  $1 = softc_string_create(s);
}


/* String output typemaps */
%typemap("doc") (softc_string_s *value) "String."

%typemap(in,numinputs=0) softc_string_s *value (softc_string_s value) {
  $1 = &value;
}

%typemap(argout,numinputs=0) (softc_string_s *value) {
  softc_string_s* v = $1;
#if PY_MAJOR_VERSION <= 2
  $result = PyString_FromString( from_softc_string( *v ));
#else  /* FIXME - not called from Python3 - Why? */
  $result = PyUnicode_FromString( from_softc_string( *v ));
#endif
}

%typemap(freearg) softc_string_s {
   softc_string_destroy( $1 );
}


/* String_list input typemaps */

/*
 * The next two typemaps should specifically match this prototype, including the
 * names of the arguments:
 * bool softc_datamodel_append_string_list(softc_datamodel_t *model, const char *key,
 *                        const softc_string_s *strlist, size_t n_elements);
 */
/* Converts a single Python string list argument into 2 arguments for the C routine */
%typemap(in,numinputs=1) (const softc_string_s* strlist, size_t n_elements)
                         (const softc_string_s* strlist, size_t n) {
  int i;
  char** lst;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = n = PyList_Size($input);
  lst = (char **) malloc(n * sizeof(char *));
  for (i = 0; i < n; ++i) {
    PyObject *s = PyList_GetItem($input, i);
    if (PyUnicode_Check(s)) {
      lst[i] = PyString_AsString(PyUnicode_AsUTF8String(s));
    } else if (PyBytes_Check(s)) {
      lst[i] = PyBytes_AsString(s);
    } else if (PyString_Check(s)) {
      lst[i] = PyString_AsString(s);
    } else {
      free(lst);
      PyErr_SetString(PyExc_ValueError, "List items must be strings");
      return NULL;
    }
  }
  $1 = softc_string_createlist( (const char**)lst, n );
  free(lst);
}

/* After having called softc_datamodel_append_string_list, this one cleans up temporary
 * resources; It complements the 'softc_string_createlist' above.
 */
%typemap(freearg) (const softc_string_s* strlist, size_t n_elements)
                  (softc_string_s* strlist, size_t n) {
  softc_string_destroylist( $1, $2 );
}


/* String_list output typemaps */

/*
 * The next two typemaps should specifically match this prototype:
 * bool softc_datamodel_get_string_list(const softc_datamodel_t *model, const char *key,
 *                        softc_string_s **strlist, size_t *n_elements);
 */
/* Typemap to strip away the 2 return values from the Python argument list */
%typemap(in,numinputs=0) (softc_string_s **strlist, size_t *n_elements)
                         (softc_string_s* strlist, size_t n) {
  $1 = &strlist;
  $2 = &n;
}

/* Typemap to converted the 2 return values from the argument list to the Python result & type */
%typemap(argout,numinputs=0) (softc_string_s **strlist, size_t *n_elements)
                             (softc_string_s* strlist, size_t n) {
  int i;
  size_t len = 0;
  PyObject *lst = 0;
  softc_string_s* a = 0;
  PyObject *s = 0;
  const char* p = 0;
  len = *$2;
  a = *$1;
  lst = PyList_New( len );
  for (i = 0; i < len; ++i) {
    p = from_softc_string( a[i] );
#if PY_MAJOR_VERSION <= 2
    s = PyString_FromString( p );
#else
    s = PyUnicode_FromString( p );
#endif
    PyList_SetItem( lst, i, s );
  }
  softc_string_destroylist( a, len );
  $result = lst;
}

