#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>
#include "testentity.h"

class MongoPluginTest : public ::testing::Test {
protected:
  static void SetUpTestCase()
  {}
  
  static void TearDownTestCase()
  {}
};

TEST_F(MongoPluginTest, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("mongo"));
  ASSERT_TRUE(it != driver.end());
}

TEST_F(MongoPluginTest, storeData)
{
  auto storage = new soft::Storage("mongo", "mongodb://localhost", "db=test;coll=coll");
  ASSERT_TRUE (nullptr != storage);
  TestEntity test;
  test.text = "Test";
  test.a = 3.14;
  storage->save(&test);

  TestEntity cpy;
  storage->load(&cpy);
}
