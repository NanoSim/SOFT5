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

bool JSONModel :: appendDoubleArray(const char *key, const std::vector<double> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  d->jsonObject.insert(key, jsonArray);
}

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

bool JSONModel :: getString (const char *key, std::string &str) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isString()) return false;
  auto qstr = (*it).toString();
  str.resize(qstr.length());
  str = qstr.toStdString();

  return true;    
}

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

bool JSONModel :: getDouble        (const char *key, double &value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isDouble()) return false;
  value = (*it).toDouble();
  return true;
}

bool JSONModel :: getBool          (const char *, bool &) const
{}

bool JSONModel :: getInt32Array    (const char *, std::vector<int32_t> &) const
{}

bool JSONModel :: getDoubleArray   (const char *key, std::vector<double> &value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;
  if (!(*it).isArray()) return false;
  QJsonArray array = (*it).toArray();
  value.resize(array.size());

  auto array_it = array.constBegin();
  auto value_it = value.begin();
  while (array_it !=array.constEnd() && value_it != value.end()) {
    (*value_it++) = (*array_it++).toDouble();
  }

  return true;
}

bool JSONModel :: getByteArray     (const char *, std::vector<unsigned char> &) const
{}

bool JSONModel :: getStringArray(const char *, std::vector<std::string> &) const
{}

bool JSONModel :: getArray         (const char *, IDataModel *) const
{}

bool JSONModel :: getModel         (const char *, IDataModel *) const
{}

const QJsonObject *JSONModel :: json() const
{
  return &d->jsonObject;
}

void JSONModel :: setJson(QJsonObject const &obj)
{
  d->jsonObject = obj;
}

bool JSONModel :: appendDoubleArray2D (const char *, const std::vector<std::vector<double> >&) 
{
  return false;
}

bool JSONModel :: appendDoubleArray3D (const char *, const std::vector<std::vector<std::vector<double> > >&)
{
  return false;
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
