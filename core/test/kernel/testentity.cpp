#include <string>
#include "idatamodel.h"
#include "testentity.h"

using namespace soft;

TestEntity :: TestEntity()
  : IEntity()
{}


TestEntity :: ~TestEntity()
{}

IEntity* create (const std::string &)
{
  return dynamic_cast<IEntity*>(new TestEntity());
}

void TestEntity :: save(IDataModel *dataModel) const
{
  dataModel->appendDouble("a", this->a);
  dataModel->appendDoubleArray("vec", this->vec.size(), vec.data());
  dataModel->appendString("text", this->text.c_str());
}

void TestEntity :: load(IDataModel const *dataModel)
{
  
}
