#include "softc-datamodel-private.h"
#include "softc-datamodel-private.hpp"


void softc_datamodel_private_append_string       (softc_datamodel_t *model, const char *key, const char *value)
{
  if (model->ref) {
    model->ref->appendString(key, value);
  }
}

void softc_datamodel_private_append_int8         (softc_datamodel_t *model, const char *key, int8_t value)
{
}

void softc_datamodel_private_append_uint8        (softc_datamodel_t *model, const char *key, uint8_t value)
{
}

void softc_datamodel_private_append_int16        (softc_datamodel_t *model, const char *key, int16_t value)
{
}

void softc_datamodel_private_append_uint16       (softc_datamodel_t *model, const char *key, uint16_t value)
{
}

void softc_datamodel_private_append_int32        (softc_datamodel_t *model, const char *key, int32_t value)
{
  if (model->ref) {
    model->ref->appendInt32(key, value);
  }
}

void softc_datamodel_private_append_uint32       (softc_datamodel_t *model, const char *key, uint32_t value)
{
}

void softc_datamodel_private_append_int64        (softc_datamodel_t *model, const char *key, int64_t value)
{
}

void softc_datamodel_private_append_uint64       (softc_datamodel_t *model, const char *key, uint64_t value)
{
}

void softc_datamodel_private_append_float        (softc_datamodel_t *model, const char *key, float value)
{
}

void softc_datamodel_private_append_double       (softc_datamodel_t *model, const char *key, double value)
{
  if (model->ref) {
    model->ref->appendDouble(key, value);
  }
}

void softc_datamodel_private_append_bool         (softc_datamodel_t *model, const char *key, bool value)
{
  if (model->ref) {
    model->ref->appendBool(key, value);
  }
}

void softc_datamodel_private_append_blob         (softc_datamodel_t *model, const char *key, unsigned char *value, size_t length)
{
}

void softc_datamodel_private_append_string_list   (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements)
{
}

void softc_datamodel_private_append_array_int32  (softc_datamodel_t *model, const char *key, int32_t *value, size_t size)
{
}

void softc_datamodel_private_append_array_double (softc_datamodel_t *model, const char *key, double *value, size_t size)
{
  if (model->ref) {
    model->ref->appendDoubleArray(key, value, size);
  }
}

void softc_datamodel_private_get_string          (const softc_datamodel_t *model, const char *key, char **value)
{
}

void softc_datamodel_private_get_int8            (const softc_datamodel_t *model, const char *key, int8_t *value)
{
}

void softc_datamodel_private_get_uint8           (const softc_datamodel_t *model, const char *key, uint8_t *value)
{
}

void softc_datamodel_private_get_int16           (const softc_datamodel_t *model, const char *key, int16_t *value)
{
}

void softc_datamodel_private_get_uint16          (const softc_datamodel_t *model, const char *key, uint16_t *value)
{
}

void softc_datamodel_private_get_int32           (const softc_datamodel_t *model, const char *key, int32_t *value)
{
}

void softc_datamodel_private_get_uint32          (const softc_datamodel_t *model, const char *key, uint32_t *value)
{
}

void softc_datamodel_private_get_int64           (const softc_datamodel_t *model, const char *key, int64_t *value)
{
}

void softc_datamodel_private_get_uint64          (const softc_datamodel_t *model, const char *key, uint64_t *value)
{
}

void softc_datamodel_private_get_float           (const softc_datamodel_t *model, const char *key, float *value)
{
}

void softc_datamodel_private_get_double          (const softc_datamodel_t *model, const char *key, double *value)
{
}

void softc_datamodel_private_get_bool            (const softc_datamodel_t *model, const char *key, bool *value)
{
}

void softc_datamodel_private_get_blob            (const softc_datamodel_t *model, const char *key, unsigned char *value, size_t *length)
{
}

void softc_datamodel_private_get_string_list      (const softc_datamodel_t *model, const char *key, const char **value, size_t *n_elements)
{
}

void softc_datamodel_private_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t *value, size_t *size)
{
}

void softc_datamodel_private_get_array_double    (const softc_datamodel_t *model, const char *key, double *value, size_t *size)
{
}

