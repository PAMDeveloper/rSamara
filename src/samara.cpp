#include "samara.h"

#include "processes/processes.h"

#include <iostream>
#include <algorithm>
#include <string>


pair <vector <string>, vector < vector <double> > > Samara::run_samara_2_1(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_1();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    double DateEnCours = DateDebutSimul;
    NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters_2_1(parameters);
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
                   Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);
    EToFao(ETP, Altitude, RgMax, RgCalc,
           TMin, TMax,
           HMin, HMax, HMoyCalc,
           TMoyCalc, Vt, ETo,
           TMoyPrec, VPDCalc);

    cumRain = 0;
    bool simulationFinished = false;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        set_meteo_vars(parameters, (int)(DateEnCours-DateDebutSimul),
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);
        NbJAS = DateEnCours - DateSemis;
        if(NumPhase > 0)
            cumRain += Pluie;
        //crop entity
        if(NbJAS == 0) {
            init_culture();
            crop = true;
            samara::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                        CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                        DryMatStructPaniclePop, DryMatResInternodePop);
        }


        eval_Par(DateEnCours);
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
        samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
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

        samara::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
        samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

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
        samara::RS_EvalDemandStructSheath(NumPhase, WtRatioLeafSheath, SlaMin, SlaMax, SlaMitch, StressCold, Par, PARCritSLA, SLASwitch, DemStructSheathPop, DemStructLeafPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                           DemStructSheathPop, DryMatStructRootPop, RootLignin,
										   RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
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
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, RootLignin, GrowthStructRootPop);
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
                                               AssimNotUsedCum, MobiliLeafDeath, RootLignin,
												DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                               DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                               SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                               GrowthPop, CumCarbonUsedPop, RootLigninPop);

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
                                DryMatAboveGroundTotPop, DryMatAboveGroundPop, RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                TrEff, WueEt, WueTot, ConversionEff, RUEGreen);

        samara::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                   GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                   DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                   CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                   IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                   CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                   ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);

        if ( NumPhase >= 6 ) {
            GrowthPop = 0;
            GrowthDryMatPop = 0;
        }
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_1();
            }
            DryMatResInternodePop = 0;
        }

        /** OUTPUT VECTORS **/
		vector <double> result = get_variable_values();

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
                simulationFinished = true;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
		daily_reset_variables();
        //        if(simulationFinished) break;
    }


    /** OUTPUT VECTORS **/
	vector <string> names = get_variable_names();

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

pair <vector <string>, vector < vector <double> > > Samara::run_samara_2_1_micha(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_1_micha();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    double DateEnCours = DateDebutSimul;
    NbJAS = DateEnCours - DateSemis;
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

    cumRain = 0;
    //Main loop
    bool simulationFinished = false;
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        A_ResInternodeMobiliDay = 0;
        ResInternodeMobiliDay = 0;

        set_meteo_vars(parameters, (int)(DateEnCours-DateDebutSimul),
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);
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
        samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
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

        samara::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
        samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

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
        samara::RS_EvalSupplyTot_V2_1_micha(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                            SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                             DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
		samara::RS_EvalDemandStructSheath(NumPhase, WtRatioLeafSheath, SlaMin, SlaMax, SlaMitch, StressCold, Par, PARCritSLA, SLASwitch, DemStructSheathPop, DemStructLeafPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                           DemStructSheathPop, DryMatStructRootPop, RootLignin, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
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
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, RootLignin, GrowthStructRootPop);
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
                                               AssimNotUsedCum, MobiliLeafDeath, RootLignin, 
											DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                               DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                               SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                               GrowthPop, CumCarbonUsedPop, RootLigninPop);

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
                                DryMatAboveGroundTotPop, DryMatAboveGroundPop, RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                TrEff, WueEt, WueTot, ConversionEff, RUEGreen);

        samara::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                   GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                   DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                   CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                   IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                   CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                   ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_1();
                CumWUseFin = CumWReceived;
                CumIrrigFin = CumIrrig;
                CumDrFin = CumDr;
            }
            DryMatResInternodePop = 0;
            GrowthDryMatPop = 0;
        }

        /** OUTPUT VECTORS **/
		vector <double> result = get_variable_values();

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
                simulationFinished = true;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
		daily_reset_variables();

        if(simulationFinished)
            break;
    }


    /** OUTPUT VECTORS **/
	vector <string> names = get_variable_names();

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

