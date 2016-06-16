#include <cstdint>
#include <algorithm>
#include <gtest/gtest.h>
#include <json.h>

#include "json/jsonmodel.h"
#include "softc/softc-datamodel-private.hpp"

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
 
  // Construct a dummy data model
  // TODO: We may need some name/versioning and check here?
  soft::JSONModel j;
  j.appendDouble("amount", 40.0);

  // Construct a wrapper for the data model so that we can pass it to the c interface
  softc_datamodel_t d{&j};
  
  int error;
  const std::string uri = "test.json";
  
  // Check that it is possible to store the data in the given .json file
  error = softc_plugin_save(&d, uri.c_str(), "");
  ASSERT_EQ(SOFTC_STATUS_OK, error);

  // Construct a dummy target data model
  soft::JSONModel k;
  softc_datamodel_t e{&k};

  // Then check that we can read the data from the same .json file
  error = softc_plugin_load(&e, uri.c_str(), "");
  ASSERT_EQ(SOFTC_STATUS_OK, error);

  // Finally, check that the data models (j created above, k read from file) are identical
  double j_amount, k_amount;
  ASSERT_TRUE(j.getDouble("amount", j_amount));
  ASSERT_TRUE(k.getDouble("amount", k_amount));
  ASSERT_EQ(j_amount, k_amount);

  // TODO: Introduce an equality operator for JSONModel or models in general? 
  // TODO: Add version/name check between j and k?
}
