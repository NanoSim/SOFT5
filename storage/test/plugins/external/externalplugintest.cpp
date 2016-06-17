#include <list>
#include <algorithm>
#include <gtest/gtest.h>
#include <Soft>

class ExternalPluginTest : public ::testing::Test {
protected:
  ExternalPluginTest()
{}
  static void SetUpTestCase()
  {}
  static void TearDownTestCase()
  {}  
};


TEST_F(ExternalPluginTest, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("external"));
  ASSERT_TRUE(it != driver.end());
}
