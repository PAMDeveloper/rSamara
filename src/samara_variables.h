#pragma once
//#include <QDebug>
using namespace std;
#include <array>

class Samara_Variables {

public:
	enum SamaraLogType { SMALL, COMPLETE, ECOTROP };
	SamaraLogType LOG;


	//variables parameters
	double TMax = 0;
	double TMin = 0;
	double TMoy = 0;
	double HMax = 0;
	double HMin = 0;
	double HMoy = 0;
	double Vt = 0;
	double Ins = 0;
	double Rg = 0;
	double ETP = 0;
	double Pluie = 0;

	//simu variables
	double NbJAS = 0;
	double cumRain = 0;

	//var
	double MatuSDJ = 0;
	double MatuProgress = 0;
	double GrainMoisture = 0;
	double FreshMatPanicleTotPop = 0;
	double StemVigor = 0;
	double LodgingIndex = 0;
	double FreshMatAbovegroundPop = 0;
	double LodgingResistance = 0;
	double LodgingResistance2 = 0;
	double LodgingDay = 0;
	double Lodging = 0;
	double StemSurfMean = 0;
	double StemDiaMean = 0;
	double StemDiaBase = 0;
	double LodgingPot = 0;
	
	//variables
	double ApexHeight = 0;
	double ApexHeightGain = 0;
	double Assim = 0;
	double AssimNotUsed = 0;
	double AssimNotUsedCum = 0;
	double AssimPot = 0;
	double AssimSurplus = 0;
	double A_AssimSurplus = 0;
	double A_DemStructLeaf = 0;
	double A_DemStructTot = 0;
	double A_GrowthStructLeaf = 0;
	double A_GrowthStructTot = 0;
	double A_IncreaseResInternodePop = 0;
	double A_ResInternodeMobiliDay = 0;
	double CapaRDE = 0;
	double CapaREvap = 0;
	double CapaRFE = 0;
	double ChangeNurseryStatus = 0;
	double ChangePhase = 0;
	double ChangeSsPhase = 0;
	double CoeffCO2Assim = 0;
	double CoeffCO2Tr = 0;
	double CoeffStressLogging = 0;
	double Conversion = 0;
	double ConversionEff = 0;
	double CounterNursery = 0;
	double Cstr = 0;
	double CstrAssim = 0;
	double CstrCum = 0;
	double CstrMean = 0;
	double CstrPhase2 = 0;
	double CstrPhase3 = 0;
	double CstrPhase4 = 0;
	double CstrPhase5 = 0;
	double CstrPhase6 = 0;
	double CulmsPerHill = 0;
	double CulmsPerHillFin = 0;
	double CulmsPerHillMax = 0;
	double CulmsPerPlant = 0;
	double CulmsPerPlantFin = 0;
	double CulmsPerPlantMax = 0;
	double CulmsPop = 0;
	double CumCarbonUsedPop = 0;
	double CumCstrPhase2 = 0;
	double CumCstrPhase3 = 0;
	double CumCstrPhase4 = 0;
	double CumCstrPhase5 = 0;
	double CumCstrPhase6 = 0;
	double CumDr = 0;
	double CumDrFin = 0;
	double CumEt = 0;
	double CumFTSWPhase2 = 0;
	double CumFTSWPhase3 = 0;
	double CumFTSWPhase4 = 0;
	double CumFTSWPhase5 = 0;
	double CumFTSWPhase6 = 0;
	double CumGrowthPop = 0;
	double CumIcPhase2 = 0;
	double CumIcPhase3 = 0;
	double CumIcPhase4 = 0;
	double CumIcPhase5 = 0;
	double CumIcPhase6 = 0;
	double CumIrrig = 0;
	double CumIrrigFin = 0;
	double CumLr = 0;
	double CumPAR = 0;
	double CumSupplyTot = 0;
	double CumTr = 0;
	double CumWReceived = 0;
	double CumWUse = 0;
	double CumWUseFin = 0;
	double DAF = 0;
	double DayLength = 0;
	double DeadLeafdrywtPop = 0;
	double Decli = 0;
	double DegresDuJour = 0;
	double DegresDuJourCor = 0;
	double DemLeafAreaPlant = 0;
	double DemPanicleFillPop = 0;
	double DemResInternodePop = 0;
	double DemStructInternodePlant = 0;
	double DemStructInternodePop = 0;
	double DemStructLeafPlant = 0;
	double DemStructLeafPop = 0;
	double DemStructPaniclePlant = 0;
	double DemStructPaniclePop = 0;
	double DemStructRootPlant = 0;
	double DemStructRootPop = 0;
	double DemStructSheathPop = 0;
	double DemStructTotPop = 0;
	double Density = 0;
	double Dr = 0;
	double DryMatAboveGroundPop = 0;
	double DryMatAboveGroundPopFin = 0;
	double DryMatAboveGroundTotPop = 0;
	double DryMatPanicleTotPop = 0;
	double DryMatResInternodePop = 0.;
	double DryMatResInternodePopOld = 0;
	double DryMatStructInternodePop = 0;
	double DryMatStructLeafPop = 0;
	double DryMatStructPaniclePop = 0;
	double DryMatStructRootPop = 0;
	double DryMatStructSheathPop = 0;
	double DryMatStemPop = 0;
	double DryMatStructTotPop = 0;
	double DryMatTotPop = 0;
	double DryMatTotPopFin = 0;
	double DryMatVegeTotPop = 0;
	double DurGermFlow = 0;
	double DurGermMat = 0;
	double DurPhase1 = 0;
	double DurPhase2 = 0;
	double DurPhase3 = 0;
	double DurPhase4 = 0;
	double DurPhase5 = 0;
	double DurPhase6 = 0;
	double EauDispo = 0;
	double ETM = 0;
	double ETo = 0;
	double ETR = 0;
	double Evap = 0;
	double EvapPot = 0;
	double FertSpikeNumPop = 0;
	double FloodwaterDepth = 0;
	double FloodwaterGain = 0;
	double FractionPlantHeightSubmer = 0;
	double FractionRootsLogged = 0;
	double FTSW = 0;
	double FtswMoy = 0;
	double FtswPhase2 = 0;
	double FtswPhase3 = 0;
	double FtswPhase4 = 0;
	double FtswPhase5 = 0;
	double FtswPhase6 = 0;
	double GainRootSystSoilSurfPop = 0;
	double GainRootSystVolPop = 0;
	double GrainFillingStatus = 0;
	double GrainFillingStatusFin = 0;
	double GrainYieldPanicle = 0;
	double GrainYieldPop = 0;
	double GrainYieldPopFin = 0;
	double GrowthDryMatPop = 0;
	double GrowthPop = 0;
	double GrowthResInternodePop = 0;
	double GrowthStructDeficit = 0;
	double GrowthStructInternodePop = 0;
	double GrowthStructLeafPop = 0;
	double GrowthStructPaniclePop = 0;
	double GrowthStructRootPop = 0;
	double GrowthStructSheathPop = 0;
	double GrowthStructTotPop = 0;
	double HarvestIndex = 0;
	double HaunGain = 0;
	double HaunIndex = 0;
	double HMoyCalc = 0;
	double Hum = 0;
	double Ic = 0;
	double IcCum = 0;
	double IcMean = 0;
	double IcPhase2 = 0;
	double IcPhase3 = 0;
	double IcPhase4 = 0;
	double IcPhase5 = 0;
	double IcPhase6 = 0;
	double IncreaseResInternodePop = 0;
	double InternodeResStatus = 0;
	double Irrigation = 0;
	double IrrigAutoDay = 0;
	double IrrigTotDay = 0;
	double Kce = 0;
	double KceReal = 0;
	double Kcl = 0;
	double Kcp = 0;
	double KcTot = 0;
	double Kr = 0;
	double KRolling = 0;
	double Lai = 0;
	double LaiDead = 0;
	double LaiFin = 0;
	double LastLeafLength = 0;
	double LastLeafLengthPot = 0;
	double LatRad = 0;
	double LeafDeathPop = 0;
	double LIRkdf = 0;
	double LIRkdfcl = 0;
	double Lr = 0;
	double LTRkdf = 0;
	double LTRkdfcl = 0;
	double MaxLai = 0;
	double MobiliLeafDeath = 0;
	double NumPhase = 0;
	double NumSsPhase = 0;
	double NurseryStatus = 0;
	double PanicleFilDeficit = 0;
	double PanicleFilPop = 0;
	double PanicleNumPlant = 0;
	double PanicleNumPop = 0;
	double PanicleSinkPop = 0;
	double PanStructMass = 0;
	double Par = 0;
	double PARIntercepte = 0;
	double PhaseStemElongation = 0;
	double PlantHeight = 0;
	double PlantLeafNumNew = 0;
	double PlantLeafNumTot = 0;
	double PlantWidth = 0;
	double ProfRu = 0;
	double RayExtra = 0;
	double RelPotLeafLength = 0;
	double ResCapacityInternodePop = 0;
	double ReservePopFin = 0;
	double ResInternodeMobiliDay = 0;
	double ResInternodeMobiliDayPot = 0;
	double RespMaintDebt = 0;
	double RespMaintTot = 0;
	double ResUtil = 0;
	double RgCalc = 0;
	double RgMax = 0;
	double RootFront = 0;
	double RootFrontOld = 0;
	double RootMassPerVol = 0;
	double RootShootRatio = 0;
	double RootSystSoilSurfPop = 0;
	double RootSystSoilSurfPopOld = 0;
	double RootSystVolPop = 0;
	double RootSystVolPopOld = 0;
	double RUE = 0;
	double RUEGreen = 0;
	double RuRac = 0;
	double RuSurf = 0;
	double SDJCorPhase4 = 0;
	double SeuilCstrMortality = 0;
	double SeuilTemp = 0;
	double SeuilTempSsPhase = 0;
	double SimAnthesis50 = 0;
	double SimEmergence = 0;
	double SimEndCycle = 0;
	double SimPanIni = 0;
	double SimStartGermin = 0;
	double SimStartMatu2 = 0;
	double SimStartPSP = 0;
	double Sla = 0;
	double SlaMitch = 0;
	double SlaNew = 0;
	double SommeDegresJourMax = 0;
	double SpikeNumPanicle = 0;
	double SpikeNumPop = 0;
	double SterilityCold = 0;
	double SterilityDrought = 0;
	double SterilityHeat = 0;
	double SterilityTot = 0;
	double SterilityTotFin = 0;
	double StockMacropores = 0;
	double StockRac = 0;
	double StockSurface = 0;
	double StockTotal = 0;
	double StressCold = 0;
	double StRuMax = 0;
	double SumDDPhasePrec = 0;
	double SumDegreDayCor = 0;
	double SumDegresDay = 0;
	double SumPP = 0;
	double SunDistance = 0;
	double SunPosi = 0;
	double SupplyTot = 0;
	double TempLai = 0;
	double TillerDeathPop = 0;
	double TmaxMoy = 0;
	double TminMoy = 0;
	double TMoyCalc = 0;
	double TMoyPrec = 0;
	double Tr = 0;
	double TrEff = 0;
	double TrEffInst = 0;
	double TrPot = 0;
	double ValRDE = 0;
	double ValRFE = 0;
	double ValRSurf = 0;
	double VPDCalc = 0;
	double VitesseRacinaire = 0;
	double VitesseRacinaireDay = 0;
	double VolMacropores = 0;
	double VolRelMacropores = 0;
	double WueEt = 0;
	double WueTot = 0;

