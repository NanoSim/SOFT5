#!/usr/bin/env softshell

var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

__main__ = function (args)
{
    try {
        var inputUUID = args[1];
        var collection  = new porto.Collection(inputUUID);
        var storage = new porto.Storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
        storage.load(collection);
        
        var chemkinCollectionUUID = collection.getUuid("chemKinData");
        var chemkinCollecton = new porto.Collection(chemkinCollectionUUID);
        storage.load(chemkinCollecton);
        
        var reactionUUID = chemkinCollecton.getUuid("reaction_1");
        var reaction = porto.ChemkinReaction(reactionUUID);
        storage.load(reaction);
        
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
