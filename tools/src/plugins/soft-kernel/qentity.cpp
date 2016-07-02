#include "qentity.h"

QEntity :: QEntity(QObject *parent)
  : QObject(parent)
  , generic(new soft::GenericEntity())
{}
  
QEntity :: QEntity(QEntity const &other)
  : QObject()
  , generic(other.generic)
{}

QEntity :: QEntity(QString const &id, QObject *parent)
  : QObject(parent)
  , generic(new soft::GenericEntity(qPrintable(id)))
{}

QEntity :: ~QEntity()
{}

QString QEntity :: id() const
{
  return QString::fromStdString(generic->id());
}

QString QEntity :: metaName() const
{
  return QString(); // ::fromStdString(generic->id());
}

QString QEntity :: metaNamespace() const
{
  return QString();
}

QString QEntity :: metaVersion() const
{
  return QString();
}

void QEntity :: debug() const
{
  generic->debug();
}

void QEntity :: setSchema(QString const& schema)
{
  generic->setSchema(schema.toStdString());
}

QVariant QEntity :: property(QString const &key) const
{
  return generic->property(key);
}

void QEntity :: setProperty(QString const& key, QVariant const &value)
{
  generic->setProperty(key,value);
}