	double RootLigninPop = 0;
	double SlaMitchAdjusted = 0;


	//Sorghum Mortality por
    std::array<double,5> tabCstr = {0,0,0,0,0};
//    double tabCstr[6] = {0,0,0,0,0,0};
	int tabCstrIndiceCourant = 0;
	int NbJourCompte = 0;

    double FirstDayIrrig = -1;

	vector<string> get_variable_names() {
		if (LOG == ECOTROP) {
			vector <string> n{ "Jour","NbJas","NumPhase","DegresDuJour","DegresDuJourCor",
				"Lai","MaxLai","FTSW","Cstr","RootFront","Eto","ETM","ETR","RUE",
				"ConversionEff","Ic","CulmsPerPlant","CulmsPop","CulmsPerHill",
				"GrainYieldPop","DryMatStructLeafPop","DryMatStructSheathPop",
				"DryMatStructRootPop","DryMatStructInternodePop","DryMatResInternodePop",
				"DryMatStructPaniclePop","DryMatStructTotPop","DryMatVegeTotPop",
				"DryMatPanicleTotPop","DryMatStemPop","DryMatAboveGroundPop","DryMatTotPop",
				"SterilityCold","SterilityHeat","SterilityDrought","SterilityTot","HarvestIndex",
				"PanicleNumPop","PanicleNumPlant","GrainYieldPanicle","SpikeNumPop",
				"SpikeNumPanicle","FertSpikeNumPop","GrainFillingStatus","PhaseStemElongation",
				"Sla","HaunIndex","ApexHeight","PlantHeight","PlantWidth","VitesseRacinaireDay",
				"Kcl","KRolling","LIRkdfcl","LTRkdfcl","AssimPot","Assim","RespMaintTot",
				"SupplyTot","AssimSurplus","AssimNotUsed","AssimNotUsedCum","IcMean","CstrMean",
				"TillerDeathPop","PlantLeafNumTot","DeadLeafdrywtPop","LaiDead",
				"ResCapacityInternodePop","InternodeResStatus","DayLength","Par","RgCalc",
				"VDPCalc","TMoyCalc","HMoyCalc","KceReal","StockTotal","EauDispo",
				"StockSurface","StockRac","RURac","Kcp","Kce","EvapPot","Evap","TrPot",
				"Tr","Lr","Dr","ParIntercepte","SumDegresDay","VitesseRacinaire",
				"CstrAssim","RayExtra","CumPAR","SumDegreDayCor","SumPP","SumDDPhasePrec",
				"SeuilTemp","RespMaintDebt","TMinMoy","TMaxMoy","FtswMoy","ResInternodeMobiliDay",
				"RootShootRatio","TrEffInst","TrEff","WueEt","WueTot","SlaMitch","FloodwaterDepth",
				"IrrigAutoDay","IrrigTotDay","FractionPlantHeightSubmer","Density","RootMassPerVol",
				"CumTr","CumEt","CumWUse","CumWReceived","CumIrrig","CumDr","CumLr","VolMacropores",
				"StockMacropores","IcCum","CstrCum","SlaNew","LTRkdf","LIRkdf","RelPotLeafLength",
				"ApexHeightGain","Haungain","Conversion","RootFrontOld","SimStartGermin",
				"SimEmergence","SimStartPSP","SimPanIni","SimAnthesis50","SimStartMatu2",
				"SimEndCycle","StressCold","CulmsPerPlantMax","CulmsPerHillMax","DurGermFlow",
				"DurGermMat","LaiFin","CulmsPerHillFin","CulmsPerPlantFin","GrainYieldPopFin",
				"DryMatAboveGroundPopFin","ReservePopFin","FractionRootsLogged","ResUtil","KcTot",
				"DryMatAboveGroundTotPop","DemResInternodePop","LastLeafLengthPot","LastLeafLength",
				"CumSupplyTot","DryMatResInternodePopOld","CumGrowthPop","GrowthPop",
				"A_DemStructLeaf","A_GrowthStructLeaf","A_DemStructTot","A_GrowthStructTot",
				"CoeffCO2Tr","CoeffCO2Assim","A_AssimSurplus","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CumCarbonUsedPop","DryMatTotPopFin","GrainFillingStatusFin","SterilityTotFin","CumIrrigFin","CumWUseFin","GrowthDryMatPop_V2_1"
				, "Conversion", "MonCumTr", "SlaMitchAdjusted"};
			return n;
		}
		else if (LOG == COMPLETE) {
            vector <string> n{ "ObsPlantDate","NbJAS","ApexHeight","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","CoeffStressLogging","Conversion","ConversionEff","Cstr","CstrAssim","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CstrPhase6","CulmsPerHill","CulmsPerHillFin","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantFin","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumDr","CumEt","CumIrrig","CumLr","CumPAR","cumRain","CumTr","CumWReceived","CumWUse","DAF","DayLength","DeadLeafdrywtPop","DegresDuJour","DegresDuJourCor","Dr","DryMatAboveGroundPop","DryMatAboveGroundPopFin","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatResInternodePopOld","DryMatStemPop","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStructTotPop","DryMatTotPop","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETP","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FloodwaterGain","FractionPlantHeightSubmer","FractionRootsLogged","FreshMatAbovegroundPop","FreshMatPanicleTotPop","FTSW","FtswMoy","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GrainFillingStatus","GrainFillingStatusFin","GrainMoisture","GrainYieldPanicle","GrainYieldPop","GrainYieldPopFin","HarvestIndex","HaunIndex","HMax","HMin","HMoy","HMoyCalc","Hum","Ic","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","Ins","InternodeResStatus","IrrigAutoDay","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LaiFin","LastLeafLength","LastLeafLengthPot","LeafDeathPop","LIRkdf","LIRkdfcl","Lodging","LodgingDay","LodgingIndex","LodgingPot","LodgingResistance","LodgingResistance2","Lr","LTRkdf","LTRkdfcl","MaxLai","NumPhase","PanicleFilPop","PanicleNumPlant","PanicleNumPop","PanStructMass","Par","PARIntercepte","PlantHeight","PlantWidth","Pluie","ProfRu","ResCapacityInternodePop","RespMaintTot","ResUtil","Rg","RgCalc","RootFront","RootLigninPop","RootMassPerVol","RootShootRatio","RootSystSoilSurfPop","RootSystSoilSurfPopOld","RUE","RUEGreen","RuRac","RuSurf","SeuilCstrMortality","SimAnthesis50","Sla","SlaMitch","SpikeNumPanicle","SpikeNumPop","StemDiaBase","StemDiaMean","StemSurfMean","StemVigor","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","StRuMax","SupplyTot","TempLai","TillerDeathPop","TMax","TMin","TMoy","TMoyCalc","Tr","TrEff","TrEffInst","TrPot","ValRDE","ValRFE","ValRSurf","VitesseRacinaire","VitesseRacinaireDay","VolMacropores","VolRelMacropores","VPDCalc","Vt","WueEt","WueTot" };
			return n;
		}
		else if (LOG == SMALL) {
            vector <string> n{ "ObsPlantDate","NbJAS","Conversion","ConversionEff","Cstr","CstrAssim","CstrMean","CulmsPerHill","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumDr","CumEt","CumIrrig","CumLr","CumPAR","cumRain","CumTr","CumWReceived","CumWUse","DayLength","DeadLeafdrywtPop","DegresDuJour","DegresDuJourCor","Dr","DryMatAboveGroundPop","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatStemPop","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStructTotPop","DryMatTotPop","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETP","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GrainFillingStatus","GrainYieldPanicle","GrainYieldPop","HarvestIndex","HaunIndex","HMax","HMin","HMoy","Hum","Ic","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","Ins","InternodeResStatus","Irrigation","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LastLeafLength","LastLeafLengthPot","LeafDeathPop","LIRkdfcl","Lr","LTRkdfcl","MaxLai","NumPhase","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PlantHeight","PlantWidth","Pluie","ProfRu","ResCapacityInternodePop","ReservePopFin","RespMaintTot","ResUtil","Rg","RgCalc","RootFront","RootMassPerVol","RootShootRatio"
			};
			return n;
		}
		return vector<string>();
	}

