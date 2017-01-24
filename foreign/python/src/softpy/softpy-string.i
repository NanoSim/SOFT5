/* -*- C -*-  (not really, but good for syntax highlighting) */

%{
#include "softc-string.h"
%}


/* String input typemaps */
%typemap("doc") (softc_string_s value) "String."
%typemap(in,numinputs=1) softc_string_s value {
  char *s;
  if (PyUnicode_Check( $input )) {
    s = PyString_AsString(PyUnicode_AsUTF8String($input));
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

%typemap(in) (const softc_string_list_s *strlist) {
  int i;
  PyObject *s;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a sequence");
    return NULL;
  }
  $1 = softc_string_list_create();
  for (i = 0; i < PySequence_Size($input); i++) {
    s = PySequence_GetItem($input, i);
    if (PyUnicode_Check(s)) {
      softc_string_list_append_cstr($1, PyString_AsString(PyUnicode_AsUTF8String(s)));
    } else if (PyString_Check(s)) {
      softc_string_list_append_cstr($1, PyString_AsString(s));
    } else {
      softc_string_list_free($1);
      Py_DECREF(s);
      PyErr_SetString(PyExc_ValueError, "Sequence items must be strings");
      return NULL;
    }
  }
  Py_DECREF(s);
}


/* String_list output typemaps */

/* Set output argument to point to temporary variable */
%typemap(in,numinputs=0) (softc_string_list_s **strlist)
                         (softc_string_list_s* tmp) {
  $1 = &tmp;
}

%typemap(argout,numinputs=0) (softc_string_list_s **strlist) {
  int i;
  size_t n = softc_string_list_count(*$1);
  PyObject *lst = PyList_New(n);
  for (i = 0; i < n; ++i) {
    softc_string_s s = softc_string_at(*$1, i);
    const char *p = from_softc_string(s);
#if PY_MAJOR_VERSION <= 2
    PyObject *str = PyString_FromString(p);
#else
    PyObject *str = PyUnicode_FromString(p);
#endif
    PyList_SetItem(lst, i, str);
  }
  $result = lst;
}