pair <vector <string>, vector < vector <double> > > Samara::run_samara_2_3(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_3();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    double DateEnCours = DateDebutSimul;
    NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters_2_3(parameters);

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
    //    EToFao_2_3(ETP, Altitude, RgMax, RgCalc,
    //           TMin, TMax,
    //           HMin, HMax, HMoyCalc,
    //           TMoyCalc, Vt, ETo,
    //           TMoyPrec, VPDCalc);
    //

    cumRain = 0;
    bool simulationFinished = false;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        A_ResInternodeMobiliDay = 0;
        ResInternodeMobiliDay = 0;

        set_meteo_vars(parameters, (int)(DateEnCours-DateDebutSimul),
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);
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
        if(crop) samara::RS_Transplanting_V2_2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                               CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatResInternodePop, DeadLeafdrywtPop, ResCapacityInternodePop);

        eval_Par(DateEnCours);
        EToFao_2_3(ETP, Altitude, RgMax, RgCalc,
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
        samara::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
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

        samara::RS_EvalTMaxMoy_V2_3(TMax, TMin, HMax, HMin, NumSsPhase, NumSsPhase, TmaxMoy);
        samara::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

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
		samara::RS_EvalDemandStructSheath(NumPhase, WtRatioLeafSheath, SlaMin, SlaMax, SlaMitch, StressCold, Par, PARCritSLA, SLASwitch, DemStructSheathPop, DemStructLeafPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                           DemStructSheathPop, DryMatStructRootPop, RootLignin, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
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
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, RootLignin, GrowthStructRootPop);
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
                                               AssimNotUsedCum, MobiliLeafDeath, RootLignin, 
										DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                               DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                               SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                               GrowthPop, CumCarbonUsedPop, RootLigninPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);

        if(crop)samara::RS_EvalSlaMitch_V2_2(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase,
			TempSLA, DryMatStructLeafPop, LeafDeathPop, GrowthStructLeafPop, Par, PARCritSLA, SLASwitch, SlaMitch, SlaNew, Sla, SlaMitchAdjusted);

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



        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_3();
                CumIrrigFin = CumIrrig;
            }
            DryMatResInternodePop = 0;
            GrowthDryMatPop = 0;
        }

        /** OUTPUT VECTORS **/
		vector <double> result = get_variable_values();

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
                simulationFinished = true;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
		daily_reset_variables();

        if(simulationFinished)
            break;
    }


    /** OUTPUT VECTORS **/
	vector <string> names = get_variable_names();

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

pair <vector <string>, vector < vector <double> > > Samara::run_samara_2_3_lodging(SamaraParameters * parameters, SamaraLogType log) {
    LOG = log;
    //Simu parameters
    init_all_variables_2_3();
    double DateDebutSimul = parameters->getDouble("startingdate");
    double DateFinSimul = parameters->getDouble("endingdate");
    double DateSemis = parameters->getDouble("sowing");
    double DateEnCours = DateDebutSimul;
    NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters_2_3(parameters);
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
    //    EToFao_2_3(ETP, Altitude, RgMax, RgCalc,
    //           TMin, TMax,
    //           HMin, HMax, HMoyCalc,
    //           TMoyCalc, Vt, ETo,
    //           TMoyPrec, VPDCalc);
    //

    cumRain = 0;
    bool simulationFinished = false;
    //Main loop
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        A_ResInternodeMobiliDay = 0;
        ResInternodeMobiliDay = 0;

        set_meteo_vars(parameters, (int)(DateEnCours-DateDebutSimul),
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);

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
        if(crop) samara::RS_Transplanting_V2_2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                               CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatResInternodePop, DeadLeafdrywtPop, ResCapacityInternodePop);


        eval_Par(DateEnCours);
        //        if(start++ < 2) ETP = 0;
        EToFao_2_3(ETP, Altitude, RgMax, RgCalc,
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
        samara::RS_EvalDAF_V2_lodging(NumPhase, DegresDuJour, SDJMatu1, SDJMatu2,
                                      DAF, MatuSDJ, MatuProgress);
        samara::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
        samara::RS_EvolHauteur_SDJ_cstr_V2_1_lodging(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
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
        samara::RS_EvolMobiliTillerDeath_V2_2_lodging(  NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic,
                                                        PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                                        CulmsPerHill, DryMatStructPaniclePop);
        samara::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        samara::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                      SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                             DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
		samara::RS_EvalDemandStructSheath(NumPhase, WtRatioLeafSheath, SlaMin, SlaMax, SlaMitch, StressCold, Par, PARCritSLA, SLASwitch, DemStructSheathPop, DemStructLeafPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                           DemStructSheathPop, DryMatStructRootPop, RootLignin, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
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
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, RootLignin, GrowthStructRootPop);
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
                                               AssimNotUsedCum, MobiliLeafDeath, RootLignin, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                               DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                               SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                               GrowthPop, CumCarbonUsedPop, RootLigninPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);

		if (crop)samara::RS_EvalSlaMitch_V2_2(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase,
			TempSLA, DryMatStructLeafPop, LeafDeathPop, GrowthStructLeafPop, Par, PARCritSLA, SLASwitch, SlaMitch, SlaNew, Sla, SlaMitchAdjusted);

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

        samara::EvalLodgingResistance( NumPhase,  MatuProgress,  DryMatStructLeafPop,
                                       DryMatStemPop,  DeadLeafdrywtPop,  DryMatPanicleTotPop,
                                       PlantHeight,  CoeffLodging, StemPorosity, ApexHeight, CulmsPop,
                                       GrainMoisture,  FreshMatPanicleTotPop,  StemVigor,  LodgingIndex,
                                       FreshMatAbovegroundPop,  LodgingResistance, LodgingResistance2,
                                       StemSurfMean, StemDiaMean, StemDiaBase);
        samara::EvalLodgingIncidence(NumPhase, LodgingResistance, Vt,
                                     Pluie,
                                     LodgingDay, Lodging, LodgingPot);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                   GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                   DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                   CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                   IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                   CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                   ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);



        //                samara2_1::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_3();
                CumIrrigFin = CumIrrig;
            }
            DryMatResInternodePop = 0;
            GrowthDryMatPop = 0;
        }

        /** OUTPUT VECTORS **/
		vector <double> result = get_variable_values();
        
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
                simulationFinished = true;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
		daily_reset_variables();

        if(simulationFinished)
            break;
    }


    /** OUTPUT VECTORS **/
	vector <string> names = get_variable_names();

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

