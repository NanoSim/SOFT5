#!/usr/bin/env softshell

// This array comparison code has been borrows from stackoverflow.com
// https://stackoverflow.com/questions/7837456/how-to-compare-arrays-in-javascript
if(!Array.prototype.equals) {
    // attach the .equals method to Array's prototype to call it on any array
    Array.prototype.equals = function (array) {
	// if the other array is a falsy value, return
	if (!array)
            return false;
	
	// compare lengths - can save a lot of time 
	if (this.length != array.length)
            return false;
	
	for (var i = 0, l=this.length; i < l; i++) {
            // Check if we have nested arrays
            if (this[i] instanceof Array && array[i] instanceof Array) {
		// recurse into the nested arrays
		if (!this[i].equals(array[i]))
                    return false;       
            }           
            else if (this[i] != array[i]) { 
		// Warning - two different object instances will never be equal: {x:20} != {x:20}
		return false;   
            }           
	}       
	return true;
    };
    // Hide method from for-in loops
    Object.defineProperty(Array.prototype, "equals", {enumerable: false});
}


readEntities = {
    name: "soft-kernel-test",    
    create_reaction: function() {
	// Write a set of reactants to the output file
	var reactants = ["C", "O"];
	try {
	    SimpleReaction = require('porto.entity').using('simpleReaction', 'http://www.sintef.no/soft5/schema', '1');
	} catch (err) {
	    console.error (err.name + ":" + err.message);
	    console.error ("Please ensure that MongoDB is currenly running and that the entity is registered with the 'soft-register-entity' tool");
	}
	var uuid = "";
	try {	    
	    var storage = require('porto.storage')
		    .createStorage('json', 'output-test.json');

	    var reaction = new SimpleReaction();
	    reaction.setReactants(reactants);
	    uuid = reaction.id();
	    storage.save(reaction);	    
	    
	} catch (err) {
	    qFail(err);
	}

	// Read back the set of reactants from the output file and compare with
	// the original.
	try {
	    var readStorage = require('porto.storage')
		    .createStorage('json', 'output-test.json');
	    var readReaction = new SimpleReaction(uuid);
	    qCompare(uuid, readReaction.id());
	    storage.load(readReaction);
	    var reacts = readReaction.getReactants();
	    qCompare(readReaction.getReactants(), reactants, function(xs, ys){
		return xs.equals(ys);
	    });
	} catch (err) {
	    qFail(err);
	}
    }
};

__main__ = function (args)
{
    try {
	QTest = require('utils.qstestlib');
	if (args.length > 1 && args[1] == "--xml")
	    QTest.logger = QTest.xmlLogger;
	return (QTest.exec(readEntities) == true ? 0 : 1);
    } catch (err) {
	console.error(err);
	return 1;
    } 
};
