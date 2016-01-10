#include <QtCore>
#include <cstdint>
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

class JSONModel :: Private
{
  friend class JSONModel;
  Private()
  {}

  QJsonObject jsonObject;
  std::function<bool(const char *, std::string&)>          getString;
  std::function<bool(const char *, int8_t&)>               getInt8;
  std::function<bool(const char *, uint8_t&)>              getUInt8;
  std::function<bool(const char *, int16_t &)>             getInt16;
  std::function<bool(const char *, uint16_t &)>            getUInt16;
  std::function<bool(const char *, int32_t &)>             getInt32;
  std::function<bool(const char *, uint32_t &)>            getUInt32;
  std::function<bool(const char *, int64_t &)>             getInt64;
  std::function<bool(const char *, uint64_t &)>            getUInt64;
  std::function<bool(const char *, float &)>               getFloat;
  std::function<bool(const char *, double &)>              getDouble;
  std::function<bool(const char *, bool &)>                getBool;
  std::function<bool(const char *, std::vector<int32_t>&)> getInt32Array;
  std::function<bool(const char *, std::vector<double>&)>  getDoubleArray;
  std::function<bool(const char *, std::vector<unsigned char>&)> getByteArray;
  std::function<bool(const char *, std::vector<std::string> &)> getStringArray;
};

JSONModel :: JSONModel()
  : IDataModel()
  , data (nullptr)
  , d (new JSONModel::Private)
{}

JSONModel :: ~JSONModel()
{
  delete d;
}

void JSONModel :: registerGetInt8Fn(std::function<bool(const char *,int8_t&)> &fn)
{
  d->getInt8 = fn;
}

void JSONModel :: registerGetUInt8Fn(std::function<bool(const char *,uint8_t&)> &fn)
{
  d->getUInt8 = fn;
}

void JSONModel :: registerGetStringFn(std::function<bool(const char *,std::string&)> &fn)
{
  d->getString = fn;
}

void JSONModel :: registerGetInt16Fn(std::function<bool(const char *, int16_t &)> &fn)
{
  d->getInt16 = fn;
}

void JSONModel :: registerGetUInt16Fn(std::function<bool(const char *, uint16_t &)> &fn)
{
  d->getUInt16 = fn;
}

void JSONModel :: registerGetInt32Fn(std::function<bool(const char *, int32_t &)> &fn)
{
  d->getInt32 = fn;
}

void JSONModel :: registerGetUInt32Fn(std::function<bool(const char *, uint32_t &)> &fn)
{
  d->getUInt32 = fn;
}

void JSONModel :: registerGetInt64Fn(std::function<bool(const char *, int64_t &)> &fn)
{
  d->getInt64 = fn;
}

void JSONModel :: registerGetUInt64Fn(std::function<bool(const char *, uint64_t &)> &fn)
{
  d->getUInt64 = fn;
}

void JSONModel :: registerGetFloatFn(std::function<bool(const char *, float &)> &fn)
{
  d->getFloat = fn;
}

void JSONModel :: registerGetDoubleFn(std::function<bool(const char *, double &)> &fn)
{
  d->getDouble = fn;
}

void JSONModel :: registerGetBoolFn(std::function<bool(const char *, bool &)> &fn)
{
  d->getBool = fn;
}

void JSONModel :: registerGetInt32ArrayFn(std::function<bool(const char *, std::vector<int32_t> &)> &fn)
{
  d->getInt32Array = fn;
}

void JSONModel :: registerGetDoubleArrayFn(std::function<bool(const char *, std::vector<double> &)> &fn)
{
  d->getDoubleArray = fn;
}

void JSONModel :: registerGetByteArrayFn(std::function<bool(const char *, std::vector<unsigned char>&)> &fn)
{
  d->getByteArray = fn;
}

void JSONModel :: registerGetStringArrayFn(std::function<bool(const char *, std::vector<std::string> &)> &fn)
{
  d->getStringArray = fn;
}



IDataModel *JSONModel :: createModel()
{
  return new JSONModel;
}

bool JSONModel :: appendVariant (const char *key, StdVariant const &value)
{
  switch (value.type()) {
  case soft::StdTypes::String:
    d->jsonObject.insert(key, QJsonValue(QString::fromStdString(value.get<StdString>())));
    break;
  case soft::StdTypes::Int:
  case soft::StdTypes::UInt:
  case soft::StdTypes::Int8:
  case soft::StdTypes::UInt8:
  case soft::StdTypes::Int64:
  case soft::StdTypes::UInt64:
    d->jsonObject.insert(key, QJsonValue(value.toInt()));
    break;
  case soft::StdTypes::Double:
    d->jsonObject.insert(key, QJsonValue(value.get<StdDouble>()));
    break;
  case soft::StdTypes::DoubleArray:
    QJsonArray array;
    auto v = value.get<StdDoubleArray>();
    for (StdDoubleArray::iterator it = v.begin(); it != v.end(); ++it) {
      array.append(QJsonValue(*it));
    }
    d->jsonObject.insert(key, array);
    break;   
  }
  return true;
}

