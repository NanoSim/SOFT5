#!/usr/bin/env softshell

__main__ = function (args)
{
    try {
	var controller = require('soft.mvc').create({
	    model: {
		min_cutoff: 1e-6,
		dp : 0.00001,
		k0 : 2.5,
		Temp: 1173.,
		EA: 70000.,
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
