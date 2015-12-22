#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>

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

}
