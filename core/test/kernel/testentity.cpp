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
  dataModel->appendDoubleArray("vec", vec.data(), this->vec.size());
  dataModel->appendString("text", this->text.c_str());
}

void TestEntity :: load(IDataModel const *dataModel)
{
  dataModel->getDouble("a", &this->a);
  double **vecptr = new double*;
  size_t len;
  dataModel->getDoubleArray("vec", vecptr, &len);
  this->vec.clear();
  this->vec.reserve(len);
  double *ptr = *vecptr;
  for (size_t i = 0; i < len; ++i){
    this->vec.push_back(*ptr++);
  }
  dataModel->getString("text", this->text);
}
