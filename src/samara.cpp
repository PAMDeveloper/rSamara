#include "samara.h"
#include "variables.h"
#include <iostream>
#include <algorithm>
#include "processes/processes.h"
#include <string>

SamaraLogType LOG;
pair <vector <string>, vector < vector <double> > > run_samara_2_1(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_1();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    //    double DateDebutSimul = parameters->getDouble("datedebut");
    //    double DateFinSimul = parameters->getDouble("datefin");
    //    double DateSemis = parameters->getDouble("datesemis");
    double DateEnCours = DateDebutSimul;
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters(parameters);
    init_parcelle(VolMacropores, HumSat, HumFC, ResUtil
                  , ProfRu, EpaisseurSurf, EpaisseurProf
                  , RuSurf, CapaREvap, CapaRFE, CapaRDE
                  , StRuMax, StockIniProf, StockIniSurf
                  , ValRSurf, ValRDE, ValRFE
                  , StockSurface, StockTotal, Hum
                  , LTRkdfcl, LIRkdf, LTRkdf
                  , CounterNursery, VolRelMacropores);
    LatRad = Latitude * M_PI * 1.0 / 180;


    vector <  vector <double> > currentResults;

    bool crop = false;
    //Compute day before for TMoyPrec
    set_meteo_vars(parameters, -1,
                   TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                   Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
    EToFao(ETP, Altitude, RgMax, RgCalc,
           TMin, TMax,
           HMin, HMax, HMoyCalc,
           TMoyCalc, Vt, ETo,
           TMoyPrec, VPDCalc);
    //

    double cumRain = 0;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        set_meteo_vars(parameters, DateEnCours-DateDebutSimul,
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
        NbJAS = DateEnCours - DateSemis;
        if(NumPhase > 0)
            cumRain += Pluie;
        //crop entity
        if(NbJAS == 0) {
            init_culture();
            crop = true;
        }

        samara::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                       CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                       DryMatStructPaniclePop, DryMatResInternodePop);

        eval_Par(DateEnCours);
        //        if(start++ < 2) ETP = 0;
        EToFao(ETP, Altitude, RgMax, RgCalc,
               TMin, TMax,
               HMin, HMax, HMoyCalc,
               TMoyCalc, Vt, ETo,
               TMoyPrec, VPDCalc);

        if(crop) samara::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                                               DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);

        /** Phyllo **/
        samara::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
        samara::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
        samara::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
        samara::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
        samara::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
        samara::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
        samara::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
        samara::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);
        /*if (crop) */samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
        samara::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
        samara::RS_EvalDAF_V2(NumPhase, DAF);
        samara::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
        samara::RS_EvolHauteur_SDJ_cstr_V2_1(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
                                                CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold, CstrMean, ApexHeightGain, ApexHeight, PlantHeight, PlantWidth);
        samara::RS_EvolKcpKceBilhy(LTRkdfcl, KcMax, Mulch, Kcp, Kce, KcTot);
        if(DateEnCours != DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        samara::RS_EvolEvapSurfRFE_RDE_V2_1(NumPhase, Kce, EvapPot, CapaREvap, CapaRDE, CapaRFE,
                                               RuRac, RuSurf, BundHeight, EpaisseurSurf, EpaisseurProf,
                                               RootFront, ResUtil, Evap, ValRSurf, ValRFE, ValRDE, StockRac,
                                               StockTotal, StockSurface, Kr, KceReal, FloodwaterDepth, StockMacropores);
        if(DateEnCours == DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        /** CROP **/
        if (crop) samara::RS_EvalFTSW_V2(RuRac, StockTotal, StockMacropores, StRuMax, StockRac, FTSW);
        if (crop) samara::RS_EvalCstrPFactorFAO_V2(PFactor, FTSW, ETo, KcTot, StockMacropores, CoeffStressLogging, Cstr);
        if (crop) samara::DemandePlante_V2_1(Kcp, ETo, Ca, CO2Slopetr, TrPot, CoeffCO2Tr);
        if (crop) samara::EvalTranspi(TrPot, Cstr, Tr);
        samara::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
        if (crop) samara::RS_EvolConsRes_Flood_V2(NumPhase, RuRac, RuSurf, CapaREvap, Tr, Evap, CapaRDE, CapaRFE, EpaisseurSurf, EpaisseurProf, ResUtil, StockRac,
                                                     StockSurface, StockTotal, ValRFE, ValRDE, ValRSurf, FloodwaterDepth, StockMacropores);

        /*if (crop) */samara::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
        /*if (crop) */samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

        samara::RS_EvalFtswMoy(FTSW, NumPhase, NumSsPhase, FtswMoy);
        samara::RS_EvalSterility(NumPhase, ChangePhase, KCritSterCold1, KCritSterCold2, KCritSterHeat1, KCritSterHeat2, KCritSterFtsw1, KCritSterFtsw2, TminMoy,
                                    TmaxMoy, FtswMoy, SterilityCold, SterilityHeat, SterilityDrought, SterilityTot);
        samara::RS_EvalVitesseRacinaire(VRacLevee, VRacBVP, VRacRPR, VRacPSP, VRacMatu1, VRacMatu2, RootCstr, Cstr, NumPhase, DegresDuJourCor, VitesseRacinaire, VitesseRacinaireDay);

        if (crop) samara::EvalConversion(NumPhase, TxConversion, TxAssimBVP, SumDegresDay, SumDDPhasePrec, TxAssimMatu1, TxAssimMatu2, SeuilTemp, Conversion);

        samara::RS_EvalParIntercepte_V2_1(Par, Lai, Kdf, PARIntercepte, LIRkdfcl);
        if(crop)samara::RS_EvalAssimPot_V2_1(PARIntercepte, Par, Conversion, TMax, TMin, TBase, TOpt1, DayLength, StressCold, CO2Exp, Ca, CO2Cp, SlaMin, Sla, CoeffAssimSla,
                                                AssimPot, CoeffCO2Assim);

        if (crop) samara::RS_EvalCstrAssim(Cstr, ASScstr, CstrAssim);
        if (crop) samara::RS_EvalAssim(AssimPot, CstrAssim, Assim);

        samara::RS_TransplantingShock_V2(CounterNursery, CoeffTransplantingShock, Assim);
        samara::RS_EvalRespMaint(KRespMaintLeaf, KRespMaintSheath, KRespMaintRoot, KRespInternode, KRespPanicle, DryMatStructLeafPop, DryMatStructSheathPop,
                                    DryMatStructRootPop, DryMatStructInternodePop, DryMatStructPaniclePop, TMoyCalc, KTempMaint, CoefficientQ10, RespMaintTot);
        samara::RS_EvalRelPotLeafLength(NumPhase, HaunIndex, RankLongestLeaf, RelPotLeafLength);
        samara::RS_EvolPlantTilNumTot_V2(NumPhase, ChangePhase, PlantsPerHill, TilAbility, Density, Ic, IcTillering, Cstr, HaunIndex, HaunCritTillering, LTRkdfcl,
                                            CulmsPerHill, CulmsPerPlant, CulmsPop);
        samara::RS_EvolPlantLeafNumTot(NumPhase, CulmsPerHill, HaunGain, PlantLeafNumNew, PlantLeafNumTot);
        samara::RS_EvolMobiliTillerDeath_V2_1(NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic, PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                                 CulmsPerHill, DryMatStructPaniclePop);
        samara::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        samara::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                         SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                                DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
        samara::RS_EvalDemandStructSheath(NumPhase, DemStructLeafPop, WtRatioLeafSheath, SlaMin, SlaMax, Sla, StressCold, DemStructSheathPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                              DemStructSheathPop, DryMatStructRootPop, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
                                              DemStructRootPop, RootSystSoilSurfPopOld, RootFrontOld, RootSystVolPopOld, DemStructRootPlant);
        samara::RS_EvalDemandStructIN_V2_1(PhaseStemElongation, ApexHeightGain, CulmsPerHill, CoeffInternodeMass, Density, Ic, ResCapacityInternodePop, DryMatResInternodePop,
                                              CoeffReserveSink, NumPhase, DemStructInternodePlant, DemStructInternodePop, DemResInternodePop);
        samara::RS_EvalDemandStructPanicle_V2(NumPhase, CoeffPanicleMass, CulmsPerHill, Ic, DryMatStructPaniclePop, Density, PanStructMassMax, StressCold, DemStructPaniclePlant,
                                                 PanStructMass, DemStructPaniclePop);
        samara::RS_EvalDemandTotAndIcPreFlow_V2_1(NumPhase, RespMaintTot, DemStructLeafPop, DemStructSheathPop, DemStructRootPop, DemStructInternodePop, DemStructPaniclePop,
                                                     SupplyTot, NbDaysSinceGermination, PlantHeight, Cstr, DemResInternodePop, DemStructTotPop, Ic, IcCum, IcMean, CstrCum, CstrMean,
                                                     A_DemStructTot);
        samara::RS_EvolGrowthStructLeafPop_V2_1(NumPhase, Ic, SupplyTot, DemStructLeafPop, DemStructTotPop, GrowthStructLeafPop, A_GrowthStructLeaf);
        samara::RS_EvolGrowthStructSheathPop(NumPhase, Ic, SupplyTot, DemStructSheathPop, DemStructTotPop, GrowthStructSheathPop);
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, GrowthStructRootPop);
        samara::RS_EvolGrowthStructINPop_V2_1(NumPhase, Ic, SupplyTot, DemStructInternodePop, DemStructTotPop, DemResInternodePop, GrowthStructInternodePop, GrowthResInternodePop);
        samara::RS_EvolGrowthStructPanPop(NumPhase, Ic, SupplyTot, DemStructPaniclePop, DemStructTotPop, GrowthStructPaniclePop);
        samara::RS_Priority2GrowthPanStrctPop_V2_1(PriorityPan, DemStructPaniclePop, NumPhase, GrowthStructTotPop, DemStructInternodePop, DemStructTotPop, DemStructLeafPop,
                                                      DemStructSheathPop, DemStructRootPop, DemResInternodePop, GrowthStructPaniclePop, GrowthStructInternodePop,
                                                      GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthResInternodePop);
        if(crop) samara::RS_EvolGrowthStructTot_V2_1(NumPhase, SupplyTot, GrowthResInternodePop, GrowthStructTotPop, AssimSurplus, GrowthStructLeafPop, GrowthStructSheathPop,
                                                        GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus);
        if(crop) samara::RS_AddResToGrowthStructPop_V2_1(NumPhase, Ic, PhaseStemElongation, DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
                                                            DemStructRootPop, DemStructInternodePop, DemStructPaniclePop, RelMobiliInternodeMax, GrowthResInternodePop,
                                                            ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
                                                            GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay, A_GrowthStructLeaf,
                                                            A_GrowthStructTot, A_ResInternodeMobiliDay);
        if(crop) samara::RS_EvolDemPanFilPopAndIcPFlow_V2_1(NumPhase, DryMatStructPaniclePop, CoeffPanSinkPop, SterilityTot, DegresDuJourCor, SDJMatu1, SupplyTot, Assim,
                                                               RespMaintTot, StressCold, PanicleSinkPop, DemPanicleFillPop, AssimSurplus, Ic, A_AssimSurplus);
        if(crop) samara::RS_EvolPanicleFilPop_V2_1(NumPhase, Ic, DryMatResInternodePop, DemPanicleFillPop, SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim,
                                                      ResInternodeMobiliDayPot, AssimSurplus, PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop, GrainYieldPop,
                                                      A_AssimSurplus, A_ResInternodeMobiliDay);
        //        if(DryMatResInternodePop > 5000) {
        //            DryMatResInternodePop = DryMatResInternodePop;
        //        }

        if(crop) samara::RS_EvolGrowthReserveInternode_V2_1(NumPhase, PhaseStemElongation, DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
                                                               AssimSurplus, ResInternodeMobiliDay, ResCapacityInternodePop, IncreaseResInternodePop, DryMatResInternodePop,
                                                               AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, DryMatResInternodePopOld, A_IncreaseResInternodePop);

        if(crop) samara::RS_EvolGrowthTot_V2_1(NumPhase, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
                                                  GrowthResInternodePop, PanicleFilPop, DryMatResInternodePop, DryMatResInternodePopOld, GrowthStructTotPop, GrowthDryMatPop,
                                                  A_GrowthStructTot);
        if(crop)samara::RS_ExcessAssimilToRoot_V2(NumPhase, ExcessAssimToRoot, DryMatStructRootPop, RootSystVolPop, CoeffRootMassPerVolMax, RootMassPerVol, GrowthStructRootPop,
                                                     AssimNotUsed);
        if(crop) samara::RS_EvolDryMatTot_V2_1(NumPhase, ChangePhase, PlantsPerHill, TxResGrain, PoidsSecGrain, Density, GrowthStructLeafPop, GrowthStructSheathPop,
                                                  GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, GrowthResInternodePop, GrainYieldPop,
                                                  ResCapacityInternodePop, CulmsPerPlant, CoeffPanSinkPop, SterilityTot, DeadLeafdrywtPop, DryMatResInternodePopOld, PanicleFilPop,
                                                  AssimNotUsedCum, MobiliLeafDeath, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                                  DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                                  DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                                  SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                                  GrowthPop, CumCarbonUsedPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                   DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);
        if(crop)samara::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase, TempSLA, DryMatStructLeafPop,
                                           GrowthStructLeafPop, SlaMitch, SlaNew, Sla);

        samara::RS_EvalRuiss_FloodDyna_V2(NumPhase, Pluie, SeuilRuiss, PourcRuiss, BundHeight, Irrigation, PlantHeight, LifeSavingDrainage, PlotDrainageDAF,
                                             VolMacropores, SeuilRuiss, PercolationMax, DAF, StockMacropores, FloodwaterDepth, EauDispo, Lr);

        if(crop)samara::RS_AutomaticIrrigation_V2_1(NumPhase, IrrigAuto, IrrigAutoTarget, BundHeight, PlantHeight, Irrigation, PlotDrainageDAF, DAF, VolMacropores,
                                                       VolRelMacropores, Pluie, FTSWIrrig, IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus, PercolationMax, NbJAS,
                                                       RuSurf, ResUtil, RootFront, EpaisseurSurf, EpaisseurProf, ProfRacIni, FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
                                                       StockMacropores, EauDispo, RuRac, StockRac, FTSW, Lr);

        samara::RS_EvolRempliResRFE_RDE_V2(NumPhase, RuSurf, EauDispo, RuRac, CapaRFE, CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight, EpaisseurSurf,
                                              EpaisseurProf, VolMacropores, FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr, ValRDE, ValRFE, ValRSurf,
                                              FloodwaterGain, StockMacropores);

        if (crop) samara::RS_EvolWaterLoggingUpland_V2(PercolationMax, BundHeight, VolMacropores, Dr, Lr, StockMacropores);
        if (crop) samara::RS_EvalStressWaterLogging_V2(StockMacropores, VolMacropores, RootFront, EpaisseurSurf, EpaisseurProf, WaterLoggingSens, FractionRootsLogged,
                                                          CoeffStressLogging);

        samara::RS_EvolRempliMacropores_V2(NumPhase, EpaisseurSurf, EpaisseurProf, ResUtil, StockMacropores, RootFront, CapaRDE, CapaRFE, FloodwaterDepth,
                                              StockTotal, Hum, StockSurface, StockRac, ValRDE, ValRFE, ValRSurf);

        samara::RS_EvolRurRFE_RDE_V2_1(VitesseRacinaire, Hum, ResUtil, StockSurface, RuSurf, ProfRacIni, EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase,
                                          ChangePhase, FloodwaterDepth, StockMacropores, RootFrontMax, ChangeNurseryStatus, Transplanting, TransplantingDepth, RuRac,
                                          StockRac, StockTotal, FloodwaterGain, RootFront);
        samara::RS_PlantSubmergence_V2(PlantHeight, FloodwaterDepth, FractionPlantHeightSubmer);

        /*if (crop) */samara::RS_EvalRootFront(NumPhase, RuRac, ResUtil, RootFront);

        samara::RS_EvolPSPMVMD(NumPhase, ChangePhase, SumDegreDayCor, DegresDuJourCor, SeuilPP, PPCrit, DayLength, PPExp, SumPP, SumDDPhasePrec, SeuilTemp);

        if (crop) samara::EvolSomDegresJour(DegresDuJour, NumPhase, SumDegresDay);
        if (crop) samara::RS_EvolSomDegresJourCor(DegresDuJourCor, NumPhase, SumDegreDayCor);

        samara::RS_EvalRUE_V2_1(NumPhase, ChangePhase, PARIntercepte, DryMatTotPop, DeadLeafdrywtPop, DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
                                   Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot, Conversion, NbJAS, Transplanting, NurseryStatus, Density, DensityNursery,
                                   DryMatAboveGroundTotPop, /*DryMatAboveGroundPop,*/ RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                   TrEff, WueEt, WueTot, ConversionEff);//, RUEgreen);

        samara::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                      GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                      DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                      CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                      IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                      CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                      ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);



        //                samara2_1::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

        if ( NumPhase >= 6 ) {
            GrowthPop = 0;
            GrowthDryMatPop = 0;
        }
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_1();
                //                CumWUseFin = cumRain;
                //                CumIrrigFin = CumDr;
            }
            DryMatResInternodePop = 0;
        }

        /** OUTPUT VECTORS **/
        vector <double> result;
        if(LOG == ECOTROP) {
            vector <double> r{NbJAS,NumPhase,DegresDuJour,DegresDuJourCor,Lai,MaxLai,FTSW,Cstr,RootFront,ETo,ETM,ETR,RUE,ConversionEff,Ic,CulmsPerPlant,CulmsPop,CulmsPerHill,GrainYieldPop,DryMatStructLeafPop,DryMatStructSheathPop,DryMatStructRootPop,DryMatStructInternodePop,DryMatResInternodePop,DryMatStructPaniclePop,DryMatStructTotPop,DryMatVegeTotPop,DryMatPanicleTotPop,DryMatStemPop,DryMatAboveGroundPop,DryMatTotPop,SterilityCold,SterilityHeat,SterilityDrought,SterilityTot,HarvestIndex,PanicleNumPop,PanicleNumPlant,GrainYieldPanicle,SpikeNumPop,SpikeNumPanicle,FertSpikeNumPop,GrainFillingStatus,PhaseStemElongation,Sla,HaunIndex,ApexHeight,PlantHeight,PlantWidth,VitesseRacinaireDay,Kcl,KRolling,LIRkdfcl,LTRkdfcl,AssimPot,Assim,RespMaintTot,SupplyTot,AssimSurplus,AssimNotUsed,AssimNotUsedCum,IcMean,CstrMean,TillerDeathPop,PlantLeafNumTot,DeadLeafdrywtPop,LaiDead,ResCapacityInternodePop,InternodeResStatus,DayLength,Par,RgCalc,VPDCalc,TMoyCalc,HMoyCalc,KceReal,StockTotal,EauDispo,StockSurface,StockRac,RuRac,Kcp,Kce,EvapPot,Evap,TrPot,Tr,Lr,Dr,PARIntercepte,SumDegresDay,VitesseRacinaire,CstrAssim,RayExtra,CumPAR,SumDegreDayCor,SumPP,SumDDPhasePrec,SeuilTemp,RespMaintDebt,TminMoy,TmaxMoy,FtswMoy,ResInternodeMobiliDay,RootShootRatio,TrEff,WueEt,WueTot,ConversionEff,SlaMitch,FloodwaterDepth,IrrigAutoDay,IrrigTotDay,FractionPlantHeightSubmer,Density,RootMassPerVol,CumEt,CumWUse,cumRain,CumWReceived,CumDr,CumLr,TrEffInst,VolMacropores,StockMacropores,IcCum,CstrCum,SlaNew,LTRkdf,LIRkdf,RelPotLeafLength,ApexHeightGain,HaunGain,Conversion,RootFrontOld,SimStartGermin,SimEmergence,SimStartPSP,SimPanIni,SimAnthesis50,SimStartMatu2,SimEndCycle,StressCold,CulmsPerPlantMax,CulmsPerHillMax,DurGermFlow,DurGermMat,LaiFin,CulmsPerHillFin,CulmsPerPlantFin,GrainYieldPopFin,DryMatAboveGroundPopFin,ReservePopFin,FractionRootsLogged,ResUtil,KcTot,DryMatAboveGroundTotPop,DemResInternodePop,LastLeafLengthPot,LastLeafLength,CumSupplyTot,DryMatResInternodePopOld,CumGrowthPop,GrowthPop,A_DemStructLeaf,A_GrowthStructLeaf,A_DemStructTot,A_GrowthStructTot,CoeffCO2Tr,CoeffCO2Assim,A_AssimSurplus,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CumCarbonUsedPop,DryMatTotPopFin,GrainFillingStatusFin,SterilityTotFin,CumIrrigFin,CumWUseFin,GrowthDryMatPop
                        , Conversion, CumTr};
            result = r;
        } else if (LOG == COMPLETE) {
            vector <double> r {ApexHeight,ApexHeightGain,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,A_AssimSurplus,A_DemStructLeaf,A_DemStructTot,A_GrowthStructLeaf,A_GrowthStructTot,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CapaRDE,CapaREvap,CapaRFE,ChangeNurseryStatus,ChangePhase,ChangeSsPhase,CoeffCO2Assim,CoeffCO2Tr,CoeffStressLogging,Conversion,ConversionEff,CounterNursery,Cstr,CstrAssim,CstrCum,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CulmsPerHillFin,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantFin,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumCstrPhase2,CumCstrPhase3,CumCstrPhase4,CumCstrPhase5,CumCstrPhase6,CumDr,CumEt,CumFTSWPhase2,CumFTSWPhase3,CumFTSWPhase4,CumFTSWPhase5,CumFTSWPhase6,CumGrowthPop,CumIcPhase2,CumIcPhase3,CumIcPhase4,CumIcPhase5,CumIcPhase6,CumIrrig,CumIrrigFin,CumLr,CumPAR,CumSupplyTot,CumTr,CumWReceived,CumWUse,CumWUseFin,DAF,DayLength,DeadLeafdrywtPop,Decli,DegresDuJour,DegresDuJourCor,DemLeafAreaPlant,DemPanicleFillPop,DemResInternodePop,DemStructInternodePlant,DemStructInternodePop,DemStructLeafPlant,DemStructLeafPop,DemStructPaniclePlant,DemStructPaniclePop,DemStructRootPlant,DemStructRootPop,DemStructSheathPop,DemStructTotPop,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundPopFin,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatResInternodePopOld,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatTotPopFin,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FloodwaterGain,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswMoy,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GainRootSystSoilSurfPop,GainRootSystVolPop,GrainFillingStatus,GrainFillingStatusFin,GrainYieldPanicle,GrainYieldPop,GrainYieldPopFin,GrowthDryMatPop,GrowthPop,GrowthResInternodePop,GrowthStructDeficit,GrowthStructInternodePop,GrowthStructLeafPop,GrowthStructPaniclePop,GrowthStructRootPop,GrowthStructSheathPop,GrowthStructTotPop,HarvestIndex,HaunGain,HaunIndex,HMoyCalc,Hum,Ic,IcCum,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,IncreaseResInternodePop,InternodeResStatus,Irrigation,IrrigAutoDay,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LaiFin,LastLeafLength,LastLeafLengthPot,LatRad,LeafDeathPop,LIRkdf,LIRkdfcl,Lr,LTRkdf,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NumSsPhase,NurseryStatus,PanicleFilDeficit,PanicleFilPop,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PhaseStemElongation,PlantHeight,PlantLeafNumNew,PlantLeafNumTot,PlantWidth,ProfRu,RayExtra,RelPotLeafLength,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,RgCalc,RgMax,RootFront,RootFrontOld,RootMassPerVol,RootShootRatio,RootSystSoilSurfPop,RootSystSoilSurfPopOld,RootSystVolPop,RootSystVolPopOld,RUE,RUEGreen,RuRac,RuSurf,SDJCorPhase4,SeuilCstrMortality,SeuilTemp,SeuilTempSsPhase,SimAnthesis50,SimEmergence,SimEndCycle,SimPanIni,SimStartGermin,SimStartMatu2,SimStartPSP,Sla,SlaMitch,SlaNew,SommeDegresJourMax,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,SterilityTotFin,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,StRuMax,SumDDPhasePrec,SumDegreDayCor,SumDegresDay,SumPP,SunDistance,SunPosi,SupplyTot,TempLai,TillerDeathPop,TmaxMoy,TminMoy,TMoyCalc,TMoyPrec,Tr,TrEff,TrEffInst,TrPot,ValRDE,ValRFE,ValRSurf,VPDCalc,VitesseRacinaire,VitesseRacinaireDay,VolMacropores,VolRelMacropores,WueEt,WueTot,
                        TMax,TMin,TMoy,HMax,HMin,HMoy,Vt,Ins,Rg,ETP,Pluie};
            result = r;
        } else if (LOG == SMALL) {
            vector <double> r {NbJAS,ApexHeight,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CapaREvap,CapaRFE,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,cumRain,CulmsPerHill,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,CumTr,CumWReceived,CumWUse,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainYieldPanicle,GrainYieldPop,HarvestIndex,HaunIndex,HMax,HMin,HMoy,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,Irrigation,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdfcl,Lr,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NurseryStatus,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantLeafNumTot,PlantWidth,Pluie,ProfRu,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,Rg,RgCalc,RgMax,RootFront,RootMassPerVol,RootShootRatio,RUE,RUEGreen,RuRac,RuSurf,Sla,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,SumDegreDayCor,SumDegresDay,SupplyTot,TillerDeathPop,TMax,TMin,TMoy,Tr,TrEff,TrEffInst,TrPot,VitesseRacinaire,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot};
            result = r;
        }

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
        FractionRootsLogged = 0;
    }


    /** OUTPUT VECTORS **/
    vector <string> names;
    if(LOG == ECOTROP) {
        vector <string> n {"Jour","NbJas","NumPhase","DegresDuJour","DegresDuJourCor","Lai","MaxLai","FTSW","Cstr","RootFront","Eto","ETM","ETR","RUE","ConversionEff","Ic","CulmsPerPlant","CulmsPop","CulmsPerHill","GrainYieldPop","DryMatStructLeafPop","DryMatStructSheathPop","DryMatStructRootPop","DryMatStructInternodePop","DryMatResInternodePop","DryMatStructPaniclePop","DryMatStructTotPop","DryMatVegeTotPop","DryMatPanicleTotPop","DryMatStemPop","DryMatAboveGroundPop","DryMatTotPop","SterilityCold","SterilityHeat","SterilityDrought","SterilityTot","HarvestIndex","PanicleNumPop","PanicleNumPlant","GrainYieldPanicle","SpikeNumPop","SpikeNumPanicle","FertSpikeNumPop","GrainFillingStatus","PhaseStemElongation","Sla","HaunIndex","ApexHeight","PlantHeight","PlantWidth","VitesseRacinaireDay","Kcl","KRolling","LIRkdfcl","LTRkdfcl","AssimPot","Assim","RespMaintTot","SupplyTot","AssimSurplus","AssimNotUsed","AssimNotUsedCum","IcMean","CstrMean","TillerDeathPop","PlantLeafNumTot","DeadLeafdrywtPop","LaiDead","ResCapacityInternodePop","InternodeResStatus","DayLength","Par","RgCalc","VDPCalc","TMoyCalc","HMoyCalc","KceReal","StockTotal","EauDispo","StockSurface","StockRac","RURac","Kcp","Kce","EvapPot","Evap","TrPot","Tr","Lr","Dr","ParIntercepte","SumDegresDay","VitesseRacinaire","CstrAssim","RayExtra","CumPAR","SumDegreDayCor","SumPP","SumDDPhasePrec","SeuilTemp","RespMaintDebt","TMinMoy","TMaxMoy","FtswMoy","ResInternodeMobiliDay","RootShootRatio","TrEffInst","TrEff","WueEt","WueTot","SlaMitch","FloodwaterDepth","IrrigAutoDay","IrrigTotDay","FractionPlantHeightSubmer","Density","RootMassPerVol","CumTr","CumEt","CumWUse","CumWReceived","CumIrrig","CumDr","CumLr","VolMacropores","StockMacropores","IcCum","CstrCum","SlaNew","LTRkdf","LIRkdf","RelPotLeafLength","ApexHeightGain","Haungain","Conversion","RootFrontOld","SimStartGermin","SimEmergence","SimStartPSP","SimPanIni","SimAnthesis50","SimStartMatu2","SimEndCycle","StressCold","CulmsPerPlantMax","CulmsPerHillMax","DurGermFlow","DurGermMat","LaiFin","CulmsPerHillFin","CulmsPerPlantFin","GrainYieldPopFin","DryMatAboveGroundPopFin","ReservePopFin","FractionRootsLogged","ResUtil","KcTot","DryMatAboveGroundTotPop","DemResInternodePop","LastLeafLengthPot","LastLeafLength","CumSupplyTot","DryMatResInternodePopOld","CumGrowthPop","GrowthPop","A_DemStructLeaf","A_GrowthStructLeaf","A_DemStructTot","A_GrowthStructTot","CoeffCO2Tr","CoeffCO2Assim","A_AssimSurplus","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CumCarbonUsedPop","DryMatTotPopFin","GrainFillingStatusFin","SterilityTotFin","CumIrrigFin","CumWUseFin","GrowthDryMatPop_V2_1"
                           , "Conversion", "MonCumTr"};
        names = n;
    } else if (LOG == COMPLETE) {
        vector <string> n {"ObsPlantDate","ApexHeight","ApexHeightGain","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","A_AssimSurplus","A_DemStructLeaf","A_DemStructTot","A_GrowthStructLeaf","A_GrowthStructTot","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CapaRDE","CapaREvap","CapaRFE","ChangeNurseryStatus","ChangePhase","ChangeSsPhase","CoeffCO2Assim","CoeffCO2Tr","CoeffStressLogging","Conversion","ConversionEff","CounterNursery","Cstr","CstrAssim","CstrCum","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CstrPhase6","CulmsPerHill","CulmsPerHillFin","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantFin","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumCstrPhase2","CumCstrPhase3","CumCstrPhase4","CumCstrPhase5","CumCstrPhase6","CumDr","CumEt","CumFTSWPhase2","CumFTSWPhase3","CumFTSWPhase4","CumFTSWPhase5","CumFTSWPhase6","CumGrowthPop","CumIcPhase2","CumIcPhase3","CumIcPhase4","CumIcPhase5","CumIcPhase6","CumIrrig","CumIrrigFin","CumLr","CumPAR","CumSupplyTot","CumTr","CumWReceived","CumWUse","CumWUseFin","DAF","DayLength","DeadLeafdrywtPop","Decli","DegresDuJour","DegresDuJourCor","DemLeafAreaPlant","DemPanicleFillPop","DemResInternodePop","DemStructInternodePlant","DemStructInternodePop","DemStructLeafPlant","DemStructLeafPop","DemStructPaniclePlant","DemStructPaniclePop","DemStructRootPlant","DemStructRootPop","DemStructSheathPop","DemStructTotPop","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundPopFin","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatResInternodePopOld","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatTotPopFin","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FloodwaterGain","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswMoy","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GainRootSystSoilSurfPop","GainRootSystVolPop","GrainFillingStatus","GrainFillingStatusFin","GrainYieldPanicle","GrainYieldPop","GrainYieldPopFin","GrowthDryMatPop","GrowthPop","GrowthResInternodePop","GrowthStructDeficit","GrowthStructInternodePop","GrowthStructLeafPop","GrowthStructPaniclePop","GrowthStructRootPop","GrowthStructSheathPop","GrowthStructTotPop","HarvestIndex","HaunGain","HaunIndex","HMoyCalc","Hum","Ic","IcCum","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","IncreaseResInternodePop","InternodeResStatus","Irrigation","IrrigAutoDay","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LaiFin","LastLeafLength","LastLeafLengthPot","LatRad","LeafDeathPop","LIRkdf","LIRkdfcl","Lr","LTRkdf","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NumSsPhase","NurseryStatus","PanicleFilDeficit","PanicleFilPop","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PhaseStemElongation","PlantHeight","PlantLeafNumNew","PlantLeafNumTot","PlantWidth","ProfRu","RayExtra","RelPotLeafLength","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","RgCalc","RgMax","RootFront","RootFrontOld","RootMassPerVol","RootShootRatio","RootSystSoilSurfPop","RootSystSoilSurfPopOld","RootSystVolPop","RootSystVolPopOld","RUE","RUEGreen","RuRac","RuSurf","SDJCorPhase4","SeuilCstrMortality","SeuilTemp","SeuilTempSsPhase","SimAnthesis50","SimEmergence","SimEndCycle","SimPanIni","SimStartGermin","SimStartMatu2","SimStartPSP","Sla","SlaMitch","SlaNew","SommeDegresJourMax","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","SterilityTotFin","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","StRuMax","SumDDPhasePrec","SumDegreDayCor","SumDegresDay","SumPP","SunDistance","SunPosi","SupplyTot","TempLai","TillerDeathPop","TmaxMoy","TminMoy","TMoyCalc","TMoyPrec","Tr","TrEff","TrEffInst","TrPot","ValRDE","ValRFE","ValRSurf","VPDCalc","VitesseRacinaire","VitesseRacinaireDay","VolMacropores","VolRelMacropores","WueEt","WueTot",
                           "TMax","TMin","TMoy","HMax","HMin","HMoy","Vt","Ins","Rg","ETP","Rain"};
        names = n;
    } else if (LOG == SMALL) {
        vector <string> n {"ObsPlantDate","NbJas","ApexHeight","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","CapaREvap","CapaRFE","Conversion","ConversionEff","Cstr","CstrAssim","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CumRain","CulmsPerHill","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumDr","CumEt","CumIrrig","CumLr","CumPAR","CumTr","CumWReceived","CumWUse","DayLength","DeadLeafdrywtPop","DegresDuJour","DegresDuJourCor","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETP","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GrainFillingStatus","GrainYieldPanicle","GrainYieldPop","HarvestIndex","HaunIndex","HMax","HMin","HMoy","Hum","Ic","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","Ins","InternodeResStatus","Irrigation","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LastLeafLength","LastLeafLengthPot","LeafDeathPop","LIRkdfcl","Lr","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NurseryStatus","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PlantHeight","PlantLeafNumTot","PlantWidth","Pluie","ProfRu","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","Rg","RgCalc","RgMax","RootFront","RootMassPerVol","RootShootRatio","RUE","RUEGreen","RuRac","RuSurf","Sla","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","SumDegreDayCor","SumDegresDay","SupplyTot","TillerDeathPop","TMax","TMin","TMoy","Tr","TrEff","TrEffInst","TrPot","VitesseRacinaire","VolMacropores","VolRelMacropores","VPDCalc","Vt","WueEt","WueTot"};
        names = n;
    }

    vector < vector <double> > results;
    vector <double> dayValues;
    for (int j = 0; j < currentResults.size(); ++j) {
        dayValues.push_back(j + DateDebutSimul);
    }
    results.push_back(dayValues);

    for (int i = 0; i < names.size()-1; ++i) {
        vector <double> values;
        for (int j = 0; j < currentResults.size(); ++j) {
            values.push_back(currentResults[j][i]);
        }
        results.push_back(values);
    }

    return pair <vector <string>, vector < vector <double> > > (names, results);
}

