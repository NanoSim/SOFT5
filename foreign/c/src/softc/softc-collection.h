#ifndef SOFTC_COLLECTION_H_DEF
#define SOFTC_COLLECTION_H_DEF

#include "softc-macros.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_collection_s softc_collection_s;

softc_collection_s * softc_collection_create(const char *id);
void                 softc_collection_free(softc_collection_s *self);
void                 softc_collection_register_entity(softc_collection_s *self, const char *label, const softc_entity_t *entity);

SOFTC_END_DECLS

#endif /* SOFTC_COLLECTION_H_DEF */
