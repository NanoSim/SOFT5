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

void JSONModel :: appendDoubleArray(const char *key, size_t length, const double *value)
{}

void JSONModel :: appendByteArray(const char* key, const unsigned char *data, size_t size)
{}


SOFT_END_NAMESPACE
