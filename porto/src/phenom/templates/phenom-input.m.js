function par = ReactionParams
    par.Nads = @{soft.model.Nads};
    par.Ncomp = @{soft.model.Ncomp};
    par.Ncomp_T = @{soft.model.Ncomp_T};
    par.Nrx_g = @{soft.model.Nrx_g};
    par.Nrx_s = @{soft.model.Nrx_s};

    par.Ac = @{soft.model.Ac}
    par.ADENT = @{soft.model.ADENT};
    par.B = @{soft.model.B};
    par.BET_a0 = @{soft.model.BET_a0};
    par.CP = @{soft.model.CP};
    par.CP_s = @{soft.model.CP_s};
    par.deltaHf = @{soft.model.deltaHf};
    par.deltaHf_s = @{soft.model.deltaHf_s};
    par.Dt = @{soft.model.Dt};
    par.E = @{soft.model.E};
    par.Ea_s = @{soft.model.Ea_s};
    par.ENT298 = @{soft.model.ENT298};
    par.F_sol_w = @{soft.model.F_sol_w};
    par.Flowin_w = @{soft.model.Flowin_w};
    par.Fluxin_w = @{soft.model.Fluxin_w};
    par.FRACin = @{soft.model.FRACin};
    par.FRACin_s = @{soft.model.FRACin_s};
    par.Fw = @{soft.model.Fw};
    par.Gs0 = @{soft.model.Gs0};
    par.Heq = @{soft.model.Heq};
    par.K_0i = @{soft.model.K_0i};
    par.k_0j = @{soft.model.k_0j};
    par.k0_s = @{soft.model.k0_s};
    par.Keq_j = @{soft.model.Keq_j};
    par.MW = @{soft.model.MW};
    par.MW_O2 = @{soft.model.MW_O2};
    par.MW_s = @{soft.model.MW_s};
    par.n_rx = @{soft.model.n_rx};

    par.Pin = @{soft.model.Pin};
    par.R0_NiAl2O4 = @{soft.model.R0_NiAl2O4};
    par.R0_NiO = @{soft.model.R0_NiO};
    par.rhogm = @{soft.model.rhogm};
    par.rhogmWin = @{soft.model.rhogmWin};
    par.RHOin = @{soft.model.RHOin};
    par.rhos0 = @{soft.model.rhos0};
    par.Smu = @{soft.model.Smu};
    par.Stoich = @{soft.model.Stoich};
    par.Stoich_s = @{soft.model.Stoich_s};
    par.Tin = @{soft.model.Tin};
    par.Uin = @{soft.model.Uin};
    par.Yin = @{soft.model.Yin};

    par.Param.g = @{soft.model.g};
    par.Param.R = @{soft.model.R};
    par.Param.C1 = @{soft.model.C1};
    par.Param.C2 = @{soft.model.C2};
    par.Param.A0 = @{soft.model.A0};
    par.Param.dg = @{soft.model.dg};
    par.Param.dp = @{soft.model.dp};
    par.Param.mug = @{soft.model.mug};
    par.Param.rhog = @{soft.model.rhog};

    par.membrane.P0 = @{soft.model.P0};
    par.membrane.Ea = @{soft.model.Ea};
    par.membrane.tm = @{soft.model.tm};
    par.membrane.dm = @{soft.model.dm};
    par.membrane.Nm = @{soft.model.Nm};

    par.Param.memb.P0 = @{soft.model.P0};
    par.Param.memb.Ea = @{soft.model.Ea};
    par.Param.memb.tm = @{soft.model.tm};
    par.Param.memb.dm = @{soft.model.dm};
    par.Param.memb.Nm = @{soft.model.Nm};

end
