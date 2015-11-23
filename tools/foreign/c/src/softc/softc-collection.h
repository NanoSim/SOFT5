#ifndef SOFTC_COLLECTION_H_DEF
#define SOFTC_COLLECTION_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS
typedef struct _softc_collection_t softc_collection_t;

softc_collection_t * softc_collection_new();
void                 softc_collection_delete(softc_collection_t *coll);
SOFTC_END_DECLS

#endif /* SOFTC_COLLECTION_H_DEF */
