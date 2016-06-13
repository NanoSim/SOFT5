#ifndef EXTERNALSTRATEGY_H_DEF
#define EXTERNALSTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"
#include "istoragestrategy.h"

SOFT_BEGIN_NAMESPACE

class ExternalStrategy : public IStorageStrategy
{
public:
  ExternalStrategy();
  ExternalStrategy(const char *uri, const char *options);
  ExternalStrategy& operator= (ExternalStrategy const&) =delete;
  virtual ~ExternalStrategy();

  IDataModel      *dataModel   () const   override;
  void             store       (IDataModel const *) override;
  void             retrieve    (IDataModel *) const override;
  
  const char *metaType() const override;
  static IStorageStrategy* create(char const *uri, char const *opts);
  static const char *staticMetaType;
private:
};

SOFT_END_NAMESPACE

#endif // EXTERNALSTRATEGY_H_DEF
