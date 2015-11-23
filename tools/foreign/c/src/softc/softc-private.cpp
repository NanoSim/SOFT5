#include <QCoreApplication>
#include "softc-private.h"

struct _softc_t *softc_private_init(int argc, char *argv[])
{
  struct _softc_t *ret;
  ret = (struct _softc_t*) malloc (sizeof (struct _softc_t));
  ret->app = (void*) new QCoreApplication(argc, argv);
  return ret;
}
