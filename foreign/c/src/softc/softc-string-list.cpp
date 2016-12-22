#include "softc-string-list.h"
#include <vector>


struct _softc_string_list_s
{
  std::vector<softc_string_s> string_list;
};

softc_string_list_s* softc_string_list_create()
{
  softc_string_list_s* ret = new softc_string_list_s;
}

void softc_string_list_free(softc_string_list_s *self)
{
  while(!self->string_list.empty()) {
    softc_string_destroy(self->string_list.back());
    self->string_list.pop_back();
  }
  delete self;
}

void softc_string_list_append(softc_string_list_s *self, softc_string_s str)
{
  auto it = self->string_list.begin();
  self->string_list.insert(it, str);
}

void softc_string_list_clear(softc_string_list_s *self)
{
  while(!self->string_list.empty()) {
    softc_string_destroy(self->string_list.back());
    self->string_list.pop_back();
  }
}

int softc_string_list_count(softc_string_list_s *self)
{
  return self->string_list.size();
}

softc_string_s softc_string_list_first(softc_string_list_s *self)
{
  return self->string_list.front();
}

softc_string_s softc_string_list_last(softc_string_list_s *self)
{
  return self->string_list.back();
}

