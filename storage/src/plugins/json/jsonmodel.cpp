#include <QtCore>
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

class JSONModel :: Private
{
  friend class JSONModel;
  Private()
  {}

  QJsonObject jsonObject;
};

JSONModel :: JSONModel()
  : IDataModel()
  , d (new JSONModel::Private)
{}

JSONModel :: ~JSONModel()
{
  delete d;
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

bool JSONModel :: appendBool       (const char *, bool)
{}

bool JSONModel :: appendInt32Array (const char *, const std::vector<int32_t> &)
{}

bool JSONModel :: appendDoubleArray(const char *, const std::vector<double> &)
{}

bool JSONModel :: appendByteArray  (const char *, const std::vector<unsigned char> &)
{}

bool JSONModel :: appendStringArray(const char *, const std::vector<std::string> &)
{}

bool JSONModel :: appendArray      (const char *, const IDataModel *)
{}

bool JSONModel :: appendModel      (const char *, const IDataModel *)
{}

bool JSONModel :: getVariant       (const char *, StdVariant &) const
{}

bool JSONModel :: getString        (const char *, std::string &str) const
{}

bool JSONModel :: getInt8          (const char *, int8_t &) const
{}

bool JSONModel :: getUInt8         (const char *, uint8_t &) const
{}

bool JSONModel :: getInt16         (const char *, int16_t &) const
{}

bool JSONModel :: getUInt16        (const char *, uint16_t &) const
{}

bool JSONModel :: getInt32         (const char *, int32_t &) const
{}

bool JSONModel :: getUInt32        (const char *, uint32_t &) const
{}

bool JSONModel :: getInt64         (const char *, int64_t &) const
{}

bool JSONModel :: getUInt64        (const char *, uint64_t &) const
{}

bool JSONModel :: getFloat         (const char *, float &) const
{}

bool JSONModel :: getDouble        (const char *, double &) const
{}

bool JSONModel :: getBool          (const char *, bool &) const
{}

bool JSONModel :: getInt32Array    (const char *, std::vector<int32_t> &) const
{}

bool JSONModel :: getDoubleArray   (const char *, std::vector<double> &) const
{}

bool JSONModel :: getByteArray     (const char *, std::vector<unsigned char> &) const
{}

bool JSONModel :: appendStringArray(const char *, std::vector<std::string> &) const
{}

bool JSONModel :: getArray         (const char *, IDataModel *) const
{}

bool JSONModel :: getModel         (const char *, IDataModel *) const
{}

/*
void JSONModel :: appendInt32(const char *key, int value)
{
  QJsonValue jsonValue(value);
  d->jsonObject.insert(key, jsonValue);
}

void JSONModel :: appendString(const char *key, const std::string &str)
{
  QJsonValue jsonValue(QString::fromStdString(str));
  d->jsonObject.insert(key, jsonValue);
}

void JSONModel :: appendDouble(const char *key, double value)
{
  QJsonValue jsonValue(value);
  d->jsonObject.insert(key, jsonValue);
}

void JSONModel :: appendBool(const char *key, bool value)
{
  QJsonValue jsonValue(value);
  d->jsonObject.insert(key, jsonValue);
}

void JSONModel :: appendArray(const char *key, IDataModel *model)
{
  auto m = dynamic_cast<JSONModel*>(model);
  QJsonValue jsonValue(m->d->jsonObject);
  d->jsonObject.insert(key, jsonValue);
}

void JSONModel :: appendModel(const char *key, IDataModel *model)
{
  auto m = dynamic_cast<JSONModel*>(model);
  QJsonValue jsonValue(m->d->jsonObject);
  d->jsonObject.insert(key, jsonValue);
}

const QJsonObject *JSONModel :: json() const
{
  return &d->jsonObject;
}

void JSONModel :: setJson(QJsonObject const &obj)
{
  d->jsonObject = obj;
}

void JSONModel :: appendDoubleArray(const char *key, const double *value, size_t length)
{
  QJsonArray jsonArray;
  double const *ptr = value;
  for (size_t idx = 0; idx < length; ++idx) {
    jsonArray.append(QJsonValue(*(ptr++)));;
  }
  d->jsonObject.insert(key, jsonArray);
}

void JSONModel :: appendByteArray(const char* key, const unsigned char *data, size_t size)
{}

bool JSONModel :: getInt32(const char *key, int *value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isDouble()) return false;
  *value = (*it).toInt();
  return true;
}

bool JSONModel :: getDouble(const char *key, double *value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isDouble()) return false;
  *value = (*it).toDouble();
  return true;
}

bool JSONModel :: getBool(const char *key, bool *value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isBool()) return false;
  *value = (*it).toBool();
  return true;
}

bool JSONModel :: getDoubleArray(const char *key, double **valueptr, size_t *len) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;
  if (!(*it).isArray()) return false;
  QJsonArray array = (*it).toArray();
  *len = array.size();
  valueptr[0] = new double[*len];
  double *ptr = valueptr[0];
  for (auto v : array) {
    *ptr++ = v.toDouble();
  }

  return true;
}

bool JSONModel :: getString(const char *key, std::string &str) const 
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isString()) return false;
  str = (*it).toString().toStdString();

  return true;    
}

void JSONModel :: appendVariant(const char *key, StdVariant const & value)
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
}

bool JSONModel :: getVariant(const char *key, StdVariant &value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  
  return true;
}
*/
SOFT_END_NAMESPACE
