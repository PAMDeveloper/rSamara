#pragma once

#include "parameters.h"

class Samara_Attributes {

public: 
    double ASScstr;
    double Altitude;
    double AttenMitch;
    double BundHeight;
    double CO2Cp;
    double CO2Exp;
    double CO2Slopetr;
    double Ca;
    double CoeffAssimSla;
    double CoeffFixedTillerDeath;
    double CoeffInternodeMass;
    double CoeffLeafDeath;
    double CoeffLeafWLRatio;
    double CoeffLodging;
    double CoeffPanSinkPop;
    double CoeffPanicleMass;
    double CoeffResCapacityInternode;
    double CoeffReserveSink;
    double CoeffRootMassPerVolMax;
    double CoeffTerminalLeafDeath;
    double CoeffTillerDeath;
    double CoeffTransplantingShock;
    double CoefficientQ10;
    double DEVcstr;
    double DensityField;
    double DensityNursery;
    double DurationNursery;
    double EpaisseurProf;
    double EpaisseurSurf;
    double ExcessAssimToRoot;
    double FTSWIrrig;
    double HaunCritTillering;
    double HumFC;
    double HumPF;
    double HumSat;
    double InternodeLengthMax;
    double IrrigAuto;
    double IrrigAutoResume;
    double IrrigAutoStop;
    double IrrigAutoTarget;
    double KCritSterCold1;
    double KCritSterCold2;
    double KCritSterFtsw1;
    double KCritSterFtsw2;
    double KCritSterHeat1;
    double KCritSterHeat2;
    double KCritStressCold1;
    double KCritStressCold2;
    double KPar;
    double KRespInternode;
    double KRespMaintLeaf;
    double KRespMaintRoot;
    double KRespMaintSheath;
    double KRespPanicle;
    double KcMax;
    double Kdf;
    double Latitude;
    double LeafLengthMax;
    double LifeSavingDrainage;
    double Mulch;
    double PARCritSLA;
    double PEvap;
    double PFactor;
    double PPCrit;
    double PPExp;
    double PPSens;
    double PanStructMassMax;
    double PercolationMax;
    double Phyllo;
    double PlantsPerHill;
    double PlotDrainageDAF;
    double PoidsSecGrain;
    double PourcRuiss;
    double PriorityPan;
    double ProfRacIni;
    double RankLongestLeaf;
    double RelMobiliInternodeMax;
    double RelPhylloPhaseStemElong;
    double RollingBase;
    double RollingSens;
    double RootCstr;
    double RootFrontMax;
    double RootLignin;
    double RootPartitMax;
    double SDJBVP;
    double SDJLevee;
    double SDJMatu1;
    double SDJMatu2;
    double SDJRPR;
    double SLASwitch;
    double SeuilPP;
    double SeuilRuiss;
    double SlaMax;
    double SlaMin;
    double StemPorosity;
    double StockIniProf;
    double StockIniSurf;
    double TBase;
    double TLim;
    double TOpt1;
    double TOpt2;
    double TempSLA;
    double TilAbility;
    double Transplanting;
    double TransplantingDepth;
    double TxAssimBVP;
    double TxAssimMatu1;
    double TxAssimMatu2;
    double TxConversion;
    double TxResGrain;
    double TxRuSurfGermi;
    double VRacBVP;
    double VRacLevee;
    double VRacMatu1;
    double VRacMatu2;
    double VRacPSP;
    double VRacRPR;
    double WaterLoggingSens;
    double WtRatioLeafSheath;
	//

    //@removed variables 24/04
    //CONSTANT
    double CoeffInternodeNum;
    double IcTillering;
    double KTempMaint;
    double DrySeedingSwitch;