pair <vector <string>, vector < vector <double> > > run_samara_2_1_micha(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_1_micha();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    //    double DateDebutSimul = parameters->getDouble("datedebut");
    //    double DateFinSimul = parameters->getDouble("datefin");
    //    double DateSemis = parameters->getDouble("datesemis");
    double DateEnCours = DateDebutSimul;
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters_2_1_micha(parameters);
    init_parcelle(VolMacropores, HumSat, HumFC, ResUtil
                  , ProfRu, EpaisseurSurf, EpaisseurProf
                  , RuSurf, CapaREvap, CapaRFE, CapaRDE
                  , StRuMax, StockIniProf, StockIniSurf
                  , ValRSurf, ValRDE, ValRFE
                  , StockSurface, StockTotal, Hum
                  , LTRkdfcl, LIRkdf, LTRkdf
                  , CounterNursery, VolRelMacropores);
    LatRad = Latitude * M_PI * 1.0 / 180;


    vector <  vector <double> > currentResults;

    bool crop = false;
    //Compute day before for TMoyPrec
//    set_meteo_vars(parameters, -1,
//                   TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
//                   Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
//    EToFao(ETP, Altitude, RgMax, RgCalc,
//           TMin, TMax,
//           HMin, HMax, HMoyCalc,
//           TMoyCalc, Vt, ETo,
//           TMoyPrec, VPDCalc);
    //

    double cumRain = 0;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        set_meteo_vars(parameters, DateEnCours-DateDebutSimul,
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
        NbJAS = DateEnCours - DateSemis;
        if(NumPhase > 0)
            cumRain += Pluie;
        //crop entity
        if(NbJAS == 0) {
            init_culture();
            crop = true;
        }

        if(crop) samara::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                       CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                       DryMatStructPaniclePop, DryMatResInternodePop);

        eval_Par(DateEnCours);
        //        if(start++ < 2) ETP = 0;
        EToFao(ETP, Altitude, RgMax, RgCalc,
               TMin, TMax,
               HMin, HMax, HMoyCalc,
               TMoyCalc, Vt, ETo,
               TMoyPrec, VPDCalc);

        if(crop) samara::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                                               DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);

        /** Phyllo **/
        samara::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
        samara::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
        samara::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
        samara::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
        samara::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
        samara::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
        samara::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
        samara::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);
        /*if (crop) */samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
        samara::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
        samara::RS_EvalDAF_V2(NumPhase, DAF);
        samara::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
        samara::RS_EvolHauteur_SDJ_cstr_V2_1(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
                                                CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold, CstrMean, ApexHeightGain, ApexHeight, PlantHeight, PlantWidth);
        samara::RS_EvolKcpKceBilhy(LTRkdfcl, KcMax, Mulch, Kcp, Kce, KcTot);
        if(DateEnCours != DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        samara::RS_EvolEvapSurfRFE_RDE_V2_1(NumPhase, Kce, EvapPot, CapaREvap, CapaRDE, CapaRFE,
                                               RuRac, RuSurf, BundHeight, EpaisseurSurf, EpaisseurProf,
                                               RootFront, ResUtil, Evap, ValRSurf, ValRFE, ValRDE, StockRac,
                                               StockTotal, StockSurface, Kr, KceReal, FloodwaterDepth, StockMacropores);
        if(DateEnCours == DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        /** CROP **/
        if (crop) samara::RS_EvalFTSW_V2(RuRac, StockTotal, StockMacropores, StRuMax, StockRac, FTSW);
        if (crop) samara::RS_EvalCstrPFactorFAO_V2(PFactor, FTSW, ETo, KcTot, StockMacropores, CoeffStressLogging, Cstr);
        if (crop) samara::DemandePlante_V2_1(Kcp, ETo, Ca, CO2Slopetr, TrPot, CoeffCO2Tr);
        if (crop) samara::EvalTranspi(TrPot, Cstr, Tr);
        samara::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
        if (crop) samara::RS_EvolConsRes_Flood_V2(NumPhase, RuRac, RuSurf, CapaREvap, Tr, Evap, CapaRDE, CapaRFE, EpaisseurSurf, EpaisseurProf, ResUtil, StockRac,
                                                     StockSurface, StockTotal, ValRFE, ValRDE, ValRSurf, FloodwaterDepth, StockMacropores);

        /*if (crop) */samara::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
//        samara::RS_EvalTMaxMoy_V2_3(TMax, TMin, HMax, HMin, NumPhase, NumSsPhase, TmaxMoy);
        /*if (crop) */samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

        samara::RS_EvalFtswMoy(FTSW, NumPhase, NumSsPhase, FtswMoy);
        samara::RS_EvalSterility(NumPhase, ChangePhase, KCritSterCold1, KCritSterCold2, KCritSterHeat1, KCritSterHeat2, KCritSterFtsw1, KCritSterFtsw2, TminMoy,
                                    TmaxMoy, FtswMoy, SterilityCold, SterilityHeat, SterilityDrought, SterilityTot);
        samara::RS_EvalVitesseRacinaire(VRacLevee, VRacBVP, VRacRPR, VRacPSP, VRacMatu1, VRacMatu2, RootCstr, Cstr, NumPhase, DegresDuJourCor, VitesseRacinaire, VitesseRacinaireDay);

        if (crop) samara::EvalConversion(NumPhase, TxConversion, TxAssimBVP, SumDegresDay, SumDDPhasePrec, TxAssimMatu1, TxAssimMatu2, SeuilTemp, Conversion);

        samara::RS_EvalParIntercepte_V2_1(Par, Lai, Kdf, PARIntercepte, LIRkdfcl);
        if(crop)samara::RS_EvalAssimPot_V2_1(PARIntercepte, Par, Conversion, TMax, TMin, TBase, TOpt1, DayLength, StressCold, CO2Exp, Ca, CO2Cp, SlaMin, Sla, CoeffAssimSla,
                                                AssimPot, CoeffCO2Assim);

        if (crop) samara::RS_EvalCstrAssim(Cstr, ASScstr, CstrAssim);
        if (crop) samara::RS_EvalAssim(AssimPot, CstrAssim, Assim);

        samara::RS_TransplantingShock_V2(CounterNursery, CoeffTransplantingShock, Assim);
        samara::RS_EvalRespMaint(KRespMaintLeaf, KRespMaintSheath, KRespMaintRoot, KRespInternode, KRespPanicle, DryMatStructLeafPop, DryMatStructSheathPop,
                                    DryMatStructRootPop, DryMatStructInternodePop, DryMatStructPaniclePop, TMoyCalc, KTempMaint, CoefficientQ10, RespMaintTot);
        samara::RS_EvalRelPotLeafLength(NumPhase, HaunIndex, RankLongestLeaf, RelPotLeafLength);
        samara::RS_EvolPlantTilNumTot_V2(NumPhase, ChangePhase, PlantsPerHill, TilAbility, Density, Ic, IcTillering, Cstr, HaunIndex, HaunCritTillering, LTRkdfcl,
                                            CulmsPerHill, CulmsPerPlant, CulmsPop);
        samara::RS_EvolPlantLeafNumTot(NumPhase, CulmsPerHill, HaunGain, PlantLeafNumNew, PlantLeafNumTot);
        samara::RS_EvolMobiliTillerDeath_V2_1(NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic, PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                                 CulmsPerHill, DryMatStructPaniclePop);
        samara::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        samara::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                         SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                                DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
        samara::RS_EvalDemandStructSheath(NumPhase, DemStructLeafPop, WtRatioLeafSheath, SlaMin, SlaMax, Sla, StressCold, DemStructSheathPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                              DemStructSheathPop, DryMatStructRootPop, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
                                              DemStructRootPop, RootSystSoilSurfPopOld, RootFrontOld, RootSystVolPopOld, DemStructRootPlant);
        samara::RS_EvalDemandStructIN_V2_1(PhaseStemElongation, ApexHeightGain, CulmsPerHill, CoeffInternodeMass, Density, Ic, ResCapacityInternodePop, DryMatResInternodePop,
                                              CoeffReserveSink, NumPhase, DemStructInternodePlant, DemStructInternodePop, DemResInternodePop);
        samara::RS_EvalDemandStructPanicle_V2(NumPhase, CoeffPanicleMass, CulmsPerHill, Ic, DryMatStructPaniclePop, Density, PanStructMassMax, StressCold, DemStructPaniclePlant,
                                                 PanStructMass, DemStructPaniclePop);
        samara::RS_EvalDemandTotAndIcPreFlow_V2_1(NumPhase, RespMaintTot, DemStructLeafPop, DemStructSheathPop, DemStructRootPop, DemStructInternodePop, DemStructPaniclePop,
                                                     SupplyTot, NbDaysSinceGermination, PlantHeight, Cstr, DemResInternodePop, DemStructTotPop, Ic, IcCum, IcMean, CstrCum, CstrMean,
                                                     A_DemStructTot);
        samara::RS_EvolGrowthStructLeafPop_V2_1(NumPhase, Ic, SupplyTot, DemStructLeafPop, DemStructTotPop, GrowthStructLeafPop, A_GrowthStructLeaf);
        samara::RS_EvolGrowthStructSheathPop(NumPhase, Ic, SupplyTot, DemStructSheathPop, DemStructTotPop, GrowthStructSheathPop);
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, GrowthStructRootPop);
        samara::RS_EvolGrowthStructINPop_V2_1(NumPhase, Ic, SupplyTot, DemStructInternodePop, DemStructTotPop, DemResInternodePop, GrowthStructInternodePop, GrowthResInternodePop);
        samara::RS_EvolGrowthStructPanPop(NumPhase, Ic, SupplyTot, DemStructPaniclePop, DemStructTotPop, GrowthStructPaniclePop);
        samara::RS_Priority2GrowthPanStrctPop_V2_1(PriorityPan, DemStructPaniclePop, NumPhase, GrowthStructTotPop, DemStructInternodePop, DemStructTotPop, DemStructLeafPop,
                                                      DemStructSheathPop, DemStructRootPop, DemResInternodePop, GrowthStructPaniclePop, GrowthStructInternodePop,
                                                      GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthResInternodePop);
        if(crop) samara::RS_EvolGrowthStructTot_V2_1(NumPhase, SupplyTot, GrowthResInternodePop, GrowthStructTotPop, AssimSurplus, GrowthStructLeafPop, GrowthStructSheathPop,
                                                        GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus);
        if(crop) samara::RS_AddResToGrowthStructPop_V2_1(NumPhase, Ic, PhaseStemElongation, DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
                                                            DemStructRootPop, DemStructInternodePop, DemStructPaniclePop, RelMobiliInternodeMax, GrowthResInternodePop,
                                                            ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
                                                            GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay, A_GrowthStructLeaf,
                                                            A_GrowthStructTot, A_ResInternodeMobiliDay);
        if(crop) samara::RS_EvolDemPanFilPopAndIcPFlow_V2_1(NumPhase, DryMatStructPaniclePop, CoeffPanSinkPop, SterilityTot, DegresDuJourCor, SDJMatu1, SupplyTot, Assim,
                                                               RespMaintTot, StressCold, PanicleSinkPop, DemPanicleFillPop, AssimSurplus, Ic, A_AssimSurplus);
        if(crop) samara::RS_EvolPanicleFilPop_V2_1(NumPhase, Ic, DryMatResInternodePop, DemPanicleFillPop, SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim,
                                                      ResInternodeMobiliDayPot, AssimSurplus, PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop, GrainYieldPop,
                                                      A_AssimSurplus, A_ResInternodeMobiliDay);
        //        if(DryMatResInternodePop > 5000) {
        //            DryMatResInternodePop = DryMatResInternodePop;
        //        }

        if(crop) samara::RS_EvolGrowthReserveInternode_V2_1(NumPhase, PhaseStemElongation, DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
                                                               AssimSurplus, ResInternodeMobiliDay, ResCapacityInternodePop, IncreaseResInternodePop, DryMatResInternodePop,
                                                               AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, DryMatResInternodePopOld, A_IncreaseResInternodePop);

        if(crop) samara::RS_EvolGrowthTot_V2_1(NumPhase, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
                                                  GrowthResInternodePop, PanicleFilPop, DryMatResInternodePop, DryMatResInternodePopOld, GrowthStructTotPop, GrowthDryMatPop,
                                                  A_GrowthStructTot);
        if(crop)samara::RS_ExcessAssimilToRoot_V2(NumPhase, ExcessAssimToRoot, DryMatStructRootPop, RootSystVolPop, CoeffRootMassPerVolMax, RootMassPerVol, GrowthStructRootPop,
                                                     AssimNotUsed);
        if(crop) samara::RS_EvolDryMatTot_V2_1(NumPhase, ChangePhase, PlantsPerHill, TxResGrain, PoidsSecGrain, Density, GrowthStructLeafPop, GrowthStructSheathPop,
                                                  GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, GrowthResInternodePop, GrainYieldPop,
                                                  ResCapacityInternodePop, CulmsPerPlant, CoeffPanSinkPop, SterilityTot, DeadLeafdrywtPop, DryMatResInternodePopOld, PanicleFilPop,
                                                  AssimNotUsedCum, MobiliLeafDeath, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                                  DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                                  DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                                  SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                                  GrowthPop, CumCarbonUsedPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                   DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);
        if(crop)samara::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase, TempSLA, DryMatStructLeafPop,
                                           GrowthStructLeafPop, SlaMitch, SlaNew, Sla);

        samara::RS_EvalRuiss_FloodDyna_V2(NumPhase, Pluie, SeuilRuiss, PourcRuiss, BundHeight, Irrigation, PlantHeight, LifeSavingDrainage, PlotDrainageDAF,
                                             VolMacropores, SeuilRuiss, PercolationMax, DAF, StockMacropores, FloodwaterDepth, EauDispo, Lr);

        if(crop)samara::RS_AutomaticIrrigation_V2_1(NumPhase, IrrigAuto, IrrigAutoTarget, BundHeight, PlantHeight, Irrigation, PlotDrainageDAF, DAF, VolMacropores,
                                                       VolRelMacropores, Pluie, FTSWIrrig, IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus, PercolationMax, NbJAS,
                                                       RuSurf, ResUtil, RootFront, EpaisseurSurf, EpaisseurProf, ProfRacIni, FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
                                                       StockMacropores, EauDispo, RuRac, StockRac, FTSW, Lr);

        samara::RS_EvolRempliResRFE_RDE_V2(NumPhase, RuSurf, EauDispo, RuRac, CapaRFE, CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight, EpaisseurSurf,
                                              EpaisseurProf, VolMacropores, FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr, ValRDE, ValRFE, ValRSurf,
                                              FloodwaterGain, StockMacropores);

        if (crop) samara::RS_EvolWaterLoggingUpland_V2(PercolationMax, BundHeight, VolMacropores, Dr, Lr, StockMacropores);
        if (crop) samara::RS_EvalStressWaterLogging_V2(StockMacropores, VolMacropores, RootFront, EpaisseurSurf, EpaisseurProf, WaterLoggingSens, FractionRootsLogged,
                                                          CoeffStressLogging);

        samara::RS_EvolRempliMacropores_V2(NumPhase, EpaisseurSurf, EpaisseurProf, ResUtil, StockMacropores, RootFront, CapaRDE, CapaRFE, FloodwaterDepth,
                                              StockTotal, Hum, StockSurface, StockRac, ValRDE, ValRFE, ValRSurf);

        samara::RS_EvolRurRFE_RDE_V2_1(VitesseRacinaire, Hum, ResUtil, StockSurface, RuSurf, ProfRacIni, EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase,
                                          ChangePhase, FloodwaterDepth, StockMacropores, RootFrontMax, ChangeNurseryStatus, Transplanting, TransplantingDepth, RuRac,
                                          StockRac, StockTotal, FloodwaterGain, RootFront);
        samara::RS_PlantSubmergence_V2(PlantHeight, FloodwaterDepth, FractionPlantHeightSubmer);

        /*if (crop) */samara::RS_EvalRootFront(NumPhase, RuRac, ResUtil, RootFront);

        samara::RS_EvolPSPMVMD(NumPhase, ChangePhase, SumDegreDayCor, DegresDuJourCor, SeuilPP, PPCrit, DayLength, PPExp, SumPP, SumDDPhasePrec, SeuilTemp);

        if (crop) samara::EvolSomDegresJour(DegresDuJour, NumPhase, SumDegresDay);
        if (crop) samara::RS_EvolSomDegresJourCor(DegresDuJourCor, NumPhase, SumDegreDayCor);

        samara::RS_EvalRUE_V2_1(NumPhase, ChangePhase, PARIntercepte, DryMatTotPop, DeadLeafdrywtPop, DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
                                   Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot, Conversion, NbJAS, Transplanting, NurseryStatus, Density, DensityNursery,
                                   DryMatAboveGroundTotPop, /*DryMatAboveGroundPop,*/ RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                   TrEff, WueEt, WueTot, ConversionEff);//, RUEgreen);

        samara::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                      GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                      DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                      CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                      IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                      CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                      ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);



        //                samara2_1::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

        if ( NumPhase >= 6 ) {
            GrowthPop = 0;
            GrowthDryMatPop = 0;
        }
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_1();
                //                CumWUseFin = cumRain;
                //                CumIrrigFin = CumDr;
            }
            DryMatResInternodePop = 0;
        }

        /** OUTPUT VECTORS **/
        vector <double> result;
        if(LOG == ECOTROP) {
            vector <double> r{
                NbJAS,NumPhase,NumSsPhase,DegresDuJour,DegresDuJourCor,Lai,MaxLai,FTSW,Cstr,RootFront,ETo,ETM,ETR,RUE,
                ConversionEff,Ic,CulmsPerPlant,CulmsPop,CulmsPerHill,GrainYieldPop,DryMatStructLeafPop,DryMatStructSheathPop,
                DryMatStructRootPop,DryMatStructInternodePop,DryMatResInternodePop,DryMatStructPaniclePop,DryMatStructTotPop,
                DryMatVegeTotPop,DryMatPanicleTotPop,DryMatStemPop,DryMatAboveGroundPop,DryMatTotPop,SterilityCold,SterilityHeat,
                SterilityDrought,SterilityTot,HarvestIndex,PanicleNumPop,PanicleNumPlant,GrainYieldPanicle,SpikeNumPop,
                SpikeNumPanicle,FertSpikeNumPop,GrainFillingStatus,PhaseStemElongation,Sla,HaunIndex,ApexHeight,PlantHeight,
                PlantWidth,VitesseRacinaireDay,Kcl,KRolling,LIRkdfcl,LTRkdfcl,AssimPot,Assim,RespMaintTot,SupplyTot,AssimSurplus,
                AssimNotUsed,AssimNotUsedCum,IcMean,CstrMean,TillerDeathPop,PlantLeafNumTot,DeadLeafdrywtPop,LaiDead,
                ResCapacityInternodePop,InternodeResStatus,DayLength,Par,RgCalc,VPDCalc,TMoyCalc,HMoyCalc,KceReal,StockTotal,
                EauDispo,StockSurface,StockRac,RuRac,Kcp,Kce,EvapPot,Evap,TrPot,Tr,Lr,Dr,PARIntercepte,SumDegresDay,
                VitesseRacinaire,CstrAssim,RayExtra,CumPAR,SumDegreDayCor,SumPP,SumDDPhasePrec,SeuilTemp,RespMaintDebt,
                TminMoy,TmaxMoy,FtswMoy,ResInternodeMobiliDay,RootShootRatio,TrEff,WueEt,WueTot,ConversionEff,SlaMitch,
                FloodwaterDepth,IrrigAutoDay,IrrigTotDay,FractionPlantHeightSubmer,Density,RootMassPerVol,CumEt,CumWUse,
                /*cumRain*/CumWReceived,CumWReceived,CumDr,CumLr,TrEffInst,VolMacropores,StockMacropores,IcCum,CstrCum,SlaNew,LTRkdf,LIRkdf,
                RelPotLeafLength,ApexHeightGain,HaunGain,Conversion,RootFrontOld,SimStartGermin,SimEmergence,SimStartPSP,SimPanIni,
                SimAnthesis50,SimStartMatu2,SimEndCycle,StressCold,CulmsPerPlantMax,CulmsPerHillMax,DurGermFlow,DurGermMat,LaiFin,
                CulmsPerHillFin,CulmsPerPlantFin,GrainYieldPopFin,DryMatAboveGroundPopFin,ReservePopFin,FractionRootsLogged,ResUtil,
                KcTot,DryMatAboveGroundTotPop,DemResInternodePop,LastLeafLengthPot,LastLeafLength,CumSupplyTot,DryMatResInternodePopOld,
                CumGrowthPop,GrowthPop,A_DemStructLeaf,A_GrowthStructLeaf,A_DemStructTot,A_GrowthStructTot,CoeffCO2Tr,CoeffCO2Assim,
                A_AssimSurplus,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CumCarbonUsedPop,DryMatTotPopFin,GrainFillingStatusFin,
                SterilityTotFin,CumIrrigFin,CumWUseFin,GrowthDryMatPop,Conversion,CumTr};
            result = r;
        } else if (LOG == COMPLETE) {
            vector <double> r {ApexHeight,ApexHeightGain,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,A_AssimSurplus,A_DemStructLeaf,A_DemStructTot,A_GrowthStructLeaf,A_GrowthStructTot,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CapaRDE,CapaREvap,CapaRFE,ChangeNurseryStatus,ChangePhase,ChangeSsPhase,CoeffCO2Assim,CoeffCO2Tr,CoeffStressLogging,Conversion,ConversionEff,CounterNursery,Cstr,CstrAssim,CstrCum,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CulmsPerHillFin,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantFin,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumCstrPhase2,CumCstrPhase3,CumCstrPhase4,CumCstrPhase5,CumCstrPhase6,CumDr,CumEt,CumFTSWPhase2,CumFTSWPhase3,CumFTSWPhase4,CumFTSWPhase5,CumFTSWPhase6,CumGrowthPop,CumIcPhase2,CumIcPhase3,CumIcPhase4,CumIcPhase5,CumIcPhase6,CumIrrig,CumIrrigFin,CumLr,CumPAR,CumSupplyTot,CumTr,CumWReceived,CumWUse,CumWUseFin,DAF,DayLength,DeadLeafdrywtPop,Decli,DegresDuJour,DegresDuJourCor,DemLeafAreaPlant,DemPanicleFillPop,DemResInternodePop,DemStructInternodePlant,DemStructInternodePop,DemStructLeafPlant,DemStructLeafPop,DemStructPaniclePlant,DemStructPaniclePop,DemStructRootPlant,DemStructRootPop,DemStructSheathPop,DemStructTotPop,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundPopFin,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatResInternodePopOld,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatTotPopFin,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FloodwaterGain,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswMoy,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GainRootSystSoilSurfPop,GainRootSystVolPop,GrainFillingStatus,GrainFillingStatusFin,GrainYieldPanicle,GrainYieldPop,GrainYieldPopFin,GrowthDryMatPop,GrowthPop,GrowthResInternodePop,GrowthStructDeficit,GrowthStructInternodePop,GrowthStructLeafPop,GrowthStructPaniclePop,GrowthStructRootPop,GrowthStructSheathPop,GrowthStructTotPop,HarvestIndex,HaunGain,HaunIndex,HMoyCalc,Hum,Ic,IcCum,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,IncreaseResInternodePop,InternodeResStatus,Irrigation,IrrigAutoDay,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LaiFin,LastLeafLength,LastLeafLengthPot,LatRad,LeafDeathPop,LIRkdf,LIRkdfcl,Lr,LTRkdf,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NumSsPhase,NurseryStatus,PanicleFilDeficit,PanicleFilPop,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PhaseStemElongation,PlantHeight,PlantLeafNumNew,PlantLeafNumTot,PlantWidth,ProfRu,RayExtra,RelPotLeafLength,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,RgCalc,RgMax,RootFront,RootFrontOld,RootMassPerVol,RootShootRatio,RootSystSoilSurfPop,RootSystSoilSurfPopOld,RootSystVolPop,RootSystVolPopOld,RUE,RUEGreen,RuRac,RuSurf,SDJCorPhase4,SeuilCstrMortality,SeuilTemp,SeuilTempSsPhase,SimAnthesis50,SimEmergence,SimEndCycle,SimPanIni,SimStartGermin,SimStartMatu2,SimStartPSP,Sla,SlaMitch,SlaNew,SommeDegresJourMax,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,SterilityTotFin,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,StRuMax,SumDDPhasePrec,SumDegreDayCor,SumDegresDay,SumPP,SunDistance,SunPosi,SupplyTot,TempLai,TillerDeathPop,TmaxMoy,TminMoy,TMoyCalc,TMoyPrec,Tr,TrEff,TrEffInst,TrPot,ValRDE,ValRFE,ValRSurf,VPDCalc,VitesseRacinaire,VitesseRacinaireDay,VolMacropores,VolRelMacropores,WueEt,WueTot,
                        TMax,TMin,TMoy,HMax,HMin,HMoy,Vt,Ins,Rg,ETP,Pluie};
            result = r;
        } else if (LOG == SMALL) {
            vector <double> r {NbJAS,ApexHeight,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CapaREvap,CapaRFE,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,cumRain,CulmsPerHill,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,CumTr,CumWReceived,CumWUse,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainYieldPanicle,GrainYieldPop,HarvestIndex,HaunIndex,HMax,HMin,HMoy,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,Irrigation,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdfcl,Lr,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NurseryStatus,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantLeafNumTot,PlantWidth,Pluie,ProfRu,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,Rg,RgCalc,RgMax,RootFront,RootMassPerVol,RootShootRatio,RUE,RUEGreen,RuRac,RuSurf,Sla,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,SumDegreDayCor,SumDegresDay,SupplyTot,TillerDeathPop,TMax,TMin,TMoy,Tr,TrEff,TrEffInst,TrPot,VitesseRacinaire,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot};
            result = r;
        }

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
        FractionRootsLogged = 0;
    }


    /** OUTPUT VECTORS **/
    vector <string> names;
    if(LOG == ECOTROP) {
        vector <string> n {"Jour","NbJas","NumPhase","NumSsPhase","DegresDuJour","DegresDuJourCor","Lai","MaxLai","FTSW","Cstr","RootFront","Eto","ETM","ETR","RUE","ConversionEff","Ic","CulmsPerPlant","CulmsPop","CulmsPerHill","GrainYieldPop","DryMatStructLeafPop","DryMatStructSheathPop","DryMatStructRootPop","DryMatStructInternodePop","DryMatResInternodePop","DryMatStructPaniclePop","DryMatStructTotPop","DryMatVegeTotPop","DryMatPanicleTotPop","DryMatStemPop","DryMatAboveGroundPop","DryMatTotPop","SterilityCold","SterilityHeat","SterilityDrought","SterilityTot","HarvestIndex","PanicleNumPop","PanicleNumPlant","GrainYieldPanicle","SpikeNumPop","SpikeNumPanicle","FertSpikeNumPop","GrainFillingStatus","PhaseStemElongation","Sla","HaunIndex","ApexHeight","PlantHeight","PlantWidth","VitesseRacinaireDay","Kcl","KRolling","LIRkdfcl","LTRkdfcl","AssimPot","Assim","RespMaintTot","SupplyTot","AssimSurplus","AssimNotUsed","AssimNotUsedCum","IcMean","CstrMean","TillerDeathPop","PlantLeafNumTot","DeadLeafdrywtPop","LaiDead","ResCapacityInternodePop","InternodeResStatus","DayLength","Par","RgCalc","VDPCalc","TMoyCalc","HMoyCalc","KceReal","StockTotal","EauDispo","StockSurface","StockRac","RURac","Kcp","Kce","EvapPot","Evap","TrPot","Tr","Lr","Dr","ParIntercepte","SumDegresDay","VitesseRacinaire","CstrAssim","RayExtra","CumPAR","SumDegreDayCor","SumPP","SumDDPhasePrec","SeuilTemp","RespMaintDebt","TMinMoy","TMaxMoy","FtswMoy","ResInternodeMobiliDay","RootShootRatio","TrEffInst","TrEff","WueEt","WueTot","SlaMitch","FloodwaterDepth","IrrigAutoDay","IrrigTotDay","FractionPlantHeightSubmer","Density","RootMassPerVol","CumTr","CumEt","CumWUse","CumWReceived","CumIrrig","CumDr","CumLr","VolMacropores","StockMacropores","IcCum","CstrCum","SlaNew","LTRkdf","LIRkdf","RelPotLeafLength","ApexHeightGain","Haungain","Conversion","RootFrontOld","SimStartGermin","SimEmergence","SimStartPSP","SimPanIni","SimAnthesis50","SimStartMatu2","SimEndCycle","StressCold","CulmsPerPlantMax","CulmsPerHillMax","DurGermFlow","DurGermMat","LaiFin","CulmsPerHillFin","CulmsPerPlantFin","GrainYieldPopFin","DryMatAboveGroundPopFin","ReservePopFin","FractionRootsLogged","ResUtil","KcTot","DryMatAboveGroundTotPop","DemResInternodePop","LastLeafLengthPot","LastLeafLength","CumSupplyTot","DryMatResInternodePopOld","CumGrowthPop","GrowthPop","A_DemStructLeaf","A_GrowthStructLeaf","A_DemStructTot","A_GrowthStructTot","CoeffCO2Tr","CoeffCO2Assim","A_AssimSurplus","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CumCarbonUsedPop","DryMatTotPopFin","GrainFillingStatusFin","SterilityTotFin","CumIrrigFin","CumWUseFin","GrowthDryMatPop_V2_1"
                           , "Conversion", "MonCumTr"};
        names = n;
    } else if (LOG == COMPLETE) {
        vector <string> n {"ObsPlantDate","ApexHeight","ApexHeightGain","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","A_AssimSurplus","A_DemStructLeaf","A_DemStructTot","A_GrowthStructLeaf","A_GrowthStructTot","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CapaRDE","CapaREvap","CapaRFE","ChangeNurseryStatus","ChangePhase","ChangeSsPhase","CoeffCO2Assim","CoeffCO2Tr","CoeffStressLogging","Conversion","ConversionEff","CounterNursery","Cstr","CstrAssim","CstrCum","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CstrPhase6","CulmsPerHill","CulmsPerHillFin","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantFin","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumCstrPhase2","CumCstrPhase3","CumCstrPhase4","CumCstrPhase5","CumCstrPhase6","CumDr","CumEt","CumFTSWPhase2","CumFTSWPhase3","CumFTSWPhase4","CumFTSWPhase5","CumFTSWPhase6","CumGrowthPop","CumIcPhase2","CumIcPhase3","CumIcPhase4","CumIcPhase5","CumIcPhase6","CumIrrig","CumIrrigFin","CumLr","CumPAR","CumSupplyTot","CumTr","CumWReceived","CumWUse","CumWUseFin","DAF","DayLength","DeadLeafdrywtPop","Decli","DegresDuJour","DegresDuJourCor","DemLeafAreaPlant","DemPanicleFillPop","DemResInternodePop","DemStructInternodePlant","DemStructInternodePop","DemStructLeafPlant","DemStructLeafPop","DemStructPaniclePlant","DemStructPaniclePop","DemStructRootPlant","DemStructRootPop","DemStructSheathPop","DemStructTotPop","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundPopFin","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatResInternodePopOld","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatTotPopFin","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FloodwaterGain","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswMoy","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GainRootSystSoilSurfPop","GainRootSystVolPop","GrainFillingStatus","GrainFillingStatusFin","GrainYieldPanicle","GrainYieldPop","GrainYieldPopFin","GrowthDryMatPop","GrowthPop","GrowthResInternodePop","GrowthStructDeficit","GrowthStructInternodePop","GrowthStructLeafPop","GrowthStructPaniclePop","GrowthStructRootPop","GrowthStructSheathPop","GrowthStructTotPop","HarvestIndex","HaunGain","HaunIndex","HMoyCalc","Hum","Ic","IcCum","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","IncreaseResInternodePop","InternodeResStatus","Irrigation","IrrigAutoDay","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LaiFin","LastLeafLength","LastLeafLengthPot","LatRad","LeafDeathPop","LIRkdf","LIRkdfcl","Lr","LTRkdf","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NumSsPhase","NurseryStatus","PanicleFilDeficit","PanicleFilPop","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PhaseStemElongation","PlantHeight","PlantLeafNumNew","PlantLeafNumTot","PlantWidth","ProfRu","RayExtra","RelPotLeafLength","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","RgCalc","RgMax","RootFront","RootFrontOld","RootMassPerVol","RootShootRatio","RootSystSoilSurfPop","RootSystSoilSurfPopOld","RootSystVolPop","RootSystVolPopOld","RUE","RUEGreen","RuRac","RuSurf","SDJCorPhase4","SeuilCstrMortality","SeuilTemp","SeuilTempSsPhase","SimAnthesis50","SimEmergence","SimEndCycle","SimPanIni","SimStartGermin","SimStartMatu2","SimStartPSP","Sla","SlaMitch","SlaNew","SommeDegresJourMax","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","SterilityTotFin","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","StRuMax","SumDDPhasePrec","SumDegreDayCor","SumDegresDay","SumPP","SunDistance","SunPosi","SupplyTot","TempLai","TillerDeathPop","TmaxMoy","TminMoy","TMoyCalc","TMoyPrec","Tr","TrEff","TrEffInst","TrPot","ValRDE","ValRFE","ValRSurf","VPDCalc","VitesseRacinaire","VitesseRacinaireDay","VolMacropores","VolRelMacropores","WueEt","WueTot",
                           "TMax","TMin","TMoy","HMax","HMin","HMoy","Vt","Ins","Rg","ETP","Rain"};
        names = n;
    } else if (LOG == SMALL) {
        vector <string> n {"ObsPlantDate","NbJas","ApexHeight","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","CapaREvap","CapaRFE","Conversion","ConversionEff","Cstr","CstrAssim","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CumRain","CulmsPerHill","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumDr","CumEt","CumIrrig","CumLr","CumPAR","CumTr","CumWReceived","CumWUse","DayLength","DeadLeafdrywtPop","DegresDuJour","DegresDuJourCor","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETP","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GrainFillingStatus","GrainYieldPanicle","GrainYieldPop","HarvestIndex","HaunIndex","HMax","HMin","HMoy","Hum","Ic","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","Ins","InternodeResStatus","Irrigation","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LastLeafLength","LastLeafLengthPot","LeafDeathPop","LIRkdfcl","Lr","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NurseryStatus","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PlantHeight","PlantLeafNumTot","PlantWidth","Pluie","ProfRu","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","Rg","RgCalc","RgMax","RootFront","RootMassPerVol","RootShootRatio","RUE","RUEGreen","RuRac","RuSurf","Sla","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","SumDegreDayCor","SumDegresDay","SupplyTot","TillerDeathPop","TMax","TMin","TMoy","Tr","TrEff","TrEffInst","TrPot","VitesseRacinaire","VolMacropores","VolRelMacropores","VPDCalc","Vt","WueEt","WueTot"};
        names = n;
    }

    vector < vector <double> > results;
    vector <double> dayValues;
    for (int j = 0; j < currentResults.size(); ++j) {
        dayValues.push_back(j + DateDebutSimul);
    }
    results.push_back(dayValues);

    for (int i = 0; i < names.size()-1; ++i) {
        vector <double> values;
        for (int j = 0; j < currentResults.size(); ++j) {
            values.push_back(currentResults[j][i]);
        }
        results.push_back(values);
    }

    return pair <vector <string>, vector < vector <double> > > (names, results);
}


