#include <gtest/gtest.h>
#include <collection.h>
#include "simple.h"
#include "financial.h"
#include "physics.h"


TEST(codegen, dummy)
{
  ASSERT_TRUE(!false);
}

TEST(codegen, construct)
{
  using namespace soft;
  Simple *simple = new Simple (3,2,3,0); 
  ASSERT_TRUE(nullptr != simple);

  auto dims = simple->dimensions();
  ASSERT_STREQ(dims[0].c_str(), "NI");
  ASSERT_STREQ(dims[1].c_str(), "NJ");
  ASSERT_STREQ(dims[2].c_str(), "NK");
  ASSERT_STREQ(dims[3].c_str(), "NL");

  delete simple;
}

TEST(codegen, collectionAdd)
{
  using namespace soft;
  Simple *simple = new Simple (3,2,3,0); 
  Collection collection;
  collection.attachEntity("simple1", simple);
  auto instance = collection.findInstance("simple1");
  ASSERT_EQ(instance, simple);
}
