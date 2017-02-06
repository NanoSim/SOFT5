/* -*- C -*-  (not really, but good for syntax highlighting) */

%{
#include "softc-string.h"

PyObject *string_FromString(const char *s)
{
#if PY_MAJOR_VERSION <= 2
  return PyString_FromString(s);
#else
  return  PyUnicode_FromString(s);
#endif

}

%}


/* String input typemaps */
%typemap("doc") softc_string_s IN_SOFTSTRING "$1_name: string"

%typemap(in) softc_string_s IN_SOFTSTRING {
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

%typemap(freearg) softc_string_s IN_SOFTSTRING {
  softc_string_destroy($1);
}


/* String output typemaps */
%typemap(in,numinputs=0) softc_string_s *ARGOUT_SOFTSTRING (softc_string_s v) {
  $1 = &v;
}

%typemap(argout,numinputs=0) (softc_string_s *ARGOUT_SOFTSTRING) {
  const char *s = from_softc_string(*$1);
  $result = string_FromString(s);
}


/* Add descructor */
//%extend softc_string_s {
//  ~softc_string_s() {
//    softc_string_destroy($self);
//  }
//}



/* Declarations */
softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);
void softc_string_assign(softc_string_s str, const char *text);
int softc_string_compare(softc_string_s str, const char *text);
bool softc_string_isequal(softc_string_s first, softc_string_s second);
