#include <stdlib.h>
#include "softc-collection.h"
#include "softc-collection-private.h"

softc_collection_t * softc_collection_new()
{
  return NULL;
}

void softc_collection_delete(softc_collection_t *coll)
{
  if (coll != NULL)
    free(coll);
}
