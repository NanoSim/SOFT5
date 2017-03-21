#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

// Filters out duplicates of a list
var unique = function(xs) {
    return xs.filter(function(item, i, ar){ return ar.indexOf(item) === i; });
};

// Returns true if a substring is inside the string
var find = function(x, xs) {
    if (xs.search(x) > -1) {
        return true;
    }
    else {
        return false;
    }
}

__main__ = function (args) {
    try {
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        if (args.length < 2) {
            print("Usage " + args[0] + " <uuid>");
            return;
        }

        var uuid = args[1];
        collection = new porto.Collection(uuid);
        storage.load(collection);

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");

        // Collects all species, for example H2O, FeO3, CH4, etc.
        var species = [];
        // Collects all reaction lines, on the form <reactants> => <product> <pre.exp.factor> <b> <activation energy>
        var all_reactions = []

        reactionDataIds.forEach(function (reactionId) {
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);

            var reactants = reaction.reactants;
            var products = reaction.products;

            for (var r in reactants) {
                species.push(reactants[r]);
            }

            for (var p in products) {
                species.push(products[p]);
            }

            if (products.length > 0 && reactants.length > 0) {
                all_reactions.push(reactants.join(" + ") + " => " + products.join(" + ") + " " + reaction.A + " " + reaction.b + " " + reaction.Ea);
            }

        });

        var all_elements = ["Fe", "H", "C", "O"];
        var elements = [];
        unique(species).forEach(function(s) {
            all_elements.forEach(function(e) {
                // For each unique species, loop over each element, then ...
                var ss = s;
                // ... check if the element is in the species.
                if (find(e, ss)) {
                    // We found an element, push it to the total list of elements and
                    // strip it away from the string we are searching in.
                    elements.push(e);
                    ss = ss.replace(e, "");
                }
            });
        });

        // @TODO Identify stochimetry

        var controller = require('soft.mvc').create({
            model: {
                elements: unique(elements).join(" "),
                species: unique(species).join(" "),
                reactions: all_reactions.join("\n")
            },
            view: "./template/chemkin.cjs"
        });
        console.raw(controller());

    } catch (err) {
        console.raw("failed with" + err);
    }

    console.raw("--------------------\n");

};
