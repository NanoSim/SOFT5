#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.EffectivenessFactor = entity.using('effectivenessFactor', 'eu.nanosim.parscale', '0.1');
porto.EffectiveReactionparameters = entity.using('effectiveReactionparameters', 'eu.nanosim.parscale', '0.1');

// Simple assertion function used to check consistency in JSON files
var check = function(exp, msg) {
    if (!exp) {
        console.raw("ERROR: " + msg + "\n");
    }
}

__main__ = function (args) {
    try {
        // Check that the user provides sufficient arguments to the program
        if (args.length < 4) {
            print("Usage " + args[0] + " <effectiveness_factor.json> <effective_reaction_parameters.json> <uuid>");
            return;
        }

        var effectivenessFactorFileName = args[1];
        var effectiveReactionParametersFileName = args[2];
        var uuid = args[3];

        // Attempt to read the effectiveness factor .json file
        var effectivenessfactor = JSON.parse(readFile(effectivenessFactorFileName));

        check(effectivenessfactor.name == "effectivenessFactor", 
            "In " + effectivenessFactorFileName + ": Expected 'name' == 'effectivenessFactor' but encountered '" + effectivenessfactor.name + "'\n");

        check(effectivenessfactor.parameters.length == effectivenessfactor.nparameters,
            "In " + effectivenessFactorFileName + ": Expected 'nparameters' == '" + effectivenessfactor.parameters.length + "'");

        check(effectivenessfactor.argumentCount > -1,
            "In " + effectivenessFactorFileName + ": Expected 'argumentCount' > '-1'");

        // Debug output
        for (var k in effectivenessfactor) {
            console.raw(k + ": " + effectivenessfactor[k] + "\n");
        }

        // Attempt to talk to the local mongodb
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        // Load the collection given by the uuid on the command line, then retrieve all
        // reaction entities in this collection
        collection = new porto.Collection(uuid);
        storage.load(collection);

        console.raw("Collection uuid: " + collection.id() + "\n");

        eEF = new porto.EffectivenessFactor();
        eEF.nparameters = effectivenessfactor.nparameters;
        eEF.functionalForm = effectivenessfactor.functionalForm;
        eEF.parameters = effectivenessfactor.parameters;
        collection.registerEntity(eEF.name, eEF);

        console.raw("Created entity effectivenessFactor with uuid = " + eEF.id() + "\n");
        
        // collection.registerEntity(eEF);

        // storage.save(collection);

        for (var k in collection) {
            console.raw(k + " : " + collection[k] + "\n");
        }

        /*


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
            
            // Collect a list of all reactants and product species
            for (var r in reactants) {
                species.push(reactants[r]);
            }
            for (var p in products) {
                species.push(products[p]);
            }

            // Construct the reactions
            if (products.length > 0 && reactants.length > 0) {
                all_reactions.push(reactants.join(" + ") + " => " + products.join(" + ") + " " + reaction.A + " " + reaction.b + " " + reaction.Ea);
            }

        });

        // All elements we want to check for, ideally this could be the entire periodic table
        var all_elements = ["Fe", "H", "C", "O"];
        var elements = [];

        // TODO: Before this point we should capture the stochimetric factors before each species, to balance
        //       the reactions properly.
        
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

        // Pass the aggregated information from the entity to the code generator (soft.mvc). The
        // variables here (elements, species, reactions) can be found in the template specified
        // below. This will substitute the template entrires such as @{elements} with the contents
        // specified below.
        var controller = require('soft.mvc').create({
            model: {
                elements: unique(elements).join(" "),
                species: unique(species).join(" "),
                reactions: all_reactions.join("\n")
            },
            view: "./template/chemkin.cjs"
        });

        // Output the generated code directly to the console.
        // TODO: Add capability to write it to a named file instead.
        console.raw(controller());

        */

    } catch (err) {
        // Any error caught during execution is logged to the console.
        console.raw("ERROR: Failed reading reactions.\nReason: " + err + "\n");
    }

};
