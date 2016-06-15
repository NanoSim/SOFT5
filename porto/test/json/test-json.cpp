#include <cstdint>
#include <algorithm>
#include <gtest/gtest.h>
#include <json.h>

#include "json/jsonmodel.h"

TEST(PortoJsonTests, TestIdentify) {
  {
    const char expected[] = "porto-json";
    const int maxlen = sizeof(expected);
  
    char name[maxlen + 1]; // String with a canari bird at the end
    name[maxlen] = '*'; // The function should never overwrite past maxlen!

    int error = softc_plugin_identify(name, maxlen);

    ASSERT_EQ(SOFTC_STATUS_OK, error);

    ASSERT_STREQ("porto-json", name);
    ASSERT_EQ('*', name[maxlen]);
  }

  {
    // Here we construct a too small buffer, but expect the canari to be intact as well
    // as a null-termination on the actual string
    const char expected[] = "porto-json";
    const int maxlen = sizeof(expected) - 2;

    char name[maxlen + 1];
    name[maxlen] = '*';

    int error = softc_plugin_identify(name, maxlen);

    ASSERT_EQ(SOFTC_STATUS_OK, error);
    
    ASSERT_STREQ("porto-js", name);
    ASSERT_EQ('*', name[maxlen]);
    ASSERT_EQ('\0', name[maxlen - 1]);
  }
}


TEST(PortoJsonTests, TestCapabilities) 
{
  int capabilities = softc_plugin_capabilities();

  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_READ);
  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_WRITE);
}

TEST(PortoJsonTests, TestSaveLoad) {
  // TODO: Construct a datamodel!
  
  softc_datamodel_t d;
  
  int error = softc_plugin_save(&d, "test.json", "");

  // TODO: Load the saved plugin and check that the data models are the same

  ASSERT_EQ(error, SOFTC_STATUS_OK);
}