bool JSONModel :: appendString     (const char* key, const std::string &value)
{
  QJsonValue jsonValue(QString::fromStdString(value));
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendInt8       (const char *key, int8_t value)
{
    QJsonValue jsonValue(value);
    return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendUInt8      (const char *key, uint8_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendInt16      (const char *key, int16_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendUInt16     (const char *key, uint16_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendInt32      (const char *key, int32_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendUInt32     (const char *key, uint32_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendInt64      (const char *key, int64_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendUInt64     (const char *key, uint64_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendFloat      (const char *key, float value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendDouble     (const char *key, double value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendBool       (const char *key, bool value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool JSONModel :: appendInt32Array (const char *key, const std::vector<int32_t> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  d->jsonObject.insert(key, jsonArray);
  return true;
}

bool JSONModel :: appendDoubleArray(const char *key, const std::vector<double> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  d->jsonObject.insert(key, jsonArray);
  return true;
}

bool JSONModel :: appendDoubleArray2D (const char *key, const std::vector<std::vector<double> >& value) 
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    QJsonArray array;
    for (auto &d: v) {
      array.append(QJsonValue(d));
    }    
    jsonArray.append(array);
  }
  d->jsonObject.insert(key, jsonArray);
  return true;
}

bool JSONModel :: appendDoubleArray3D (const char *key, const std::vector<std::vector<std::vector<double> > >& value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    QJsonArray aa;
    for (auto &a: v) {
      QJsonArray array;
      for (auto &d: a) {
	array.append(QJsonValue(d));
      }    
      aa.append(array);
    }
    jsonArray.append(aa);
  }
  d->jsonObject.insert(key, jsonArray);

  return true;
}


bool JSONModel :: appendByteArray  (const char *, const std::vector<unsigned char> &)
{
  return false;
}

bool JSONModel :: appendStringArray(const char *key, const std::vector<std::string> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(QString::fromStdString(v)));
  }
  d->jsonObject.insert(key, jsonArray);
  return true;
}

bool JSONModel :: appendArray      (const char *, const IDataModel *)
{
  return false;
}

bool JSONModel :: appendModel      (const char *, const IDataModel *)
{
  return false;
}

bool JSONModel :: getVariant       (const char *, StdVariant &) const
{
  return false;
}

bool JSONModel :: getString (const char *key, std::string &value) const
{
  if (!d->getString) return false;
  return d->getString(key, value);
}

bool JSONModel :: getInt8          (const char *key, int8_t &value) const
{
  if (!d->getInt8) return false;
  return d->getInt8(key,value);
}

bool JSONModel :: getUInt8         (const char *key, uint8_t &value) const
{
  if (!d->getUInt8) return false;
  return d->getUInt8(key, value);
}

bool JSONModel :: getInt16         (const char *key, int16_t &value) const
{
  if (!d->getInt16) return false;
  return d->getInt16(key, value);
}

bool JSONModel :: getUInt16        (const char *key, uint16_t &value) const
{
  if (!d->getUInt16) return false;
  return d->getUInt16(key, value);
}

bool JSONModel :: getInt32         (const char *key, int32_t &value) const
{
  if (!d->getInt32) return false;
  return d->getInt32(key, value);
}

bool JSONModel :: getUInt32        (const char *key, uint32_t &value) const
{
  if (!d->getUInt32) return false;
  return d->getUInt32(key, value);
}

bool JSONModel :: getInt64         (const char *key, int64_t &value) const
{
  if (!d->getInt64) return false;
  return d->getInt64(key, value);
}

bool JSONModel :: getUInt64        (const char *key, uint64_t &value) const
{
  if (!d->getUInt64) return false;
  return d->getUInt64(key, value);
}

bool JSONModel :: getFloat         (const char *key, float &value) const
{
  if (!d->getFloat) return false;
  return d->getFloat(key, value);
}

bool JSONModel :: getDouble        (const char *key, double &value) const
{
  if (!d->getDouble) return false;
  return d->getDouble(key, value);
}

bool JSONModel :: getBool          (const char *key, bool &value) const
{
  if (!d->getBool) return false;
  return d->getBool(key, value);
}

bool JSONModel :: getInt32Array    (const char *key, std::vector<int32_t> &value) const
{
  if (!d->getInt32Array) return false;
  return d->getInt32Array(key, value);
}

bool JSONModel :: getDoubleArray   (const char *key, std::vector<double> &value) const
{
  if (!d->getDoubleArray) return false;
  return d->getDoubleArray(key, value);
}

bool JSONModel :: getByteArray     (const char *key, std::vector<unsigned char> &value) const
{
  if (!d->getByteArray) return false;
  return d->getByteArray(key, value);
}

bool JSONModel :: getStringArray(const char *key, std::vector<std::string> &value) const
{
  if (!d->getStringArray) return false;
  return d->getStringArray(key, value);
}

bool JSONModel :: getArray         (const char *, IDataModel *) const
{
  return false;
}

bool JSONModel :: getModel         (const char *, IDataModel *) const
{
  return false;
}

const QJsonObject *JSONModel :: json() const
{
  return &d->jsonObject;
}

void JSONModel :: setJson(QJsonObject const &obj)
{
  d->jsonObject = obj;
}

bool JSONModel :: getDoubleArray2D (const char *, std::vector<std::vector<double> > &) const
{
  return false;
}

bool JSONModel :: getDoubleArray3D (const char *, std::vector<std::vector<std::vector<double> > > &) const
{
  return false;
}

SOFT_END_NAMESPACE
