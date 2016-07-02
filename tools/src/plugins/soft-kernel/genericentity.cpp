#include <QtCore>
#include <tuple>
#include <map>
#include "genericentity.h"
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

void appendInt(IDataModel* dm, const char* key, QJsonValue& value)
{
  dm->appendInt32(key, value.toInt());
}

typedef void(*FnPtr)(IDataModel*,const char*,QJsonValue&);
using PropertyClass = QPair<QString,int>;
using PropertyClassMap = std::map<PropertyClass, FnPtr>;
static PropertyClassMap appendMap = {{qMakePair<QString,int>("int", 0), &appendInt}};

struct GenericEntity :: Private
{
  QJsonObject schema;
  QJsonObject doc;
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
    qDebug() << propName << propType << propRank;
  }
}

void GenericEntity :: load(IDataModel const *dataModel) 
{
  //JSONModel const *jsonModel = reinterpret_cast<JSONModel const*>(dataModel);
  //d->doc.setObject(*jsonModel->json());
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
  qDebug() << sch.toJson();
  qDebug() << doc.toJson();
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