pair <vector <string>, vector < vector <double> > > run_samara_2_3(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_3();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    //    double DateDebutSimul = parameters->getDouble("datedebut");
    //    double DateFinSimul = parameters->getDouble("datefin");
    //    double DateSemis = parameters->getDouble("datesemis");
    double DateEnCours = DateDebutSimul;
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters(parameters);
    init_parcelle(VolMacropores, HumSat, HumFC, ResUtil
                  , ProfRu, EpaisseurSurf, EpaisseurProf
                  , RuSurf, CapaREvap, CapaRFE, CapaRDE
                  , StRuMax, StockIniProf, StockIniSurf
                  , ValRSurf, ValRDE, ValRFE
                  , StockSurface, StockTotal, Hum
                  , LTRkdfcl, LIRkdf, LTRkdf
                  , CounterNursery, VolRelMacropores);
    LatRad = Latitude * M_PI * 1.0 / 180;


    vector <  vector <double> > currentResults;

    bool crop = false;
    //Compute day before for TMoyPrec
//    set_meteo_vars(parameters, -1,
//                   TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
//                   Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
//    EToFao(ETP, Altitude, RgMax, RgCalc,
//           TMin, TMax,
//           HMin, HMax, HMoyCalc,
//           TMoyCalc, Vt, ETo,
//           TMoyPrec, VPDCalc);
    //

    double cumRain = 0;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        set_meteo_vars(parameters, DateEnCours-DateDebutSimul,
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc);
        NbJAS = DateEnCours - DateSemis;
        if(NumPhase > 0)
            cumRain += Pluie;
        //crop entity
        if(NbJAS == 0) {
            init_parcelle(VolMacropores, HumSat, HumFC, ResUtil
                          , ProfRu, EpaisseurSurf, EpaisseurProf
                          , RuSurf, CapaREvap, CapaRFE, CapaRDE
                          , StRuMax, StockIniProf, StockIniSurf
                          , ValRSurf, ValRDE, ValRFE
                          , StockSurface, StockTotal, Hum
                          , LTRkdfcl, LIRkdf, LTRkdf
                          , CounterNursery, VolRelMacropores);
            init_culture();
            crop = true;
        }
        //        else {
        if(crop) samara::RS_Transplanting_V2_2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                         CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                         DryMatStructPaniclePop, DryMatResInternodePop, DeadLeafdrywtPop, ResCapacityInternodePop);
        //        }


        eval_Par(DateEnCours);
        //        if(start++ < 2) ETP = 0;
        EToFao(ETP, Altitude, RgMax, RgCalc,
               TMin, TMax,
               HMin, HMax, HMoyCalc,
               TMoyCalc, Vt, ETo,
               TMoyPrec, VPDCalc);


        if(crop) samara::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                                               DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);

        /** Phyllo **/
        samara::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
        samara::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
        samara::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
        samara::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
        samara::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
        samara::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
        samara::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
        samara::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);
        /*if (crop) */samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
        samara::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
        samara::RS_EvalDAF_V2(NumPhase, DAF);
        samara::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
        samara::RS_EvolHauteur_SDJ_cstr_V2_1(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
                                                CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold, CstrMean, ApexHeightGain, ApexHeight, PlantHeight, PlantWidth);
        samara::RS_EvolKcpKceBilhy(LTRkdfcl, KcMax, Mulch, Kcp, Kce, KcTot);
        if(DateEnCours != DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        samara::RS_EvolEvapSurfRFE_RDE_V2_1(NumPhase, Kce, EvapPot, CapaREvap, CapaRDE, CapaRFE,
                                               RuRac, RuSurf, BundHeight, EpaisseurSurf, EpaisseurProf,
                                               RootFront, ResUtil, Evap, ValRSurf, ValRFE, ValRDE, StockRac,
                                               StockTotal, StockSurface, Kr, KceReal, FloodwaterDepth, StockMacropores);
        if(DateEnCours == DateDebutSimul) samara::RS_EvalEvapPot(ETo, Kce, EvapPot); //ADDED BY G.B. to debug
        /** CROP **/
        if (crop) samara::RS_EvalFTSW_V2(RuRac, StockTotal, StockMacropores, StRuMax, StockRac, FTSW);
        if (crop) samara::RS_EvalCstrPFactorFAO_V2(PFactor, FTSW, ETo, KcTot, StockMacropores, CoeffStressLogging, Cstr);
        if (crop) samara::DemandePlante_V2_1(Kcp, ETo, Ca, CO2Slopetr, TrPot, CoeffCO2Tr);
        if (crop) samara::EvalTranspi(TrPot, Cstr, Tr);
        if(!NumPhase == 0 && !(NumPhase == 1 && ChangePhase == 1)) samara::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
        if (crop) samara::RS_EvolConsRes_Flood_V2(NumPhase, RuRac, RuSurf, CapaREvap, Tr, Evap, CapaRDE, CapaRFE, EpaisseurSurf, EpaisseurProf, ResUtil, StockRac,
                                                     StockSurface, StockTotal, ValRFE, ValRDE, ValRSurf, FloodwaterDepth, StockMacropores);

        /*if (crop) */samara::RS_EvalTMaxMoy_V2_3(TMax, TMin, HMax, HMin, NumPhase, NumSsPhase, TmaxMoy);
        /*if (crop) */samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

        samara::RS_EvalFtswMoy(FTSW, NumPhase, NumSsPhase, FtswMoy);
        samara::RS_EvalSterility(NumPhase, ChangePhase, KCritSterCold1, KCritSterCold2, KCritSterHeat1, KCritSterHeat2, KCritSterFtsw1, KCritSterFtsw2, TminMoy,
                                    TmaxMoy, FtswMoy, SterilityCold, SterilityHeat, SterilityDrought, SterilityTot);
        samara::RS_EvalVitesseRacinaire(VRacLevee, VRacBVP, VRacRPR, VRacPSP, VRacMatu1, VRacMatu2, RootCstr, Cstr, NumPhase, DegresDuJourCor, VitesseRacinaire, VitesseRacinaireDay);

        if (crop) samara::EvalConversion(NumPhase, TxConversion, TxAssimBVP, SumDegresDay, SumDDPhasePrec, TxAssimMatu1, TxAssimMatu2, SeuilTemp, Conversion);

        samara::RS_EvalParIntercepte_V2_1(Par, Lai, Kdf, PARIntercepte, LIRkdfcl);
        if(crop)samara::RS_EvalAssimPot_V2_1(PARIntercepte, Par, Conversion, TMax, TMin, TBase, TOpt1, DayLength, StressCold, CO2Exp, Ca, CO2Cp, SlaMin, Sla, CoeffAssimSla,
                                                AssimPot, CoeffCO2Assim);

        if (crop) samara::RS_EvalCstrAssim(Cstr, ASScstr, CstrAssim);
        if (crop) samara::RS_EvalAssim(AssimPot, CstrAssim, Assim);

        samara::RS_TransplantingShock_V2(CounterNursery, CoeffTransplantingShock, Assim);
        samara::RS_EvalRespMaint_V2_2(KRespMaintLeaf, KRespMaintSheath, KRespMaintRoot, KRespInternode, KRespPanicle, DryMatStructLeafPop, DryMatStructSheathPop,
                                         DryMatStructRootPop, DryMatStructInternodePop, DryMatStructPaniclePop, TMoyCalc, KTempMaint, CoefficientQ10, Par, RespMaintTot);
        samara::RS_EvalRelPotLeafLength_V2_2(NumPhase, HaunIndex, RankLongestLeaf, RelPotLeafLength);
        samara::RS_EvolPlantTilNumTot_V2_2(NumPhase, ChangePhase, PlantsPerHill, TilAbility, Density, Ic, IcTillering, Cstr, HaunIndex, HaunCritTillering, LTRkdfcl,
                                              CulmsPerHill, CulmsPerPlant, CulmsPop);
        samara::RS_EvolPlantLeafNumTot(NumPhase, CulmsPerHill, HaunGain, PlantLeafNumNew, PlantLeafNumTot);
        samara::RS_EvolMobiliTillerDeath_V2_2(NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic, PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                                 CulmsPerHill, DryMatStructPaniclePop);
        samara::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        samara::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                         SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                                DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
        samara::RS_EvalDemandStructSheath(NumPhase, DemStructLeafPop, WtRatioLeafSheath, SlaMin, SlaMax, Sla, StressCold, DemStructSheathPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                              DemStructSheathPop, DryMatStructRootPop, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
                                              DemStructRootPop, RootSystSoilSurfPopOld, RootFrontOld, RootSystVolPopOld, DemStructRootPlant);
        samara::RS_EvalDemandStructIN_V2_1(PhaseStemElongation, ApexHeightGain, CulmsPerHill, CoeffInternodeMass, Density, Ic, ResCapacityInternodePop, DryMatResInternodePop,
                                              CoeffReserveSink, NumPhase, DemStructInternodePlant, DemStructInternodePop, DemResInternodePop);
        samara::RS_EvalDemandStructPanicle_V2(NumPhase, CoeffPanicleMass, CulmsPerHill, Ic, DryMatStructPaniclePop, Density, PanStructMassMax, StressCold, DemStructPaniclePlant,
                                                 PanStructMass, DemStructPaniclePop);
        samara::RS_EvalDemandTotAndIcPreFlow_V2_1(NumPhase, RespMaintTot, DemStructLeafPop, DemStructSheathPop, DemStructRootPop, DemStructInternodePop, DemStructPaniclePop,
                                                     SupplyTot, NbDaysSinceGermination, PlantHeight, Cstr, DemResInternodePop, DemStructTotPop, Ic, IcCum, IcMean, CstrCum, CstrMean,
                                                     A_DemStructTot);
        samara::RS_EvolGrowthStructLeafPop_V2_1(NumPhase, Ic, SupplyTot, DemStructLeafPop, DemStructTotPop, GrowthStructLeafPop, A_GrowthStructLeaf);
        samara::RS_EvolGrowthStructSheathPop(NumPhase, Ic, SupplyTot, DemStructSheathPop, DemStructTotPop, GrowthStructSheathPop);
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, GrowthStructRootPop);
        samara::RS_EvolGrowthStructINPop_V2_1(NumPhase, Ic, SupplyTot, DemStructInternodePop, DemStructTotPop, DemResInternodePop, GrowthStructInternodePop, GrowthResInternodePop);
        samara::RS_EvolGrowthStructPanPop(NumPhase, Ic, SupplyTot, DemStructPaniclePop, DemStructTotPop, GrowthStructPaniclePop);
        samara::RS_Priority2GrowthPanStrctPop_V2_1(PriorityPan, DemStructPaniclePop, NumPhase, GrowthStructTotPop, DemStructInternodePop, DemStructTotPop, DemStructLeafPop,
                                                      DemStructSheathPop, DemStructRootPop, DemResInternodePop, GrowthStructPaniclePop, GrowthStructInternodePop,
                                                      GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthResInternodePop);
        if(crop) samara::RS_EvolGrowthStructTot_V2_1(NumPhase, SupplyTot, GrowthResInternodePop, GrowthStructTotPop, AssimSurplus, GrowthStructLeafPop, GrowthStructSheathPop,
                                                        GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus);
        if(crop) samara::RS_AddResToGrowthStructPop_V2_1(NumPhase, Ic, PhaseStemElongation, DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
                                                            DemStructRootPop, DemStructInternodePop, DemStructPaniclePop, RelMobiliInternodeMax, GrowthResInternodePop,
                                                            ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
                                                            GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay, A_GrowthStructLeaf,
                                                            A_GrowthStructTot, A_ResInternodeMobiliDay);
        if(crop) samara::RS_EvolDemPanFilPopAndIcPFlow_V2_1(NumPhase, DryMatStructPaniclePop, CoeffPanSinkPop, SterilityTot, DegresDuJourCor, SDJMatu1, SupplyTot, Assim,
                                                               RespMaintTot, StressCold, PanicleSinkPop, DemPanicleFillPop, AssimSurplus, Ic, A_AssimSurplus);
        if(crop) samara::RS_EvolPanicleFilPop_V2_1(NumPhase, Ic, DryMatResInternodePop, DemPanicleFillPop, SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim,
                                                      ResInternodeMobiliDayPot, AssimSurplus, PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop, GrainYieldPop,
                                                      A_AssimSurplus, A_ResInternodeMobiliDay);

        if(crop) samara::RS_EvolGrowthReserveInternode_V2_1(NumPhase, PhaseStemElongation, DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
                                                               AssimSurplus, ResInternodeMobiliDay, ResCapacityInternodePop, IncreaseResInternodePop, DryMatResInternodePop,
                                                               AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, DryMatResInternodePopOld, A_IncreaseResInternodePop);

        if(crop) samara::RS_EvolGrowthTot_V2_1(NumPhase, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
                                                  GrowthResInternodePop, PanicleFilPop, DryMatResInternodePop, DryMatResInternodePopOld, GrowthStructTotPop, GrowthDryMatPop,
                                                  A_GrowthStructTot);
        if(crop)samara::RS_ExcessAssimilToRoot_V2(NumPhase, ExcessAssimToRoot, DryMatStructRootPop, RootSystVolPop, CoeffRootMassPerVolMax, RootMassPerVol, GrowthStructRootPop,
                                                     AssimNotUsed);
        if(crop) samara::RS_EvolDryMatTot_V2_1(NumPhase, ChangePhase, PlantsPerHill, TxResGrain, PoidsSecGrain, Density, GrowthStructLeafPop, GrowthStructSheathPop,
                                                  GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, GrowthResInternodePop, GrainYieldPop,
                                                  ResCapacityInternodePop, CulmsPerPlant, CoeffPanSinkPop, SterilityTot, DeadLeafdrywtPop, DryMatResInternodePopOld, PanicleFilPop,
                                                  AssimNotUsedCum, MobiliLeafDeath, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                                  DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                                  DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                                  SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                                  GrowthPop, CumCarbonUsedPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                   DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);

        if(crop)samara::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase,
                                                TempSLA, DryMatStructLeafPop, GrowthStructLeafPop, SlaMitch, SlaNew, Sla);