	vector<double> get_variable_values() {
		if (LOG == ECOTROP) {
            vector <double> r{ NbJAS,NumPhase,DegresDuJour,DegresDuJourCor,Lai,MaxLai,FTSW,Cstr,RootFront,ETo,ETM,ETR,RUE,ConversionEff,Ic,CulmsPerPlant,CulmsPop,CulmsPerHill,GrainYieldPop,DryMatStructLeafPop,DryMatStructSheathPop,DryMatStructRootPop,DryMatStructInternodePop,DryMatResInternodePop,DryMatStructPaniclePop,DryMatStructTotPop,DryMatVegeTotPop,DryMatPanicleTotPop,DryMatStemPop,DryMatAboveGroundPop,DryMatTotPop,SterilityCold,SterilityHeat,SterilityDrought,SterilityTot,HarvestIndex,PanicleNumPop,PanicleNumPlant,GrainYieldPanicle,SpikeNumPop,SpikeNumPanicle,FertSpikeNumPop,GrainFillingStatus,PhaseStemElongation,Sla,HaunIndex,ApexHeight,PlantHeight,PlantWidth,VitesseRacinaireDay,Kcl,KRolling,LIRkdfcl,LTRkdfcl,AssimPot,Assim,RespMaintTot,SupplyTot,AssimSurplus,AssimNotUsed,AssimNotUsedCum,IcMean,CstrMean,TillerDeathPop,PlantLeafNumTot,DeadLeafdrywtPop,LaiDead,ResCapacityInternodePop,InternodeResStatus,DayLength,Par,RgCalc,VPDCalc,TMoyCalc,HMoyCalc,KceReal,StockTotal,EauDispo,StockSurface,StockRac,RuRac,Kcp,Kce,EvapPot,Evap,TrPot,Tr,Lr,Dr,PARIntercepte,SumDegresDay,VitesseRacinaire,CstrAssim,RayExtra,CumPAR,SumDegreDayCor,SumPP,SumDDPhasePrec,SeuilTemp,RespMaintDebt,TminMoy,TmaxMoy,FtswMoy,ResInternodeMobiliDay,RootShootRatio,TrEff,WueEt,WueTot,ConversionEff,SlaMitch,FloodwaterDepth,IrrigAutoDay,IrrigTotDay,FractionPlantHeightSubmer,Density,RootMassPerVol,CumEt,CumWUse,cumRain,CumWReceived,CumDr,CumLr,TrEffInst,VolMacropores,StockMacropores,IcCum,CstrCum,SlaNew,LTRkdf,LIRkdf,RelPotLeafLength,ApexHeightGain,HaunGain,Conversion,RootFrontOld,SimStartGermin,SimEmergence,SimStartPSP,SimPanIni,SimAnthesis50,SimStartMatu2,SimEndCycle,StressCold,CulmsPerPlantMax,CulmsPerHillMax,DurGermFlow,DurGermMat,LaiFin,CulmsPerHillFin,CulmsPerPlantFin,GrainYieldPopFin,DryMatAboveGroundPopFin,ReservePopFin,FractionRootsLogged,ResUtil,KcTot,DryMatAboveGroundTotPop,DemResInternodePop,LastLeafLengthPot,LastLeafLength,CumSupplyTot,DryMatResInternodePopOld,CumGrowthPop,GrowthPop,A_DemStructLeaf,A_GrowthStructLeaf,A_DemStructTot,A_GrowthStructTot,CoeffCO2Tr,CoeffCO2Assim,A_AssimSurplus,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,CumCarbonUsedPop,DryMatTotPopFin,GrainFillingStatusFin,SterilityTotFin,CumIrrigFin,CumWUseFin,GrowthDryMatPop
				, Conversion, CumTr, SlaMitchAdjusted };
			return r;
		}
		else if (LOG == COMPLETE) {
            vector <double> r{ NbJAS,ApexHeight,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CoeffStressLogging,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CulmsPerHillFin,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantFin,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,cumRain,CumTr,CumWReceived,CumWUse,DAF,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Dr,DryMatAboveGroundPop,DryMatAboveGroundPopFin,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatResInternodePopOld,DryMatStemPop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FloodwaterGain,FractionPlantHeightSubmer,FractionRootsLogged,FreshMatAbovegroundPop,FreshMatPanicleTotPop,FTSW,FtswMoy,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainFillingStatusFin,GrainMoisture,GrainYieldPanicle,GrainYieldPop,GrainYieldPopFin,HarvestIndex,HaunIndex,HMax,HMin,HMoy,HMoyCalc,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,IrrigAutoDay,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LaiFin,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdf,LIRkdfcl,Lodging,LodgingDay,LodgingIndex,LodgingPot,LodgingResistance,LodgingResistance2,Lr,LTRkdf,LTRkdfcl,MaxLai,NumPhase,PanicleFilPop,PanicleNumPlant,PanicleNumPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantWidth,Pluie,ProfRu,ResCapacityInternodePop,RespMaintTot,ResUtil,Rg,RgCalc,RootFront,RootLigninPop,RootMassPerVol,RootShootRatio,RootSystSoilSurfPop,RootSystSoilSurfPopOld,RUE,RUEGreen,RuRac,RuSurf,SeuilCstrMortality,SimAnthesis50,Sla,SlaMitch,SpikeNumPanicle,SpikeNumPop,StemDiaBase,StemDiaMean,StemSurfMean,StemVigor,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,StRuMax,SupplyTot,TempLai,TillerDeathPop,TMax,TMin,TMoy,TMoyCalc,Tr,TrEff,TrEffInst,TrPot,ValRDE,ValRFE,ValRSurf,VitesseRacinaire,VitesseRacinaireDay,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot };
			return r;
		}
		else if (LOG == SMALL) {
            vector <double> r{ NbJAS,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CulmsPerHill,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,cumRain,CumTr,CumWReceived,CumWUse,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Dr,DryMatAboveGroundPop,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatStemPop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainYieldPanicle,GrainYieldPop,HarvestIndex,HaunIndex,HMax,HMin,HMoy,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,Irrigation,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdfcl,Lr,LTRkdfcl,MaxLai,NumPhase,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantWidth,Pluie,ProfRu,ResCapacityInternodePop,ReservePopFin,RespMaintTot,ResUtil,Rg,RgCalc,RootFront,RootMassPerVol,RootShootRatio};
			return r;
		}

		return vector<double>();
	}

