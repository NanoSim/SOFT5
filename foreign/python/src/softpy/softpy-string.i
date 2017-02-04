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
%typemap(in,numinputs=0) softc_string_s *value (softc_string_s value) {
  $1 = &value;
}

%typemap(argout,numinputs=0) (softc_string_s *value) {
  softc_string_s* v = $1;
#if PY_MAJOR_VERSION <= 2
  $result = PyString_FromString( from_softc_string( *v ));
#else
  $result = PyUnicode_FromString( from_softc_string( *v ));
#endif
}

%typemap(freearg) softc_string_s {
   softc_string_destroy( $1 );
}


/* Declarations */
softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);
//void softc_string_assign(softc_string_s str, const char *text);
int softc_string_compare(softc_string_s str, const char *text);
bool softc_string_isequal(softc_string_s first, softc_string_s second);