//        if(crop)samara::RS_EvalSlaMitch_V2_2(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase,
//                                             TempSLA, DryMatStructLeafPop, Par, PARCritSLA,
//                                             GrowthStructLeafPop, SlaMitch, SlaNew, Sla);

        samara::RS_EvalRuiss_FloodDyna_V2(NumPhase, Pluie, SeuilRuiss, PourcRuiss, BundHeight, Irrigation, PlantHeight, LifeSavingDrainage, PlotDrainageDAF,
                                             VolMacropores, SeuilRuiss, PercolationMax, DAF, StockMacropores, FloodwaterDepth, EauDispo, Lr);

        if(crop)samara::RS_AutomaticIrrigation_V2_1(NumPhase, IrrigAuto, IrrigAutoTarget, BundHeight, PlantHeight, Irrigation, PlotDrainageDAF, DAF, VolMacropores,
                                                       VolRelMacropores, Pluie, FTSWIrrig, IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus, PercolationMax, NbJAS,
                                                       RuSurf, ResUtil, RootFront, EpaisseurSurf, EpaisseurProf, ProfRacIni, FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
                                                       StockMacropores, EauDispo, RuRac, StockRac, FTSW, Lr);

        samara::RS_EvolRempliResRFE_RDE_V2(NumPhase, RuSurf, EauDispo, RuRac, CapaRFE, CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight, EpaisseurSurf,
                                              EpaisseurProf, VolMacropores, FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr, ValRDE, ValRFE, ValRSurf,
                                              FloodwaterGain, StockMacropores);

        if (crop) samara::RS_EvolWaterLoggingUpland_V2(PercolationMax, BundHeight, VolMacropores, Dr, Lr, StockMacropores);
        if (crop) samara::RS_EvalStressWaterLogging_V2(StockMacropores, VolMacropores, RootFront, EpaisseurSurf, EpaisseurProf, WaterLoggingSens, FractionRootsLogged,
                                                          CoeffStressLogging);

        samara::RS_EvolRempliMacropores_V2(NumPhase, EpaisseurSurf, EpaisseurProf, ResUtil, StockMacropores, RootFront, CapaRDE, CapaRFE, FloodwaterDepth,
                                              StockTotal, Hum, StockSurface, StockRac, ValRDE, ValRFE, ValRSurf);

        samara::RS_EvolRurRFE_RDE_V2_1(VitesseRacinaire, Hum, ResUtil, StockSurface, RuSurf, ProfRacIni, EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase,
                                          ChangePhase, FloodwaterDepth, StockMacropores, RootFrontMax, ChangeNurseryStatus, Transplanting, TransplantingDepth, RuRac,
                                          StockRac, StockTotal, FloodwaterGain, RootFront);
        samara::RS_PlantSubmergence_V2(PlantHeight, FloodwaterDepth, FractionPlantHeightSubmer);

        /*if (crop) */samara::RS_EvalRootFront(NumPhase, RuRac, ResUtil, RootFront);

        samara::RS_EvolPSPMVMD(NumPhase, ChangePhase, SumDegreDayCor, DegresDuJourCor, SeuilPP, PPCrit, DayLength, PPExp, SumPP, SumDDPhasePrec, SeuilTemp);

        if (crop) samara::EvolSomDegresJour(DegresDuJour, NumPhase, SumDegresDay);
        if (crop) samara::RS_EvolSomDegresJourCor(DegresDuJourCor, NumPhase, SumDegreDayCor);

        samara::RS_EvalRUE_V2_2(NumPhase, ChangePhase, PARIntercepte, DryMatTotPop, DeadLeafdrywtPop, DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
                                   Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot, Conversion, NbJAS, Transplanting, NurseryStatus, Density, DensityNursery,
                                   DryMatAboveGroundTotPop, DryMatAboveGroundPop, RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                   TrEff, WueEt, WueTot, ConversionEff, RUEGreen);

        samara::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);

        //Lodging modules

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                      GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                      DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                      CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                      IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                      CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                      ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);



        //                samara2_1::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

        if ( NumPhase >= 6 ) {
            GrowthPop = 0;
            GrowthDryMatPop = 0;
        }
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_3();
                //                CumWUseFin = cumRain;
                //                CumIrrigFin = CumDr;
            }
            DryMatResInternodePop = 0;
        }

        /** OUTPUT VECTORS **/
        vector <double> result;
        if(LOG == ECOTROP) {
            vector <double> r{NbJAS,NumPhase,DegresDuJour,DegresDuJourCor,Lai,MaxLai,FTSW,Cstr,RootFront,ETo,ETM,ETR,RUE,ConversionEff,Ic,CulmsPerPlant,CulmsPop,CulmsPerHill,GrainYieldPop,DryMatStructLeafPop,DryMatStructSheathPop,DryMatStructRootPop,DryMatStructInternodePop,DryMatResInternodePop,DryMatStructPaniclePop,DryMatStructTotPop,DryMatVegeTotPop,DryMatPanicleTotPop,DryMatStemPop,DryMatAboveGroundPop,DryMatTotPop,SterilityCold,SterilityHeat,SterilityDrought,SterilityTot,HarvestIndex,PanicleNumPop,PanicleNumPlant,GrainYieldPanicle,SpikeNumPop,SpikeNumPanicle,FertSpikeNumPop,GrainFillingStatus,PhaseStemElongation,Sla,HaunIndex,ApexHeight,PlantHeight,PlantWidth,VitesseRacinaireDay,Kcl,KRolling,LIRkdfcl,LTRkdfcl,AssimPot,Assim,RespMaintTot,SupplyTot,AssimSurplus,AssimNotUsed,AssimNotUsedCum,IcMean,CstrMean,TillerDeathPop,PlantLeafNumTot,DeadLeafdrywtPop,LaiDead,ResCapacityInternodePop,InternodeResStatus,DayLength,Par,RgCalc,VPDCalc,TMoyCalc,HMoyCalc,KceReal,StockTotal,EauDispo,StockSurface,StockRac,RuRac,Kcp,Kce,EvapPot,Evap,TrPot,Tr,Lr,Dr,PARIntercepte,SumDegresDay,VitesseRacinaire,CstrAssim,RayExtra,CumPAR,SumDegreDayCor,SumPP,SumDDPhasePrec,SeuilTemp,RespMaintDebt,TminMoy,TmaxMoy,FtswMoy,ResInternodeMobiliDay,RootShootRatio,TrEff,WueEt,WueTot,ConversionEff,SlaMitch,FloodwaterDepth,IrrigAutoDay,IrrigTotDay,FractionPlantHeightSubmer,Density,RootMassPerVol,CumEt,CumWUse,cumRain,CumWReceived,CumDr,CumLr,TrEffInst,VolMacropores,StockMacropores,IcCum,CstrCum,SlaNew,LTRkdf,LIRkdf,RelPotLeafLength,ApexHeightGain,HaunGain,Conversion,RootFrontOld,SimStartGermin,SimEmergence,SimStartPSP,SimPanIni,SimAnthesis50,SimStartMatu2,SimEndCycle,StressCold,CulmsPerPlantMax,CulmsPerHillMax,DurGermFlow,DurGermMat,LaiFin,CulmsPerHillFin,CulmsPerPlantFin,GrainYieldPopFin,DryMatAboveGroundPopFin,ReservePopFin,FractionRootsLogged,ResUtil,KcTot,DryMatAboveGroundTotPop,DemResInternodePop,LastLeafLengthPot,LastLeafLength,CumSupplyTot,DryMatResInternodePopOld,CumGrowthPop,GrowthPop,A_DemStructLeaf,A_GrowthStructLeaf,A_DemStructTot,A_GrowthStructTot,CoeffCO2Tr,CoeffCO2Assim,A_AssimSurplus,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CumCarbonUsedPop,DryMatTotPopFin,GrainFillingStatusFin,SterilityTotFin,CumIrrigFin,CumWUseFin,GrowthDryMatPop
                        , Conversion, CumTr};
            result = r;
        } else if (LOG == COMPLETE) {
            vector <double> r {NbJAS,ApexHeight,ApexHeightGain,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,A_AssimSurplus,A_DemStructLeaf,A_DemStructTot,A_GrowthStructLeaf,A_GrowthStructTot,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CapaRDE,CapaREvap,CapaRFE,ChangeNurseryStatus,ChangePhase,ChangeSsPhase,CoeffCO2Assim,CoeffCO2Tr,CoeffStressLogging,Conversion,ConversionEff,CounterNursery,Cstr,CstrAssim,CstrCum,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CulmsPerHillFin,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantFin,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumCstrPhase2,CumCstrPhase3,CumCstrPhase4,CumCstrPhase5,CumCstrPhase6,CumDr,CumEt,CumFTSWPhase2,CumFTSWPhase3,CumFTSWPhase4,CumFTSWPhase5,CumFTSWPhase6,CumGrowthPop,CumIcPhase2,CumIcPhase3,CumIcPhase4,CumIcPhase5,CumIcPhase6,CumIrrig,CumIrrigFin,CumLr,CumPAR,CumSupplyTot,CumTr,CumWReceived,CumWUse,CumWUseFin,DAF,DayLength,DeadLeafdrywtPop,Decli,DegresDuJour,DegresDuJourCor,DemLeafAreaPlant,DemPanicleFillPop,DemResInternodePop,DemStructInternodePlant,DemStructInternodePop,DemStructLeafPlant,DemStructLeafPop,DemStructPaniclePlant,DemStructPaniclePop,DemStructRootPlant,DemStructRootPop,DemStructSheathPop,DemStructTotPop,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundPopFin,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatResInternodePopOld,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatTotPopFin,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FloodwaterGain,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswMoy,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GainRootSystSoilSurfPop,GainRootSystVolPop,GrainFillingStatus,GrainFillingStatusFin,GrainYieldPanicle,GrainYieldPop,GrainYieldPopFin,GrowthDryMatPop,GrowthPop,GrowthResInternodePop,GrowthStructDeficit,GrowthStructInternodePop,GrowthStructLeafPop,GrowthStructPaniclePop,GrowthStructRootPop,GrowthStructSheathPop,GrowthStructTotPop,HarvestIndex,HaunGain,HaunIndex,HMoyCalc,Hum,Ic,IcCum,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,IncreaseResInternodePop,InternodeResStatus,Irrigation,IrrigAutoDay,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LaiFin,LastLeafLength,LastLeafLengthPot,LatRad,LeafDeathPop,LIRkdf,LIRkdfcl,Lr,LTRkdf,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NumSsPhase,NurseryStatus,PanicleFilDeficit,PanicleFilPop,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PhaseStemElongation,PlantHeight,PlantLeafNumNew,PlantLeafNumTot,PlantWidth,ProfRu,RayExtra,RelPotLeafLength,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,RgCalc,RgMax,RootFront,RootFrontOld,RootMassPerVol,RootShootRatio,RootSystSoilSurfPop,RootSystSoilSurfPopOld,RootSystVolPop,RootSystVolPopOld,RUE,RUEGreen,RuRac,RuSurf,SDJCorPhase4,SeuilCstrMortality,SeuilTemp,SeuilTempSsPhase,SimAnthesis50,SimEmergence,SimEndCycle,SimPanIni,SimStartGermin,SimStartMatu2,SimStartPSP,Sla,SlaMitch,SlaNew,SommeDegresJourMax,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,SterilityTotFin,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,StRuMax,SumDDPhasePrec,SumDegreDayCor,SumDegresDay,SumPP,SunDistance,SunPosi,SupplyTot,TempLai,TillerDeathPop,TmaxMoy,TminMoy,TMoyCalc,TMoyPrec,Tr,TrEff,TrEffInst,TrPot,ValRDE,ValRFE,ValRSurf,VPDCalc,VitesseRacinaire,VitesseRacinaireDay,VolMacropores,VolRelMacropores,WueEt,WueTot,
                        TMax,TMin,TMoy,HMax,HMin,HMoy,Vt,Ins,Rg,ETP,Pluie};
            result = r;
        } else if (LOG == SMALL) {
            vector <double> r {NbJAS,ApexHeight,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CapaREvap,CapaRFE,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,cumRain,CulmsPerHill,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,CumTr,CumWReceived,CumWUse,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainYieldPanicle,GrainYieldPop,HarvestIndex,HaunIndex,HMax,HMin,HMoy,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,Irrigation,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdfcl,Lr,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NurseryStatus,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantLeafNumTot,PlantWidth,Pluie,ProfRu,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,Rg,RgCalc,RgMax,RootFront,RootMassPerVol,RootShootRatio,RUE,RUEGreen,RuRac,RuSurf,Sla,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,SumDegreDayCor,SumDegresDay,SupplyTot,TillerDeathPop,TMax,TMin,TMoy,Tr,TrEff,TrEffInst,TrPot,VitesseRacinaire,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot};
            result = r;
        }

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
        FractionRootsLogged = 0;
    }


    /** OUTPUT VECTORS **/
    vector <string> names;
    if(LOG == ECOTROP) {
        vector <string> n {"Jour","NbJas","NumPhase","DegresDuJour","DegresDuJourCor","Lai","MaxLai","FTSW","Cstr","RootFront","Eto","ETM","ETR","RUE","ConversionEff","Ic","CulmsPerPlant","CulmsPop","CulmsPerHill","GrainYieldPop","DryMatStructLeafPop","DryMatStructSheathPop","DryMatStructRootPop","DryMatStructInternodePop","DryMatResInternodePop","DryMatStructPaniclePop","DryMatStructTotPop","DryMatVegeTotPop","DryMatPanicleTotPop","DryMatStemPop","DryMatAboveGroundPop","DryMatTotPop","SterilityCold","SterilityHeat","SterilityDrought","SterilityTot","HarvestIndex","PanicleNumPop","PanicleNumPlant","GrainYieldPanicle","SpikeNumPop","SpikeNumPanicle","FertSpikeNumPop","GrainFillingStatus","PhaseStemElongation","Sla","HaunIndex","ApexHeight","PlantHeight","PlantWidth","VitesseRacinaireDay","Kcl","KRolling","LIRkdfcl","LTRkdfcl","AssimPot","Assim","RespMaintTot","SupplyTot","AssimSurplus","AssimNotUsed","AssimNotUsedCum","IcMean","CstrMean","TillerDeathPop","PlantLeafNumTot","DeadLeafdrywtPop","LaiDead","ResCapacityInternodePop","InternodeResStatus","DayLength","Par","RgCalc","VDPCalc","TMoyCalc","HMoyCalc","KceReal","StockTotal","EauDispo","StockSurface","StockRac","RURac","Kcp","Kce","EvapPot","Evap","TrPot","Tr","Lr","Dr","ParIntercepte","SumDegresDay","VitesseRacinaire","CstrAssim","RayExtra","CumPAR","SumDegreDayCor","SumPP","SumDDPhasePrec","SeuilTemp","RespMaintDebt","TMinMoy","TMaxMoy","FtswMoy","ResInternodeMobiliDay","RootShootRatio","TrEffInst","TrEff","WueEt","WueTot","SlaMitch","FloodwaterDepth","IrrigAutoDay","IrrigTotDay","FractionPlantHeightSubmer","Density","RootMassPerVol","CumTr","CumEt","CumWUse","CumWReceived","CumIrrig","CumDr","CumLr","VolMacropores","StockMacropores","IcCum","CstrCum","SlaNew","LTRkdf","LIRkdf","RelPotLeafLength","ApexHeightGain","Haungain","Conversion","RootFrontOld","SimStartGermin","SimEmergence","SimStartPSP","SimPanIni","SimAnthesis50","SimStartMatu2","SimEndCycle","StressCold","CulmsPerPlantMax","CulmsPerHillMax","DurGermFlow","DurGermMat","LaiFin","CulmsPerHillFin","CulmsPerPlantFin","GrainYieldPopFin","DryMatAboveGroundPopFin","ReservePopFin","FractionRootsLogged","ResUtil","KcTot","DryMatAboveGroundTotPop","DemResInternodePop","LastLeafLengthPot","LastLeafLength","CumSupplyTot","DryMatResInternodePopOld","CumGrowthPop","GrowthPop","A_DemStructLeaf","A_GrowthStructLeaf","A_DemStructTot","A_GrowthStructTot","CoeffCO2Tr","CoeffCO2Assim","A_AssimSurplus","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CumCarbonUsedPop","DryMatTotPopFin","GrainFillingStatusFin","SterilityTotFin","CumIrrigFin","CumWUseFin","GrowthDryMatPop_V2_1"
                           , "Conversion", "MonCumTr"};
        names = n;
    } else if (LOG == COMPLETE) {
        vector <string> n {"ObsPlantDate","NbJas","ApexHeight","ApexHeightGain","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","A_AssimSurplus","A_DemStructLeaf","A_DemStructTot","A_GrowthStructLeaf","A_GrowthStructTot","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CapaRDE","CapaREvap","CapaRFE","ChangeNurseryStatus","ChangePhase","ChangeSsPhase","CoeffCO2Assim","CoeffCO2Tr","CoeffStressLogging","Conversion","ConversionEff","CounterNursery","Cstr","CstrAssim","CstrCum","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CstrPhase6","CulmsPerHill","CulmsPerHillFin","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantFin","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumCstrPhase2","CumCstrPhase3","CumCstrPhase4","CumCstrPhase5","CumCstrPhase6","CumDr","CumEt","CumFTSWPhase2","CumFTSWPhase3","CumFTSWPhase4","CumFTSWPhase5","CumFTSWPhase6","CumGrowthPop","CumIcPhase2","CumIcPhase3","CumIcPhase4","CumIcPhase5","CumIcPhase6","CumIrrig","CumIrrigFin","CumLr","CumPAR","CumSupplyTot","CumTr","CumWReceived","CumWUse","CumWUseFin","DAF","DayLength","DeadLeafdrywtPop","Decli","DegresDuJour","DegresDuJourCor","DemLeafAreaPlant","DemPanicleFillPop","DemResInternodePop","DemStructInternodePlant","DemStructInternodePop","DemStructLeafPlant","DemStructLeafPop","DemStructPaniclePlant","DemStructPaniclePop","DemStructRootPlant","DemStructRootPop","DemStructSheathPop","DemStructTotPop","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundPopFin","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatResInternodePopOld","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatTotPopFin","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FloodwaterGain","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswMoy","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GainRootSystSoilSurfPop","GainRootSystVolPop","GrainFillingStatus","GrainFillingStatusFin","GrainYieldPanicle","GrainYieldPop","GrainYieldPopFin","GrowthDryMatPop","GrowthPop","GrowthResInternodePop","GrowthStructDeficit","GrowthStructInternodePop","GrowthStructLeafPop","GrowthStructPaniclePop","GrowthStructRootPop","GrowthStructSheathPop","GrowthStructTotPop","HarvestIndex","HaunGain","HaunIndex","HMoyCalc","Hum","Ic","IcCum","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","IncreaseResInternodePop","InternodeResStatus","Irrigation","IrrigAutoDay","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LaiFin","LastLeafLength","LastLeafLengthPot","LatRad","LeafDeathPop","LIRkdf","LIRkdfcl","Lr","LTRkdf","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NumSsPhase","NurseryStatus","PanicleFilDeficit","PanicleFilPop","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PhaseStemElongation","PlantHeight","PlantLeafNumNew","PlantLeafNumTot","PlantWidth","ProfRu","RayExtra","RelPotLeafLength","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","RgCalc","RgMax","RootFront","RootFrontOld","RootMassPerVol","RootShootRatio","RootSystSoilSurfPop","RootSystSoilSurfPopOld","RootSystVolPop","RootSystVolPopOld","RUE","RUEGreen","RuRac","RuSurf","SDJCorPhase4","SeuilCstrMortality","SeuilTemp","SeuilTempSsPhase","SimAnthesis50","SimEmergence","SimEndCycle","SimPanIni","SimStartGermin","SimStartMatu2","SimStartPSP","Sla","SlaMitch","SlaNew","SommeDegresJourMax","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","SterilityTotFin","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","StRuMax","SumDDPhasePrec","SumDegreDayCor","SumDegresDay","SumPP","SunDistance","SunPosi","SupplyTot","TempLai","TillerDeathPop","TmaxMoy","TminMoy","TMoyCalc","TMoyPrec","Tr","TrEff","TrEffInst","TrPot","ValRDE","ValRFE","ValRSurf","VPDCalc","VitesseRacinaire","VitesseRacinaireDay","VolMacropores","VolRelMacropores","WueEt","WueTot",
                           "TMax","TMin","TMoy","HMax","HMin","HMoy","Vt","Ins","Rg","ETP","Rain"};
        names = n;
    } else if (LOG == SMALL) {
        vector <string> n {"ObsPlantDate","NbJas","ApexHeight","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","CapaREvap","CapaRFE","Conversion","ConversionEff","Cstr","CstrAssim","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CumRain","CulmsPerHill","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumDr","CumEt","CumIrrig","CumLr","CumPAR","CumTr","CumWReceived","CumWUse","DayLength","DeadLeafdrywtPop","DegresDuJour","DegresDuJourCor","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStemPop","DryMatStructTotPop","DryMatTotPop","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETP","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GrainFillingStatus","GrainYieldPanicle","GrainYieldPop","HarvestIndex","HaunIndex","HMax","HMin","HMoy","Hum","Ic","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","Ins","InternodeResStatus","Irrigation","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LastLeafLength","LastLeafLengthPot","LeafDeathPop","LIRkdfcl","Lr","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NurseryStatus","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PlantHeight","PlantLeafNumTot","PlantWidth","Pluie","ProfRu","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","Rg","RgCalc","RgMax","RootFront","RootMassPerVol","RootShootRatio","RUE","RUEGreen","RuRac","RuSurf","Sla","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","SumDegreDayCor","SumDegresDay","SupplyTot","TillerDeathPop","TMax","TMin","TMoy","Tr","TrEff","TrEffInst","TrPot","VitesseRacinaire","VolMacropores","VolRelMacropores","VPDCalc","Vt","WueEt","WueTot"};
        names = n;
    }

    vector < vector <double> > results;
    vector <double> stepValues;
    vector <double> dayValues;
    for (int j = 0; j < currentResults.size(); ++j) {
        dayValues.push_back(j + DateDebutSimul);
        stepValues.push_back(j);
    }
    results.push_back(dayValues);

    for (int i = 0; i < names.size()-1; ++i) {
        vector <double> values;
        for (int j = 0; j < currentResults.size(); ++j) {
            values.push_back(currentResults[j][i]);
        }
        results.push_back(values);
    }

    return pair <vector <string>, vector < vector <double> > > (names, results);
}

