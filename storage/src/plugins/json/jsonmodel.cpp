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

SOFT_END_NAMESPACE
