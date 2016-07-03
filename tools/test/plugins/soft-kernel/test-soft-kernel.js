var storage = new soft.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
var entity = new soft.Entity();
var id = entity.id();
var schema = readFile("/home/thomas/Project/soft5/tools/test/plugins/soft-kernel/simple.json");
print (entity.id());


entity.setSchema(schema);
entity.setProperty("a", 42);
entity.setProperty("avec", [1,2,3,4]);
entity.setProperty("mtx", [[[1,2,3],[4,5,6],[7,8,9]],
                           [[1,2,3],[4,5,6],[7,8,9]],
                           [[1,2,3],[4,5,6],[7,8,9]]]);


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

print (entity2.property("mtx"));

function extend(base, sub) {
  // Avoid instantiating the base class just to setup inheritance
  // See https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create
  // for a polyfill
  // Also, do a recursive merge of two prototypes, so we don't overwrite 
  // the existing prototype, but still maintain the inheritance chain
  // Thanks to @ccnokes
  var origProto = sub.prototype;
  sub.prototype = Object.create(base.prototype);
  for (var key in origProto)  {
     sub.prototype[key] = origProto[key];
  }
  // Remember the constructor property was set wrong, let's fix it
  sub.prototype.constructor = sub;
  // In ECMAScript5+ (all modern browsers), you can make the constructor property
  // non-enumerable if you define it like this instead
  Object.defineProperty(sub.prototype, 'constructor', { 
    enumerable: false, 
    value: sub 
  });
}


M

function MyEntity(id) {
  // Call the parent's constructor
    soft.IEntity(id);
}

MyEntity.prototype = {
    sayHello: function(){
      return "Hello";
    }
}  

extend(soft.IEntity, MyEntity);

var e = new MyEntity(id);
e.sayHello();
e.debug();


/*
// Let's try this
function Animal(name) {
  this.name = name;
}

Animal.prototype = {
  sayMyName: function() {
    console.log(this.getWordsToSay() + " " + this.name);
  },
  getWordsToSay: function() {
    // Abstract
  }
}

function Dog(name) {
  // Call the parent's constructor
  Animal.call(this, name);
}

Dog.prototype = {
    getWordsToSay: function(){
      return "Ruff Ruff";
    }
}    

// Setup the prototype chain the right way
extend(Animal, Dog);

// Here is where the Dog (and Animal) constructors are called
var dog = new Dog("Lassie");
dog.sayMyName(); // Outputs Ruff Ruff Lassie
console.log(dog instanceof Animal); // true
console.log(dog.constructor); // Dog




*/