	void reset_variables() {
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
		RootLigninPop = 0;

		//Sorghum Mortality por
        tabCstr = {0,0,0,0,0};
		tabCstrIndiceCourant = 0;
		NbJourCompte = 0;
//        FirstDayIrrig = -1;
	}

	void init_all_variables() {
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
		SlaMitchAdjusted = 0;
		//Sorghum Mortality por
        tabCstr = {0,0,0,0,0};
		tabCstrIndiceCourant = 0;
		NbJourCompte = 0;
        FirstDayIrrig = -1;
	}

	void init_culture_variables() {
		try {
			NumPhase = 0;
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
			//Sorghum Mortality por
            tabCstr = {0,0,0,0,0};
			tabCstrIndiceCourant = 0;
			NbJourCompte = 0;
		}
		catch (...) {
			error_message("RS_InitiationCulture", URisocas);
		}
	}

	void kill_crop_variables() {
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
		//Sorghum Mortality por
        tabCstr = {0,0,0,0,0};
		tabCstrIndiceCourant = 0;
		NbJourCompte = 0;
	}

	void daily_reset_variables() {
        if (FirstDayIrrig == -1 && IrrigAutoDay > 0) {
            FirstDayIrrig = IrrigAutoDay;
        }
		FractionRootsLogged = 0;
		IrrigTotDay = 0;
		IrrigAutoDay = 0;
	}

};