//#include <QDebug>
pair <vector <string>, vector < vector <double> > > Samara::run_samara_2_3_lodging_test(SamaraParameters * parameters, SamaraLogType log) {
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
    NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parameters_2_3(parameters);
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

    cumRain = 0;
    bool simulationFinished = false;
    //Main loop
//	int step_ = 0;
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
		//qDebug() << fixed << DateEnCours << step_++;

        A_ResInternodeMobiliDay = 0;
        ResInternodeMobiliDay = 0;

        set_meteo_vars(parameters, static_cast<int>(DateEnCours-DateDebutSimul),
                       TMax, TMin, TMoy, HMax, HMin, HMoy, Vt,
                       Ins, Rg, ETP, Pluie, TMoyCalc, HMoyCalc, Irrigation);
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
        samara::RS_EvalDAF_V2_lodging(NumPhase, DegresDuJour, SDJMatu1, SDJMatu2,
                                      DAF, MatuSDJ, MatuProgress);
        samara::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
        samara::RS_EvolHauteur_SDJ_cstr_V2_1_lodging(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
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
        samara::RS_EvolMobiliTillerDeath_V2_2_lodging(  NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic,
                                                        PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                                        CulmsPerHill, DryMatStructPaniclePop);
        samara::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        samara::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                      SupplyTot, CumSupplyTot);
        samara::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                             DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
		samara::RS_EvalDemandStructSheath(NumPhase, WtRatioLeafSheath, SlaMin, SlaMax, SlaMitch, StressCold, Par, PARCritSLA, SLASwitch, DemStructSheathPop, DemStructLeafPop);
        samara::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                           DemStructSheathPop, DryMatStructRootPop, RootLignin, 
										   RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
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
        samara::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, RootLignin, GrowthStructRootPop);
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
                                               AssimNotUsedCum, MobiliLeafDeath, RootLignin,
											   DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                               DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                               DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                               SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                               GrowthPop, CumCarbonUsedPop, RootLigninPop);

        samara::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        samara::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        samara::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)samara::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);

        if(crop)samara::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase, TempSLA, DryMatStructLeafPop,
                                        GrowthStructLeafPop, SlaMitch, SlaNew, Sla);

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

        samara::EvalLodgingResistance( NumPhase,  MatuProgress,  DryMatStructLeafPop,
                                       DryMatStemPop,  DeadLeafdrywtPop,  DryMatPanicleTotPop,
                                       PlantHeight,  CoeffLodging, StemPorosity, ApexHeight, CulmsPop,
                                       GrainMoisture,  FreshMatPanicleTotPop,  StemVigor,  LodgingIndex,
                                       FreshMatAbovegroundPop,  LodgingResistance, LodgingResistance2,
                                       StemSurfMean, StemDiaMean, StemDiaBase);

        samara::EvalLodgingIncidence(NumPhase, LodgingResistance, Vt,
                                     Pluie,
                                     LodgingDay, Lodging, LodgingPot);

        samara::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                   GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                   DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                   CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                   IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                   CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                   ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);



        //                samara2_1::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                reset_variables_2_3();
                CumIrrigFin = CumIrrig;
            }
            DryMatResInternodePop = 0;
            GrowthDryMatPop = 0;
        }

        /** OUTPUT VECTORS **/
		vector <double> result = get_variable_values();
        
        if ( NumPhase == 7 ) {
            if(ChangePhase == 1) {
                kill_crop();
                crop = false;
                simulationFinished = true;
            }
            cumRain = 0;
        }

        currentResults.push_back(result);
		
		daily_reset_variables();

        if(simulationFinished)
            break;
    }


    /** OUTPUT VECTORS **/
    vector <string> names = get_variable_names();
    

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