void init_parameters(SamaraParameters * params) {
    SamaraParameters parameters = *params;
    Altitude = parameters.getDouble("wsalt");
    ASScstr = parameters.getDouble("asscstr");
    AttenMitch = parameters.getDouble("attenmitch");
    BundHeight = parameters.getDouble("bundheight");
    Ca = parameters.getDouble("ca");
    CO2Cp = parameters.getDouble("co2cp");
    CO2Exp = parameters.getDouble("co2exp");
    CO2Slopetr = parameters.getDouble("co2slopetr");
    CoeffAssimSla = parameters.getDouble("coeffassimsla");
    CoefficientQ10 = parameters.getDouble("coefficientq10");
    CoeffInternodeMass = parameters.getDouble("coeffinternodemass");
    CoeffInternodeNum = parameters.getDouble("coeffinternodenum");
    CoeffLeafDeath = parameters.getDouble("coeffleafdeath");
    CoeffLeafWLRatio = parameters.getDouble("coeffleafwlratio");
    CoeffPanicleMass = parameters.getDouble("coeffpaniclemass");
    CoeffPanSinkPop = parameters.getDouble("coeffpansinkpop");
    CoeffResCapacityInternode = parameters.getDouble("coeffrescapacityinternode");
    CoeffReserveSink = parameters.getDouble("coeffreservesink");
    CoeffRootMassPerVolMax = parameters.getDouble("coeffrootmasspervolmax");
    CoeffTillerDeath = parameters.getDouble("coefftillerdeath");
    CoeffTransplantingShock = parameters.getDouble("coefftransplantingshock");
    DensityField = parameters.getDouble("densityfield");
    DensityNursery = parameters.getDouble("densitynursery");
    DEVcstr = parameters.getDouble("devcstr");
    DurationNursery = parameters.getDouble("durationnursery");
    EpaisseurProf = parameters.getDouble("epaisseurprof");
    EpaisseurSurf = parameters.getDouble("epaisseursurf");
    ExcessAssimToRoot = parameters.getDouble("excessassimtoroot");
    FTSWIrrig = parameters.getDouble("ftswirrig");
    HaunCritTillering = parameters.getDouble("hauncrittillering");
    HumFC = parameters.getDouble("humfc");
    HumPF = parameters.getDouble("humpf");
    HumSat = parameters.getDouble("humsat");
    IcTillering = parameters.getDouble("ictillering");
    InternodeLengthMax = parameters.getDouble("internodelengthmax");
    IrrigAuto = parameters.getDouble("irrigauto");
    IrrigAutoResume = parameters.getDouble("irrigautoresume");
    IrrigAutoStop = parameters.getDouble("irrigautostop");
    IrrigAutoTarget = parameters.getDouble("irrigautotarget");
    KcMax = parameters.getDouble("kcmax");
    KCritSterCold1 = parameters.getDouble("kcritstercold1");
    KCritSterCold2 = parameters.getDouble("kcritstercold2");
    KCritSterFtsw1 = parameters.getDouble("kcritsterftsw1");
    KCritSterFtsw2 = parameters.getDouble("kcritsterftsw2");
    KCritSterHeat1 = parameters.getDouble("kcritsterheat1");
    KCritSterHeat2 = parameters.getDouble("kcritsterheat2");
    KCritStressCold1 = parameters.getDouble("kcritstresscold1");
    KCritStressCold2 = parameters.getDouble("kcritstresscold2");
    Kdf = parameters.getDouble("kdf");
    KPar = parameters.getDouble ("kpar");
    KRespInternode = parameters.getDouble("krespinternode");
    KRespMaintLeaf = parameters.getDouble("krespmaintleaf");
    KRespMaintRoot = parameters.getDouble("krespmaintroot");
    KRespMaintSheath = parameters.getDouble("krespmaintsheath");
    KRespPanicle = parameters.getDouble("kresppanicle");
    KTempMaint = parameters.getDouble("ktempmaint");
    Latitude = parameters.getDouble("wslat");
    LeafLengthMax = parameters.getDouble("leaflengthmax");
    LifeSavingDrainage = parameters.getDouble("lifesavingdrainage");
    Mulch = parameters.getDouble("mulch");
    PanStructMassMax = parameters.getDouble("panstructmassmax");
    PARCritSLA = parameters.getDouble("parcritsla");
    PercolationMax = parameters.getDouble("percolationmax");
    PEvap = parameters.getDouble("pevap");
    PFactor = parameters.getDouble("pfactor");
    Phyllo = parameters.getDouble("phyllo");
    PlantsPerHill = parameters.getDouble("plantsperhill");
    PlotDrainageDAF = parameters.getDouble("plotdrainagedaf");
    PoidsSecGrain = parameters.getDouble("poidssecgrain");
    PourcRuiss = parameters.getDouble("pourcruiss");
    PPCrit = parameters.getDouble("ppcrit");
    PPExp = parameters.getDouble("ppexp");
    PPSens = parameters.getDouble("ppsens");
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
    StockIniProf = parameters.getDouble("stockiniprof");
    StockIniSurf = parameters.getDouble("stockinisurf");
    TBase = parameters.getDouble("tbase");
    TempSLA = parameters.getDouble("tempsla");
    TilAbility = parameters.getDouble("tilability");
    TLim = parameters.getDouble("tlim");
    TOpt1 = parameters.getDouble("topt1");
    TOpt2 = parameters.getDouble("topt2");
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
}

