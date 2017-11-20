#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.EffectivenessFactor = entity.using('effectivenessfactor', 'eu.nanosim.parscale', '0.2');
porto.EffectiveReactionparameters = entity.using('effectivereactionparameters', 'eu.nanosim.parscale', '0.2');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');


__main__ = function (args) {
    try {
        // Check that the user provides sufficient arguments to the program
        if (args.length < 2) {
            print("Usage " + args[0] + " <uuid>");
            return;
        }

        var uuid = args[1];

        // Attempt to talk to the local mongodb
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        // Load the collection given by the uuid on the command line, then retrieve all
        // reaction entities in this collection
        collection = new porto.Collection(uuid);
        storage.load(collection);

        var effectivenessfactorID = collection.findRelations("Effectivenessfactor", "id");
        var effectivereactionparametersID = collection.findRelations("Effectivereactionparameters", "id");

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");

        console.raw("Collection uuid: " + collection.id() + "\n");
        console.raw("effectivenessfactorID: " + effectivenessfactorID + "\n");
        console.raw("effectivereactionparametersID: " + effectivereactionparametersID + "\n");

        var ef = new porto.EffectivenessFactor(effectivenessfactorID);
        ef.read(storage);

        var erp = new porto.EffectiveReactionparameters(effectivereactionparametersID);
        erp.read(storage);
        
        reactionDataIds.forEach(function (reactionId) {
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);

            console.raw("ChemkinReactionID:" + reactionId + "\n");

            // var controller = require('soft.mvc').create({
            //     model: {
            //         min_cutoff: 1e-6,
            //         dp: 0.00001,
            //         k0: 2.5,
            //         Temp: 1173.,
            //         EA: reaction.Ea, /* Read value from coming for the Porto database */
            //         Gas_Const: 8.314,
            //         S0Eq: "6.0 * 0.2066 * vol_frac_solid / dp",
            //         kEq: "k0 * exp(-EA/(Gas_Const*Temp))",
            //         RrateEq: "k * S0 * pow((C_R(c,gas_thread) * X_CH4 / MW_CH4 * 1000.0 ),0.6)"
            //     },
            //     view: "./template/udf.cjs"
            // });
            // console.raw(controller());
        }); 


        // Pass the aggregated information from the entity to the code generator (soft.mvc). The
        // variables here (elements, species, reactions) can be found in the template specified
        // below. This will substitute the template entrires such as @{elements} with the contents
        // specified below.
        var controller = require('soft.mvc').create({
            model: {
                effectivenessFactor: {
                    functionalForm : ef.functionalForm,
                    parameters : ef.parameters.join(", ") // TODO: Also concat the arguments
                }, 
                effectiveReactionparameters: {
                    functionalForm : erp.functionalForm,
                    parameters : erp.parameters.join(", "), // TODO: Also concat the arguments
                    multiplyByParticleVolumeFraction : (erp.multiplyByParticleVolumeFraction == 1 ? true : false)
                }
            },
            view: "./template/udf.cjs"
        });

        // Output the generated code directly to the console.
        // TODO: Add capability to write it to a named file instead.
        console.raw(controller());

    } catch (err) {
        // Any error caught during execution is logged to the console.
        console.raw("ERROR: Failed generating code.\nReason: " + err + "\n");
    }

};
