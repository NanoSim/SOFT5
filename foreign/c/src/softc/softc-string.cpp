#include "softc-string.h"
#include "softc-string-private.hpp"
#include <string>
#include <new>

softc_string_s softc_string_create0()
{
  softc_string_s s = new _softc_string_s(nullptr);
  return s;
}

softc_string_s softc_string_create(const char *str)
{
  softc_string_s s = new _softc_string_s(str);
  return s;
}

const char *from_softc_string(const softc_string_s s)
{
  return s->str.c_str();
}

void softc_string_destroy(softc_string_s str)
{
  delete (str);
}

typedef _softc_string_s* string_p;
softc_string_s* softc_string_createlist0(size_t n)
{
  string_p* a = new string_p[n];
  for (size_t i = 0; i < n; ++i) {
    a[i] = softc_string_create0();
  }
  return a;
}

softc_string_s* softc_string_createlist(const char *str[], size_t n)
{
  string_p* a = new string_p[n];
  for (size_t i = 0; i < n; ++i) {
    a[i] = softc_string_create( str[i] );
  }
  return a;
}

void softc_string_destroylist(softc_string_s* strlist, size_t n)
{
  if (strlist) {
    for (size_t i = 0; i < n; ++i) {
      softc_string_destroy( strlist[i] );
    }
    delete[] strlist;
  }
}

void softc_string_assign(softc_string_s s, const char *text)
{
  if (text) {
    s->str.assign(text);
  }
}

int softc_string_compare(softc_string_s s, const char *text)
{
  if (text) {
    return s->str.compare(text);
  } else {
    return -1;
  }
}