    void init_parameters_2_1_subset(SamaraParameters * params) {
        SamaraParameters parameters = *params;

        ASScstr = parameters.getDouble("asscstr");
        Altitude = parameters.getDouble("wsalt");
        AttenMitch = parameters.getDouble("attenmitch");
        BundHeight = parameters.getDouble("bundheight");
        CO2Cp = parameters.getDouble("co2cp");
        CO2Exp = parameters.getDouble("co2exp");
        CO2Slopetr = parameters.getDouble("co2slopetr");
        Ca = parameters.getDouble("ca");
        CoeffAssimSla = parameters.getDouble("coeffassimsla");
        CoeffInternodeMass = parameters.getDouble("coeffinternodemass");
        CoeffLeafDeath = parameters.getDouble("coeffleafdeath");
        CoeffLeafWLRatio = parameters.getDouble("coeffleafwlratio");
        CoeffLodging  = parameters.getDouble("coefflodging");
        CoeffPanSinkPop = parameters.getDouble("coeffpansinkpop");
        CoeffPanicleMass = parameters.getDouble("coeffpaniclemass");
        CoeffResCapacityInternode = parameters.getDouble("coeffrescapacityinternode");
        CoeffReserveSink = parameters.getDouble("coeffreservesink");
        CoeffRootMassPerVolMax = parameters.getDouble("coeffrootmasspervolmax");
        CoeffTransplantingShock = parameters.getDouble("coefftransplantingshock");
        CoefficientQ10 = parameters.getDouble("coefficientq10");
        DEVcstr = parameters.getDouble("devcstr");
        DensityField = parameters.getDouble("densityfield");
        DensityNursery = parameters.getDouble("densitynursery");
        DurationNursery = parameters.getDouble("durationnursery");
        EpaisseurProf = parameters.getDouble("epaisseurprof");
        EpaisseurSurf = parameters.getDouble("epaisseursurf");
        ExcessAssimToRoot = parameters.getDouble("excessassimtoroot");
        FTSWIrrig = parameters.getDouble("ftswirrig");
        HaunCritTillering = parameters.getDouble("hauncrittillering");
        HumFC = parameters.getDouble("humfc");
        HumPF = parameters.getDouble("humpf");
        HumSat = parameters.getDouble("humsat");
        InternodeLengthMax = parameters.getDouble("internodelengthmax");
        IrrigAuto = parameters.getDouble("irrigauto");
        IrrigAutoResume = parameters.getDouble("irrigautoresume");
        IrrigAutoStop = parameters.getDouble("irrigautostop");
        IrrigAutoTarget = parameters.getDouble("irrigautotarget");
        KCritSterCold1 = parameters.getDouble("kcritstercold1");
        KCritSterCold2 = parameters.getDouble("kcritstercold2");
        KCritSterFtsw1 = parameters.getDouble("kcritsterftsw1");
        KCritSterFtsw2 = parameters.getDouble("kcritsterftsw2");
        KCritSterHeat1 = parameters.getDouble("kcritsterheat1");
        KCritSterHeat2 = parameters.getDouble("kcritsterheat2");
        KCritStressCold1 = parameters.getDouble("kcritstresscold1");
        KCritStressCold2 = parameters.getDouble("kcritstresscold2");
        KPar = parameters.getDouble("kpar");
        KRespInternode = parameters.getDouble("krespinternode");
        KRespMaintLeaf = parameters.getDouble("krespmaintleaf");
        KRespMaintRoot = parameters.getDouble("krespmaintroot");
        KRespMaintSheath = parameters.getDouble("krespmaintsheath");
        KRespPanicle = parameters.getDouble("kresppanicle");
        KcMax = parameters.getDouble("kcmax");
        Kdf = parameters.getDouble("kdf");
        Latitude = parameters.getDouble("wslat");
        LeafLengthMax = parameters.getDouble("leaflengthmax");
        LifeSavingDrainage = parameters.getDouble("lifesavingdrainage");
        Mulch = parameters.getDouble("mulch");
        PARCritSLA = parameters.getDouble("parcritsla");
        PEvap = parameters.getDouble("pevap");
        PFactor = parameters.getDouble("pfactor");
        PPCrit = parameters.getDouble("ppcrit");
        PPExp = parameters.getDouble("ppexp");
        PPSens = parameters.getDouble("ppsens");
        PanStructMassMax = parameters.getDouble("panstructmassmax");
        PercolationMax = parameters.getDouble("percolationmax");
        Phyllo = parameters.getDouble("phyllo");
        PlantsPerHill = parameters.getDouble("plantsperhill");
        PlotDrainageDAF = parameters.getDouble("plotdrainagedaf");
        PoidsSecGrain = parameters.getDouble("poidssecgrain");
        PourcRuiss = parameters.getDouble("pourcruiss");
        PriorityPan = parameters.getDouble("prioritypan");
        ProfRacIni = parameters.getDouble("profracini");
        RankLongestLeaf = parameters.getDouble("ranklongestleaf");
        RelMobiliInternodeMax = parameters.getDouble("relmobiliinternodemax");
        RelPhylloPhaseStemElong = parameters.getDouble("relphyllophasestemelong");
        RollingBase = parameters.getDouble("rollingbase");
        RollingSens = parameters.getDouble("rollingsens");
        RootCstr = parameters.getDouble("rootcstr");
        RootFrontMax = parameters.getDouble("rootfrontmax");

        RootPartitMax = parameters.getDouble("rootpartitmax");
        SDJBVP = parameters.getDouble("sdjbvp");
        SDJLevee = parameters.getDouble("sdjlevee");
        SDJMatu1 = parameters.getDouble("sdjmatu1");
        SDJMatu2 = parameters.getDouble("sdjmatu2");
        SDJRPR = parameters.getDouble("sdjrpr");
        SeuilPP = parameters.getDouble("seuilpp");
        SeuilRuiss = parameters.getDouble("seuilruiss");
        SlaMax = parameters.getDouble("slamax");
        SlaMin = parameters.getDouble("slamin");
        StemPorosity = parameters.getDouble("stemporosity");
        StockIniProf = parameters.getDouble("stockiniprof");
        StockIniSurf = parameters.getDouble("stockinisurf");
        TBase = parameters.getDouble("tbase");
        TLim = parameters.getDouble("tlim");
        TOpt1 = parameters.getDouble("topt1");
        TOpt2 = parameters.getDouble("topt2");
        TempSLA = parameters.getDouble("tempsla");
        TilAbility = parameters.getDouble("tilability");
        Transplanting = parameters.getDouble("transplanting");
        TransplantingDepth = parameters.getDouble("transplantingdepth");
        TxAssimBVP = parameters.getDouble("txassimbvp");
        TxAssimMatu1 = parameters.getDouble("txassimmatu1");
        TxAssimMatu2 = parameters.getDouble("txassimmatu2");
        TxConversion = parameters.getDouble("txconversion");
        TxResGrain = parameters.getDouble("txresgrain");
        TxRuSurfGermi = parameters.getDouble("txrusurfgermi");
        VRacBVP = parameters.getDouble("vracbvp");
        VRacLevee = parameters.getDouble("vraclevee");
        VRacMatu1 = parameters.getDouble("vracmatu1");
        VRacMatu2 = parameters.getDouble("vracmatu2");
        VRacPSP = parameters.getDouble("vracpsp");
        VRacRPR = parameters.getDouble("vracrpr");
        WaterLoggingSens = parameters.getDouble("waterloggingsens");
        WtRatioLeafSheath = parameters.getDouble("wtratioleafsheath");
        DrySeedingSwitch = parameters.getDouble("dryseedingswitch");

        CoeffInternodeNum = 2;
        IcTillering = 0.5;
        KTempMaint = 25;
    }

	void init_parameters(SamaraParameters * params) {
        init_parameters_2_1_subset(params);
		SamaraParameters parameters = *params;
        SLASwitch =  parameters.getDouble("slaswitch");
        RootLignin = parameters.getDouble("rootlignin");
        CoeffTerminalLeafDeath = parameters.getDouble("coeffterminalleafdeath");
        CoeffTillerDeath = parameters.getDouble("coefftillerdeath");
        CoeffFixedTillerDeath = parameters.getDouble("coefffixedtillerdeath");
	}

};
