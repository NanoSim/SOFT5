#include <stdlib.h>
#include "softc.h"
#include "softc-private.h"

softc_t *softc_init(int argc, char *argv[])
{
  return softc_private_init(argc, argv);
}

int softc_storage_driver_count()
{
  return softc_private_registered_storage_driver_count();
}

char ** softc_get_storage_drivers()
{
  return softc_private_get_storage_drivers();
}
