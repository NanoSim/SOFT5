#!/usr/bin/env softshell

var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

__main__ = function (args)
{
    try {
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
        
        var chemkinCollectionId = args[1]);
        var chemkinCollection = new porto.Collection(chemkinCollectionId);
	storage.load(chemkinCollection);
	print("num entities: " + chemkinCollection.numEntities());
        
        var reactionId = chemkinCollection.entityId("reaction_1");
	print("found reaction with uuid: " + reactionId);

        var reaction = new porto.ChemkinReaction(reactionId);
        reaction.read(storage);
        
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
	    view: "./template/udf.cjs"
	});

	console.raw(controller());
    } catch (err) {
	console.raw(err);
    }
};
