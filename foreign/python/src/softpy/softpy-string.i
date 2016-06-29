/* -*- C -*-  (not really, but good for syntax highlighting) */
softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);

softc_string_s* softc_string_createlist(const char *str[], size_t n);
void softc_string_destroylist(softc_string_s* strlist, size_t n);

%typemap(in,numinputs=0) char **value (const char *str)   { $1 = &temp; };
%typemap(argout)         char **value                     { $result = PyString_FromString(*$1); };
