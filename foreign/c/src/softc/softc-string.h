#ifndef SOFTC_STRING_H_DEF
#define SOFTC_STRING_H_DEF

#include "softc-macros.h"
SOFTC_BEGIN_DECLS
typedef struct _softc_string_s* softc_string_s;

softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);
SOFTC_END_DECLS

#endif
