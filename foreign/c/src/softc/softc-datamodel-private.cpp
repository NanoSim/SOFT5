#include <cstring>
#include "softtypes.h"
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
    soft::StdBlob blob;
    blob.reserve(length);
    blob.assign(value, value+length);
    return model->ref->appendByteArray(key, blob);
  }
  return false;
}

bool softc_datamodel_private_append_string_list (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements)
{
  if (model->ref) {
    soft::StdStringList stringList;
    stringList.reserve(n_elements);
    for (size_t i = 0; i < n_elements; ++i) {
      stringList[i].assign(value[i]);
      return model->ref->appendStringArray(key, stringList);
    }
  }
  return false;
}

bool softc_datamodel_private_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size)
{
  if (model->ref) {
    soft::StdIntArray valueVec(value, value+size);
    return model->ref->appendInt32Array(key, valueVec);
  }
  return false;
}

bool softc_datamodel_private_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size)
{
  if (model->ref) {
    soft::StdDoubleArray valueVec;
    valueVec.reserve(size);
    valueVec.assign (value, value+size);
    return model->ref->appendDoubleArray(key, valueVec);
  }
  return false;
}

bool softc_datamodel_private_append_array_double_2d (softc_datamodel_t *model, const char *key, const double **value, size_t size_i, size_t size_j)
{
  if (model->ref) {
    soft::StdDoubleArray2D valueVec2D(size_i);
    for (size_t i = 0; i < size_i; ++i) {
      soft::StdDoubleArray valueVec(size_j);
      std::copy(value[i], value[i]+size_j, valueVec.begin());
      valueVec2D[i] = valueVec;      
    }
    return model->ref->appendDoubleArray2D(key, valueVec2D);
  }
  return false;
}

bool softc_datamodel_private_append_array_double_3d (softc_datamodel_t *model, const char *key, const double ***value, size_t size_i, size_t size_j, size_t size_k)
{
  return false;
}

bool softc_datamodel_private_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j)
{
  return false;
}

bool softc_datamodel_private_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k)
{
  return false;
}

bool softc_datamodel_private_get_string (const softc_datamodel_t *model, const char *key, char **value)
{
  if (model->ref) {
    soft::StdString str;
    if (model->ref->getString(key, str)) {
      *value = new char[str.size()+1];
      std::copy(str.begin(), str.end(), *value);
      (*value)[str.size()] = '\0';
      return true;
    }
  }
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
  if (model->ref) {
    soft::StdIntArray res;
    if (model->ref->getInt32Array(key, res)) {
      *size = res.size();
      *value = (int32_t*)malloc(sizeof(int32_t)*res.size());
      std::memcpy(*value, res.data(), sizeof(int32_t)*res.size());
      return true;
    }
  }
  return false;
}

bool softc_datamodel_private_get_array_double (const softc_datamodel_t *model, const char *key, double **value, size_t *size)
{
  if (model->ref) {
    soft::StdDoubleArray res;
    if (model->ref->getDoubleArray(key, res)) {
      *size = res.size();
      *value = (double*)malloc(sizeof(double)*res.size());
      std::memcpy(*value, res.data(), sizeof(double)*res.size());
      return true;
    }
  }
  return false;
}

bool softc_datamodel_private_set_id              (softc_datamodel_t* model, const char *id)
{
  if (!model) return false;
  model->ref->setId(id);
  return true;
}

bool softc_datamodel_private_set_meta_name       (softc_datamodel_t* model, const char *meta_name)
{
  if (!model) return false;
  model->ref->setMetaName(meta_name);
  return true;
}

bool softc_datamodel_private_set_meta_version    (softc_datamodel_t* model, const char *meta_version)
{
  if (!model) return false;
  model->ref->setMetaVersion(meta_version);
  return true;

}

bool softc_datamodel_private_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace)
{
  if (!model) return false;
  model->ref->setMetaNamespace(meta_namespace);
  return true;

}

const char * softc_datamodel_private_get_id            (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto std_id = model->ref->id();
  return std_id.c_str();
}

const char * softc_datamodel_private_get_meta_name     (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaName();
  return ret.c_str();
}

const char * softc_datamodel_private_get_meta_version  (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaVersion();
  return ret.c_str();
}

const char * softc_datamodel_private_get_meta_namespace(const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaNamespace();
  return ret.c_str();
}

