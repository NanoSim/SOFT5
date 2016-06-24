#include <string>
#include <cstdlib>
#include "softc-string.h"
#include "softc-string-private.hpp"

softc_string_s softc_string_create(const char *str)
{
  softc_string_s s = new _softc_string_s(str);
  return s;
}

const char *from_softc_string(const softc_string_s s)
{
  return s->str.data();
}

void softc_string_destroy(softc_string_s str)
{
  delete (str);
}