void init_parameters_2_1_micha(SamaraParameters * params) {
    SamaraParameters parameters = *params;
    Altitude = parameters.getDouble("wsalt");
    ASScstr = parameters.getDouble("asscstr");
    AttenMitch = parameters.getDouble("attenmitch");
    BundHeight = parameters.getDouble("bundheight");
    Ca = parameters.getDouble("ca");
    CO2Cp = parameters.getDouble("co2cp");
    CO2Exp = parameters.getDouble("co2exp");
    CO2Slopetr = parameters.getDouble("co2slopetr");
    CoeffAssimSla = parameters.getDouble("coeffassimsla");
    CoefficientQ10 = parameters.getDouble("coefficientq10");
    CoeffInternodeMass = parameters.getDouble("coeffinternodemass");
    CoeffInternodeNum = parameters.getDouble("coeffinternodenum");
    CoeffLeafDeath = parameters.getDouble("coeffleafdeath");
    CoeffLeafWLRatio = parameters.getDouble("coeffleafwlratio");
    CoeffPanicleMass = parameters.getDouble("coeffpaniclemass");
    CoeffPanSinkPop = parameters.getDouble("coeffpansinkpop");
    CoeffResCapacityInternode = parameters.getDouble("coeffrescapacityinternode");
    CoeffReserveSink = parameters.getDouble("coeffreservesink");
    CoeffRootMassPerVolMax = parameters.getDouble("coeffrootmasspervolmax");
    CoeffTillerDeath = parameters.getDouble("coefftillerdeath");
    CoeffTransplantingShock = parameters.getDouble("coefftransplantingshock");
    DensityField = parameters.getDouble("densityfield");
    DensityNursery = parameters.getDouble("densitynursery");
    DEVcstr = parameters.getDouble("devcstr");
    DurationNursery = parameters.getDouble("durationnursery");
    EpaisseurProf = parameters.getDouble("epaisseurprof");
    EpaisseurSurf = parameters.getDouble("epaisseursurf");
    ExcessAssimToRoot = parameters.getDouble("excessassimtoroot");
    FTSWIrrig = parameters.getDouble("ftswirrig");
    HaunCritTillering = parameters.getDouble("hauncrittillering");
    HumFC = parameters.getDouble("humfc");
    HumPF = parameters.getDouble("humpf");
    HumSat = parameters.getDouble("humsat");
    IcTillering = parameters.getDouble("ictillering");
    InternodeLengthMax = parameters.getDouble("internodelengthmax");
    IrrigAuto = parameters.getDouble("irrigauto");
    IrrigAutoResume = parameters.getDouble("irrigautoresume");
    IrrigAutoStop = parameters.getDouble("irrigautostop");
    IrrigAutoTarget = parameters.getDouble("irrigautotarget");
    KcMax = parameters.getDouble("kcmax");
    KCritSterCold1 = parameters.getDouble("kcritstercold1");
    KCritSterCold2 = parameters.getDouble("kcritstercold2");
    KCritSterFtsw1 = parameters.getDouble("kcritsterftsw1");
    KCritSterFtsw2 = parameters.getDouble("kcritsterftsw2");
    KCritSterHeat1 = parameters.getDouble("kcritsterheat1");
    KCritSterHeat2 = parameters.getDouble("kcritsterheat2");
    KCritStressCold1 = parameters.getDouble("kcritstresscold1");
    KCritStressCold2 = parameters.getDouble("kcritstresscold2");
    Kdf = parameters.getDouble("kdf");
    KPar = parameters.getDouble ("kpar");
    KRespInternode = parameters.getDouble("krespinternode");
    KRespMaintLeaf = parameters.getDouble("krespmaintleaf");
    KRespMaintRoot = parameters.getDouble("krespmaintroot");
    KRespMaintSheath = parameters.getDouble("krespmaintsheath");
    KRespPanicle = parameters.getDouble("kresppanicle");
    KTempMaint = parameters.getDouble("ktempmaint");
    Latitude = parameters.getDouble("wslat");
    LeafLengthMax = parameters.getDouble("leaflengthmax");
    LifeSavingDrainage = parameters.getDouble("lifesavingdrainage");
    Mulch = parameters.getDouble("mulch");
    PanStructMassMax = parameters.getDouble("panstructmassmax");
    PARCritSLA = parameters.getDouble("parcritsla");
    PercolationMax = parameters.getDouble("percolationmax");
    PEvap = parameters.getDouble("pevap");
    PFactor = parameters.getDouble("pfactor");
    Phyllo = parameters.getDouble("phyllo");
    PlantsPerHill = parameters.getDouble("plantsperhill");
    PlotDrainageDAF = parameters.getDouble("plotdrainagedaf");
    PoidsSecGrain = parameters.getDouble("poidssecgrain");
    PourcRuiss = parameters.getDouble("pourcruiss");
    PPCrit = parameters.getDouble("ppcrit");
    PPExp = parameters.getDouble("ppexp");
    PPSens = parameters.getDouble("ppsens");
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
    StockIniProf = parameters.getDouble("stockiniprof");
    StockIniSurf = parameters.getDouble("stockinisurf");
    TBase = parameters.getDouble("tbase");
    TempSLA = parameters.getDouble("tempsla");
    TilAbility = parameters.getDouble("tilability");
    TLim = parameters.getDouble("tlim");
    TOpt1 = parameters.getDouble("topt1");
    TOpt2 = parameters.getDouble("topt2");
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
    StockIniSurf = 0;
}

void reset_variables_2_1() {
    ConversionEff = 0;
    CstrMean = 0;
    IcMean = 0;
    CulmsPerPlant = 0;
    CulmsPerHill = 0;
    CulmsPop = 0;
    GrainYieldPop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStructRootPop = 0;
    DryMatStructInternodePop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatVegeTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatAboveGroundPop = 0;
    DryMatTotPop = 0;
    HarvestIndex = 0;
    PanicleNumPop = 0;
    PanicleNumPlant = 0;
    GrainYieldPanicle = 0;
    SpikeNumPop = 0;
    SpikeNumPanicle = 0;
    FertSpikeNumPop = 0;
    GrainFillingStatus = 0;
    PhaseStemElongation = 0;
    Sla = 0;
    HaunIndex = 0;
    ApexHeight = 0;
    PlantHeight = 0;
    PlantWidth = 0;
    VitesseRacinaireDay = 0;
    Kcl = 0;
    KRolling = 0;
    LIRkdfcl = 0;
    LTRkdfcl = 1;
    AssimPot = 0;
    Assim = 0;
    RespMaintTot = 0;
    SupplyTot = 0;
    AssimSurplus = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    TillerDeathPop = 0;
    DeadLeafdrywtPop = 0;
    ResCapacityInternodePop = 0;
    InternodeResStatus = 0;
    Cstr = 0;
    FTSW = 0;
    DryMatAboveGroundTotPop = 0;
}

