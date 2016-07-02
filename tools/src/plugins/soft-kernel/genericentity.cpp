#include <QtCore>
#include <tuple>
#include <map>
#include <algorithm>
#include <cassert>
#include <softtypes.h>
#include "genericentity.h"
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

void appendString(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendString(key, value.toString().toStdString());
}

void appendBool(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendBool(key, value.toBool());
}

void appendFloat(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendFloat(key, (float)value.toDouble());
}

void appendDouble(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendDouble(key, value.toDouble());
}

void appendInt32(IDataModel* dm, const char* key, QJsonValue const &value)
{
  dm->appendInt32(key, value.toInt());
}

void appendInt32Array(IDataModel* dm, const char* key, QJsonValue const &value)
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

void appendDoubleArray(IDataModel* dm, const char* key, QJsonValue const &value)
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

void getInt32(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdInt v;
  dm->getInt32(key, v);
  value = QJsonValue(v);
}

void getInt32Array(IDataModel const* dm, const char* key, QJsonValue &value)
{
  soft::StdIntArray stdIntArray;
  dm->getInt32Array(key, stdIntArray);
  QJsonArray jsonArray;
  for(auto i: stdIntArray) {
    jsonArray.push_back(i);
  }
  value = jsonArray;
}

typedef void(*GetFnPtr)(IDataModel const*,const char*,QJsonValue&);
typedef void(*AppendFnPtr)(IDataModel*,const char*,QJsonValue const&);
using PropertyClass = QPair<QString,int>;
static std::map<PropertyClass, GetFnPtr> getMap = {
  {qMakePair<QString,int>("int", 0), &getInt32},
  {qMakePair<QString,int>("int32", 0), &getInt32},
  {qMakePair<QString,int>("integer", 0), &getInt32},
  {qMakePair<QString,int>("int", 1), &getInt32Array}
};
static std::map<PropertyClass, AppendFnPtr> appendMap = {
  {qMakePair<QString,int>("int", 0), &appendInt32},
  {qMakePair<QString,int>("int32", 0), &appendInt32},
  {qMakePair<QString,int>("integer", 0), &appendInt32},
  {qMakePair<QString,int>("int", 1), &appendInt32Array},
  {qMakePair<QString,int>("string", 0), &appendString},
  {qMakePair<QString,int>("bool", 0), &appendBool},
  {qMakePair<QString,int>("float", 0), &appendFloat},
  {qMakePair<QString,int>("double", 0), &appendDouble},
  {qMakePair<QString,int>("double", 1), &appendDoubleArray}
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
