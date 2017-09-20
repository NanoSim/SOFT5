Use Case 3. Coupling of parScale to Ansys FLUENT
================================================

Demonstrate that data from the particle scale model parScale can be used in the generation of an Ansys FLUENT UDF using Porto. 


Walkthrough
-----------

* Simulate reaction and record (i) average particle temperature, (ii) current total consumption rate of gas-phase species A, and (iii) conversion of the solid
* Postprocessing step 1: calculate and plot (i) consumption rate of A versus conversion X for different C_A. Repeat exercise for different temperature to determine k, m, and n.
* Calculate the fitting values for k, n and m in the rate expression
* The effectiveness factor (parameters k_fit and m, as well as the functional form) is written to a .json file from an Octave script that processes the ParScale output
* then read into Porto using an External Plugin

(Alternative approach, if feasible: Ef is written directly from ParScale using Porto. We will investigate whether this is a feasible approach within the time available.)

* Porto stores Ef in an entity
* An UDF for Fluent is generated using the UDF generation in Porto


Details
-------

### `generate-effectiveness-udf.js`

      #!/usr/bin/env softshell
      var entity = require('porto.entity');
      porto.EffectivenessFactor = entity.using('effectivenessfactor', 'eu.nanosim.parscale', '0.2');
      porto.EffectiveReactionparameters = entity.using('effectivereactionparameters', 'eu.nanosim.parscale', '0.2');

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

            console.raw("Collection uuid: " + collection.id() + "\n");
            console.raw("effectivenessfactorID: " + effectivenessfactorID + "\n");
            console.raw("effectivereactionparametersID: " + effectivereactionparametersID + "\n");

            var ef = new porto.EffectivenessFactor(effectivenessfactorID);
            ef.read(storage);

            var erp = new porto.EffectiveReactionparameters(effectivereactionparametersID);
            erp.read(storage);
            
            // TODO: Map the arguments to variable names here


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




### `effectivenessFactor.json`

      {
         "name": "effectivenessFactor",
         "nparameters": 1,
         "functionalForm": "constant",
         "parameters": [
            0.1234
         ],
         "argumentCount": 0
      }

### `effectiveReactionparameters.json`
      {
         "name": "effectiveReactionparameters",
         "nparameters": 2,
         "multiplyByParticleVolumeFraction": true,
         "functionalForm": "OneMinusConversionTimesConcentration",
         "parameters": [
            0.6667,
            1
         ],
         "argumentCount": 3
      }


### `udf.cjs`

      #include "udf.h"

      /* 
         TODO This is a preliminary template.

         The input provided to this are at the moment:

            @{soft.model.effectivenessFactor.functionalForm}
            @{soft.model.effectivenessFactor.parameters}

         Producing the code:

            real x = effectiveness_factor_functional_form_constant(0.1234);

         The same goes for:

            @{soft.model.effectiveReactionparameters.functionalForm}
            @{soft.model.effectiveReactionparameters.parameters}
            @{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction}

         so that we for example can generate the code:

         if (@{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction} == 1) { // There is a bug with storing bools across language barriers, so I use an int for now 
            ...
         }

         Assumption: We can drop argumentCount.

      */

      real effectiveness_factor_functional_form_constant(real p1) {
         return 0.0; // TODO Something here
      }

      real effectiveness_factor_functional_form_somethingelse(real p1, real p2, real p3) {
         return 0.0; // TODO Something here
      }

      real effective_reaction_parameters_functional_form_constant(real p1) {
         return 0.0; // TODO Something here
      }

      real effective_reaction_parameters_functional_form_somethingelse(real p1, real p2, real p3) {
         return 0.0; // TODO Something here
      }


      // TODO The name defined here should probably be something else
      DEFINE_HET_RXN_RATE(NiO_CH4,c,t,r,mw,yi,rr,rr_t)
      {

         // NOTE: There are some issue with storing and communicating bools across language barriers, so we use an integer for now: 
         if (@{soft.model.effectiveReactionparameters.multiplyByParticleVolumeFraction} == 1) {
            // TODO Something here
         }

         real x = effectiveness_factor_functional_form_@{soft.model.effectivenessFactor.functionalForm}(@{soft.model.effectivenessFactor.parameters})



         // ------------------------------------------------------
         // TODO The below code is from the previous generator. I've kept this in place just to give an example.

         real Rrate, k, S0, X_CH4, MW_CH4, vol_frac_solid;
         Thread *gas_thread = THREAD_SUB_THREAD(t,0);
         Thread *solid_thread = THREAD_SUB_THREAD(t,1);

         vol_frac_solid = C_VOF(c,solid_thread);
         X_CH4 = yi[0][2];    /*   Check second index if order of species in gas is altered */
         MW_CH4 = mw[0][2];	 /*   Check second index if order of species in gas is altered */

         S0 = @{soft.model.S0Eq}; 
         k = @{soft.model.kEq};
         Rrate = @{soft.model.RrateEq};  

         if (X_CH4 < min_cutoff) Rrate = 0;     

         if (vol_frac_solid < min_cutoff) Rrate = 0;		  

         *rr = Rrate / 1000.0;
      }






