#!/usr/bin/env softshell

var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

var print = function(x) {
    console.raw(x + "\n");
}

__main__ = function (args) {
    try {
        // Check that the user provides sufficient arguments to the program
        if (args.length < 2) {
            print("Usage " + args[0] + " <uuid>");
            return;
        }

        // Attempt to talk to the local mongodb
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        // Load the collection given by the uuid on the command line, then retrieve all
        // reaction entities in this collection
        var uuid = args[1];
        collection = new porto.Collection(uuid);
        storage.load(collection);

        print("Collection (uuid = " + uuid + ")");

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");
        print("|    Reactants => Products, A, b, Ea");
        
        // Print each reaction
        reactionDataIds.forEach(function (reactionId) {
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);
            print("+--- " + reaction["reactants"] + " => " + reaction["products"] + ", " + reaction["A"] + ", " + reaction["b"] + ", " + reaction["Ea"]);
        });
    } catch (err) {
        // Any error caught during execution is logged to the console.
        console.raw("ERROR: Failed retrieving entity.\nReason: " + err);
    }

};