void reset_variables_2_1_micha() {
    ConversionEff = 0;
    CstrMean = 0;
    IcMean = 0;
    CulmsPerPlant = 0;
    CulmsPerHill = 0;
    CulmsPop = 0;
    GrainYieldPop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStructRootPop = 0;
    DryMatStructInternodePop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatVegeTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatAboveGroundPop = 0;
    DryMatTotPop = 0;
    HarvestIndex = 0;
    PanicleNumPop = 0;
    PanicleNumPlant = 0;
    GrainYieldPanicle = 0;
    SpikeNumPop = 0;
    SpikeNumPanicle = 0;
    FertSpikeNumPop = 0;
    GrainFillingStatus = 0;
    PhaseStemElongation = 0;
    Sla = 0;
    HaunIndex = 0;
    ApexHeight = 0;
    PlantHeight = 0;
    PlantWidth = 0;
    VitesseRacinaireDay = 0;
    Kcl = 0;
    KRolling = 0;
    LIRkdfcl = 0;
    LTRkdfcl = 1;
    AssimPot = 0;
    Assim = 0;
    RespMaintTot = 0;
    SupplyTot = 0;
    AssimSurplus = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    TillerDeathPop = 0;
    DeadLeafdrywtPop = 0;
    ResCapacityInternodePop = 0;
    InternodeResStatus = 0;
    Cstr = 0;
    FTSW = 0;
    DryMatAboveGroundTotPop = 0;
}

void reset_variables_2_3() {
    ConversionEff = 0;
    CstrMean = 0;
    IcMean = 0;
    CulmsPerPlant = 0;
    CulmsPerHill = 0;
    CulmsPop = 0;
    GrainYieldPop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStructRootPop = 0;
    DryMatStructInternodePop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatVegeTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatAboveGroundPop = 0;
    DryMatTotPop = 0;
    HarvestIndex = 0;
    PanicleNumPop = 0;
    PanicleNumPlant = 0;
    GrainYieldPanicle = 0;
    SpikeNumPop = 0;
    SpikeNumPanicle = 0;
    FertSpikeNumPop = 0;
    GrainFillingStatus = 0;
    PhaseStemElongation = 0;
    Sla = 0;
    HaunIndex = 0;
    ApexHeight = 0;
    PlantHeight = 0;
    PlantWidth = 0;
    VitesseRacinaireDay = 0;
    Kcl = 0;
    KRolling = 0;
    LIRkdfcl = 0;
    LTRkdfcl = 1;
    AssimPot = 0;
    Assim = 0;
    RespMaintTot = 0;
    SupplyTot = 0;
    AssimSurplus = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    TillerDeathPop = 0;
    DeadLeafdrywtPop = 0;
    ResCapacityInternodePop = 0;
    InternodeResStatus = 0;
    Cstr = 0;
    FTSW = 0;
    DryMatAboveGroundTotPop = 0;
    LaiDead = 0;
}


void set_meteo_vars(SamaraParameters * parameters, double t, double &TMax, double &TMin, double &TMoy
                    , double &HMax, double &HMin, double &HMoy
                    , double &Vt, double &Ins, double &Rg, double &ETP
                    , double &Pluie, double & TMoyCalc, double & HMoyCalc) {
    TMax = parameters->getClimate(t).TMax;
    TMin = parameters->getClimate(t).TMin;
    TMoy = parameters->getClimate(t).TMoy; //local
    HMax = parameters->getClimate(t).HMax;
    HMin = parameters->getClimate(t).HMin;
    HMoy = parameters->getClimate(t).HMoy;//local
    Vt = parameters->getClimate(t).Vt;
    Ins = parameters->getClimate(t).Ins;
    Rg = parameters->getClimate(t).Rg;
    ETP = parameters->getClimate(t).ETP;
    Pluie = parameters->getClimate(t).Rain;
    if (((TMin != NilValue) && (TMax != NilValue))) {
        TMoyCalc = (TMax + TMin) * 1.0 / 2;
    } else {
        TMoyCalc = TMoy;
    }
    if (((HMin != NilValue) && (HMax != NilValue))) {
        HMoyCalc = (HMax + HMin) * 1.0 / 2;
    } else {
        HMoyCalc = HMoy;
    }
}


void init_parcelle(double &VolMacropores, double &HumSat, double &HumFC, double &ResUtil
                   , double &ProfRu, double &EpaisseurSurf, double &EpaisseurProf
                   , double &RuSurf, double &CapaREvap, double &CapaRFE, double &CapaRDE
                   , double &StRuMax, double &StockIniProf, double &StockIniSurf
                   , double &ValRSurf, double &ValRDE, double &ValRFE
                   , double &StockSurface, double &StockTotal, double &Hum
                   , double &LTRkdfcl, double &LIRkdf, double &LTRkdf
                   , double &CounterNursery, double &VolRelMacropores) {
    try {
        double Stockini2;
        double Stockini1;
        VolRelMacropores = 100 * (HumSat - HumFC);
        ResUtil = (HumFC - HumPF) * 1000;
        ProfRu = EpaisseurSurf + EpaisseurProf;
        // à supprimer ultérieurement dans les différents modules
        RuSurf = ResUtil * EpaisseurSurf * 1.0 / 1000;
        CapaREvap = 0.5 * EpaisseurSurf * HumPF;
        CapaRFE = PEvap * (CapaREvap + RuSurf);
        CapaRDE = RuSurf - CapaRFE;
        StRuMax = ResUtil * ProfRu * 1.0 / 1000;
        Stockini1 = min(StockIniSurf, CapaREvap + RuSurf);
        Stockini2 = min(StockIniProf, ResUtil * EpaisseurProf * 1.0 / 1000);
        ValRSurf = min(Stockini1, CapaREvap + CapaRDE);
        ValRDE = max(0., ValRSurf - CapaREvap);
        ValRFE = max(0., Stockini1 - (CapaREvap + CapaRDE));
        StockSurface = ValRDE + ValRFE;
        StockTotal = StockSurface + Stockini2; //transpirable
        Hum = StockTotal;
        LTRkdfcl = 1;
        LIRkdf = 0;
        LTRkdf = 0;
        CounterNursery = 0;
        VolMacropores = VolRelMacropores * (EpaisseurSurf + EpaisseurProf) * 1.0 / 100;

    } catch (...) {
        error_message("RS_InitParcelle_V2", URisocas);
    }
}

void eval_Par(double t) {
    try {
        double tDay = JulianCalculator::dayNumber(t);
        Decli = 0.409 * sin(0.0172 * tDay - 1.39);
        SunPosi = acos(-tan(LatRad) * tan(Decli));
        DayLength = 7.64 * SunPosi;
        SunDistance = 1 + 0.033 * cos(2 * M_PI * 1.0 / 365 * tDay);
        RayExtra = 24 * 60 * 0.0820 * 1.0 / M_PI * SunDistance *
                (SunPosi * sin(Decli) * sin(LatRad) +
                 cos(Decli) * cos(LatRad) * sin(SunPosi));
        RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;

        if ((Rg == NilValue)) {
            RgCalc = (0.25 + 0.50 * std::min(Ins * 1.0 / DayLength, 1.)) * RayExtra;
        } else {
            RgCalc = Rg;
        }

        Par = KPar * RgCalc;
    } catch (...) {
        error_message("EvalRg", UMeteo);
    }
}


void EToFao(double const &ETP, double const &Alt, double const &RgMax, double const &RgCalc,
            double const &TMin, double const &TMax,
            double const &HMin, double const &HMax, double const &HMoyCalc,
            double const &TMoyCalc, double const &Vt, double &ETo,
            double &TMoyPrec, double &VPDCalc) {
    try {
        if(Vt == 0) {
            VPDCalc = VPDCalc;
        }
        //        if ((ETP == NilValue)) {
        double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;
        eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
                         exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
        if ((HMax == NilValue))
            eActual = eSat * HMoyCalc * 1.0 / 100;
        else
            eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
                                HMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
                                HMax * 1.0 / 100);

        VPDCalc = eSat - eActual;
        RgRgMax = RgCalc * 1.0 / RgMax;
        if ((RgRgMax > 1))
            RgRgMax = 1;
        Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
                (0.34 - 0.14 * std::pow(eActual, 0.5)) *
                (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10, -9);

        // chaleur latente de vaporisation de l'eau
        TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoyCalc;
        //  pente de la courbe de pression de vapeur saturante en kPa/°C
        delta = 4098 * (0.6108 * exp(17.27 * TMoyCalc * 1.0 / (TMoyCalc + 237.3))) * 1.0 / std::pow(TMoyCalc
                                                                                                    + 237.3, 2);
        // constante psychrométrique en kPa/°C
        KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) * 1.0 / TLat;
        // Radiative
        G = 0.38 * (TMoyCalc - TMoyPrec);
        Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vt));
        // Partie évaporative de ET0 = Eaéro
        Eaero = (900 * 1.0 / (TMoyCalc + 273.16)) * ((eSat - eActual) * Vt) * KPsy * 1.0 /
                (delta + KPsy * (1 + 0.34 * Vt));
        ETo = Erad + Eaero;
        //        } else {
        //            ETo = ETP;
        //        }
        TMoyPrec = TMoyCalc;

    } catch (...) {
        error_message("EToFAO", UMeteo);
    }
}

void kill_crop(){
    WueTot = 0;
    A_AssimSurplus = 0;
    A_DemStructLeaf = 0;
    A_DemStructTot = 0;
    CoeffCO2Assim = 0;
    CoeffCO2Tr = 0;
    ConversionEff = 0;
    CumCarbonUsedPop = 0;
    CumGrowthPop = 0;
    CstrAssim = 0;
    CumWUse = 0;
    DemResInternodePop = 0;
    DryMatResInternodePopOld = 0;
    ETM = 0;
    GrowthDryMatPop = 0;
    GrowthPop = 0;
    Ic = 0;
    Lai = 0;
    LIRkdfcl = 0;
    NumPhase = 0;
    RelPotLeafLength = 0;
    RespMaintDebt = 0;
    RootFront = 0;
    RootMassPerVol = 0;
    SterilityDrought = 0;
    SterilityTot = 0;
    SumDegreDayCor = 0;
    Tr = 0;
    TrEff = 0;
    TrPot = 0;
    WueEt = 0;
}


void init_culture() {
    try {
        NumPhase = 0;
        SommeDegresJourMax = SDJLevee + SDJBVP + SDJRPR + SDJMatu1 + SDJMatu2;
        SumDegresDay = 0;
        SeuilTemp = 0;
        Lai = 0;
        IcCum = 0;
        FTSW = 1;
        Cstr = 1;
        DurPhase1 = 0;
        DurPhase2 = 0;
        DurPhase3 = 0;
        DurPhase4 = 0;
        DurPhase5 = 0;
        DurPhase6 = 0;
        TempLai = 0;
        ApexHeightGain = 0;
        ChangeNurseryStatus = 0;
        ChangePhase = 0;
        ChangeSsPhase = 0;
        CstrPhase2 = 0;
        CstrPhase3 = 0;
        CstrPhase4 = 0;
        CstrPhase5 = 0;
        CstrPhase6 = 0;
        CumCstrPhase2 = 0;
        CumCstrPhase3 = 0;
        CumCstrPhase4 = 0;
        CumCstrPhase5 = 0;
        CumCstrPhase6 = 0;
        CumFTSWPhase2 = 0;
        CumFTSWPhase3 = 0;
        CumFTSWPhase4 = 0;
        CumFTSWPhase5 = 0;
        CumFTSWPhase6 = 0;
        CumIcPhase2 = 0;
        CumIcPhase3 = 0;
        CumIcPhase4 = 0;
        CumIcPhase5 = 0;
        CumIcPhase6 = 0;
        DAF = 0;
        DemLeafAreaPlant = 0;
        DemPanicleFillPop = 0;
        DemStructInternodePlant = 0;
        DemStructInternodePop = 0;
        DemStructLeafPlant = 0;
        DemStructLeafPop = 0;
        DemStructPaniclePlant = 0;
        DemStructPaniclePop = 0;
        DemStructRootPlant = 0;
        DemStructRootPop = 0;
        DemStructSheathPop = 0;
        DemStructTotPop = 0;
        FloodwaterGain = 0;
        FtswPhase2 = 0;
        FtswPhase3 = 0;
        FtswPhase4 = 0;
        FtswPhase5 = 0;
        FtswPhase6 = 0;
        GainRootSystSoilSurfPop = 0;
        GainRootSystVolPop = 0;
        GrowthDryMatPop = 0;
        GrowthResInternodePop = 0;
        GrowthStructDeficit = 0;
        GrowthStructInternodePop = 0;
        GrowthStructLeafPop = 0;
        GrowthStructPaniclePop = 0;
        GrowthStructRootPop = 0;
        GrowthStructSheathPop = 0;
        GrowthStructTotPop = 0;
        HaunGain = 0;
        IcPhase2 = 0;
        IcPhase3 = 0;
        IcPhase4 = 0;
        IcPhase5 = 0;
        IcPhase6 = 0;
        IncreaseResInternodePop = 0;
        Kcl = 0;
        Kr = 0;
        MobiliLeafDeath = 0;
        NurseryStatus = 0;
        PanicleFilDeficit = 0;
        PanicleFilPop = 0;
        PanicleSinkPop = 0;
        PanStructMass = 0;
        PlantLeafNumNew = 0;
        ResInternodeMobiliDay = 0;
        ResInternodeMobiliDayPot = 0;
        RootFrontOld = 0;
        RootSystSoilSurfPop = 0;
        RootSystSoilSurfPopOld = 0;
        RootSystVolPop = 0;
        RootSystVolPopOld = 0;
        SDJCorPhase4 = 0;
    } catch (...) {
        error_message("RS_InitiationCulture", URisocas);
    }
}



void init_all_variables_2_1(){
    ApexHeight = 0;
    ApexHeightGain = 0;
    Assim = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    AssimPot = 0;
    AssimSurplus = 0;
    A_AssimSurplus = 0;
    A_DemStructLeaf = 0;
    A_DemStructTot = 0;
    A_GrowthStructLeaf = 0;
    A_GrowthStructTot = 0;
    A_IncreaseResInternodePop = 0;
    A_ResInternodeMobiliDay = 0;
    CapaRDE = 0;
    CapaREvap = 0;
    CapaRFE = 0;
    ChangeNurseryStatus = 0;
    ChangePhase = 0;
    ChangeSsPhase = 0;
    CoeffCO2Assim = 0;
    CoeffCO2Tr = 0;
    CoeffStressLogging = 0;
    Conversion = 0;
    ConversionEff = 0;
    CounterNursery = 0;
    Cstr = 0;
    CstrAssim = 0;
    CstrCum = 0;
    CstrMean = 0;
    CstrPhase2 = 0;
    CstrPhase3 = 0;
    CstrPhase4 = 0;
    CstrPhase5 = 0;
    CstrPhase6 = 0;
    CulmsPerHill = 0;
    CulmsPerHillFin = 0;
    CulmsPerHillMax = 0;
    CulmsPerPlant = 0;
    CulmsPerPlantFin = 0;
    CulmsPerPlantMax = 0;
    CulmsPop = 0;
    CumCarbonUsedPop = 0;
    CumCstrPhase2 = 0;
    CumCstrPhase3 = 0;
    CumCstrPhase4 = 0;
    CumCstrPhase5 = 0;
    CumCstrPhase6 = 0;
    CumDr = 0;
    CumEt = 0;
    CumFTSWPhase2 = 0;
    CumFTSWPhase3 = 0;
    CumFTSWPhase4 = 0;
    CumFTSWPhase5 = 0;
    CumFTSWPhase6 = 0;
    CumGrowthPop = 0;
    CumIcPhase2 = 0;
    CumIcPhase3 = 0;
    CumIcPhase4 = 0;
    CumIcPhase5 = 0;
    CumIcPhase6 = 0;
    CumIrrig = 0;
    CumIrrigFin = 0;
    CumLr = 0;
    CumPAR = 0;
    CumSupplyTot = 0;
    CumTr = 0;
    CumWReceived = 0;
    CumWUse = 0;
    CumWUseFin = 0;
    DAF = 0;
    DayLength = 0;
    DeadLeafdrywtPop = 0;
    Decli = 0;
    DegresDuJour = 0;
    DegresDuJourCor = 0;
    DemLeafAreaPlant = 0;
    DemPanicleFillPop = 0;
    DemResInternodePop = 0;
    DemStructInternodePlant = 0;
    DemStructInternodePop = 0;
    DemStructLeafPlant = 0;
    DemStructLeafPop = 0;
    DemStructPaniclePlant = 0;
    DemStructPaniclePop = 0;
    DemStructRootPlant = 0;
    DemStructRootPop = 0;
    DemStructSheathPop = 0;
    DemStructTotPop = 0;
    Density = 0;
    Dr = 0;
    DryMatAboveGroundPop = 0;
    DryMatAboveGroundPopFin = 0;
    DryMatAboveGroundTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatResInternodePop = 0.;
    DryMatResInternodePopOld = 0;
    DryMatStructInternodePop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStructRootPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatTotPop = 0;
    DryMatTotPopFin = 0;
    DryMatVegeTotPop = 0;
    DurGermFlow = 0;
    DurGermMat = 0;
    DurPhase1 = 0;
    DurPhase2 = 0;
    DurPhase3 = 0;
    DurPhase4 = 0;
    DurPhase5 = 0;
    DurPhase6 = 0;
    EauDispo = 0;
    ETM = 0;
    ETo = 0;
    ETR = 0;
    Evap = 0;
    EvapPot = 0;
    FertSpikeNumPop = 0;
    FloodwaterDepth = 0;
    FloodwaterGain = 0;
    FractionPlantHeightSubmer = 0;
    FractionRootsLogged = 0;
    FTSW = 0;
    FtswMoy = 0;
    FtswPhase2 = 0;
    FtswPhase3 = 0;
    FtswPhase4 = 0;
    FtswPhase5 = 0;
    FtswPhase6 = 0;
    GainRootSystSoilSurfPop = 0;
    GainRootSystVolPop = 0;
    GrainFillingStatus = 0;
    GrainFillingStatusFin = 0;
    GrainYieldPanicle = 0;
    GrainYieldPop = 0;
    GrainYieldPopFin = 0;
    GrowthDryMatPop = 0;
    GrowthPop = 0;
    GrowthResInternodePop = 0;
    GrowthStructDeficit = 0;
    GrowthStructInternodePop = 0;
    GrowthStructLeafPop = 0;
    GrowthStructPaniclePop = 0;
    GrowthStructRootPop = 0;
    GrowthStructSheathPop = 0;
    GrowthStructTotPop = 0;
    HarvestIndex = 0;
    HaunGain = 0;
    HaunIndex = 0;
    HMoyCalc = 0;
    Hum = 0;
    Ic = 0;
    IcCum = 0;
    IcMean = 0;
    IcPhase2 = 0;
    IcPhase3 = 0;
    IcPhase4 = 0;
    IcPhase5 = 0;
    IcPhase6 = 0;
    IncreaseResInternodePop = 0;
    InternodeResStatus = 0;
    Irrigation = 0;
    IrrigAutoDay = 0;
    IrrigTotDay = 0;
    Kce = 0;
    KceReal = 0;
    Kcl = 0;
    Kcp = 0;
    KcTot = 0;
    Kr = 0;
    KRolling = 0;
    Lai = 0;
    LaiDead = 0;
    LaiFin = 0;
    LastLeafLength = 0;
    LastLeafLengthPot = 0;
    LatRad = 0;
    LeafDeathPop = 0;
    LIRkdf = 0;
    LIRkdfcl = 0;
    Lr = 0;
    LTRkdf = 0;
    LTRkdfcl = 0;
    MaxLai = 0;
    MobiliLeafDeath = 0;
    NumPhase = 0;
    NumSsPhase = 0;
    NurseryStatus = 0;
    PanicleFilDeficit = 0;
    PanicleFilPop = 0;
    PanicleNumPlant = 0;
    PanicleNumPop = 0;
    PanicleSinkPop = 0;
    PanStructMass = 0;
    Par = 0;
    PARIntercepte = 0;
    PhaseStemElongation = 0;
    PlantHeight = 0;
    PlantLeafNumNew = 0;
    PlantLeafNumTot = 0;
    PlantWidth = 0;
    ProfRu = 0;
    RayExtra = 0;
    RelPotLeafLength = 0;
    ResCapacityInternodePop = 0;
    ReservePopFin = 0;
    ResInternodeMobiliDay = 0;
    ResInternodeMobiliDayPot = 0;
    RespMaintDebt = 0;
    RespMaintTot = 0;
    ResUtil = 0;
    RgCalc = 0;
    RgMax = 0;
    RootFront = 0;
    RootFrontOld = 0;
    RootMassPerVol = 0;
    RootShootRatio = 0;
    RootSystSoilSurfPop = 0;
    RootSystSoilSurfPopOld = 0;
    RootSystVolPop = 0;
    RootSystVolPopOld = 0;
    RUE = 0;
    RuRac = 0;
    RuSurf = 0;
    SDJCorPhase4 = 0;
    SeuilCstrMortality = 0;
    SeuilTemp = 0;
    SeuilTempSsPhase = 0;
    SimAnthesis50 = 0;
    SimEmergence = 0;
    SimEndCycle = 0;
    SimPanIni = 0;
    SimStartGermin = 0;
    SimStartMatu2 = 0;
    SimStartPSP = 0;
    Sla = 0;
    SlaMitch = 0;
    SlaNew = 0;
    SommeDegresJourMax = 0;
    SpikeNumPanicle = 0;
    SpikeNumPop = 0;
    SterilityCold = 0;
    SterilityDrought = 0;
    SterilityHeat = 0;
    SterilityTot = 0;
    SterilityTotFin = 0;
    StockMacropores = 0;
    StockRac = 0;
    StockSurface = 0;
    StockTotal = 0;
    StressCold = 0;
    StRuMax = 0;
    SumDDPhasePrec = 0;
    SumDegreDayCor = 0;
    SumDegresDay = 0;
    SumPP = 0;
    SunDistance = 0;
    SunPosi = 0;
    SupplyTot = 0;
    TempLai = 0;
    TillerDeathPop = 0;
    TmaxMoy = 0;
    TminMoy = 0;
    TMoyCalc = 0;
    TMoyPrec = 0;
    Tr = 0;
    TrEff = 0;
    TrEffInst = 0;
    TrPot = 0;
    ValRDE = 0;
    ValRFE = 0;
    ValRSurf = 0;
    VPDCalc = 0;
    VitesseRacinaire = 0;
    VitesseRacinaireDay = 0;
    VolMacropores = 0;
    VolRelMacropores = 0;
    WueEt = 0;
    WueTot = 0;
}


