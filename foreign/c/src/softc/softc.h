#ifndef SOFTC_H
#define SOFTC_H

#include "softc-entity.h"
#include "softc-utils.h"
#include "softc-collection.h"
#include "softc-macros.h"

typedef struct _softc_t softc_t;

SOFTC_BEGIN_DECLS

softc_t * softc_init(int argc, char *argv[]);
void      softc_application_dir_path(char **path, size_t *len);

SOFTC_END_DECLS

#endif 
