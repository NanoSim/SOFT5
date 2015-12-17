#include "softc-datamodel-private.h"
#include "softc-datamodel-private.hpp"


bool softc_datamodel_private_append_string (softc_datamodel_t *model, const char *key, const char *value)
{
  if (model->ref) {
    return model->ref->appendString(key, value);    
  }
  return false;
}

bool softc_datamodel_private_append_int8 (softc_datamodel_t *model, const char *key, int8_t value)
{
  if (model->ref) {
    return model->ref->appendInt8(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_uint8 (softc_datamodel_t *model, const char *key, uint8_t value)
{
  if (model->ref) {
    return model->ref->appendUInt8(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_int16 (softc_datamodel_t *model, const char *key, int16_t value)
{
  if (model->ref) {
    return model->ref->appendInt16(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_uint16 (softc_datamodel_t *model, const char *key, uint16_t value)
{
  if (model->ref) {
    return model->ref->appendUInt16(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_int32 (softc_datamodel_t *model, const char *key, int32_t value)
{
  if (model->ref) {
    return model->ref->appendInt32(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_uint32 (softc_datamodel_t *model, const char *key, uint32_t value)
{
  if (model->ref) {
    return model->ref->appendUInt32(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_int64 (softc_datamodel_t *model, const char *key, int64_t value)
{
  if (model->ref) {
    return model->ref->appendInt64(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_uint64 (softc_datamodel_t *model, const char *key, uint64_t value)
{
  if (model->ref) {
    return model->ref->appendUInt64(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_float (softc_datamodel_t *model, const char *key, float value)
{
  if (model->ref) {
    return model->ref->appendFloat(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_double (softc_datamodel_t *model, const char *key, double value)
{
  if (model->ref) {
    return model->ref->appendDouble(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_bool (softc_datamodel_t *model, const char *key, bool value)
{
  if (model->ref) {
    return model->ref->appendBool(key, value);
  }
  return false;
}

bool softc_datamodel_private_append_blob (softc_datamodel_t *model, const char *key, const unsigned char *value, size_t length)
{
  if (model->ref) {
    //  return model->ref->appendByteArray(key, value, length);
  }
  return false;
}

bool softc_datamodel_private_append_string_list (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements)
{
  if (model->ref) {
    //   return model->ref->appendStringList(key, value, n_elements);
  }
  return false;
}

bool softc_datamodel_private_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size)
{
  if (model->ref) {
    //    return model->ref->appendInt32Array(key, value, size);
  }
  return false;
}

bool softc_datamodel_private_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size)
{
  if (model->ref) {
    std::vector<double> valueVec;
    valueVec.reserve(size);
    valueVec.assign (value, value+size);
    return model->ref->appendDoubleArray(key, valueVec);
  }
  return false;
}

bool softc_datamodel_private_get_string (const softc_datamodel_t *model, const char *key, char **value)
{
  return false;
}

bool softc_datamodel_private_get_int8 (const softc_datamodel_t *model, const char *key, int8_t *value)
{
  return false;
}

bool softc_datamodel_private_get_uint8 (const softc_datamodel_t *model, const char *key, uint8_t *value)
{
  return false;
}

bool softc_datamodel_private_get_int16 (const softc_datamodel_t *model, const char *key, int16_t *value)
{
  return false;
}

bool softc_datamodel_private_get_uint16 (const softc_datamodel_t *model, const char *key, uint16_t *value)
{
  return false;
}

bool softc_datamodel_private_get_int32 (const softc_datamodel_t *model, const char *key, int32_t *value)
{
  return false;
}

bool softc_datamodel_private_get_uint32 (const softc_datamodel_t *model, const char *key, uint32_t *value)
{
  return false;
}

bool softc_datamodel_private_get_int64 (const softc_datamodel_t *model, const char *key, int64_t *value)
{
  return false;
}

bool softc_datamodel_private_get_uint64 (const softc_datamodel_t *model, const char *key, uint64_t *value)
{
  return false;
}

bool softc_datamodel_private_get_float (const softc_datamodel_t *model, const char *key, float *value)
{
  return false;
}

bool softc_datamodel_private_get_double (const softc_datamodel_t *model, const char *key, double *value)
{
  return false;
}

bool softc_datamodel_private_get_bool (const softc_datamodel_t *model, const char *key, bool *value)
{
  return false;
}

bool softc_datamodel_private_get_blob (const softc_datamodel_t *model, const char *key, unsigned char **value, size_t *length)
{
  return false;
}

bool softc_datamodel_private_get_array_string (const softc_datamodel_t *model, const char *key, char ***value, size_t *n_elements)
{
  return false;
}

bool softc_datamodel_private_get_array_int32 (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size)
{
  return false;
}

bool softc_datamodel_private_get_array_double (const softc_datamodel_t *model, const char *key, double **value, size_t *size)
{
  return false;
}

