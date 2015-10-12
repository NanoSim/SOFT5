#ifndef SOFTC_H
#define SOFTC_H

#include "softc-entity.h"
#include "softc-utils.h"
#include "softc-collection.h"
#include "softc-macros.h"

typedef struct _softc_t softc_t;

SOFTC_BEGIN_DECLS

softc_t * softc_init(int argc, char *argv[]);

SOFTC_END_DECLS

#endif 
