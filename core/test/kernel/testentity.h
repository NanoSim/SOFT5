#pragma once

#include <vector>
#include <ientity.h>


class TestEntity : public soft::IEntity
{
public:
  SOFT_ENTITY_METADATA("TestEntity", "test", "0.1");
  
public:
  TestEntity();
  virtual ~TestEntity();

  void save(soft::IDataModel *) const;
  void load(soft::IDataModel const *);
  static IEntity* create (const std::string &uuid);

  double a;
  std::vector<double> vec;
  std::string text;
};
