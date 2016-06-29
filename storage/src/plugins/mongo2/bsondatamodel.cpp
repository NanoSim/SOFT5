#include "bsondatamodel.h"

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

BsonDataModel :: BsonDataModel()
  : IDataModel()
{}

BsonDataModel :: ~BsonDataModel() 
{}

IDataModel* BsonDataModel ::createModel() 
{
  return new BsonDataModel;
}

bool BsonDataModel :: appendDimension (const char *key, StdUInt value) 
{
  return dimsObject.append(key, (StdInt)value);
}

bool BsonDataModel :: appendVariant (const char *key, StdVariant const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendString (const char*, const StdString &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendInt8 (const char *key, StdInt8 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendUInt8 (const char *key, StdUInt8 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendInt16 (const char *key, StdInt16 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendUInt16 (const char *key, StdUInt16 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendInt32 (const char *key, StdInt value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendUInt32 (const char *key, StdUInt value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendInt64 (const char *key, StdInt64 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendUInt64 (const char *key, StdUInt64 value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendFloat (const char *key, StdFloat value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendDouble (const char *key, StdDouble value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendBool (const char *key, StdBool value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendInt32Array (const char *key, StdIntArray const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendDoubleArray (const char *key, StdDoubleArray const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendDoubleArray2D (const char *key, StdDoubleArray2D const&value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendDoubleArray3D (const char *key, StdDoubleArray3D const&value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendByteArray (const char *key, StdBlob const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendStringArray (const char *key, StdStringList const &value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendArray (const char *key, IDataModel const * value) 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDimension (const char *key, StdUInt &value) const 
{
  return dimsObject.get(key, reinterpret_cast<StdInt&>(value));
}

bool BsonDataModel :: getVariant (const char *key, StdVariant &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getString (const char *key, StdString &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt8 (const char *key, StdInt8 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getUInt8 (const char *key, StdUInt8 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt16 (const char *key, StdInt16 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getUInt16 (const char *key, StdUInt16 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt32 (const char *key, StdInt &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getUInt32 (const char *key, StdUInt &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt64 (const char *key, StdInt64 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getUInt64 (const char *key, StdUInt64 &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getFloat (const char *key, StdFloat &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDouble (const char *key, StdDouble &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getBool (const char *key, StdBool &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getInt32Array (const char *key, StdIntArray &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDoubleArray (const char *key, StdDoubleArray &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDoubleArray2D (const char *key, StdDoubleArray2D &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getDoubleArray3D (const char *key, StdDoubleArray3D &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getByteArray (const char *key, StdBlob &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getStringArray (const char *key, StdStringList &value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: getArray (const char *key, IDataModel * value) const 
{
  NOT_IMPLEMENTED;
  return false;
}

IDataModel * BsonDataModel :: getModel (const char * model) const 
{
  NOT_IMPLEMENTED;
  return false;
}

bool BsonDataModel :: appendModel (const char *key, IDataModel *model) 
{
  NOT_IMPLEMENTED;
  return false;
} 

SOFT_END_NAMESPACE
