var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
var entity = new soft.Entity();
var schema = readFile("/home/thomas/Project/soft5/tools/test/plugins/soft-kernel/simple.json");
print (entity.id());
entity.setSchema(schema);
entity.setProperty("a", 42);
entity.debug();
storage.save(entity);