----------
NOTES: To be removed
====================


Connecting scales with Porto
============================

The connection from WP3 to WP4 will be the following:
* Retrieve the activation energy from Remarc, using Porto
* Write Chemkin file for use in ParScale
* The Chemkin file will be generated using Porto and can be fully generated from the reaction entity from Remarc. This is analogue to the WP3 -> WP5 connection demonstrated.
* The individual workflows for WP3 -> WP4 and WP4 -> WP5 can be combined to form a full workflow for WP3 -> WP4 -> WP5.



From Stefan Radl
-----


## 3 Effective Reaction Model
The reaction rate law is written such that the key gas phase reactant (denoted as reactant A) is consumed and has the stoichiometric factor of -1. All other reactants may have a different stoichiometric factor.
  
The reaction may be of order n with respect to species A, and of order m with respect to a solid-phase reactant that is consumed during the reaction. The concentration of the solid-phase reactant is assumed to be proportional to (1-X) where X is the conversion. Consequently, the rate law (which equals the rate of consumption of species A) is written as
 
where k is the reaction rate constant (currently this value is assumed to be constant and equal to "effectiveArrhenius").

In order to close the model, the local conversion X of the particles has to be tracked. Starting initially with 0, the time evolution of X follows directly from the consumption of the solid species (with the initial concentration Cs,0) and hence is
 
Here dt denotes the material derivative of X.

### 3.1 Future Extension
The reaction rate constant k may depend on the local temperature via Arrhenius law. This can be achieved by supplying more data to "effectiveArrhenius"

Based on the local reaction rate a Thiele modulus could be calculated:
 
Here Deff is the effective diffusivity of gas phase species A in the pores, which may depend on the local temperature.  is the particle diameter. This Thiele modulus can be used to close a more advanced effectiveness factor model.


## 4	Closing the Single Reaction Model
Note that k, m, and n can be specified by the user (if it is clear which reaction is dominating), or are the result of a fitting exercise using a simulation of a more complex reaction or reaction network (using, e.g., ParScale). For the latter the rate expression may be linearized following
 
Thus, multivariate linear regression (e.g., using Octave’s ‘ols’ function) can be used to determine k, m, and n. http://stats.stackexchange.com/questions/32504/simple-multivariate-regression-with-octave. In the present work, m is set to zero to avoid the additional complication of running multiple ParScale simulations. However, the parameter n will be fitted by ParScale’s postprocessing tool.

## 5	Overall Model for Reaction in Control Volume (for Application in WP5)

The overall volumetric consumption rate of the gas phase species A (in [kmol/m³/s]) is 

_Equation_

where p is the particle volume fraction. This rate expression must be implemented in the Fluent UDF.
Thus, the user input to the UDF is:

* the parameters to model k as a function of temperature (e.g., (i) a constant value, or (ii) using the Arrhenius law that considers a pre-exponential factor and an activation energy which can be supplied by a Porto “reaction” entity)
* the parameters m and n to model the effective reaction rate, 
* the functional form of (can be a constant, or a more elaborate mathematical expression, e.g., that shown in the first chapter)
* the parameters to model the effective diffusivity (in the simplest case this is not needed; the effective diffusivity is only needed if a complex functional form for the effectiveness factor is chosen) 
* the initial concentration and stoichiometric factor of the solid to be able to calculate X (in the simplest case this is not needed; only needed if a complex functional form for the effectiveness factor is chosen)
