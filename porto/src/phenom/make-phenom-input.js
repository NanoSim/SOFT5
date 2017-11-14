#!/usr/bin/env softshell
var entity = require('porto.entity');
porto.ChemkinReaction = entity.using('phenom-input', 'eu.nanosim.phenom', '1');

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




/*
          Nads: 4                                                                                %Number of adsorbing species                                                    int
         Ncomp: 6                                                                                %Number of gas species                                                          int                   
       Ncomp_T: 9                                                                                %Gas+solid species                                                              int               
         Nrx_g: 3                                                                                %Number of reforming reactions                                                  int                           
         Nrx_s: 3                                                                                %Number of het reactions                                                        int                		  
		  
		  
            Ac: 6.2812                                                                           %Cross sect bed area  (1)                                                       double                    
         ADENT: [-38280 -70650 -82900 88680]                                                     %Enthalpy of adsorption CH4 CO H2 H2O (Nads adsorption reactions)               double  		 
             B: [1.0000e-06 1.5000e-06 1.5000e-06 6.5000e-07 1.7400e-06 1.4000e-06]              %Viscosity coeffs CH4 CO CO2 H2 H2O N2 (Ncomp)                                  double                                       
        BET_a0: 23.3000                                                                          %Spec surface area (1)                                                          double                   
            CP: [4x6 double]                                                                     %Heat capacity coeffs 4coeff x Ncomp species                                    double                                    
          CP_s: 1200                                                                             %Solid heat transfer coeff                                                      double                       
       deltaHf: [6x1 double]                                                                     %Heat of formation, gas (Ncomp species)                                         double                                
     deltaHf_s: [3x1 double]                                                                     %Heat of form Ni NiO MgAl2O4 ((Ncomp_T-Ncomp) species)                          double                                     
            Dt: 2.9000                                                                           %ReactorE diameter (1)                                                          double                   
             E: [218550 73523 236850 5800]                                                       %Activation energies (Nads adsorption reactions)                                double                                                 
          Ea_s: [78000 26000 25000]                                                              %Act energy (Nrx_s solid rx)                                                    double                    
        ENT298: [206300 -41200 165100 0]                                                         %Reaction enthalpy 298 K ((Nrx_g+1) rx)                                         double                                   
       F_sol_w: 132.6600                                                                         %                                                                               double
      Flowin_w: 7.5200                                                                           %                                                                               double
      Fluxin_w: 1.1972                                                                           %                                                                               double
        FRACin: [0.3116 0 0.0691 0.0066 0.6079 0.0049]                                           %Mass fractions gas at inlet (Ncomp species)                                    double                                     
      FRACin_s: [0.0295 0.0852 0.8853]                                                           %Mass fract at inlet solid   ((Ncomp_T-Ncomp) species)                          double                                  
            Fw: [6x1 double]                                                                     %Kg/s of each species  (Ncomp)                                                  double                  
           Gs0: 120                                                                                                                                                              double
           Heq: [223080 -36580 186500 0]                                                         %Equilibrium enthalpies (Nads rx)                                               double                                  
          K_0i: [6.6600e-04 8.2500e-05 6.1500e-09 177000]                                        %Pre-exponential factors, Nads adsorption reactions                             double                                             
          k_0j: [5.8300e+11 25100 4.6700e+13 8.4800e-05]                                         %Preexp factors reforming (Nrx_g+1)                                             double                                    
          k0_s: [0.7100 0.0093 0.0052]                                                           %Preexp for heterogeneous kinetics (Nrx_s rx)                                   double                                      
         Keq_j: [1.2000e+13 0.0177 2.1240e+11 0]                                                 %Equilibr constants, (Nads rx)                                                  double                               
            MW: [6x1 double]                                                                     %Molar weights (Ncomp species)                                                  double                       
         MW_O2: 0.0320                                                                           %                                                                               double
          MW_s: [0.0587 0.0747 0.1423]                                                           %Molar weight, (Ncomp_T-Ncomp) solid species                                    double                           
          n_rx: [0.8000 0.5000 0.8000]                                                           %Reaction order heterogeneous rx  (Nrx_s)                                       double                             
                               
           Pin: 5050000                                                                                                                                                          double
    R0_NiAl2O4: 0.1125                                                                                                                                                           double
        R0_NiO: 0.2142                                                                                                                                                           double
         rhogm: 624.1327                                                                                                                                                         double
      rhogmWin: 10.7314                                                                                                                                                          double
         RHOin: 624.1327                                                                                                                                                         double
         rhos0: 3451                                                                                                                                                             double
           Smu: [165 220 220 67 626 108]                                                         %Viscosity parameter, (Ncomp) gases                                             double                          
        Stoich: [6x4 double]                                                                     %Stoichiometric coeffs (Ncomp x (Nrx_g+1) rx)                                   double                                                    
      Stoich_s: [9x4 double]                                                                     %Stoich for solids reactions (Ncomp_T x (Nrx_s+1) rx )                          double                                           
           Tin: 973.1500                                                                                                                                                         double
           Uin: 0.1116                                                                                                                                                           double
           Yin: [0.3340 0 0.0270 0.0560 0.5800 0.0030]                                           %Mol fractions, Ncomp species                                                   double                             
		                                                                                                                                                           
		   
Param = 

       g: 9.8100
       R: 8.3145
    memb: [1x1 struct]
      C1: 27.2000
      C2: 0.0408
      A0: 5.0000e-05
      dg: 1.0000e-06
      dp: 2.5000e-04
     mug: 3.1343e-05
    rhog: 10.7314

>> Param.memb

ans = 

    P0: 4.2400e-10
    Ea: 5800
    tm: 5.0000e-06
    dm: 0.0500
    Nm: 165

*/



