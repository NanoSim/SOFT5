#include <QtCore>
#include <tuple>
#include <map>
#include <algorithm>
#include <cassert>
#include <softtypes.h>
#include "genericentity.h"
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

namespace {
  
static void appendString(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendString(key, value.toString().toStdString());
}

static void appendBool(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendBool(key, value.toBool());
}

static void appendFloat(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendFloat(key, (float)value.toDouble());
}

static void appendDouble(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendDouble(key, value.toDouble());
}

static void appendInt32(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendInt32(key, value.toInt());
}

static void appendStringArray(IDataModel* dm, const char* key, QJsonValue const &value)
{
  assert(value.isArray());
  auto valueList = value.toArray();    
  soft::StdStringList array(valueList.size());
  std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),
		 [] (QJsonValue const &value) {
		   return value.toString().toStdString();
		 });
  dm->appendStringArray(key, array);
}

static void appendInt32Array(IDataModel* dm, const char* key, QJsonValue const &value)
{
  assert(value.isArray());
  auto valueList = value.toArray();
  soft::StdIntArray array(valueList.size());
  std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),
		 [] (QJsonValue const &value) {
		   return value.toInt();
		 });
  dm->appendInt32Array(key, array);
}

static void appendDoubleArray(IDataModel* dm, const char* key, QJsonValue const &value)
{
  assert(value.isArray());
  auto valueList = value.toArray();
  soft::StdDoubleArray array(valueList.size());
  std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),
		 [] (QJsonValue const &value) {
		   return value.toDouble();
		 });
  dm->appendDoubleArray(key, array);
}

static void appendDoubleArray2D(IDataModel* dm, const char* key, QJsonValue const &value)
{
  assert(value.isArray());
  auto valueList2D = value.toArray();

  soft::StdDoubleArray2D array2D(valueList2D.size());
  foreach(auto value2, valueList2D) {    
    assert(value2.isArray());
    auto valueList = value2.toArray();
    soft::StdDoubleArray array(valueList.size());
    std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),
		   [] (QJsonValue const &value) {
		     return value.toDouble();
		   });
    array2D.push_back(array);
  }
  dm->appendDoubleArray2D(key, array2D);
}

static void getString(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdString v;
  dm->getString(key, v);
  value = QJsonValue(QString::fromStdString(v));
}

static void getBool(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdBool v;
  dm->getBool(key, v);
  value = QJsonValue(v);
}

static void getFloat(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdFloat v;
  dm->getFloat(key, v);
  value = QJsonValue((double)v);
}

static void getDouble(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdDouble v;
  dm->getDouble(key, v);
  value = QJsonValue(v);
}

static void getStringArray(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdStringList v;
  dm->getStringArray(key, v);
  QJsonArray jsonArray;
  for (auto i: v) {
    jsonArray.push_back(QString::fromStdString(i));
  }
  value = jsonArray;
}

static void getDoubleArray(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdDoubleArray v;
  dm->getDoubleArray(key, v);
  QJsonArray jsonArray;
  for(auto i: v) {
    jsonArray.push_back(i);
  }
  value = jsonArray;
}

static void getDoubleArray2D(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdDoubleArray2D v;
  dm->getDoubleArray2D(key, v);
  QJsonArray jsonArray;
  for(auto i: v) {
    QJsonArray jsonArray2;
    for(auto i2: i) {
      jsonArray2.push_back(i2);
    }
    jsonArray.push_back(jsonArray2);
  }
  value = jsonArray;
}

static void getInt32(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdInt v;
  dm->getInt32(key, v);
  value = QJsonValue(v);
}

static void getInt32Array(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdIntArray stdIntArray;
  dm->getInt32Array(key, stdIntArray);
  QJsonArray jsonArray;
  for(auto i: stdIntArray) {
    jsonArray.push_back(i);
  }
  value = jsonArray;
}

}

typedef void(*GetFnPtr)(IDataModel const*,const char*,QJsonValue&);
typedef void(*AppendFnPtr)(IDataModel*,const char*,QJsonValue const&);
using PropertyClass = QPair<QString,int>;
static std::map<PropertyClass, GetFnPtr> getMap = {
  {qMakePair<QString,int>("int",     0), &getInt32},
  {qMakePair<QString,int>("int32",   0), &getInt32},
  {qMakePair<QString,int>("integer", 0), &getInt32},
  {qMakePair<QString,int>("int",     1), &getInt32Array},
  {qMakePair<QString,int>("int32",   1), &getInt32Array},
  {qMakePair<QString,int>("integer", 1), &getInt32Array},
  {qMakePair<QString,int>("string",  0), &getString},
  {qMakePair<QString,int>("string",  1), &getStringArray},
  {qMakePair<QString,int>("bool",    0), &getBool},
  {qMakePair<QString,int>("float",   0), &getFloat},
  {qMakePair<QString,int>("double",  0), &getDouble},
  {qMakePair<QString,int>("double",  1), &getDoubleArray},
  {qMakePair<QString,int>("double",  2), &getDoubleArray2D}
};

