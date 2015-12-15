#ifndef SOFTC_DATAMODEL_H_DEF
#define SOFTC_DATAMODEL_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_datamodel_t softc_datamodel_t;

void softc_datamodel_append_int32(softc_datamodel_t *model, int value);
void softc_datamodel_get_int32(softc_datamodel_t *model, int *value);
SOFTC_END_DECLS
#endif 
