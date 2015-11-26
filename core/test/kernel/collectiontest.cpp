#include <memory>
#include <gtest/gtest.h>
#include <soft.h>
#include <collection.h>
#include <storagestrategy.h>
#include <idatamodel.h>

//static const char * driver  = "mongo";
static const char * driver = "json";
static const char * uri    = "output.json";
static const char * options= "";

//static const char * uri     = "mongodb://localhost";
//static const char * options = "db=soft;coll=testa";

class CollectionTest : public ::testing::Test {
protected:
  CollectionTest() {
  }
  
  virtual ~CollectionTest(){
  }

  static void SetUpTestCase() {
    printf("SetUpTestCase");
    storageStrategy_ = soft::createStrategy(driver, uri, options);
    printf("%s\n", storageStrategy_->metaType());
  }

  static void TearDownTestCase() {
    delete storageStrategy_;
    storageStrategy_ = nullptr;
  }
  
  virtual void SetUp(){
  }
  
  virtual void TearDown(){
  }

  static soft::StorageStrategy *storageStrategy_;
};

soft::StorageStrategy *CollectionTest::storageStrategy_ = nullptr;

TEST_F(CollectionTest, Constructor1) {
  soft::Collection *coll = new soft::Collection();
  ASSERT_TRUE(NULL != coll);
}

TEST_F(CollectionTest, Constructor2) {
  const char *uuid = "6b3827f8-1386-481d-a6cb-396e07575666";
  soft::Collection *coll = new soft::Collection(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}

TEST_F(CollectionTest, Constructor3) {
  soft::Collection *orig = new soft::Collection();
  soft::Collection *coll = new soft::Collection(orig);
  ASSERT_TRUE(NULL != coll);
  delete coll;
  delete orig;
}

TEST_F(CollectionTest, StaticCreate) {
  const char *uuid = "88bf2784-5939-42e9-8b95-2626d21b1c07";
  soft::Collection *coll = (soft::Collection*)soft::Collection::create(uuid);
  ASSERT_TRUE(NULL != coll);
  delete coll;
}


TEST_F(CollectionTest, metaType) {
  /*
  const char *uuid = "bb1bb66e-fa49-40a2-8796-6e2ba6e3c50f";

  soft::IEntity *e = soft::Collection::create(uuid);
  ASSERT_STREQ("http://emmc.eu/TR/metadata-entity#Collection:1.0-SNAPSHOT-4", e->metaType());
  delete e;
  */
}

TEST_F(CollectionTest, save1) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  ASSERT_TRUE(storageStrategy_ != NULL);
  ASSERT_STREQ("http://sintef.no/soft/TR/storage-strategy#json:0.1-SNAPSHOT-1", storageStrategy_->metaType());

  // soft::IDataModel const* model = CollectionTest::storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);
  //  auto model = storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);

  //  model->appendInt32("test", 42);
  //  storageStrategy_->store(model);
  
  delete e;
}

TEST_F(CollectionTest, load1) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  //  auto model = storageStrategy_->dataModel();
  //  ASSERT_TRUE(model != NULL);
  delete e;
}

TEST_F(CollectionTest, registerEntity) {
  soft::IEntity *e = soft::Collection::create();
  ASSERT_TRUE(e != NULL);

  soft::Collection collection;
  collection.registerEntity("label1", e);
  ASSERT_EQ(1, collection.numEntities());
}

TEST_F(CollectionTest, collection) {
  soft::Collection collection;
  collection.addRelation("label1", "inherits", "label2");
  collection.addRelation("label2", "inherits", "label3");
}

TEST_F(CollectionTest, registerAndFetch) {
  soft::Collection subCollection;
  soft::Collection collection;
  collection.registerEntity("subCollection", &subCollection);
  collection.connect("subCollection", "is", "subCollection");

  soft::IEntity const *e = collection.findInstance("subCollection");
  ASSERT_TRUE(e != NULL);
  ASSERT_STREQ(e->id().c_str(), subCollection.id().c_str());
}

TEST_F (CollectionTest, findConnections1) {
  soft::Collection a;
  soft::Collection b;
  soft::Collection collection;
  collection.registerEntity("sub", &a);
  collection.registerEntity("obj", &b);
  collection.connect("sub", "has-a", "obj");
  auto p = collection.findRelations("sub");

  ASSERT_TRUE(p.size() == 1);
  ASSERT_STREQ(p.front().object().c_str(), "obj");
  ASSERT_STREQ(p.front().predicate().c_str(), "has-a");
}

TEST_F (CollectionTest, findConnections2) {
  soft::Collection a;
  soft::Collection b;
  soft::Collection collection;
  collection.registerEntity("sub", &a);
  collection.registerEntity("obj", &b);
  collection.connect("sub", "has-a", "obj");
  collection.connect("obj", "owned-by", "sub");
  auto p = collection.findRelations("sub");
  ASSERT_TRUE(p.size() == 1);
  ASSERT_STREQ(p.front().object().c_str(), "obj");
  ASSERT_STREQ(p.front().predicate().c_str(), "has-a");
}