static std::map<PropertyClass, AppendFnPtr> appendMap = {
  {qMakePair<QString,int>("int",     0), &appendInt32},
  {qMakePair<QString,int>("int32",   0), &appendInt32},
  {qMakePair<QString,int>("integer", 0), &appendInt32},
  {qMakePair<QString,int>("int",     1), &appendInt32Array},
  {qMakePair<QString,int>("int32",   1), &appendInt32Array},
  {qMakePair<QString,int>("integer", 1), &appendInt32Array},
  {qMakePair<QString,int>("string",  0), &appendString},
  {qMakePair<QString,int>("string",  1), &appendStringArray},
  {qMakePair<QString,int>("bool",    0), &appendBool},
  {qMakePair<QString,int>("float",   0), &appendFloat},
  {qMakePair<QString,int>("double",  0), &appendDouble},
  {qMakePair<QString,int>("double",  2), &appendDoubleArray2D}
};

struct GenericEntity :: Private
{
  QJsonObject schema;
  QJsonObject doc;
  QJsonObject dim;
};

GenericEntity :: GenericEntity()
  : IEntity()
  , d(new GenericEntity::Private())
{}

GenericEntity :: GenericEntity(std::string const &id)
  : IEntity(id)
  , d(new GenericEntity::Private())
{}

GenericEntity :: GenericEntity(const IEntity *other)
  : IEntity(other)
  , d(new GenericEntity::Private())
{}

GenericEntity :: ~GenericEntity()
{}

void GenericEntity :: save(IDataModel *dataModel) const 
{
  auto obj = d->schema;
  auto properties = obj.value("properties").toArray();
  auto dimensions = obj.value("dimensions").toArray();

  auto data = d->doc;
  
  dataModel->setMetaName(obj.value("name").toString().toStdString());
  dataModel->setMetaVersion(obj.value("version").toString().toStdString());
  dataModel->setMetaNamespace(obj.value("namespace").toString().toStdString());
  foreach(auto property, properties) {
    auto obj = property.toObject();
    auto propName = obj.value("name").toString();
    auto propType = obj.value("type").toString();
    auto propRank = obj.contains("dims") ? obj.value("dims").toArray().size() : 0;
    PropertyClass pc(propType, propRank);
    auto propValue = data.value(propName);
    if (!propValue.isUndefined()) {
      auto it = appendMap.find(pc);
      if (it != appendMap.end()) {
	(*it->second)(dataModel, qPrintable(propName), propValue);
      }
    }
  }
}

void GenericEntity :: load(IDataModel const *dataModel) 
{
  auto obj = d->schema;
  auto properties = obj.value("properties").toArray();
  auto dimensions = obj.value("dimensions").toArray();  

  foreach(auto property, properties) {
    auto obj = property.toObject();
    auto propName = obj.value("name").toString();
    auto propType = obj.value("type").toString();
    auto propRank = obj.contains("dims") ? obj.value("dims").toArray().size() : 0;
    PropertyClass pc(propType, propRank);
    
    auto it = getMap.find(pc);
    if (it != getMap.end()) {
      QJsonValue propValue;
      (*it->second)(dataModel, qPrintable(propName), propValue);
      d->doc.insert(propName, propValue);
    }     
  }
}

IEntity* GenericEntity :: create (const std::string &uuid)
{}

std::vector<std::string> GenericEntity :: dimensions() const
{}

int GenericEntity :: getDimensionSize(std::string const &) const
{}

void GenericEntity :: setSchema(std::string const &json)
{
  QByteArray byteArray(QString::fromStdString(json).toUtf8());
  auto schema = QJsonDocument::fromJson(byteArray);
  d->schema = schema.object(); 
}

void GenericEntity :: debug()
{
  QJsonDocument sch(d->schema);
  QJsonDocument doc(d->doc);
}

QVariant GenericEntity :: property(QString const &key) const
{
  return d->doc.value(key).toVariant();
}

void GenericEntity :: setProperty(QString const &key, QVariant const &value)
{
  d->doc.insert(key, QJsonValue::fromVariant(value));
}

SOFT_END_NAMESPACE
