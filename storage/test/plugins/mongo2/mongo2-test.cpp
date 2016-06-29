#include <gtest/gtest.h>
#include <Soft>
#include "chemkin_reaction.h"

TEST (MongoPluginTest2, checkRegistered)
{
  auto driver = soft::registeredStorageDrivers();
  auto it = std::find(driver.begin(), driver.end(), std::string("mongo2"));
  ASSERT_TRUE(it != driver.end());
}

TEST (MongoPluginTest2, storeData)
{
  auto storage = new soft::Storage("mongo2", "mongodb://localhost", "db=portotest;coll=coll");
  ASSERT_TRUE(nullptr != storage);

  soft::Chemkin_reaction reaction(0, 0, 0, 0, 0);
  storage->save(&reaction);

  soft::Chemkin_reaction copy(0, 0, 0, 0, 0);
  storage->load(&copy);
}