void init_all_variables_2_1_micha(){
    ApexHeight = 0;
    ApexHeightGain = 0;
    Assim = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    AssimPot = 0;
    AssimSurplus = 0;
    A_AssimSurplus = 0;
    A_DemStructLeaf = 0;
    A_DemStructTot = 0;
    A_GrowthStructLeaf = 0;
    A_GrowthStructTot = 0;
    A_IncreaseResInternodePop = 0;
    A_ResInternodeMobiliDay = 0;
    CapaRDE = 0;
    CapaREvap = 0;
    CapaRFE = 0;
    ChangeNurseryStatus = 0;
    ChangePhase = 0;
    ChangeSsPhase = 0;
    CoeffCO2Assim = 0;
    CoeffCO2Tr = 0;
    CoeffStressLogging = 0;
    Conversion = 0;
    ConversionEff = 0;
    CounterNursery = 0;
    Cstr = 0;
    CstrAssim = 0;
    CstrCum = 0;
    CstrMean = 0;
    CstrPhase2 = 0;
    CstrPhase3 = 0;
    CstrPhase4 = 0;
    CstrPhase5 = 0;
    CstrPhase6 = 0;
    CulmsPerHill = 0;
    CulmsPerHillFin = 0;
    CulmsPerHillMax = 0;
    CulmsPerPlant = 0;
    CulmsPerPlantFin = 0;
    CulmsPerPlantMax = 0;
    CulmsPop = 0;
    CumCarbonUsedPop = 0;
    CumCstrPhase2 = 0;
    CumCstrPhase3 = 0;
    CumCstrPhase4 = 0;
    CumCstrPhase5 = 0;
    CumCstrPhase6 = 0;
    CumDr = 0;
    CumEt = 0;
    CumFTSWPhase2 = 0;
    CumFTSWPhase3 = 0;
    CumFTSWPhase4 = 0;
    CumFTSWPhase5 = 0;
    CumFTSWPhase6 = 0;
    CumGrowthPop = 0;
    CumIcPhase2 = 0;
    CumIcPhase3 = 0;
    CumIcPhase4 = 0;
    CumIcPhase5 = 0;
    CumIcPhase6 = 0;
    CumIrrig = 0;
    CumIrrigFin = 0;
    CumLr = 0;
    CumPAR = 0;
    CumSupplyTot = 0;
    CumTr = 0;
    CumWReceived = 0;
    CumWUse = 0;
    CumWUseFin = 0;
    DAF = 0;
    DayLength = 0;
    DeadLeafdrywtPop = 0;
    Decli = 0;
    DegresDuJour = 0;
    DegresDuJourCor = 0;
    DemLeafAreaPlant = 0;
    DemPanicleFillPop = 0;
    DemResInternodePop = 0;
    DemStructInternodePlant = 0;
    DemStructInternodePop = 0;
    DemStructLeafPlant = 0;
    DemStructLeafPop = 0;
    DemStructPaniclePlant = 0;
    DemStructPaniclePop = 0;
    DemStructRootPlant = 0;
    DemStructRootPop = 0;
    DemStructSheathPop = 0;
    DemStructTotPop = 0;
    Density = 0;
    Dr = 0;
    DryMatAboveGroundPop = 0;
    DryMatAboveGroundPopFin = 0;
    DryMatAboveGroundTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatResInternodePop = 0.;
    DryMatResInternodePopOld = 0;
    DryMatStructInternodePop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStructRootPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatTotPop = 0;
    DryMatTotPopFin = 0;
    DryMatVegeTotPop = 0;
    DurGermFlow = 0;
    DurGermMat = 0;
    DurPhase1 = 0;
    DurPhase2 = 0;
    DurPhase3 = 0;
    DurPhase4 = 0;
    DurPhase5 = 0;
    DurPhase6 = 0;
    EauDispo = 0;
    ETM = 0;
    ETo = 0;
    ETR = 0;
    Evap = 0;
    EvapPot = 0;
    FertSpikeNumPop = 0;
    FloodwaterDepth = 0;
    FloodwaterGain = 0;
    FractionPlantHeightSubmer = 0;
    FractionRootsLogged = 0;
    FTSW = 0;
    FtswMoy = 0;
    FtswPhase2 = 0;
    FtswPhase3 = 0;
    FtswPhase4 = 0;
    FtswPhase5 = 0;
    FtswPhase6 = 0;
    GainRootSystSoilSurfPop = 0;
    GainRootSystVolPop = 0;
    GrainFillingStatus = 0;
    GrainFillingStatusFin = 0;
    GrainYieldPanicle = 0;
    GrainYieldPop = 0;
    GrainYieldPopFin = 0;
    GrowthDryMatPop = 0;
    GrowthPop = 0;
    GrowthResInternodePop = 0;
    GrowthStructDeficit = 0;
    GrowthStructInternodePop = 0;
    GrowthStructLeafPop = 0;
    GrowthStructPaniclePop = 0;
    GrowthStructRootPop = 0;
    GrowthStructSheathPop = 0;
    GrowthStructTotPop = 0;
    HarvestIndex = 0;
    HaunGain = 0;
    HaunIndex = 0;
    HMoyCalc = 0;
    Hum = 0;
    Ic = 0;
    IcCum = 0;
    IcMean = 0;
    IcPhase2 = 0;
    IcPhase3 = 0;
    IcPhase4 = 0;
    IcPhase5 = 0;
    IcPhase6 = 0;
    IncreaseResInternodePop = 0;
    InternodeResStatus = 0;
    Irrigation = 0;
    IrrigAutoDay = 0;
    IrrigTotDay = 0;
    Kce = 0;
    KceReal = 0;
    Kcl = 0;
    Kcp = 0;
    KcTot = 0;
    Kr = 0;
    KRolling = 0;
    Lai = 0;
    LaiDead = 0;
    LaiFin = 0;
    LastLeafLength = 0;
    LastLeafLengthPot = 0;
    LatRad = 0;
    LeafDeathPop = 0;
    LIRkdf = 0;
    LIRkdfcl = 0;
    Lr = 0;
    LTRkdf = 0;
    LTRkdfcl = 0;
    MaxLai = 0;
    MobiliLeafDeath = 0;
    NumPhase = 0;
    NumSsPhase = 0;
    NurseryStatus = 0;
    PanicleFilDeficit = 0;
    PanicleFilPop = 0;
    PanicleNumPlant = 0;
    PanicleNumPop = 0;
    PanicleSinkPop = 0;
    PanStructMass = 0;
    Par = 0;
    PARIntercepte = 0;
    PhaseStemElongation = 0;
    PlantHeight = 0;
    PlantLeafNumNew = 0;
    PlantLeafNumTot = 0;
    PlantWidth = 0;
    ProfRu = 0;
    RayExtra = 0;
    RelPotLeafLength = 0;
    ResCapacityInternodePop = 0;
    ReservePopFin = 0;
    ResInternodeMobiliDay = 0;
    ResInternodeMobiliDayPot = 0;
    RespMaintDebt = 0;
    RespMaintTot = 0;
    ResUtil = 0;
    RgCalc = 0;
    RgMax = 0;
    RootFront = 0;
    RootFrontOld = 0;
    RootMassPerVol = 0;
    RootShootRatio = 0;
    RootSystSoilSurfPop = 0;
    RootSystSoilSurfPopOld = 0;
    RootSystVolPop = 0;
    RootSystVolPopOld = 0;
    RUE = 0;
    RuRac = 0;
    RuSurf = 0;
    SDJCorPhase4 = 0;
    SeuilCstrMortality = 0;
    SeuilTemp = 0;
    SeuilTempSsPhase = 0;
    SimAnthesis50 = 0;
    SimEmergence = 0;
    SimEndCycle = 0;
    SimPanIni = 0;
    SimStartGermin = 0;
    SimStartMatu2 = 0;
    SimStartPSP = 0;
    Sla = 0;
    SlaMitch = 0;
    SlaNew = 0;
    SommeDegresJourMax = 0;
    SpikeNumPanicle = 0;
    SpikeNumPop = 0;
    SterilityCold = 0;
    SterilityDrought = 0;
    SterilityHeat = 0;
    SterilityTot = 0;
    SterilityTotFin = 0;
    StockMacropores = 0;
    StockRac = 0;
    StockSurface = 0;
    StockTotal = 0;
    StressCold = 0;
    StRuMax = 0;
    SumDDPhasePrec = 0;
    SumDegreDayCor = 0;
    SumDegresDay = 0;
    SumPP = 0;
    SunDistance = 0;
    SunPosi = 0;
    SupplyTot = 0;
    TempLai = 0;
    TillerDeathPop = 0;
    TmaxMoy = 0;
    TminMoy = 0;
    TMoyCalc = 0;
    TMoyPrec = 0;
    Tr = 0;
    TrEff = 0;
    TrEffInst = 0;
    TrPot = 0;
    ValRDE = 0;
    ValRFE = 0;
    ValRSurf = 0;
    VPDCalc = 0;
    VitesseRacinaire = 0;
    VitesseRacinaireDay = 0;
    VolMacropores = 0;
    VolRelMacropores = 0;
    WueEt = 0;
    WueTot = 0;
    StockIniSurf = 0;
}

void init_all_variables_2_3(){
    ApexHeight = 0;
    ApexHeightGain = 0;
    Assim = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    AssimPot = 0;
    AssimSurplus = 0;
    A_AssimSurplus = 0;
    A_DemStructLeaf = 0;
    A_DemStructTot = 0;
    A_GrowthStructLeaf = 0;
    A_GrowthStructTot = 0;
    A_IncreaseResInternodePop = 0;
    A_ResInternodeMobiliDay = 0;
    CapaRDE = 0;
    CapaREvap = 0;
    CapaRFE = 0;
    ChangeNurseryStatus = 0;
    ChangePhase = 0;
    ChangeSsPhase = 0;
    CoeffCO2Assim = 0;
    CoeffCO2Tr = 0;
    CoeffStressLogging = 0;
    Conversion = 0;
    ConversionEff = 0;
    CounterNursery = 0;
    Cstr = 0;
    CstrAssim = 0;
    CstrCum = 0;
    CstrMean = 0;
    CstrPhase2 = 0;
    CstrPhase3 = 0;
    CstrPhase4 = 0;
    CstrPhase5 = 0;
    CstrPhase6 = 0;
    CulmsPerHill = 0;
    CulmsPerHillFin = 0;
    CulmsPerHillMax = 0;
    CulmsPerPlant = 0;
    CulmsPerPlantFin = 0;
    CulmsPerPlantMax = 0;
    CulmsPop = 0;
    CumCarbonUsedPop = 0;
    CumCstrPhase2 = 0;
    CumCstrPhase3 = 0;
    CumCstrPhase4 = 0;
    CumCstrPhase5 = 0;
    CumCstrPhase6 = 0;
    CumDr = 0;
    CumEt = 0;
    CumFTSWPhase2 = 0;
    CumFTSWPhase3 = 0;
    CumFTSWPhase4 = 0;
    CumFTSWPhase5 = 0;
    CumFTSWPhase6 = 0;
    CumGrowthPop = 0;
    CumIcPhase2 = 0;
    CumIcPhase3 = 0;
    CumIcPhase4 = 0;
    CumIcPhase5 = 0;
    CumIcPhase6 = 0;
    CumIrrig = 0;
    CumIrrigFin = 0;
    CumLr = 0;
    CumPAR = 0;
    CumSupplyTot = 0;
    CumTr = 0.00001;
    CumWReceived = 0;
    CumWUse = 0;
    CumWUseFin = 0;
    DAF = 0;
    DayLength = 0;
    DeadLeafdrywtPop = 0;
    Decli = 0;
    DegresDuJour = 0;
    DegresDuJourCor = 0;
    DemLeafAreaPlant = 0;
    DemPanicleFillPop = 0;
    DemResInternodePop = 0;
    DemStructInternodePlant = 0;
    DemStructInternodePop = 0;
    DemStructLeafPlant = 0;
    DemStructLeafPop = 0;
    DemStructPaniclePlant = 0;
    DemStructPaniclePop = 0;
    DemStructRootPlant = 0;
    DemStructRootPop = 0;
    DemStructSheathPop = 0;
    DemStructTotPop = 0;
    Density = 0;
    Dr = 0;
    DryMatAboveGroundPop = 0;
    DryMatAboveGroundPopFin = 0;
    DryMatAboveGroundTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatResInternodePop = 0.;
    DryMatResInternodePopOld = 0;
    DryMatStructInternodePop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStructRootPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatTotPop = 0;
    DryMatTotPopFin = 0;
    DryMatVegeTotPop = 0;
    DurGermFlow = 0;
    DurGermMat = 0;
    DurPhase1 = 0;
    DurPhase2 = 0;
    DurPhase3 = 0;
    DurPhase4 = 0;
    DurPhase5 = 0;
    DurPhase6 = 0;
    EauDispo = 0;
    ETM = 0;
    ETo = 0;
    ETR = 0;
    Evap = 0;
    EvapPot = 0;
    FertSpikeNumPop = 0;
    FloodwaterDepth = 0;
    FloodwaterGain = 0;
    FractionPlantHeightSubmer = 0;
    FractionRootsLogged = 0;
    FTSW = 0;
    FtswMoy = 0;
    FtswPhase2 = 0;
    FtswPhase3 = 0;
    FtswPhase4 = 0;
    FtswPhase5 = 0;
    FtswPhase6 = 0;
    GainRootSystSoilSurfPop = 0;
    GainRootSystVolPop = 0;
    GrainFillingStatus = 0;
    GrainFillingStatusFin = 0;
    GrainYieldPanicle = 0;
    GrainYieldPop = 0;
    GrainYieldPopFin = 0;
    GrowthDryMatPop = 0;
    GrowthPop = 0;
    GrowthResInternodePop = 0;
    GrowthStructDeficit = 0;
    GrowthStructInternodePop = 0;
    GrowthStructLeafPop = 0;
    GrowthStructPaniclePop = 0;
    GrowthStructRootPop = 0;
    GrowthStructSheathPop = 0;
    GrowthStructTotPop = 0;
    HarvestIndex = 0;
    HaunGain = 0;
    HaunIndex = 0;
    HMoyCalc = 0;
    Hum = 0;
    Ic = 0;
    IcCum = 0;
    IcMean = 0;
    IcPhase2 = 0;
    IcPhase3 = 0;
    IcPhase4 = 0;
    IcPhase5 = 0;
    IcPhase6 = 0;
    IncreaseResInternodePop = 0;
    InternodeResStatus = 0;
    Irrigation = 0;
    IrrigAutoDay = 0;
    IrrigTotDay = 0;
    Kce = 0;
    KceReal = 0;
    Kcl = 0;
    Kcp = 0;
    KcTot = 0;
    Kr = 0;
    KRolling = 0;
    Lai = 0;
    LaiDead = 0;
    LaiFin = 0;
    LastLeafLength = 0;
    LastLeafLengthPot = 0;
    LatRad = 0;
    LeafDeathPop = 0;
    LIRkdf = 0;
    LIRkdfcl = 0;
    Lr = 0;
    LTRkdf = 0;
    LTRkdfcl = 0;
    MaxLai = 0;
    MobiliLeafDeath = 0;
    NumPhase = 0;
    NumSsPhase = 0;
    NurseryStatus = 0;
    PanicleFilDeficit = 0;
    PanicleFilPop = 0;
    PanicleNumPlant = 0;
    PanicleNumPop = 0;
    PanicleSinkPop = 0;
    PanStructMass = 0;
    Par = 0;
    PARIntercepte = 0;
    PhaseStemElongation = 0;
    PlantHeight = 0;
    PlantLeafNumNew = 0;
    PlantLeafNumTot = 0;
    PlantWidth = 0;
    ProfRu = 0;
    RayExtra = 0;
    RelPotLeafLength = 0;
    ResCapacityInternodePop = 0;
    ReservePopFin = 0;
    ResInternodeMobiliDay = 0;
    ResInternodeMobiliDayPot = 0;
    RespMaintDebt = 0;
    RespMaintTot = 0;
    ResUtil = 0;
    RgCalc = 0;
    RgMax = 0;
    RootFront = 0;
    RootFrontOld = 0;
    RootMassPerVol = 0;
    RootShootRatio = 0;
    RootSystSoilSurfPop = 0;
    RootSystSoilSurfPopOld = 0;
    RootSystVolPop = 0;
    RootSystVolPopOld = 0;
    RUE = 0;
    RuRac = 0;
    RuSurf = 0;
    SDJCorPhase4 = 0;
    SeuilCstrMortality = 0;
    SeuilTemp = 0;
    SeuilTempSsPhase = 0;
    SimAnthesis50 = 0;
    SimEmergence = 0;
    SimEndCycle = 0;
    SimPanIni = 0;
    SimStartGermin = 0;
    SimStartMatu2 = 0;
    SimStartPSP = 0;
    Sla = 0;
    SlaMitch = 0;
    SlaNew = 0;
    SommeDegresJourMax = 0;
    SpikeNumPanicle = 0;
    SpikeNumPop = 0;
    SterilityCold = 0;
    SterilityDrought = 0;
    SterilityHeat = 0;
    SterilityTot = 0;
    SterilityTotFin = 0;
    StockMacropores = 0;
    StockRac = 0;
    StockSurface = 0;
    StockTotal = 0;
    StressCold = 0;
    StRuMax = 0;
    SumDDPhasePrec = 0;
    SumDegreDayCor = 0;
    SumDegresDay = 0;
    SumPP = 0;
    SunDistance = 0;
    SunPosi = 0;
    SupplyTot = 0;
    TempLai = 0;
    TillerDeathPop = 0;
    TmaxMoy = 0;
    TminMoy = 0;
    TMoyCalc = 0;
    TMoyPrec = 0;
    Tr = 0;
    TrEff = 0;
    TrEffInst = 0;
    TrPot = 0;
    ValRDE = 0;
    ValRFE = 0;
    ValRSurf = 0;
    VPDCalc = 0;
    VitesseRacinaire = 0;
    VitesseRacinaireDay = 0;
    VolMacropores = 0;
    VolRelMacropores = 0;
    WueEt = 0;
    WueTot = 0;
}
