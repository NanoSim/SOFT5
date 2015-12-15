#include "softc-datamodel.h"
#include "softc-datamodel-private.h"

void softc_datamodel_append_string       (softc_datamodel_t *model, const char *key, const char *value)
{
  softc_datamodel_private_append_string(model, key, value);
}

void softc_datamodel_append_int8         (softc_datamodel_t *model, const char *key, int8_t value)
{
  softc_datamodel_private_append_int8(model, key, value);
}

void softc_datamodel_append_uint8        (softc_datamodel_t *model, const char *key, uint8_t value)
{
  softc_datamodel_private_append_uint8(model, key, value);
}

void softc_datamodel_append_int16        (softc_datamodel_t *model, const char *key, int16_t value)
{
  softc_datamodel_private_append_int16(model, key, value);
}

void softc_datamodel_append_uint16       (softc_datamodel_t *model, const char *key, uint16_t value)
{
  softc_datamodel_private_append_uint16(model, key, value);
}

void softc_datamodel_append_int32        (softc_datamodel_t *model, const char *key, int32_t value)
{
  softc_datamodel_private_append_int32(model, key, value);
}

void softc_datamodel_append_uint32       (softc_datamodel_t *model, const char *key, uint32_t value)
{
  softc_datamodel_private_append_uint32(model, key, value);
}

void softc_datamodel_append_int64        (softc_datamodel_t *model, const char *key, int64_t value)
{
  softc_datamodel_private_append_int64(model, key, value);
}

void softc_datamodel_append_uint64       (softc_datamodel_t *model, const char *key, uint64_t value)
{
  softc_datamodel_private_append_uint64(model, key, value);
}

void softc_datamodel_append_float        (softc_datamodel_t *model, const char *key, float value)
{
  softc_datamodel_private_append_float(model, key, value);
}

void softc_datamodel_append_double       (softc_datamodel_t *model, const char *key, double value)
{
  softc_datamodel_private_append_double(model, key, value);
}

void softc_datamodel_append_bool         (softc_datamodel_t *model, const char *key, bool value)
{
  softc_datamodel_private_append_bool(model, key, value);
}

void softc_datamodel_append_blob         (softc_datamodel_t *model, const char *key, unsigned char *value, size_t length)
{
  softc_datamodel_private_append_blob(model, key, value, length);
}

void softc_datamodel_append_string_list   (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements)
{
  softc_datamodel_private_append_string_list(model, key, value, n_elements);
}

void softc_datamodel_append_array_double (softc_datamodel_t *model, const char *key, double *value, size_t size)
{
  softc_datamodel_private_append_array_double(model, key, value, size);
}

void softc_datamodel_get_string      (softc_datamodel_t *model, const char *key, char **value)
{
  softc_datamodel_private_get_string(model, key, value);
}

void softc_datamodel_get_int8        (softc_datamodel_t *model, const char *key, int8_t *value)
{
  softc_datamodel_private_get_int8(model, key, value);
}

void softc_datamodel_get_uint8       (softc_datamodel_t *model, const char *key, uint8_t *value)
{
  softc_datamodel_get_uint8       (model, key, value);
}

void softc_datamodel_get_int16        (softc_datamodel_t *model, const char *key, int16_t *value)
{
  softc_datamodel_get_int16        (model, key, value);
}

void softc_datamodel_get_uint16       (softc_datamodel_t *model, const char *key, uint16_t *value)
{
  softc_datamodel_get_uint16       (model, key, value);
}

void softc_datamodel_get_int32        (softc_datamodel_t *model, const char *key, int32_t *value)
{
  softc_datamodel_get_int32        (model, key, value);
}

void softc_datamodel_get_uint32       (softc_datamodel_t *model, const char *key, uint32_t *value)
{
  softc_datamodel_get_uint32       (model, key, value);
}

void softc_datamodel_get_int64        (softc_datamodel_t *model, const char *key, int64_t *value)
{
  softc_datamodel_get_int64        (model, key, value);
}

void softc_datamodel_get_uint64       (softc_datamodel_t *model, const char *key, uint64_t *value)
{
  softc_datamodel_get_uint64       (model, key, value);
}

void softc_datamodel_get_float        (softc_datamodel_t *model, const char *key, float *value)
{
  softc_datamodel_get_float        (model, key, value);
}

void softc_datamodel_get_double       (softc_datamodel_t *model, const char *key, double *value)
{
  softc_datamodel_get_double       (model, key, value);
}

void softc_datamodel_get_bool         (softc_datamodel_t *model, const char *key, bool *value)
{
  softc_datamodel_get_bool         (model, key, value);
}

void softc_datamodel_get_blob         (softc_datamodel_t *model, const char *key, unsigned char *value, size_t *length)
{
  softc_datamodel_get_blob         (model, key, value, length);
}

void softc_datamodel_get_stringList   (softc_datamodel_t *model, const char *key, const char **value, size_t *n_elements)
{
  softc_datamodel_get_stringList   (model, key, value, n_elements);
}

void softc_datamodel_get_array_int32  (softc_datamodel_t *model, const char *key, int32_t *value, size_t *size)
{
  softc_datamodel_get_array_int32  (model, key, value, size);
}

void softc_datamodel_get_array_double (softc_datamodel_t *model, const char *key, double *value, size_t *size)
{
  softc_datamodel_get_array_double (model, key, value, size);
}