void Samara::init_parameters_2_1(SamaraParameters * params) {
	init_parameters(params);
    StockIniProf = 0;	
}

void Samara::init_parameters_2_1_micha(SamaraParameters * params) {
	init_parameters(params);
    StockIniSurf = 0;
}

void Samara::init_parameters_2_3(SamaraParameters * params) {
	init_parameters(params);
	//Lodging
	CoeffLodging = params->getDouble("coefflodging");
	StemPorosity = params->getDouble("stemporosity");
}

void Samara::reset_variables_2_1() {
	reset_variables();
}

void Samara::reset_variables_2_1_micha() {
	reset_variables();
}

void Samara::reset_variables_2_3() {
	reset_variables();
    LaiDead = 0;
}

void Samara::set_meteo_vars(SamaraParameters * parameters, int t, double &TMax, double &TMin, double &TMoy
                            , double &HMax, double &HMin, double &HMoy
                            , double &Vt, double &Ins, double &Rg, double &ETP
                            , double &Pluie, double & TMoyCalc, double & HMoyCalc, double & Irrigation) {
	Irrigation = parameters->getIrrigation(t);
	if (Irrigation == NilValue)
		Irrigation = 0.;

	Climate c = parameters->getClimate(t);
    TMax = c.TMax;
    TMin = c.TMin;
    TMoy = c.TMoy;
    HMax = c.HMax;
    HMin = c.HMin;
    HMoy = c.HMoy;
    Vt = c.Vt;
    Ins = c.Ins;
    Rg = c.Rg;
    ETP = c.ETP;
    Pluie = c.Rain;
    if (((TMin != NilValue) && (TMax != NilValue))) {
        TMoyCalc = (TMax + TMin) * 1.0 / 2;
    } else {
        TMoyCalc = TMoy;
    }
    if (((HMin != NilValue) && (HMax != NilValue))) {
        HMoyCalc = (HMax + HMin) * 1.0 / 2;
    } else {
        HMoyCalc = HMoy;
		HMax = 100;
		HMin = max(0., 2 * HMoy - 100);
    }
}

void Samara::init_parcelle(double &VolMacropores, double &HumSat, double &HumFC, double &ResUtil
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

void Samara::eval_Par(double t) {
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


void Samara::EToFao_2_3(double const &/*ETP*/, double const &/*Alt*/, double const &RgMax, double const &RgCalc,
                        double const &TMin, double const &TMax,
                        double const &HMin, double const &HMax, double const &HMoyCalc,
                        double const &TMoyCalc, double const &Vt, double &ETo,
                        double &TMoyPrec, double &VPDCalc) {
    try {
        if(Vt == 0) {
            VPDCalc = VPDCalc;
        }
        if ((ETP == NilValue)) {
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
        } else {
            ETo = ETP;
        }
        TMoyPrec = TMoyCalc;

    } catch (...) {
        error_message("EToFAO", UMeteo);
    }
}


void Samara::EToFao(double const &/*ETP*/, double const &/*Alt*/, double const &RgMax, double const &RgCalc,
                    double const &TMin, double const &TMax,
                    double const &HMin, double const &HMax, double const &HMoyCalc,
                    double const &TMoyCalc, double const &Vt, double &ETo,
                    double &TMoyPrec, double &VPDCalc) {
    try {
        if(Vt == 0) {
            VPDCalc = VPDCalc;
        }
        if ((ETP == NilValue)) {
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
        } else {
            ETo = ETP;
        }
        TMoyPrec = TMoyCalc;

    } catch (...) {
        error_message("EToFAO", UMeteo);
    }
}

void Samara::kill_crop(){
	kill_crop_variables();
}

void Samara::init_culture() {
    SommeDegresJourMax = SDJLevee + SDJBVP + SDJRPR + SDJMatu1 + SDJMatu2;
	init_culture_variables();
}

void Samara::init_all_variables_2_1(){
	init_all_variables();
}

void Samara::init_all_variables_2_1_micha(){
	init_all_variables();
    StockIniSurf = 0;
}

void Samara::init_all_variables_2_3(){
	init_all_variables();
	CumTr = 0.00001;
	MatuSDJ = 0;
	MatuProgress = 0;
	GrainMoisture = 0;
	FreshMatPanicleTotPop = 0;
	StemVigor = 0;
	LodgingIndex = 0;
	FreshMatAbovegroundPop = 0;
	LodgingResistance = 0;
	LodgingResistance2 = 0;
	LodgingDay = 0;
	Lodging = 0;
	StemSurfMean = 0;
	StemDiaMean = 0;
	StemDiaBase = 0;
	LodgingPot = 0;
}
