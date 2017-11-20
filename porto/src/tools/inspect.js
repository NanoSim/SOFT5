#!/usr/bin/env softshell

var entity = require('porto.entity');

var printOb = function(x) {
   console.raw(JSON.stringify(x, null, 2) + "\n");
}

var print = function(x) {
    console.raw(x + "\n");
}

var findPredicate = function(rs, pred) {
    return rs.filter(function(r) {
        if (r[1] == pred) return true;
        return false;
    });
}

__main__ = function (args) {
    try {
        // Check that the user provides sufficient arguments to the program
        if (args.length < 2) {
            print("Usage " + args[0] + " <uuid>");
            return;
        }

        // Access the mongoDB database
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        // Load the collection given by the uuid on the command line, then retrieve all
        // reaction entities in this collection
        var uuid = args[1];

        collection = new porto.Collection(uuid);
        storage.load(collection);

        var relations = collection.allRelations().split("\n").map(function(line) {return line.split(","); });
        relations.sort();
        
        var name = collection.name() + ":" + collection.version();
        print(name + " (uuid = " + collection.id() + ")");

        printOb(relations);

        var containedEntities = findPredicate(relations, "id");

        containedEntities.map(function(r) {
            print("+-- " + r[0] + " (uuid = " + r[2] + ")");
        });

    } catch (err) {
        // Any error caught during execution is logged to the console.
        console.raw("ERROR: Failed retrieving collection.\nReason: " + err);
    }

};
