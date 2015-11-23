#include <gtest/gtest.h>
#include <soft.h>
#include <storage.h>
#include "testentity.h"

class StorageTest : public ::testing::Test {
protected:
  StorageTest()
  {}

  virtual ~StorageTest()
  {}
};


TEST_F(StorageTest, construct1)
{
  auto storage = new soft::Storage("json", "storagetest.json", "");
  ASSERT_TRUE(nullptr != storage);
}
  
TEST_F(StorageTest, construct2)
{
  auto storage = new soft::Storage("json", "storagetest.json", "");
  
  ASSERT_TRUE(nullptr != storage);

  TestEntity test;
  test.vec.assign(0.1, 10);
  storage->save(&test);
}
