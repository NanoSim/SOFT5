#include <QtCore>
#include <QScopedPointer>
#include "externalstrategy.h"
#include "externalmodel.h"

SOFT_BEGIN_NAMESPACE

const char *ExternalStrategy::staticMetaType = "http://sintef.no/soft/TR/external-strategy#external:0.1-SNAPSHOT-1";

const char *ExternalStrategy::metaType() const
{
  return ExternalStrategy::staticMetaType;
}

class ExternalStrategy::Private
{
  friend class ExternalStrategy;
  Private (QString const &uri, QString const &options)
    : uri(uri)
    , options (options)
  {}
  QString uri;
  QString options;
  QScopedPointer<QLibrary> extPlugin;
};

ExternalStrategy :: ExternalStrategy()
  : IStorageStrategy()
  , d(new ExternalStrategy::Private(QString(), QString()))
{}
      
ExternalStrategy :: ExternalStrategy(const char *uri, const char *options)
  : IStorageStrategy()
  , d(new ExternalStrategy::Private(uri, options))
{}

ExternalStrategy :: ~ExternalStrategy()
{
  delete d;
}

IDataModel *ExternalStrategy::dataModel () const
{
  IDataModel *ret = (IDataModel*)new ExternalModel();
  return ret;
}

void ExternalStrategy::store (IDataModel const *model)
{
  ExternalModel const* extModel = dynamic_cast<ExternalModel const*>(model);
  // TODO: Check capability of ext-plugin
  // 
}

void ExternalStrategy::retrieve (IDataModel *model) const
{
  ExternalModel *extModel = dynamic_cast<ExternalModel*>(model);
  // TODO: Check capability
  // TODO: Ask the plugin to populate the data
}

IStorageStrategy* ExternalStrategy::create(char const *uri, char const *opts)
{
  // TODO: Parse opts and load correct plugin 
  // Set correct function pointers
  auto s = new ExternalStrategy(uri, opts);
  return s;
}


SOFT_END_NAMESPACE
