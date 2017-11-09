#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('chemkinReaction', 'eu.nanosim.vasp', '0.1');

println = function(args) {
    console.raw(JSON.stringify(args, null, 2) + "\n");
}

has = function(a, v) {
    for (i in a) {
        if (a[i] === v)
            return i;
    }
    return false;
}

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
        
        var reactants = ["CH4", "CO", "CO2", "H2", "H2O", "N2"];
        println("reactants: " + reactants);

        var Stoich = [];
        var A = [];
        var E = [];

        var reactionDataIds = collection.findRelations("reactiondata", "has-id");
        reactionDataIds.forEach(function(reactionId){
            var reaction = new porto.ChemkinReaction(reactionId);
            reaction.read(storage);

            var StoichLine =  [0, 0, 0, 0, 0, 0];

            reaction.reactants.map(function(r) {
                i = has(reactants, r);
                if (i !== false) { 
                    StoichLine[i] = 1;
                }
            });

            reaction.products.map(function(r) {
                i = has(reactants, r);
                if (i !== false) { 
                    StoichLine[i] = -1;
                }
            });
            
            Stoich = Stoich.concat([StoichLine]);

            A = A.concat([reaction.A]);
            E = E.concat([reaction.Ea]);

        }); 

        StoichString = "";
        for (var i = 0; i <  Stoich[0].length; ++i) {
            var line = [];
            for (var j = 0; j < Stoich.length; ++j) {
                line = line.concat([Stoich[j][i]]);
            }
            StoichString += line.join(" ") + "\n";
        }
        println(StoichString)

        println(Stoich);

        console.raw("/* Phenom input, using entity uuid " + uuid + " */ \n\n");
        var controller = require('soft.mvc').create({
            model: {
                par: {
                    eqType_g: 3,
                    eqType_s: "",
                    Stoich: StoichString,
                    Stoich_s: "",
                    Ireactants: "", // [3 4] = CO2 + H2
                    Iproducts: "", // [2 5] = CO + H20
                    nreactants: "", // [1 1]
                    nproducts: "", // [1 1]
                    A: "[" + A.join(" ") + "]",
                    E: "[" + E.join(" ") + "]"
                }
            },
            view: "./templates/phenom-input.m.js"
        });
        
        console.raw(controller());            
            
    } catch (err) {
	console.raw("failed with" + err);
    }
};
