#ifndef CSV_STRATEGY_H_DEF
#define CSV_STRATEGY_H_DEF

#include <memory>
#include <softns.h>
#include <idatamodel.h>
#include <istoragestrategy.h>

SOFT_BEGIN_NAMESPACE

class CsvStrategy : public IStorageStrategy
{
public:
  CsvStrategy();
  CsvStrategy(const char *uri, const char *options);
  CsvStrategy (CsvStrategy const &) =delete;
  CsvStrategy& operator= (CsvStrategy const &) =delete;  
  virtual ~CsvStrategy();

  IDataModel* dataModel() const   override;
  void store (IDataModel const *) override;
  void startRetrieve (IDataModel *model) const;
  void endRetrieve (IDataModel *model) const;

  const char *metaType() const;
  static IStorageStrategy* create(char const *, char const *);
  static const char *staticMetaType;
private:
  class Private;
  std::unique_ptr<Private> d;
};


SOFT_END_NAMESPACE

#endif // CSV_STRATEGY_H_DEF
