#include <gtest/gtest.h>
#include <algorithm>
#include <Soft>

TEST(JSONPluginTest, driverAvailability)
{
  auto drivers = soft::registeredStorageDrivers();
  for (auto d : drivers) {
      std::cout << d << std::endl;
  }
  auto findIter = std::find(drivers.begin(), drivers.end(), "json");
  ASSERT_TRUE(findIter != drivers.end());  
}

TEST(JSONPluginTest, writeData)
{
  soft::Storage storage("json", "output.json");
  auto strategy = storage.strategy();
  ASSERT_TRUE(strategy != nullptr);
  auto model = strategy->dataModel();
  ASSERT_TRUE(model != nullptr);
  model->setId("49176f0a-24bb-4595-b109-5767169691b8");
  model->setMetaName("Quazz");
  model->setMetaVersion("2.0");
  model->setMetaNamespace("http://www.sintef.no/ontology#");
  model->appendDouble("d", 3.14);
  strategy->store(model);
}

TEST(JSONPluginTest, readData)
{
  soft::Storage storage("json", "output.json");
  auto strategy = storage.strategy();
  ASSERT_TRUE(strategy != nullptr);

  auto model = strategy->dataModel();
  ASSERT_TRUE(model != nullptr);

  model->setId("49176f0a-24bb-4595-b109-5767169691b8");
  strategy->startRetrieve(model);
  double compare_d;
  model->getDouble("d", compare_d);

  ASSERT_DOUBLE_EQ(compare_d, 3.14);
}

