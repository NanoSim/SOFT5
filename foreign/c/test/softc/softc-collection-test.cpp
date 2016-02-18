#ifdef RUN_COLLECTION
#include <gtest/gtest.h>
#include <softc/softc-utils.h>
#include <softc/softc-entity.h>
#include <softc/softc-collection.h>

#include "foo.h"

class SoftC_CollectionTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}      
};

TEST_F (SoftC_CollectionTest, construct)
{
  softc_collection_s *coll = softc_collection_create(NULL);
  ASSERT_TRUE(coll != NULL);
  softc_collection_free(coll);
}


TEST_F (SoftC_CollectionTest, store1)
{
  softc_collection_s *coll = softc_collection_create(NULL);
  foo_s *foo = foo_create(NULL, 10);
  foo_property_set_n(foo, 42);
  foo_property_set_str(foo, "Dette er en test");

  softc_collection_register_entity(coll, "foo", (const softc_entity_t*)foo);
  softc_collection_free(coll);
  foo_free(foo);
}

#endif
