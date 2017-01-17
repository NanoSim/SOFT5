#ifndef SOFTC_STRING_H_DEF
#define SOFTC_STRING_H_DEF

#include "softc-macros.h"
SOFTC_BEGIN_DECLS
typedef struct _softc_string_s* softc_string_s;

softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);
void softc_string_assign(softc_string_s str, const char *text);
int softc_string_compare(softc_string_s str, const char *text);
softc_string_s* softc_string_createlist0(size_t n);
softc_string_s* softc_string_createlist(const char *str[], size_t n);
void softc_string_destroylist(softc_string_s* strlist, size_t n);
bool softc_string_isequal(softc_string_s first, softc_string_s second);
SOFTC_END_DECLS

#endif