/*

structdef_data.txt:

par
|    
|--- Param
|       |    
|       |--- memb
|       |       |    
|       |       |-- P0 : 4.24e-10
|       |       |-- Ea : 5800
|       |       |-- tm : 5e-06
|       |       |-- dm : 0.05
|       |       |-- Nm : 165
|       |    
|       |----- g : 9.81
|       |----- R : 8.31447
|       |---- C1 : 27.2
|       |---- C2 : 0.0408
|       |---- A0 : 5e-05
|       |---- dg : 1e-06
|       |---- dp : 0.00025
|       |--- mug : 3.13432e-05
|       |-- rhog : 10.7314
|    
|--- membrane
|       |    
|       |-- P0 : 4.24e-10
|       |-- Ea : 5800
|       |-- tm : 5e-06
|       |-- dm : 0.05
|       |-- Nm : 165
|    
|---------- Ac : 6.28122
|------ BET_a0 : 23.3
|-------- CP_s : 1200
|---------- Dt : 2.9
|----- F_sol_w : 132.66
|---- Flowin_w : 7.52
|---- Fluxin_w : 1.19722
|--------- Gs0 : 120
|------- MW_O2 : 0.032
|------- Ncomp : 6
|----- Ncomp_T : 9
|------- Nrx_g : 3
|------- Nrx_s : 3
|--------- Pin : 5.05e+06
|-- R0_NiAl2O4 : 0.112462
|------ R0_NiO : 0.214219
|------- rhogm : 624.133
|---- rhogmWin : 10.7314
|------- RHOin : 624.133
|------- rhos0 : 3451
|--------- Tin : 973.15
|--------- Uin : 0.111562
|------- ADENT : [-38280 -70650 -82900 88680]
|----------- B : [1e-06 1.5e-06 1.5e-06 6.5e-07 1.74e-06 1.4e-06]
|----- deltaHf : [-74900 -110600 -393800 0 -242000 0]
|--- deltaHf_s : [0 -244300 -2.33781e+06]
|----------- E : [218550 73523 236850 5800]
|-------- Ea_s : [78000 26000 25000]
|------ ENT298 : [206300 -41200 165100 0]
|------ FRACin : [0.311582 0 0.0690935 0.00657901 0.60786 0.0048854]
|---- FRACin_s : [0.0295477 0.0851943 0.885258]
|---------- Fw : [2.34309 0 0.519583 0.0494741 4.57111 0.0367382]
|--------- Heq : [223080 -36580 186500 0]
|-------- K_0i : [0.000666 8.25e-05 6.15e-09 177000]
|-------- k_0j : [5.83e+11 25100 4.67e+13 8.48e-05]
|-------- k0_s : [0.71 0.0093 0.0052]
|------- Keq_j : [1.2e+13 0.0177 2.124e+11 0]
|---------- MW : [0.01604 0.028 0.044 0.00202 0.01802 0.028]
|-------- MW_s : [0.05869 0.07469 0.14227]
|-------- n_rx : [0.8 0.5 0.8]
|--------- Smu : [165 220 220 67 626 108]
|--------- Yin : [0.334 0 0.027 0.056 0.58 0.003]
|                -------------------------------------------------------------------------
|---------- CP : |  1.93e+04 |  3.09e+04 |  1.98e+04 |  2.71e+04 |  3.22e+04 |  3.12e+04 |
|                |  5.21e+01 | -1.29e+01 |  7.34e+01 |  9.27e+00 |  1.92e+00 | -1.36e+01 |
|                |  1.20e-02 |  2.79e-02 | -5.60e-02 | -1.38e-02 |  1.06e-02 |  2.68e-02 |
|                | -1.13e-05 | -1.27e-05 |  1.72e-05 |  7.65e-06 |  3.60e-06 | -1.17e-05 |
|                -------------------------------------------------------------------------
|                -------------------------------------------------
|------ Stoich : | -1.00e+00 |  0.00e+00 | -1.00e+00 |  0.00e+00 |
|                |  1.00e+00 | -1.00e+00 |  0.00e+00 |  0.00e+00 |
|                |  0.00e+00 |  1.00e+00 |  1.00e+00 |  0.00e+00 |
|                |  3.00e+00 |  1.00e+00 |  4.00e+00 | -1.00e+00 |
|                | -1.00e+00 | -1.00e+00 | -2.00e+00 |  0.00e+00 |
|                |  0.00e+00 |  0.00e+00 |  0.00e+00 |  0.00e+00 |
|                -------------------------------------------------
|                -------------------------------------------------
|---- Stoich_s : | -1.00e+00 |  0.00e+00 |  0.00e+00 |  0.00e+00 |
|                |  0.00e+00 |  0.00e+00 | -1.00e+00 |  0.00e+00 |
|                |  1.00e+00 |  0.00e+00 |  1.00e+00 |  0.00e+00 |
|                |  0.00e+00 | -1.00e+00 |  0.00e+00 |  0.00e+00 |
|                |  2.00e+00 |  1.00e+00 |  0.00e+00 |  0.00e+00 |
|                |  0.00e+00 |  0.00e+00 |  0.00e+00 |  0.00e+00 |
|                |  4.00e+00 |  1.00e+00 |  1.00e+00 |  0.00e+00 |
|                | -4.00e+00 | -1.00e+00 | -1.00e+00 |  0.00e+00 |
|                |  0.00e+00 |  0.00e+00 |  0.00e+00 |  0.00e+00 |
|                -------------------------------------------------

*/