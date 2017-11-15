#include <stdexcept>
#include <QtCore>
#include "bsondatamodel.h"
#include "csvstrategy.h"

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

const char *CsvStrategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#csv:0.1-SNAPSHOT-1";

const QString defaultDb = "default";
const QString defaultColl = "default";

class CsvStrategy::Private
{
public:
  Private()
  {
  }

  Private(QString const &uri, QString const &options)
  {
    auto optionMap = ([options]()
                          -> QMap<QString, QString> {
      QMap<QString, QString> ret;
      auto opt = options.split(";");
      foreach (auto o, opt)
      {
        auto pair = o.split("=");
        Q_ASSERT(pair.count() == 2);
        ret.insert(pair[0], pair[1]);
      }
      return ret;
    })();

  }

  ~Private()
  {  
  }
};

const char *CsvStrategy::metaType() const
{
  return CsvStrategy::staticMetaType;
}

CsvStrategy::CsvStrategy()
    : IStorageStrategy(), d(new CsvStrategy::Private())
{
}

CsvStrategy::CsvStrategy(const char *uri, const char *options)
    : IStorageStrategy(uri, options), d(new CsvStrategy::Private(uri, options))
{
}

CsvStrategy::~CsvStrategy()
{
}

IDataModel *CsvStrategy::dataModel() const
{
  return (IDataModel *)new BsonDataModel();
}

void CsvStrategy::store(IDataModel const *model)
{
}

void CsvStrategy::startRetrieve(IDataModel *model) const
{
  QTextStream(stdout) << "CsvStrategy::startRetrieve";
}

void CsvStrategy::endRetrieve(IDataModel *model) const
{
    QTextStream(stdout) << "CsvStrategy::endRetrieve";
}

IStorageStrategy *CsvStrategy::create(char const *uri, char const *opts)
{
  auto strategy = new CsvStrategy(uri, opts);
  return strategy;
}

SOFT_END_NAMESPACE
