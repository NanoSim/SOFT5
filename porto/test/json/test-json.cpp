#include <cstdint>
#include <algorithm>
#include <gtest/gtest.h>
#include <json.h>

TEST(PortoJsonTest, TestCapabilities) 
{
  int capabilities = softc_plugin_capabilities();

  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_READ);
  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_WRITE);
}

