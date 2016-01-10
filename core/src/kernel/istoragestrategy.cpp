#include "istoragestrategy.h"
#include "idatamodel.h"

SOFT_BEGIN_NAMESPACE

IStorageStrategy :: IStorageStrategy()
{}

IStorageStrategy :: IStorageStrategy(char const *, char const *)
{}

IStorageStrategy :: ~IStorageStrategy()
{}

 void IStorageStrategy :: startRetrieve (IDataModel *m) const
 {
   this->retrieve(m);
 }

void IStorageStrategy :: endRetrieve (IDataModel*) const
{

}

SOFT_END_NAMESPACE
