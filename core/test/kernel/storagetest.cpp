#include <memory>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <QtCore>

#include <soft.h>
#include <storage.h>
#include "testentity.h"

class StorageTest : public ::testing::Test {
protected:
  StorageTest()
  {}

  virtual ~StorageTest()
  {}

  virtual void SetUp(){
    _a = 42.0;
    _v = std::vector<double>({1.0, 2.0, 3.0, 4.0, 5.0});
    _text = "this is a string";
  }

public:
  double _a;
  std::vector<double> _v;
  std::string _text;

};

static std::string _uuid;

TEST_F(StorageTest, construct1)
{
  auto storage = new soft::Storage("json", "storagetest.json", "");
  ASSERT_TRUE(nullptr != storage);
  delete storage;
}
  
TEST_F(StorageTest, construct2)
{
  QString const filename = "storagetest.json";
  auto storage = new soft::Storage("json", qPrintable(filename), "");
  
  ASSERT_TRUE(nullptr != storage);
  
  TestEntity test;
  _uuid = test.id();
  test.a = _a;
  test.vec = _v;
  test.text = _text;
  storage->save(&test);
}

TEST_F(StorageTest, construct3)
{
  QString const filename = "storagetest.json";
  auto storage = new soft::Storage("json", qPrintable(filename), "");
  ASSERT_TRUE(storage != nullptr);
  TestEntity test(_uuid);
  ASSERT_STREQ(test.id().c_str(), _uuid.c_str());
  storage->load(&test);
  //ASSERT_THROW(storage->load(&test), std::runtime_error);
  ASSERT_DOUBLE_EQ(test.a, _a);
  ASSERT_STREQ(test.text.c_str(), _text.c_str());
  ASSERT_EQ(test.vec.size(), _v.size());
  for (int i = 0; i < test.vec.size(); ++i) {
    ASSERT_DOUBLE_EQ(test.vec[i], _v[i]);
  }
}  

