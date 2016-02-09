#include "softc-datamodel.h"
#include "softc-datamodel-private.h"

bool softc_datamodel_append_string       (softc_datamodel_t *model, const char *key, const char *value)
{
  return softc_datamodel_private_append_string(model, key, value);
}

bool softc_datamodel_append_int8         (softc_datamodel_t *model, const char *key, int8_t value)
{
  return softc_datamodel_private_append_int8(model, key, value);
}

bool softc_datamodel_append_uint8        (softc_datamodel_t *model, const char *key, uint8_t value)
{
  return softc_datamodel_private_append_uint8(model, key, value);
}

bool softc_datamodel_append_int16        (softc_datamodel_t *model, const char *key, int16_t value)
{
  return softc_datamodel_private_append_int16(model, key, value);
}

bool softc_datamodel_append_uint16       (softc_datamodel_t *model, const char *key, uint16_t value)
{
  return softc_datamodel_private_append_uint16(model, key, value);
}

bool softc_datamodel_append_int32        (softc_datamodel_t *model, const char *key, int32_t value)
{
  return softc_datamodel_private_append_int32(model, key, value);
}

bool softc_datamodel_append_uint32       (softc_datamodel_t *model, const char *key, uint32_t value)
{
  return softc_datamodel_private_append_uint32(model, key, value);
}

bool softc_datamodel_append_int64        (softc_datamodel_t *model, const char *key, int64_t value)
{
  return softc_datamodel_private_append_int64(model, key, value);
}

bool softc_datamodel_append_uint64       (softc_datamodel_t *model, const char *key, uint64_t value)
{
  return softc_datamodel_private_append_uint64(model, key, value);
}

bool softc_datamodel_append_float        (softc_datamodel_t *model, const char *key, float value)
{
  return softc_datamodel_private_append_float(model, key, value);
}

bool softc_datamodel_append_double       (softc_datamodel_t *model, const char *key, double value)
{
  return softc_datamodel_private_append_double(model, key, value);
}

bool softc_datamodel_append_bool         (softc_datamodel_t *model, const char *key, bool value)
{
  return softc_datamodel_private_append_bool(model, key, value);
}

bool softc_datamodel_append_blob         (softc_datamodel_t *model, const char *key, unsigned char *value, size_t length)
{
  return softc_datamodel_private_append_blob(model, key, value, length);
}

bool softc_datamodel_append_string_list   (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements)
{
  return softc_datamodel_private_append_string_list(model, key, value, n_elements);
}

bool softc_datamodel_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size)
{
  return softc_datamodel_private_append_array_int32(model, key, value, size);
}

bool softc_datamodel_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size)
{
  return softc_datamodel_private_append_array_double(model, key, value, size);
}

bool softc_datamodel_append_array_double_2d (softc_datamodel_t *model, const char *key, const double **value, size_t size_i, size_t size_j)
{
  return softc_datamodel_private_append_array_double_2d(model, key, value, size_i, size_j);
}

bool softc_datamodel_append_array_double_3d (softc_datamodel_t *model, const char *key, const double ***value, size_t size_i, size_t size_j, size_t size_k)
{
  return softc_datamodel_private_append_array_double_3d(model, key, value, size_i, size_j, size_k);
}

bool softc_datamodel_get_string      (const softc_datamodel_t *model, const char *key, char **value)
{
  return softc_datamodel_private_get_string(model, key, value);
}

bool softc_datamodel_get_int8        (const softc_datamodel_t *model, const char *key, int8_t *value)
{
  return softc_datamodel_private_get_int8(model, key, value);
}

bool softc_datamodel_get_uint8 (const softc_datamodel_t *model, const char *key, uint8_t *value)
{
  return softc_datamodel_private_get_uint8 (model, key, value);
}

bool softc_datamodel_get_int16 (const softc_datamodel_t *model, const char *key, int16_t *value)
{
  return softc_datamodel_private_get_int16 (model, key, value);
}

bool softc_datamodel_get_uint16 (const softc_datamodel_t *model, const char *key, uint16_t *value)
{
  return softc_datamodel_private_get_uint16 (model, key, value);
}

bool softc_datamodel_get_int32 (const softc_datamodel_t *model, const char *key, int32_t *value)
{
  return softc_datamodel_private_get_int32 (model, key, value);
}

bool softc_datamodel_get_uint32 (const softc_datamodel_t *model, const char *key, uint32_t *value)
{
  return softc_datamodel_private_get_uint32 (model, key, value);
}

bool softc_datamodel_get_int64  (const softc_datamodel_t *model, const char *key, int64_t *value)
{
  return softc_datamodel_private_get_int64 (model, key, value);
}

bool softc_datamodel_get_uint64 (const softc_datamodel_t *model, const char *key, uint64_t *value)
{
  return softc_datamodel_private_get_uint64 (model, key, value);
}

bool softc_datamodel_get_float (const softc_datamodel_t *model, const char *key, float *value)
{
  return softc_datamodel_private_get_float (model, key, value);
}

bool softc_datamodel_get_double (const softc_datamodel_t *model, const char *key, double *value)
{
  return softc_datamodel_private_get_double (model, key, value);
}

bool softc_datamodel_get_bool(const softc_datamodel_t *model, const char *key, bool *value)
{
  return softc_datamodel_private_get_bool (model, key, value);
}

bool softc_datamodel_get_blob (const softc_datamodel_t *model, const char *key, unsigned char **value, size_t *length)
{
  return softc_datamodel_private_get_blob (model, key, value, length);
}

bool softc_datamodel_get_string_list   (const softc_datamodel_t *model, const char *key, char ***value, size_t *n_elements)
{
  return softc_datamodel_private_get_string_list (model, key, value, n_elements);
}

bool softc_datamodel_get_array_int32  (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size)
{
  return softc_datamodel_private_get_array_int32 (model, key, value, size);
}

bool softc_datamodel_get_array_double (const softc_datamodel_t *model, const char *key, double **value, size_t *size)
{
  return softc_datamodel_private_get_array_double (model, key, value, size);
}

bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j)
{
  return softc_datamodel_private_get_array_double_2d(model, key, value, size_i, size_j);
}

bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k)
{
  return softc_datamodel_private_get_array_double_3d(model, key, value, size_i, size_j, size_k);
}


bool softc_datamodel_set_id (softc_datamodel_t* model, const char *id)
{
  return softc_datamodel_private_set_id (model, id);
}

bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name)
{
  return softc_datamodel_private_set_meta_name(model, meta_name);
}

bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version)
{
  return softc_datamodel_private_set_meta_version(model, meta_version);
}

bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace)
{
  return softc_datamodel_private_set_meta_namespace(model, meta_namespace);
}

const char * softc_datamodel_get_id              (const softc_datamodel_t* model)
{
  return softc_datamodel_private_get_id (model);
}

const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model)
{
  return softc_datamodel_private_get_meta_name (model);
}

const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model)
{
  return softc_datamodel_private_get_meta_version (model);
}

const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model)
{
  return softc_datamodel_private_get_meta_namespace (model);
}
