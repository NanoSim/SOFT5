var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
var entity = new soft.Entity();
var id = entity.id();
var schema = readFile("/home/thomas/Project/soft5/tools/test/plugins/soft-kernel/simple.json");
print (entity.id());
entity.setSchema(schema);
entity.setProperty("a", 42);
entity.setProperty("avec", [1,2,3,4]);
/*
entity.setProperty("b", [1,2,3,4]);
entity.setProperty("c", 3.14);
entity.setProperty("d", [1.1, 2.2, 3.3, 4.4]);
entity.setProperty("e", "Dette er en test");
*/
entity.debug();
storage.save(entity);

var entity2 = new soft.Entity(id);
entity2.setSchema(schema);
storage.load(entity2);

print (entity2.property("avec"));
