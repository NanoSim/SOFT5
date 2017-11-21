#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

__main__ = function (args)
{
    try {
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

        if (args.length < 2) {
            print ("Usage " + args[0] + " <uuid>");
            return;
        }
        
        var uuid = args[1];
        collection = new porto.Collection(uuid);
        storage.load(collection);

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");
        reactionDataIds.forEach(function(reactionId){
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);

            console.raw("/* Fluent UDF using id" + reactionId + "*/ \n\n");
	    var controller = require('soft.mvc').create({
	        model: {
		    min_cutoff: 1e-6,
		    dp : 0.00001,
		    k0 : 2.5,
		    Temp: 1173.,
		    EA: reaction.Ea, /* Read value from coming for the Porto database */
		    Gas_Const: 8.314,
		    S0Eq: "6.0 * 0.2066 * vol_frac_solid / dp", 
		    kEq: "k0 * exp(-EA/(Gas_Const*Temp))",
		    RrateEq: "k * S0 * pow((C_R(c,gas_thread) * X_CH4 / MW_CH4 * 1000.0 ),0.6)"
	        },
	        view: "./templates/phenom-input.m.js"
	    });
            console.raw(controller());            
        }); 
	
    } catch (err) {
	console.raw("failed with" + err);
    }
};
