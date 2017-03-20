#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

__main__ = function (args)
{
    console.raw("--------------------\n");

    try {
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        if (args.length < 2) {
            print ("Usage " + args[0] + " <uuid>");
            return;
        }
        
        var uuid = args[1];
        collection = new porto.Collection(uuid);
        storage.load(collection);
        console.raw("Loaded collection: " + uuid + "\n");

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");

        reactionDataIds.forEach(function(reactionId) {
            console.raw("+- reactiondata has-id " + reactionId + "\n");
        });

        reactionDataIds.forEach(function(reactionId) {
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);

            reaction.setReactants(["A", "B", "C"]);

            var reactants = reaction.getReactants();
            

            // reaction[reactants] = ["H2O", "Fe", "Cake2"];

            //console.raw("Reactants: " + reaction[reactants]);

            console.raw("Reactants aggregated: " + reactants + "\n");

            for (var r in reactants) {
                console.raw("Reactant: " + reactants[r] + "\n");
            }
        }); 
	
    } catch (err) {
    	console.raw("failed with" + err);
    }

    console.raw("--------------------\n");

    for (var key in porto.ChemkinReaction.prototype) {
        console.raw("+- " + key + ": " + porto.ChemkinReaction.prototype[key] + "\n");
    }

    console.raw("--------------------\n");
};
