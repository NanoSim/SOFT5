qTestcase = {
    name: "soft-kernel-test",

    create_reaction: function() {
	try {
	    ChemkinReaction = require('porto.entity').using('simpleReaction', 'eu.nanosim.vasp', '0.1');
	    var storage = require('porto.storage')
		    .createStorage('json', 'output-test.json');

	    var reaction = new ChemkinReaction();
	    reaction.setReactants(["C", "O"]);
	    storage.save(reaction);
	    
	} catch (err) {
	    qFail(err);
	}
    }
};

QTest = require('utils.qstestlib');
//QTest.logger = QTest.xmlLogger;
QTest.exec(qTestcase);
