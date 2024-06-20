#ifndef PROCESSES_H
#define PROCESSES_H
//#include <QDebug>
#include <samara_defines.h>
#include <array>
namespace samara {

int MonCompteur;
void CalculeLaMoyenne(double const &LaValeur,   int const &LeCompteur,
                      double &LaMoyenne) {
    /*try*/ {
        LaMoyenne = (LaMoyenne * (LeCompteur - 1) + LaValeur) / (LeCompteur);

    } /*catch (...)*/ {
        error_message("CalculeLaMoyenne", URiz);
    }
}

void RS_Transplanting_V2_2( double const& NumPhase, double const& DensityNursery, double const& DensityField, double const& DurationNursery,
                           double const& PlantsPerHill, double const& Transplanting,
                           double & NurseryStatus, double & ChangeNurseryStatus, double & CounterNursery,
                           double & Density, double & DryMatStructLeafPop, double & DryMatStructSheathPop, double & DryMatStructRootPop,
                           double & DryMatStructInternodePop, double & DryMatStructPaniclePop, double & DryMatResInternodePop,
                           double & DeadLeafDrywtPop, double & ResCapacityInternodePop)
{
    double DensityChange;

    /*try*/ {
        DensityChange = DensityField / ( DensityNursery / PlantsPerHill );
        if(  ( ( Transplanting == 1 ) && ( NumPhase >= 1 ) ) )
        {
            CounterNursery = CounterNursery + 1;
        }
        if(  ( ( Transplanting == 1 ) && ( CounterNursery < DurationNursery ) &&
             ( ChangeNurseryStatus == 0 ) ) )
        {
            NurseryStatus = 0;
            ChangeNurseryStatus = 0;
        }
        else
        {
            if(  ( ( Transplanting == 1 ) && ( CounterNursery >= DurationNursery ) &&
                 ( ChangeNurseryStatus == 0 ) && ( NurseryStatus == 0 ) ) )
            {
                NurseryStatus = 1;
                ChangeNurseryStatus = 1;
            }
            else
            {
                NurseryStatus = 1;
                ChangeNurseryStatus = 0;
            }
        }
        if(  NurseryStatus == 1  )
        {
            Density = DensityField;
        }
        else
        {
            Density = DensityNursery / PlantsPerHill;
        }
        if( ChangeNurseryStatus == 1 )
        {
            DryMatStructLeafPop = DryMatStructLeafPop * DensityChange;
            DryMatStructSheathPop = DryMatStructSheathPop * DensityChange;
            DryMatStructRootPop = DryMatStructRootPop * DensityChange;
            DryMatStructInternodePop = DryMatStructInternodePop * DensityChange;
            DryMatStructPaniclePop = DryMatStructPaniclePop * DensityChange;
            DryMatResInternodePop = DryMatResInternodePop * DensityChange;
            DeadLeafDrywtPop = DeadLeafDrywtPop * DensityChange;
            ResCapacityInternodePop = ResCapacityInternodePop * DensityChange;
        }

    } /*catch (...)*/ {error_message( "RS_Transplanting_V2_2" , URisocas );
    }
}


void RS_Transplanting_V2(double const &NumPhase, double const &DensityNursery, double const &DensityField,
                         double const &DurationNursery, double const &PlantsPerHill, double const &Transplanting,
                         double &NurseryStatus, double &ChangeNurseryStatus, double &CounterNursery, double &Density,
                         double &DryMatStructLeafPop, double &DryMatStructSheathPop, double &DryMatStructRootPop,
                         double &DryMatStructInternodePop, double &DryMatStructPaniclePop, double &DryMatResInternodePop) {
    double DensityChange;

    /*try*/ {
        DensityChange = DensityField / (DensityNursery / PlantsPerHill);
        if (((Transplanting == 1) && (NumPhase >= 1))) {
            CounterNursery = CounterNursery + 1;
        }
        if (((Transplanting == 1) && (CounterNursery < DurationNursery) &&
             (ChangeNurseryStatus == 0))) {
            NurseryStatus = 0;
            ChangeNurseryStatus = 0;
        } else {
            if (((Transplanting == 1) && (CounterNursery >= DurationNursery) &&
                 (ChangeNurseryStatus == 0) && (NurseryStatus == 0))) {
                NurseryStatus = 1;
                ChangeNurseryStatus = 1;
            } else {
                NurseryStatus = 1;
                ChangeNurseryStatus = 0;
            }
        }
        if (NurseryStatus == 1) {
            Density = DensityField;
        } else {
            Density = DensityNursery / PlantsPerHill;
        }
        if (ChangeNurseryStatus == 1) {
            DryMatStructLeafPop = DryMatStructLeafPop * DensityChange;
            DryMatStructSheathPop = DryMatStructSheathPop * DensityChange;
            DryMatStructRootPop = DryMatStructRootPop * DensityChange;
            DryMatStructInternodePop = DryMatStructInternodePop * DensityChange;
            DryMatStructPaniclePop = DryMatStructPaniclePop * DensityChange;
            DryMatResInternodePop = DryMatResInternodePop * DensityChange;
        }

    } /*catch (...)*/ {
        error_message("RS_Transplanting_V2", URisocas);
    }
}


void EvolPhenoPSPStress(double const &SumPP, double const &PPsens, double const &SommeDegresJour,
                        double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR,
                        double const &SeuilTempMatu1, double const &SeuilTempMatu2, double const &StockSurface,
                        double const &PourcRuSurfGermi, double const &RuSurf, double const &/*DateDuJour*/, double const &/*DateSemis*/, double const &stRu,
                        double &NumPhase, double &SommeDegresJourPhasePrec, double &SeuilTempPhaseSuivante, double &ChangePhase, double &SeuilTempSousPhaseSuivante, double &ChangeSousPhase, double &NumSousPhase)
/*Procedure speciale pour inclure le module photoperiodique de Vaksman & Dingkuhn
qui fonctionne en degres jours et declanche IP lorsque SumPP est inferieur à PPsens*/
//JCC le 21/09/05
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodique au début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
{
    bool ChangementDePhase; bool ChangementDeSousPhase;
    // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)


    /*try*/ {
    ChangePhase = 0;
    ChangeSousPhase = 0;
    // l'initialisation quotidienne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    if ((std::trunc(NumPhase) == 0)) {     // la culture a été semée mais n'a pas germé
        if (((StockSurface >= PourcRuSurfGermi * RuSurf) || (stRu > StockSurface))) {
            // on commence ds les conditions favo aujourd'hui
            NumPhase = 1;
            ChangePhase = 1;
            SeuilTempPhaseSuivante = SeuilTempLevee;
        }
    } // fin du if NumPhase=0
    else {
        // vérification d'un éventuel changement de phase
        if (((std::trunc(NumPhase) == 1) && (SommeDegresJour >= SeuilTempPhaseSuivante)))           //si on change de phase de BVP à PSP aujourd'hui
            ChangementDePhase = true;
        else {
            //sinon
            if ((std::trunc(NumPhase) != 3)) {
                ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante);
            } else {
                ChangementDePhase = (SumPP <= PPsens);
                // true=on quittera la phase photopériodique
            }
        }
        // on a changé de phase
        if (ChangementDePhase) {
            ChangePhase = 1;
            NumPhase = NumPhase + 1;
            SommeDegresJourPhasePrec = SeuilTempPhaseSuivante;
            // utilisé dans EvalConversion
            switch ((int)std::trunc(NumPhase)) {
            case 2:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempBVP;
                // BVP Developpement vegetatif
                break;
            case 4: {
                // gestion de l'initialisation des sous-phases
                SeuilTempSousPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR
                                                                          / 4; // initialisation de la somme des DJ de la 1ère sous phase
                NumSousPhase = 1; // initialisation du n° de sous phase
                MonCompteur = 0; // on est bien le 1er jour de la 1ere sous phase
                ChangeSousPhase = 1;
                // on est bien un jour de changement de sous phase (en locurence, la première...)
                // gestion du seuil de la phase suivante
                SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR;
                // RPR Stade initiation paniculaire
                break;
            }
            case 5:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu1;
                // Matu1 remplissage grains
                break;
            case 6:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu2;
                // Matu2 dessication
                break;
            } // Case NumPhase
        } // end change
        // gestion des sous-phases de la phase RPR (4)
        if ((std::trunc(NumPhase) == 4)) {
            ChangementDeSousPhase = (SommeDegresJour >=
                                     SeuilTempSousPhaseSuivante);
            if (ChangementDeSousPhase) {
                SeuilTempSousPhaseSuivante = SeuilTempSousPhaseSuivante + SeuilTempRPR
                                                                              / 4;
                NumSousPhase = NumSousPhase + 1;
                MonCompteur = 1;
                ChangeSousPhase = 1;
            } else
                ++MonCompteur;
        } // fin du if std::trunc(NumPhase)=4 then
    }

} /*catch (...)*/ {
    error_message("EvolPhenoStress | N umPhase: " + FloatToStr(NumPhase) +
                      " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                      " SeuilTempPhaseSuivante: " + FloatToStr(SeuilTempPhaseSuivante), URiz);
}
}


void RS_EvalSimAnthesis50(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                          double &SimAnthesis50) {
    /*try*/ {
        if ((NumPhase == 5) && (ChangePhase == 1)) {
            SimAnthesis50 = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimAnthesis50", URisocas);
    }
}

void RS_EvalDateGermination(double const &NumPhase, double const &ChangePhase,
                            double &NbDaysSinceGermination) {
    /*try*/ {
        if (((NumPhase < 1) || ((NumPhase == 1) && (ChangePhase == 1)))) {
            NbDaysSinceGermination = 0;
        } else {
            NbDaysSinceGermination = NbDaysSinceGermination + 1;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDateGermination", URisocas);
    }
}

void RS_EvalColdStress(double const &KCritStressCold1, double const &KCritStressCold2, double const &TMin,
                       double &StressCold) {
    /*try*/ {
        StressCold = 1 - max(0., min(1., KCritStressCold1 / (KCritStressCold1 -
                                                             KCritStressCold2) - TMin / (KCritStressCold1 - KCritStressCold2)));
        StressCold = max(0.00001, StressCold);

    } /*catch (...)*/ {
        error_message("RS_EvalColdStress", URisocas);
    }
}


void RS_EvalSimEmergence(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                         double &SimEmergence) {
    /*try*/ {
        if ((NumPhase == 2) && (ChangePhase == 1)) {
            SimEmergence = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimEmergence", URisocas);
    }
}

void RS_EvalSimPanIni(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                      double &SimPanIni) {
    /*try*/ {
        if ((NumPhase == 4) && (ChangePhase == 1)) {
            SimPanIni = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimPanIni", URisocas);
    }
}


void RS_EvalSimStartGermin(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                           double &SimStartGermin) {
    /*try*/ {
        if ((NumPhase == 1) && (ChangePhase == 1)) {
            SimStartGermin = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimStartGermin", URisocas);
    }
}


void RS_EvalSimStartMatu2(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                          double &SimStartMatu2) {
    /*try*/ {
        if ((NumPhase == 6) && (ChangePhase == 1)) {
            SimStartMatu2 = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimStartMatu2", URisocas);
    }
}


void RS_EvalSimStartPSP(double const &NumPhase, double const &ChangePhase, double const &NbJas,
                        double &SimStartPSP) {
    /*try*/ {
        if ((NumPhase == 3) && (ChangePhase == 1)) {
            SimStartPSP = NbJas;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSimStartPSP", URisocas);
    }
}


void RS_EvalDegresJourVitMoy_V2(double const &NumPhase, double const &TMax, double const &TMin, double const &TBase, double const &TOpt1, double const &TOpt2, double const &TLet, double const &cstr, double const &DEVcstr, double const &StressCold,
                                double &DegresDuJour, double &DegresDuJourCor) {
    double v; double v1; double v3;
    double S1; double S2; double S3;
    double Tn; double Tx;

    /*try*/ {
        if ((TMax != TMin)) {
            if (((TMax <= TBase) || (TMin >= TLet))) {
                v = 0;
            } else {
                Tn = max(TMin, TBase);
                Tx = min(TMax, TLet);
                v1 = ((Tn + min(TOpt1, Tx)) / 2 - TBase) / (TOpt1 - TBase);
                S1 = v1 * max(0., min(TOpt1, Tx) - Tn);
                S2 = 1 * max(0., min(Tx, TOpt2) - max(Tn, TOpt1));
                v3 = (TLet - (max(Tx, TOpt2) + max(TOpt2, Tn)) / 2) / (TLet - TOpt2);
                S3 = v3 * max(0., Tx - max(TOpt2, Tn));
                v = (S1 + S2 + S3) / (TMax - TMin);
            }
        } else {
            if ((TMax < TOpt1)) {
                v = (TMax - TBase) / (TOpt1 - TBase);
            } else {
                if ((TMax < TOpt2)) {
                    v = 1;
                } else {
                    v = (TLet - TMax) / (TLet - TOpt2);
                }
            }
        }
        DegresDuJour = v * (TOpt1 - TBase);
        if ((NumPhase > 1) && (NumPhase < 5)) {
            DegresDuJourCor = DegresDuJour * std::pow(max(cstr, 0.00000001), DEVcstr);
        } else {
            DegresDuJourCor = DegresDuJour;
        }
//        DegresDuJourCor = DegresDuJourCor * StressCold;

    } /*catch (...)*/ {
        error_message("RS_EvalDegresJourVitMoy | TMax=" + FloatToStr(TMax) +
                          " TMin=" + FloatToStr(TMin) + "TBase=" + FloatToStr(TBase) + " TOpt1=" +
                          FloatToStr(TOpt1) +
                          " TOpt2=" + FloatToStr(TOpt2) + " TL=" + FloatToStr(TLet) +
                          " DegresDuJour=" +
                          FloatToStr(DegresDuJour) + " DegreDuJourCor=" +
                          FloatToStr(DegresDuJourCor), URisocas);
    }
}


void RS_EvalSDJPhase4(double const &NumPhase, double const &DegreDuJourCor,
                      double &SDJPhase4) {
    /*try*/ {
        if ((NumPhase == 4)) {
            SDJPhase4 = SDJPhase4 + DegreDuJourCor;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSDJPhase4", URisocas);
    }
}

void RS_EvalDAF_V2(double const &NumPhase, double &DAF) {
    /*try*/ {
        if ((NumPhase > 4)) {
            DAF = DAF + 1;
        }
    } /*catch (...)*/ {
        error_message("RS_EvalDAF_V2", URisocas);
    }
}

void RS_EvalDAF_V2_lodging(double const &NumPhase, double const &DegresDuJour,
                           double const &SDJMatu1, double const &SDJMatu2,
                           double &DAF, double &MatuSDJ, double &MatuProgress) {
    /*try*/ {
        if ((NumPhase > 4)) {
            DAF = DAF + 1;
            MatuSDJ = MatuSDJ + DegresDuJour; //Please don’t forget to initialize MatuSDJ to zero!
            MatuProgress = MatuSDJ / (SDJMatu1 + SDJMatu2);
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDAF_V2", URisocas);
    }
}


// -----------------------------------------------------------------------------
// This new module serves to simulate the number of leaves produced on the main
// stem, needed to estimate upper limits to LAI and demand for assimilates.
// The basis is cumulation of DegresDuJourCor, with the introduction of a new
// variable called HaunIndex, which indicates the number of leaves already
// produced. The new parameter Phyllo (in degree days, typical varietal values
// between 20 and 60) sets the rhythm of leaf development.
// -----------------------------------------------------------------------------

void RS_Phyllochron(double const &NumPhase, double const &DegresDuJourCor, double const &Phyllo, double const &RelPhylloPhaseStemElong,
                    double &PhaseStemElongation, double &HaunGain, double &HaunIndex) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((((NumPhase > 3) || (HaunIndex > 20)) && (NumPhase < 5))) {
                PhaseStemElongation = 1;
            } else {
                PhaseStemElongation = 0;
            }
            if (PhaseStemElongation == 0) {
                HaunGain = DegresDuJourCor / Phyllo;
                if ((HaunIndex < 3)) {
                    HaunGain = HaunGain * 2;
                }
            } else {
                if (PhaseStemElongation == 1) {
                    HaunGain = RelPhylloPhaseStemElong * (DegresDuJourCor / Phyllo);
                }
            }
            HaunIndex = HaunIndex + HaunGain;
        } else {
            HaunGain = 0;
            PhaseStemElongation = 0;
        }

    } /*catch (...)*/ {
        error_message("RS_Phyllochron", URisocas);
    }
}


// -----------------------------------------------------------------------------
// Calcul de la hauteur du couvert en fonction de DJ et cstr
// We introduce the new state variables ApexHeight and PlantHeight, which are
// also output variables. ApexHeight is the sum of Internode lengths on the main
// culm. PlantHeight is (ApexHeight + fn(HaunIndex)) because the leaves
// contribute to plant height. We introduce as new parameters InternodeLengthMax
// and LeafLengthMax.
// -----------------------------------------------------------------------------

void RS_EvolHauteur_SDJ_cstr_V2_1_lodging(double const &PhaseStemElongation, double const &CoeffInternodeNum,
                                          double const &HaunGain, double const &cstr, double const &InternodeLengthMax,
                                          double const &RelPotLeafLength, double const &LeafLengthMax, double const &/*CulmsPerHill*/,
                                          double const &IcMean, double const &Kdf, double const &Ic, double const &/*WtRatioLeafSheath*/, double const &StressCold, double const &CstrMean,
                                          double &ApexHeightGain, double &ApexHeight, double &PlantHeight, double &PlantWidth) {
    double CorrectedCstrMean;

    /*try*/ {
        if ((PhaseStemElongation == 1)) {
            ApexHeightGain = HaunGain * min(pow(min(Ic, 1.), 0.2), cstr) * StressCold
                             * InternodeLengthMax;
            ApexHeightGain = ApexHeightGain * CoeffInternodeNum;
        } else {
            ApexHeightGain = 0;
        }
        ApexHeight = ApexHeight + ApexHeightGain;
        if ((CstrMean <= 0)) {
            CorrectedCstrMean = 1;
        } else {
            CorrectedCstrMean = CstrMean;
        }
        PlantHeight = ApexHeight + (1.2 * (1 - Kdf) * RelPotLeafLength *
                                    LeafLengthMax * sqrt(IcMean) * CorrectedCstrMean);
        PlantWidth = std::pow(Kdf, 1.5) * 2 * sqrt(IcMean) * RelPotLeafLength * LeafLengthMax;/*DELETED LB*/ /**
      Min(1.4, (1 + 0.1 * (CulmsPerHill - 1)));*/

    } /*catch (...)*/ {
        error_message("RS_EvolHauteur_SDJ_cstr_V2_1_lodging", URisocas);
    }
}

void RS_EvolHauteur_SDJ_cstr_V2_1(double const &PhaseStemElongation, double const &CoeffInternodeNum,
                                  double const &HaunGain, double const &cstr, double const &InternodeLengthMax,
                                  double const &RelPotLeafLength, double const &LeafLengthMax, double const &/*CulmsPerHill*/, double const &IcMean, double const &Kdf, double const &Ic, double const &WtRatioLeafSheath, double const &StressCold, double const &CstrMean,
                                  double &ApexHeightGain, double &ApexHeight, double &PlantHeight, double &PlantWidth) {
    double CorrectedCstrMean;

    /*try*/ {
        if ((PhaseStemElongation == 1)) {
            ApexHeightGain = HaunGain * min(pow(min(Ic, 1.), 0.5), cstr) * StressCold
                             * InternodeLengthMax;
            ApexHeightGain = ApexHeightGain * CoeffInternodeNum;
        } else {
            ApexHeightGain = 0;
        }
        ApexHeight = ApexHeight + ApexHeightGain;
        if ((CstrMean <= 0)) {
            CorrectedCstrMean = 1;
        } else {
            CorrectedCstrMean = CstrMean;
        }
        PlantHeight = ApexHeight + (1.5 * (1 - Kdf) * RelPotLeafLength *
                                    LeafLengthMax * sqrt(IcMean) * CorrectedCstrMean * (1 + 1 /
                                                                                                WtRatioLeafSheath));
        PlantWidth = std::pow(Kdf, 1.5) * 2 * sqrt(IcMean) * RelPotLeafLength * LeafLengthMax;/*DELETED LB*/ /**
      Min(1.4, (1 + 0.1 * (CulmsPerHill - 1)));*/

    } /*catch (...)*/ {
        error_message("RS_EvolHauteur_SDJ_cstr_V2_1", URisocas);
    }
}


void RS_EvolKcpKceBilhy(double const &LtrKdfcl, double const &KcMax, double const &Mulch,
                        double const &Transplanting, double const &NurseryStatus,
                        double &Kcp, double &Kce, double &KcTot) {
    /*try*/ {
        Kcp = min((1 - LtrKdfcl) * KcMax, KcMax);
        Kcp = min(Kcp, KcMax);
        Kce = LtrKdfcl * 1 * (Mulch / 100);
        KcTot = Kcp + Kce;

        if (Transplanting == 1 && NurseryStatus == 0) {
            Kcp = 0;
            Kce = 1;
        }

    } /*catch (...)*/ {
        error_message("RS_BilhyEvolKcpLai", URisocas);
    }
}


void RS_EvalEvapPot(double const &Etp, double const &Kce,   double &EvapPot) {
    /*try*/ {
        EvapPot = Kce * Etp;

    } /*catch (...)*/ {
        error_message("RS_EvalEvapPot", URisocas);
    }
}


void RS_EvolEvapSurfRFE_RDE_V2_1(double const &NumPhase, double const &Kce, double const &EvapPot,
                                 double const &CapaREvap, double const &CapaRDE, double const &CapaRFE,
                                 double const &RuRac, double const &RuSurf, double const &/*BundHeight*/, double const &EpaisseurSurf, double const &EpaisseurProf, double const &RootFront, double const &ResUtil,
                                 double &Evap, double &ValRSurf, double &ValRFE, double &ValRDE, double &StockRac, double &StockTotal, double &StockSurface, double &Kr, double &KceReal, double &FloodwaterDepth, double &StockMacropores) {
    double ValRSurfPrec; double EvapRU;
    double Evap1; double Evap2;

    /*try*/ {

        // Evaporation in absence of surface water
        if (((StockMacropores + FloodwaterDepth) == 0) /*DELETED JUNE 19*//*or (NumPhase = 0)*/) {
            ValRSurfPrec = ValRSurf;
            // ValRSurf est l'eau contenue dans les réservoirs Revap (non transpirable) et RDE (transpirable et évaporable
            if ((ValRFE > 0)) {
                if ((ValRFE < EvapPot)) {
                    Evap1 = ValRFE;
                    Evap2 = max(0., min(ValRSurf, ((EvapPot - ValRFE) * ValRSurf) /
                                                      (CapaREvap + CapaRDE)));     // borné à 0 et ValRSurf le 27/04/05
                } else {
                    Evap1 = EvapPot;
                    Evap2 = 0;
                }
            } else {
                Evap1 = 0;
                Evap2 = max(0., min(ValRSurf, EvapPot * ValRSurf / (CapaREvap +
                                                                    CapaRDE)));    // borné à 0 et ValRSurf le 27/04/05
            }
            Evap = Evap1 + Evap2;
            ValRFE = ValRFE - Evap1;
            ValRSurf = ValRSurf - Evap2;
            ValRDE = max(0., ValRSurf - CapaREvap);
            if ((EvapPot == 0)) {
                Kr = 0;
            } else {
                Kr = Evap / EvapPot;
            }
            // part de l'évaporation prélevée dans les réservoirs RFE et RDE
            if ((ValRSurf >= CapaREvap)) {
                EvapRU = Evap;
            } else {
                if ((ValRSurfPrec <= CapaREvap)) {
                    EvapRU = Evap1;
                } else {
                    EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
                }
            }

            //Evaporation de Ru et Rur, MAJ
            if ((RuRac <= RuSurf)) {
                // quand les racines n'ont pas dépassé la première couche
                StockRac = max(0., StockRac - EvapRU * RuRac / RuSurf);
            } else {
                StockRac = max(0., StockRac - EvapRU);
            }
            StockTotal = StockTotal - EvapRU;
            StockRac = min(StockRac, StockTotal);
            // Ajout JCS 29/06/2009
            KceReal = Kce * Kr;
        }
        // Surface water evaporation during crop cycle
        if ((StockMacropores + FloodwaterDepth > 0) && (NumPhase > 0)) {
            Evap = EvapPot;
            ValRSurf = CapaREvap + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
                                                                       + EpaisseurProf));
            ValRFE = CapaRFE + StockMacropores * (EpaisseurSurf / (EpaisseurSurf +
                                                                   EpaisseurProf));
            ValRDE = CapaRDE;
            StockRac = RuRac + StockMacropores * (RootFront / (EpaisseurSurf +
                                                               EpaisseurProf));
            StockSurface = RuSurf + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
                                                                        + EpaisseurProf));
            StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
                         StockMacropores;
            StockRac = min(StockRac, StockTotal);
            Kr = 1;
            KceReal = Kce;
        }
        /*NEW JUNE 19*/
        // Surface water evaporation before and after crop cycle
        if ((StockMacropores + FloodwaterDepth > 0) && (NumPhase == 0)) {
            Evap = EvapPot;
            FloodwaterDepth = FloodwaterDepth - Evap;
            if ((FloodwaterDepth < 0)) {
                StockMacropores = StockMacropores + FloodwaterDepth;
                FloodwaterDepth = 0;
                StockMacropores = max(StockMacropores, 0.);
            }
            ValRSurf = CapaREvap + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
                                                                       + EpaisseurProf));
            ValRFE = CapaRFE + StockMacropores * (EpaisseurSurf / (EpaisseurSurf +
                                                                   EpaisseurProf));
            ValRDE = CapaRDE;
            StockSurface = RuSurf + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
                                                                        + EpaisseurProf));
            StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
                         StockMacropores;
            Kr = 1;
            KceReal = Kce;
        }


    } /*catch (...)*/ {
        error_message("RS_EvolEvapSurfRFE_RDE_V2_1", URisocas);
    }
}


void RS_EvalFTSW_V2(double const &RuRac, double const &StockTotal, double const &StockMacropores, double const &StRuMax,
                    double &StockRac, double &FTSW) {
    /*try*/ {
        StockRac = min(StockRac, (RuRac + (StockMacropores * RuRac / StRuMax)));
        StockRac = min(StockRac, StockTotal);
        if ((RuRac > 0)) {
            FTSW = StockRac / RuRac;
        } else {
            FTSW = 0;
        }

    } /*catch (...)*/ {
        error_message("EvalFTSW | StRurMax: " + FloatToStr(RuRac) + " StRur: "
                          + FloatToStr(StockRac) + " ftsw: " + FloatToStr(FTSW), URisocas);
    }
}


void RS_EvalCstrPFactorFAO_V2(double const &PFactor, double const &FTSW, double const &Eto, double const &KcTot, double const &StockMacropores, double const &CoeffStressLogging,
                              double const &Transplanting, double const &NurseryStatus,
                              double &cstr) {
    double pFact;

    /*try*/ {
        pFact = PFactor + 0.04 * (5 - KcTot * Eto);
        pFact = max(0., pFact);
        pFact = min(0.8, pFact);
        cstr = min((FTSW / (1 - pFact)), 1.);
        cstr = max(0., cstr);
        if ((StockMacropores > 0)) {
            cstr = cstr * CoeffStressLogging;
        }

        if (Transplanting == 1 && NurseryStatus == 0)
            cstr = 1;
    } /*catch (...)*/ {
        error_message("RS_EvalCstrPFactorFAO_V2", URisocas);
    }
}


void DemandePlante_V2_1(double const &Kcp, double const &ETo, double const &Ca, double const &CO2SlopeTr,   double &TrPot, double &CoeffCO2Tr) {
    /*try*/ {
        TrPot = Kcp * ETo;
        CoeffCO2Tr = Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1; // Coefficient for TrPot response to ambient CO2 (Ca), set to 1 for Ca=400ppm (ambient 2013)
        TrPot = TrPot * CoeffCO2Tr;

    } /*catch (...)*/ {
        error_message("DemandePlante_V2_1", UBilEau);
    }
}

void EvalTranspi(double const &TrPot, double const &cstr,   double &tr) {
    /*try*/ {
        tr = TrPot * cstr;

    } /*catch (...)*/ {
        error_message("EvalTranspi", UBilEau);
    }
}

void EvalETRETM(double const &Evap, double const &Tr, double const &Trpot,   double &ETM, double &ETR) {
    /*try*/ {
        ETM = Evap + Trpot;
        ETR = Evap + Tr;

    } /*catch (...)*/ {
        error_message("EvalETRETM", UBhyTypeFAO);
    }
}


void RS_EvolConsRes_Flood_V2(double const &NumPhase, double const &RuRac, double const &RuSurf,
                             double const &CapaREvap, double const &Tr, double const &Evap,
                             double const &/*CapaRDE*/, double const &/*CapaRFE*/, double const &EpaisseurSurf, double const &EpaisseurProf, double const &ResUtil,
                             double &StockRac, double &StockSurface, double &StockTotal, double &ValRFE, double &ValRDE, double &ValRSurf, double &FloodwaterDepth, double &StockMacropores) {
    double TrSurf;
    double WaterDeficit;

    /*try*/ {
        TrSurf = 0;
        StockSurface = ValRFE + ValRDE;
        if ((FloodwaterDepth + StockMacropores == 0) || (NumPhase == 0)) {
            // le calcul de cstr et de Tr doit intervenir après l'évaporation
            // calcul de la part de transpiration affectée aux réservoirs de surface
            if ((RuRac != 0)) {
                if ((RuRac <= RuSurf))
                //correction JCC le 21/08/02 de stRurMax<=RuSurf/stRurMax
                {
                    TrSurf = Tr;
                } else {
                    //TrSurf:=Tr*RuSurf/stRurMax;// on peut pondérer ici pour tenir compte du % racines dans chaque couche
                    if ((StockRac != 0)) {
                        TrSurf = Tr * StockSurface / StockRac;
                        // modif du 15/04/05  pondération par les stocks et non les capacités, sinon on n'extrait pas Tr si stock nul
                    }
                }
            } else {
                TrSurf = 0;
            }
            // MAJ des réservoirs de surface en répartissant TrSurf entre RFE et RDE
            ValRDE = max(0., ValRSurf - CapaREvap);
            if ((ValRDE + ValRFE < TrSurf)) {
                ValRFE = 0;
                ValRSurf = ValRSurf - ValRDE;
                ValRDE = 0;
            } else {
                if ((ValRFE > TrSurf)) {
                    ValRFE = ValRFE - TrSurf; // priorité à la RFU
                } else {
                    ValRSurf = ValRSurf - (TrSurf - ValRFE);
                    ValRDE = ValRDE - (TrSurf - ValRFE);
                    ValRFE = 0;
                }
            }
            StockSurface = ValRFE + ValRDE;
            StockRac = max(0., StockRac - Tr);
            // 18/04/05 déplacé en fin de procédure, car utilisé pour le calcul de Trsurf
            StockTotal = max(0., StockTotal - Tr);
            StockRac = min(StockRac, StockTotal);
        }
        if (((StockMacropores + FloodwaterDepth) > 0) && ((StockMacropores +
                                                           FloodwaterDepth) <= (Tr + Evap)) && (NumPhase > 0)) {
            WaterDeficit = (Tr + Evap) - (StockMacropores + FloodwaterDepth);
            StockMacropores = 0;
            FloodwaterDepth = 0;
            StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 -
                         WaterDeficit;
            StockRac = RuRac - WaterDeficit;
            StockRac = min(StockRac, StockTotal);
            StockSurface = max(EpaisseurSurf * ResUtil / 1000 - WaterDeficit, 0.);
            ValRFE = max(StockSurface - ValRDE - WaterDeficit, 0.);

            ValRSurf = ValRFE + ValRDE;
        } else {
            if (((StockMacropores + FloodwaterDepth) > (Tr + Evap)) && (NumPhase > 0)) {
                FloodwaterDepth = FloodwaterDepth - (Tr + Evap);
                StockMacropores = StockMacropores + min(0., FloodwaterDepth);
                FloodwaterDepth = max(FloodwaterDepth, 0.);
                StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
                             StockMacropores;
                StockRac = RuRac + StockMacropores;
                StockRac = min(StockRac, StockTotal);
                StockSurface = max(EpaisseurSurf * ResUtil / 1000 + StockMacropores *
                                                                        (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf)), 0.);
                ValRFE = max(StockSurface - ValRDE, 0.);

            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolConsRes_Flood_V2", URisocas);
    }
}

void RS_EvalTMaxMoy_V2_3( double const& TMax, double const& TMin, double const& HMax, double const& HMin,
                         double const& NumPhase, double const& NumSousPhase,
                         double & TMaxMoy)
{
    double TPanToa;

    /*try*/ {
        if(  ( ( NumPhase == 4 ) && ( NumSousPhase == 4 ) ) )
        {

            TPanToa = (0.8 * (TMax - TMin) + TMin) - (8.32 - 0.118 * ((1 - 0.8)* (HMax - HMin) + HMin));
            CalculeLaMoyenne( TPanToa , MonCompteur , TMaxMoy );
            //  TPanToa is panicle T at time of anthesis (TOA); TOA is estimated at 10:30h and Tair at this time is estimated to be at 88.8% of the diurnal T amplitude; RH effect on air-panicle T difference (TD) is TD=8.32-0.118RH, and this term is subtracted from the air temperature; RH is thereby estimated to be at (100-88.8)% of the diurnal RH amplitude; data from Julia.
        }
        else if(  NumPhase < 4 )
            TMaxMoy = 0;

    } /*catch (...)*/ {
        error_message( "RS_EvalTMaxMoy_V2_3" , URiz );
    }
}

void RS_EvalTMaxMoy(double const &TMax, double const &NumPhase, double const &NumSousPhase,
                    double &TMaxMoy) {
    /*try*/ {
        if (((NumPhase == 4) && (NumSousPhase == 4)))
            CalculeLaMoyenne(TMax, MonCompteur, TMaxMoy);
        else if (NumPhase < 4)
            TMaxMoy = 0;

    } /*catch (...)*/ {
        error_message("RS_EvalTMaxMoy", URiz);
    }
}


void RS_EvalTMinMoy(double const &TMin, double const &NumPhase, double const &NumSousPhase,
                    double &TMinMoy) {
    /*try*/ {
        if (((NumPhase == 4) && (NumSousPhase == 3))) {
            CalculeLaMoyenne(TMin, MonCompteur, TMinMoy);
        } else {
            if (NumPhase < 4) {
                TMinMoy = 0;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvalTMinMoy", URiz);
    }
}


void RS_EvalFtswMoy(double const &Ftsw, double const &NumPhase, double const &NumSousPhase,
                    double &FtswMoy) {
    /*try*/ {
        if (((NumPhase == 4) && (NumSousPhase == 4))) {
            CalculeLaMoyenne(Ftsw, MonCompteur, FtswMoy);
        } else {
            if (NumPhase < 4) {
                FtswMoy = 0;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvalFtswMoy", URiz);
    }
}


void RS_EvalSterility(double const &NumPhase, double const &ChangePhase, double const &KCritSterCold1,
                      double const &KCritSterCold2, double const &KCritSterHeat1, double const &KCritSterHeat2,
                      double const &KCritSterFtsw1, double const &KCritSterFtsw2,
                      double const &TMinMoy, double const &TMaxMoy, double const &FtswMoy,
                      double &SterilityCold, double &SterilityHeat, double &SterilityDrought, double &SterilityTot) {
    /*try*/ {
        if (((NumPhase == 5) && (ChangePhase == 1))) {
            SterilityCold = max(0., (min(1., KCritSterCold1 / (KCritSterCold1 -
                                                               KCritSterCold2) - TMinMoy / (KCritSterCold1 - KCritSterCold2))));
            SterilityHeat = 1 - max(0., (min(1., KCritSterHeat2 / (KCritSterHeat2 -
                                                                   KCritSterHeat1) - TMaxMoy / (KCritSterHeat2 - KCritSterHeat1))));
            SterilityDrought = max(0., (min(1., KCritSterFtsw1 / (KCritSterFtsw1 -
                                                                  KCritSterFtsw2) - FtswMoy / (KCritSterFtsw1 - KCritSterFtsw2))));
        }
        SterilityTot = min(0.999, 1 - ((1 - SterilityCold) * (1 - SterilityHeat) *
                                       (1 - SterilityDrought)));

    } /*catch (...)*/ {
        error_message("RS_EvalSterility", URisocas);
    }
}


void RS_EvalVitesseRacinaire(double const &VRacLevee, double const &RootSpeedBVP, double const &RootSpeedRPR, double const &RootSpeedPSP, double const &RootSpeedMatu1, double const &RootSpeedMatu2, double const &RootCstr, double const &cstr, double const &NumPhase, double const &DegreDuJourCor,   //DegreDuJour ou DegreDuJourCor?
                             double &VitesseRacinaire, double &VitesseRacinaireDay)
//Modif JCC du 15/03/2005 pour inclure VracLevée différente de VRacBVP
{
    /*try*/ {
        switch ((int)trunc(NumPhase)) {
        case 1:  VitesseRacinaire = VRacLevee; break;
        case 2:  VitesseRacinaire = RootSpeedBVP; break;
        case 3:  VitesseRacinaire = RootSpeedPSP; break;
        case 4:  VitesseRacinaire = RootSpeedRPR; break;
        case 5:  VitesseRacinaire = RootSpeedMatu1; break;
            /* TODO : attention en cas de gestion du champ vide... */
        case 6:  VitesseRacinaire = RootSpeedMatu2; break;
        default:
            VitesseRacinaire = 0;
        }
        VitesseRacinaireDay = VitesseRacinaire * DegreDuJourCor * std::pow(cstr
                                                                           , RootCstr);

    } /*catch (...)*/ {
        error_message("EvalVitesseRacinaire | NumPhase: " +
                          FloatToStr(NumPhase), URisocas);
    }
}


void EvalConversion(double const &NumPhase, double const &EpsiB, double const &AssimBVP, double const &SommeDegresJour, double const &SommeDegresJourPhasePrecedente, double const &AssimMatu1, double const &AssimMatu2, double const &SeuilTempPhaseSuivante,
                    double &Conversion) {
    double KAssim;

    /*try*/ {
        switch ((int)std::trunc(NumPhase)) {
        case 2:  KAssim = 1; break;
        case 3:  KAssim = AssimBVP; break;
        case 4:  KAssim = AssimBVP; break;
        case 5:  KAssim = AssimBVP + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                                    (AssimMatu1 - AssimBVP) / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente); break;
        case 6:  KAssim = AssimMatu1 + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                                      (AssimMatu2 - AssimMatu1) / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente); break;
        default:
            KAssim = 0; break;
        }
        Conversion = KAssim * EpsiB;

    } /*catch (...)*/ {
        error_message("EvalConversion | NumPhase: " + FloatToStr(NumPhase) +
                          " SommeDegresJour: " + FloatToStr(SommeDegresJour), UMilBilanCarbone);
    }
}


// -----------------------------------------------------------------------------
// PAR intercepté journalier (fonction de LTRkdfcl)
// -----------------------------------------------------------------------------

void RS_EvalParIntercepte_V2_1(double const &PAR, double const &Lai, double const &Kdf,   double &PARIntercepte, double &LIRkdfcl) {
    /*try*/ {
        /*NEW LB*/
        if ((Lai > 0) && (LIRkdfcl == 0))
            LIRkdfcl = (1 - exp(-Kdf * Lai));
        // right after germination there is a problem with module sequence, like chicken and egg; this overcomes it
        /*NEW LB*/


        PARIntercepte = PAR * LIRkdfcl;

    } /*catch (...)*/ {
        error_message("RS_EvalParIntercepte_V2_1 | PAR: " + FloatToStr(PAR) +
                          " LIRkdfcl: " + FloatToStr(LIRkdfcl), URisocas);
    }
}


void RS_EvalAssimPot_V2_1(double const &PARIntercepte, double const &PAR, double const &Conversion, double const &TMax, double const &TMin, double const &TBase, double const &TOpt1, double const &DayLength, double const &StressCold, double const &CO2Exp, double const &Ca, double const &CO2Cp, double const &SlaMin, double const &sla, double const &CoeffAssimSla,   double &AssimPot, double &CoeffCO2Assim)

{
    /*try*/ {
             {


                 if (CO2Exp != 0 && CO2Cp != 0)
                 CoeffCO2Assim = (1 - exp(-CO2Exp * (Ca - CO2Cp))) / (1 - exp(-CO2Exp * (400 - CO2Cp)));

    // This coefficient always equals 1 at 400ppm CO2 and describes AssimPot response to Ca

    AssimPot = PARIntercepte * Conversion * 10 * CoeffCO2Assim;
    // Ordinary linear effect on intercepted light on canopy assimulation , multiplied by CO2 effect

    AssimPot = AssimPot * min(((3 * TMax + TMin) / 4 - TBase) / (TOpt1 -
                                                                 TBase), 1.);


    // Reduction of assimilation at diurnal temperatures < Topt1
    AssimPot = AssimPot * sqrt(max(0.00001, StressCold));
    // Cold stress effect on AssimPot (affects also organ demands and grain filling)

    if (((PARIntercepte != 0) && (DayLength != 0))) {


        AssimPot = AssimPot *  std::pow((PAR / DayLength), 0.667) / (PAR / DayLength);
        // De-linearization of PAR response of AssimPot. At 1 MJ/h (cloudless) effect is zero

        /*NEW Y*/
        AssimPot = AssimPot * std::pow((SlaMin / max(sla, SlaMin)), CoeffAssimSla);
        // Effect of SLA on AssimPot ; AssimPot is reduced if Sla>SlaMin;
        //For no effect set parameter CoeffAssimSla=0,
        //for proportional effect set CoeffAssimSla=1.
        //Intermediate values are OK.



    }
}

} /*catch (...)*/ {
    error_message("RS_EvalAssimPot_V2_1 ", URisocas);
}

}


// -----------------------------------------------------------------------------
// Assim is less sensitive than transpiration to cstr, but currently cstrassim =
// assim.
// ASScstr is a new parameter that slows attenuates cstr effect on assimilation
// relative to transpiration, assuming values between 1 (same effect on
// transpiration and assimilation) and ca. 0.5 (assim less sensitive than
// transpiration, leasing to increased transpiration efficiency TE).
// -----------------------------------------------------------------------------

void RS_EvalCstrAssim(double const &cstr, double const &ASScstr,   double &cstrassim) {
    /*try*/ {
        cstrassim = std::pow(max(cstr, 0.00000001), ASScstr);

    } /*catch (...)*/ {
        error_message("RS_EvalCstrAssim", URisocas);
    }
}


void RS_EvalAssim(double const &AssimPot, double const &cstrassim,   double &Assim) {
    /*try*/ {
        Assim = max(AssimPot * cstrassim, 0.);

    } /*catch (...)*/ {
        error_message("EvalAssim | AssimPot: " + FloatToStr(AssimPot) +
                          " CstrAssim: " + FloatToStr(cstrassim) + " StressCold: ", URisocas);
    }
}


void RS_TransplantingShock_V2(double const &CounterNursery, double const &CoeffTransplantingShock,   double &Assim) {
    /*try*/ {
        if (((CounterNursery > 0) && (CounterNursery < 8))) {
            Assim = Assim * CoeffTransplantingShock;
        }

    } /*catch (...)*/ {
        error_message("RS_TransplantingShock_V2", URisocas);
    }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------


void RS_EvalRespMaint_V2_2( double const& kRespMaintLeaf, double const& kRespMaintSheath, double const& kRespMaintRoot, double const& kRespInternode,
                           double const& kRespPanicle,
                           double const& DryMatStructLeafPop, double const& DryMatStructSheathPop, double const& DryMatStructRootPop,
                           double const& DryMatStructInternodePop, double const& DryMatStructPaniclePop,
                           double const& TMoyCalc, double const& kTempMaint, double const& CoefficientQ10, double const& PAR,
                           double & RespMaintTot)

{
    double RespMaintLeafPop;
    double RespMaintSheathPop;
    double RespMaintRootPop;
    double RespMaintInternodePop;
    double RespMaintPaniclePop;
    double CoeffQ10;

    /*try*/ {
        CoeffQ10 = pow( CoefficientQ10 , ( TMoyCalc - kTempMaint ) / 10 );
        RespMaintLeafPop = kRespMaintLeaf * DryMatStructLeafPop * CoeffQ10 * ( 0.3 + 0.7 * min( PAR, 5.0 )/5 );
        RespMaintSheathPop = kRespMaintSheath * DryMatStructSheathPop * CoeffQ10 * ( 0.3 + 0.7 * min( PAR, 5.0 )/5 );
        RespMaintRootPop = kRespMaintRoot * DryMatStructRootPop * CoeffQ10 * ( 0.3 + 0.7 * min( PAR, 5.0 )/5 );
        RespMaintInternodePop = kRespInternode * DryMatStructInternodePop *
                                CoeffQ10 * ( 0.3 + 0.7 * min( PAR, 5.0 )/5 );
        RespMaintPaniclePop = kRespPanicle * DryMatStructPaniclePop * CoeffQ10 * ( 0.3 + 0.7 * min( PAR, 5.0 )/5 );
        RespMaintTot = RespMaintLeafPop + RespMaintSheathPop + RespMaintRootPop +
                       RespMaintInternodePop + RespMaintPaniclePop;

    } /*catch (...)*/ {error_message( "RS_EvalRespMaint_V2_2" , URisocas );
    }
}

void RS_EvalRespMaint(double const &kRespMaintLeaf, double const &kRespMaintSheath, double const &kRespMaintRoot, double const &kRespInternode, double const &kRespPanicle,
                      double const &DryMatStructLeafPop, double const &DryMatStructSheathPop, double const &DryMatStructRootPop, double const &DryMatStructInternodePop, double const &DryMatStructPaniclePop,
                      double const &TMoyCalc, double const &kTempMaint, double const &CoefficientQ10,
                      double &RespMaintTot)

{
    double RespMaintLeafPop;
    double RespMaintSheathPop;
    double RespMaintRootPop;
    double RespMaintInternodePop;
    double RespMaintPaniclePop;
    double CoeffQ10;

    /*try*/ {
        CoeffQ10 = std::pow(CoefficientQ10, (TMoyCalc - kTempMaint) / 10);
        RespMaintLeafPop = kRespMaintLeaf * DryMatStructLeafPop * CoeffQ10;
        RespMaintSheathPop = kRespMaintSheath * DryMatStructSheathPop * CoeffQ10;
        RespMaintRootPop = kRespMaintRoot * DryMatStructRootPop * CoeffQ10;
        RespMaintInternodePop = kRespInternode * DryMatStructInternodePop *
                                CoeffQ10;
        RespMaintPaniclePop = kRespPanicle * DryMatStructPaniclePop * CoeffQ10;
        RespMaintTot = RespMaintLeafPop + RespMaintSheathPop + RespMaintRootPop +
                       RespMaintInternodePop + RespMaintPaniclePop;

    } /*catch (...)*/ {
        error_message("RS_EvalRespMaint", URisocas);
    }
}

void RS_EvalRelPotLeafLength_V2_2( double const& NumPhase, double const& HaunIndex, double const& RankLongestLeaf,
                                  double & RelPotLeafLength)
{
    /*try*/ {
        if(  ( NumPhase > 1 ) )
        {
            RelPotLeafLength = min( ( 0.10 + 0.90 * HaunIndex / RankLongestLeaf ) , 1.0 );
            // 11-17-14 modified 0.15 0.85
            // 02-05-15 modified 0.10 0.90
        }

    } /*catch (...)*/ {error_message( "RS_EvalRelPotLeafLength_V2_2" , URisocas );
    }
}


void RS_EvalRelPotLeafLength(double const &NumPhase, double const &HaunIndex, double const &RankLongestLeaf,
                             double &RelPotLeafLength) {
    /*try*/ {
        if ((NumPhase > 1)) {
            RelPotLeafLength = min((0.1 + 0.9 * HaunIndex / RankLongestLeaf), 1.);
        }

    } /*catch (...)*/ {
        error_message("RS_EvalRelPotLeafLength", URisocas);
    }
}

// Modified 11/17/14
void RS_EvolPlantTilNumTot_V2_2( double const& NumPhase, double const& ChangePhase, double const& PlantsPerHill, double const& TilAbility, double const& Density,
                                double const& Ic, double const& IcTillering, double const& cstr, double const& HaunIndex, double const& HaunCritTillering, double const& LtrKdfcl,
                                double & CulmsPerHill, double & CulmsPerPlant, double & CulmsPop)
{
    double TilNewPlant;

    /*try*/ {
        if(  ( ( NumPhase == 1 ) && ( ChangePhase == 1 ) ) )
        {
            CulmsPerHill = PlantsPerHill;
        }
        else
        {
            if(  ( ( NumPhase == 2 ) && ( ChangePhase == 1 ) ) )
            {
                CulmsPerPlant = 1;
                CulmsPop = CulmsPerPlant * Density * PlantsPerHill;
                CulmsPerHill = CulmsPerPlant * PlantsPerHill;
            }
            else
            {
                if(  ( ( NumPhase > 1 ) && ( NumPhase < 4 ) && ( HaunIndex >
                                                          HaunCritTillering ) ) )
                {
                    TilNewPlant = cstr * min( max( 0.0 , ( Ic - IcTillering ) * TilAbility ) *
                                                 LtrKdfcl , CulmsPerPlant * 0.1 );
                    // 11-17-14 modified 0.1
                    CulmsPerPlant = CulmsPerPlant + TilNewPlant;
                    CulmsPerHill = CulmsPerPlant * PlantsPerHill;
                    CulmsPop = CulmsPerHill * Density;
                }

            }
        }

    } /*catch (...)*/ {error_message( "RS_EvolPlantTilNumTot_V2_2" , URisocas );
    }
}

void RS_EvolPlantTilNumTot_V2(double const &NumPhase, double const &ChangePhase, double const &PlantsPerHill, double const &TilAbility, double const &Density, double const &Ic, double const &IcTillering, double const &cstr, double const &HaunIndex, double const &HaunCritTillering, double const &LtrKdfcl,
                              double &CulmsPerHill, double &CulmsPerPlant, double &CulmsPop) {
    double TilNewPlant;

    /*try*/ {
        if (((NumPhase == 1) && (ChangePhase == 1))) {
            CulmsPerHill = PlantsPerHill;
        } else {
            if (((NumPhase == 2) && (ChangePhase == 1))) {
                CulmsPerPlant = 1;
                CulmsPop = CulmsPerPlant * Density * PlantsPerHill;
            } else {
                if (((NumPhase > 1) && (NumPhase < 4) && (HaunIndex >
                                                          HaunCritTillering))) {
                    TilNewPlant = cstr * min(max(0., (Ic - IcTillering) * TilAbility) *
                                                 sqrt(LtrKdfcl), 2.);
                    CulmsPerPlant = CulmsPerPlant + TilNewPlant;
                    CulmsPerHill = CulmsPerPlant * PlantsPerHill;
                    CulmsPop = CulmsPerHill * Density;
                }
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolPlantTilNumTot_V2", URisocas);
    }
}


void RS_EvolPlantLeafNumTot(double const &NumPhase, double const &CulmsPerHill, double const &HaunGain,
                            double &PlantLeafNumNew, double &PlantLeafNumTot) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            PlantLeafNumNew = HaunGain * CulmsPerHill;
            PlantLeafNumTot = PlantLeafNumTot + PlantLeafNumNew;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolPlantLeafNumTot", URisocas);
    }
}

void RS_EvolMobiliTillerDeath_V2_2_lodging( double const& NumPhase, double const& SDJPhase4, double const& SeuilTempRPR, double const& CoeffTillerDeath,
                                           double const& Density, double const& Ic, double const& PlantsPerHill, double const& CoeffFixedTillerDeath,
                                           double & TillerDeathPop, double & CulmsPop, double & CulmsPerPlant, double & CulmsPerHill, double & DryMatStructPaniclePop)
{
    /*try*/ {
        if(  ( ( ( NumPhase == 3 ) || ( ( NumPhase == 4 ) && ( SDJPhase4 <= 0.7 * SeuilTempRPR ) ) ) && ( CulmsPerPlant >= 1 ) ) )
        {
            //TillerDeathPop := (1 - (Min(Ic, 1))) * CulmsPop * CoeffTillerDeath;
            //TillerDeathPop := Min((1 - (Min(Ic, 1))),0.06) * CulmsPop * CoeffTillerDeath;
            // 11-13-14 introduced rate limitation of tiller abortion (not more than 6% per day)

            double LowerOrderTillersPop = CulmsPop - 2 * (PlantsPerHill * Density);

            TillerDeathPop = max(0.,
                                 min(
                                     (1 - min(Ic,1.)) * CoeffTillerDeath * LowerOrderTillersPop,
                                     0.06 * CulmsPop)
                                 ) + (CoeffFixedTillerDeath * CulmsPop);
            // Introduced rate limitation of tiller abortion (not more than 6%/day)in V2.2; additional provision added that TillerDeathPop cannot be negative

            CulmsPop = max(Density * PlantsPerHill, CulmsPop - TillerDeathPop);
            // Made sure that the main culm can never be aborted

            CulmsPerPlant = CulmsPop / ( Density * PlantsPerHill );
            CulmsPerHill = CulmsPerPlant * PlantsPerHill;
            DryMatStructPaniclePop = DryMatStructPaniclePop * max( 0.0 , CulmsPop ) /
                                     ( CulmsPop + TillerDeathPop );
        }

    } /*catch (...)*/ {error_message( "RS_EvolMobiliTillerDeath_V2_2" , URisocas );
    }
}

// Modified 11/13/14
void RS_EvolMobiliTillerDeath_V2_2( double const& NumPhase, double const& SDJPhase4, double const& SeuilTempRPR, double const& CoeffTillerDeath, double const& Density, double const& Ic, double const& PlantsPerHill,
                                   double & TillerDeathPop, double & CulmsPop, double & CulmsPerPlant, double & CulmsPerHill, double & DryMatStructPaniclePop)
{
    /*try*/ {
        if(  ( (( NumPhase == 3 ) || ( ( NumPhase == 4 ) && ( SDJPhase4 <= /*NEW*/ 0.7 * SeuilTempRPR ) ) )
             && ( CulmsPerPlant >= 1 ) ) )
        {
            //TillerDeathPop := (1 - (Min(Ic, 1))) * CulmsPop * CoeffTillerDeath;
            //TillerDeathPop := Min((1 - (Min(Ic, 1))),0.06) * CulmsPop * CoeffTillerDeath;
            // 11-13-14 introduced rate limitation of tiller abortion (not more than 6% per day)

            TillerDeathPop = min( ( ( 1 - ( min( Ic , 1.0 ) ) ) * CoeffTillerDeath * CulmsPop ) , 0.06* CulmsPop );
            // 01-14-15 corrected

            CulmsPop = CulmsPop - TillerDeathPop;
            CulmsPerPlant = CulmsPop / ( Density * PlantsPerHill );
            CulmsPerHill = CulmsPerPlant * PlantsPerHill;
            DryMatStructPaniclePop = DryMatStructPaniclePop * max( 0.0 , CulmsPop ) /
                                     ( CulmsPop + TillerDeathPop );
        }

    } /*catch (...)*/ {error_message( "RS_EvolMobiliTillerDeath_V2_2" , URisocas );
    }
}

void RS_EvolMobiliTillerDeath_V2_1(double const &NumPhase, double const &SDJPhase4, double const &SeuilTempRPR, double const &CoeffTillerDeath, double const &Density, double const &Ic, double const &PlantsPerHill,
                                   double &TillerDeathPop, double &CulmsPop, double &CulmsPerPlant, double &CulmsPerHill, double &DryMatStructPaniclePop) {
    /*try*/ {
        if ((((NumPhase == 3) || ((NumPhase == 4) && (SDJPhase4 <= /*NEW*/ 0.7 * SeuilTempRPR)))
             && (CulmsPerPlant >= 1))) {
            TillerDeathPop = (1 - (min(Ic, 1.))) * CulmsPop * CoeffTillerDeath;
            CulmsPop = CulmsPop - TillerDeathPop;
            CulmsPerPlant = CulmsPop / (Density * PlantsPerHill);
            CulmsPerHill = CulmsPerPlant * PlantsPerHill;
            DryMatStructPaniclePop = DryMatStructPaniclePop * max(0., CulmsPop) /
                                     (CulmsPop + TillerDeathPop);
        } else {
            TillerDeathPop = 0;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolMobiliTillerDeath_V2_1", URisocas);
    }
}



void RS_EvolMobiliLeafDeath_V2_1(double const &NumPhase, double const &Ic, double const &CoeffLeafDeath, double const &sla,
                                 double const &CoeffTerminalLeafDeath, double const &DegresDuJourCor, double const &SDJMatu1, double const &SDJMatu2,
                                 double &LeafDeathPop, double &DryMatStructLeafPop, double &MobiliLeafDeath, double &DeadLeafDrywtPop, double &LaiDead) {
    /*try*/ {
        if ((NumPhase > 1)) {
            LeafDeathPop = (1 - (min(Ic, 1.))) * DryMatStructLeafPop * CoeffLeafDeath;
            if(NumPhase >= 5) {
                //                LeafDeathPop = LeafDeathPop + (CoeffTerminalLeafDeath * (DryMatStructLeafPop - LeafDeathPop)* DegresDuJourCor / (SDJMatu1 + SDJMatu2));
                LeafDeathPop = min(DryMatStructLeafPop, (LeafDeathPop + (CoeffTerminalLeafDeath * DryMatStructLeafPop)));
            }

            DryMatStructLeafPop = DryMatStructLeafPop - LeafDeathPop;
            MobiliLeafDeath = 0.25 /*NEW*/ * LeafDeathPop;
            DeadLeafDrywtPop = DeadLeafDrywtPop + (0.75 * LeafDeathPop);
            LaiDead = DeadLeafDrywtPop * sla;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolMobiliLeafDeath_V2_1", URisocas);
    }
}


void RS_EvalSupplyTot_V2_1_micha(double const &NumPhase, double const &/*PhaseStemElongation*/, double const &Assim, double const &MobiliLeafDeath, double const &RespMaintTot,
                                 double &RespMaintDebt, double &/*AssimNotUsed*/, double &/*AssimNotUsedCum*/, double &/*AssimSurplus*/, double &SupplyTot, double &CumSupplyTot) {
    /*try*/ {
        SupplyTot = Assim + MobiliLeafDeath - RespMaintTot;// - max(0., RespMaintDebt);
        /*NEW*/

        if (NumPhase < 7)
            CumSupplyTot = CumSupplyTot + SupplyTot /*NEW R*/ - MobiliLeafDeath; //  Output Test variable for source for dry matter production (consider also AssimNotUsed!)
        else
            CumSupplyTot = 0;

        if (SupplyTot <= 0) {
            RespMaintDebt = 0 - SupplyTot;
            SupplyTot = 0;
        } else {
            RespMaintDebt = 0;
        }

        /*DELETED*/
        /*
    if ((NumPhase < 5) and (PhaseStemElongation = 0)) then
    begin
    AssimNotUsed := AssimSurplus;
    AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end
    else
    begin
    AssimNotUsed := 0;
    AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end;
    */
        // These commands seem redundant and in the wrong place. Denete?



    } /*catch (...)*/ {
        error_message("RS_EvalSupplyTot_V2_1", URisocas);
    }
}


void RS_EvalSupplyTot_V2_1(double const &NumPhase, double const &/*PhaseStemElongation*/, double const &Assim, double const &MobiliLeafDeath, double const &RespMaintTot,
                           double &RespMaintDebt, double &/*AssimNotUsed*/, double &/*AssimNotUsedCum*/, double &/*AssimSurplus*/, double &SupplyTot, double &CumSupplyTot) {
    /*try*/ {
        SupplyTot = Assim + MobiliLeafDeath - RespMaintTot - max(0., RespMaintDebt);
        /*NEW*/

        if (NumPhase < 7)
            CumSupplyTot = CumSupplyTot + SupplyTot /*NEW R*/ - MobiliLeafDeath; //  Output Test variable for source for dry matter production (consider also AssimNotUsed!)
        else
            CumSupplyTot = 0;

        if (SupplyTot <= 0) {
            RespMaintDebt = 0 - SupplyTot;
            SupplyTot = 0;
        } else {
            RespMaintDebt = 0;
        }

        /*DELETED*/
        /*
    if ((NumPhase < 5) and (PhaseStemElongation = 0)) then
    begin
    AssimNotUsed := AssimSurplus;
    AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end
    else
    begin
    AssimNotUsed := 0;
    AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end;
    */
        // These commands seem redundant and in the wrong place. Denete?



    } /*catch (...)*/ {
        error_message("RS_EvalSupplyTot_V2_1", URisocas);
    }
}

void RS_EvalDemandStructLeaf_V2_1(double const &NumPhase, double const &PlantLeafNumNew, double const &SlaNew, double const &SlaMax, double const &RelPotLeafLength, double const &Density, double const &LeafLengthMax, double const &CoeffLeafWLRatio, double const &cstr, double const &StressCold,
                                  double &DemLeafAreaPlant, double &DemStructLeafPlant, double &DemStructLeafPop, double &A_DemStructLeaf) {
    double CorrectedSla;

    /*try*/ {
        if (NumPhase > 1 && NumPhase < 5) {
            DemLeafAreaPlant = (pow((RelPotLeafLength * LeafLengthMax), 2) *
                                CoeffLeafWLRatio * 0.725 * PlantLeafNumNew / 1000000) * min(cstr
                                     , StressCold);
            if ((SlaNew == 0)) {
                CorrectedSla = SlaMax;
            } else {
                CorrectedSla = SlaNew;
            }
            DemStructLeafPlant = DemLeafAreaPlant * 0.1 / CorrectedSla;
            DemStructLeafPop = DemStructLeafPlant * Density / 1000;
            A_DemStructLeaf = DemStructLeafPlant * Density / 1000;
        } else {
            DemStructLeafPlant = 0;
            DemStructLeafPop = 0;
            A_DemStructLeaf = 0;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDemandStructLeaf_V2_1", URisocas);
    }
}


void RS_EvalDemandStructSheath(double const &NumPhase, double const &WtRatioLeafSheath,
                               double const &SlaMin, double const &SlaMax, double const &SlaMitch, double const &StressCold,
                               double const& PAR, double const& PARCritSLA, double const& SLASwitch,
                               double &DemStructSheathPop, double &DemStructLeafPop) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            DemStructSheathPop = (1 + ((SlaMax - SlaMitch) / (SlaMax - SlaMin))) * 0.5 *
                                 DemStructLeafPop / WtRatioLeafSheath * max(0.00001, StressCold);
            if ( SLASwitch > 0 ) {
                DemStructSheathPop = DemStructSheathPop * min(PAR / PARCritSLA, 1.);
                DemStructLeafPop = DemStructLeafPop / sqrt(min(PAR / PARCritSLA, 1.));
            }
            // If PAR is low, sheath growth is reduced and leaf blade growth is enhanced
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDemandStructSheath", URisocas);
    }
}


void RS_EvalDemandStructRoot_V2(double const &NumPhase, double const &Density, double const &CoeffRootMassPerVolMax, double const &RootPartitMax, double const &/*GrowthStructTotPop*/,
                                double const &RootFront, double const &/*SupplyTot*/, double const &DemStructLeafPop, double const &DemStructSheathPop, double const &DryMatStructRootPop,
                                double const &RootLignin,
                                double &RootSystSoilSurfPop, double &RootSystVolPop, double &GainRootSystVolPop, double &GainRootSystSoilSurfPop, double &DemStructRootPop,
                                double &RootSystSoilSurfPopOld, double &RootFrontOld, double &RootSystVolPopOld, double &DemStructRootPlant) {
    /*try*/ {
        RootSystSoilSurfPop = min(RootFront * RootFront * Density / 1000000
                                  , 10000.);
        RootSystVolPop = RootSystSoilSurfPop * RootFront / 1000;
        if (((NumPhase > 1) && (NumPhase < 5))) {
            GainRootSystSoilSurfPop = RootSystSoilSurfPop - RootSystSoilSurfPopOld;
            GainRootSystVolPop = RootSystVolPop - RootSystVolPopOld;
            DemStructRootPop = min((DemStructLeafPop + DemStructSheathPop) *
                                       RootPartitMax, max(0., CoeffRootMassPerVolMax * RootSystVolPop -
                                               DryMatStructRootPop));

            DemStructRootPop = DemStructRootPop + (DemStructRootPop * (RootLignin / 100) * 1.7);
            DemStructRootPlant = DemStructRootPop * 1000 / Density;
            RootSystSoilSurfPopOld = RootSystSoilSurfPop;
            RootFrontOld = RootFront;
            RootSystVolPopOld = RootSystVolPop;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDemandStructRoot_V2", URisocas);
    }
}

bool firstTimeDone = false;
void RS_EvalDemandStructIN_V2_1(double const &PhaseElongation, double const &ApexHeightGain, double const &CulmsPerHill, double const &CoeffInternodeMass, double const &Density, double const &Ic, double const &ResCapacityInternodePop, double const &DryMatResInternodePop, double const &CoeffReserveSink, double const &NumPhase,
                                double &DemStructInternodePlant, double &DemStructInternodePop, double &DemResInternodePop) {
    /*try*/ {
        if ((PhaseElongation == 1)) {
            DemStructInternodePlant = std::pow(min(Ic, 1.), 0.5) * ApexHeightGain *
                                      CulmsPerHill * CoeffInternodeMass;
            DemStructInternodePop = DemStructInternodePlant * Density / 1000;
        }

        if ((NumPhase > 1) && (NumPhase < 5)) {
            DemResInternodePop = (ResCapacityInternodePop - DryMatResInternodePop) * CoeffReserveSink;
        }
        // CoeffReserveSink is a crop para 0…1 that sets daily reserve sink as fraction of deficit
        else {
            DemResInternodePop = 0;//(ResCapacityInternodePop - DryMatResInternodePop) * CoeffReserveSink;
        }


    } /*catch (...)*/ {
        error_message("RS_EvalDemandStructIN_V2_1", URisocas);
    }
}


void RS_EvalDemandStructPanicle_V2(double const &NumPhase, double const &CoeffPanicleMass, double const &CulmsPerHill, double const &Ic, double const &DryMatStructPaniclePop, double const &Density, double const &PanStructMassMax, double const &StressCold,
                                   double &DemStructPaniclePlant, double &PanStructMass, double &DemStructPaniclePop) {
    /*try*/ {
        if ((NumPhase == 4)) {
            DemStructPaniclePlant = CoeffPanicleMass * CulmsPerHill * sqrt(min(Ic, 1.))
                                    * sqrt(max(0.00001, StressCold));
            PanStructMass = 1000 * DryMatStructPaniclePop / (Density * CulmsPerHill);
            if ((PanStructMass > PanStructMassMax)) {
                DemStructPaniclePlant = 0;
            }
            DemStructPaniclePop = DemStructPaniclePlant * Density / 1000;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalDemandStructPanicle_V2", URisocas);
    }
}


void RS_EvalDemandTotAndIcPreFlow_V2_1(double const &NumPhase, double const &/*RespMaintTot*/, double const &DemStructLeafPop, double const &DemStructSheathPop, double const &DemStructRootPop,
                                       double const &DemStructInternodePop, double const &DemStructPaniclePop, double const &SupplyTot,
                                       double const &NbDaysSinceGermination, double const &PlantHeight, double const &cstr, double const &DemResInternodePop,
                                       double &DemStructTotPop, double &Ic, double &IcCumul, double &IcMean, double &CstrCumul, double &CstrMean, double &A_DemStructTot)

{
    /*try*/ {

        if (((NumPhase > 1) && (NumPhase < 5))) {

            DemStructTotPop = DemStructLeafPop + DemStructSheathPop +
                              DemStructRootPop + DemStructInternodePop +
                              DemStructPaniclePop /*NEW G*/ + DemResInternodePop;

            A_DemStructTot = DemStructLeafPop + DemStructSheathPop +
                             DemStructRootPop + DemStructInternodePop +
                             DemStructPaniclePop /*NEW G*/ + DemResInternodePop;


            Ic = SupplyTot / DemStructTotPop;


            if ((Ic <= 0)) {
                Ic = 0;
            }


            if ((PlantHeight == 0)) {
                Ic = 1;
            }



            IcCumul = IcCumul + min(Ic, 1.);
            IcMean = IcCumul / NbDaysSinceGermination;
            CstrCumul = CstrCumul + cstr;
            CstrMean = CstrCumul / NbDaysSinceGermination;


        } else {
            A_DemStructTot = 0;
            DemStructTotPop = 0;
        }


        if (((NumPhase == 5) || (NumPhase == 6))) {
            IcCumul = IcCumul + min(Ic, 1.);
            IcMean = IcCumul / NbDaysSinceGermination;
            CstrCumul = CstrCumul + cstr;
            CstrMean = CstrCumul / NbDaysSinceGermination;
        }



    } /*catch (...)*/ {
        error_message("RS_EvalDemandTotAndIcPreFlow_V2_1", URisocas);
    }
}

void RS_EvolGrowthStructLeafPop_V2_1(double const &NumPhase, double const &Ic, double const &SupplyTot, double const &DemStructLeafPop, double const &DemStructTotPop,
                                     double &GrowthStructLeafPop, double &A_GrowthStructLeaf) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((Ic < 1)) {

                /*YIELD ERROR 34.5*/
                GrowthStructLeafPop = SupplyTot * (DemStructLeafPop / DemStructTotPop);
                A_GrowthStructLeaf = SupplyTot * (DemStructLeafPop / DemStructTotPop);

            } else {

                GrowthStructLeafPop = DemStructLeafPop;
                A_GrowthStructLeaf = DemStructLeafPop;
                //showMessage('DEM:_'+ FloatToStr(DemStructLeafPop)+' A_G: _' +FloatToStr(A_GrowthStructLeaf));
            }
        }


        /*GrowthView := GrowthStructLeafPop;*/

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructLeafPop_V2_1", URisocas);
    }
}


void RS_EvolGrowthStructSheathPop(double const &NumPhase, double const &Ic, double const &SupplyTot, double const &DemStructSheathPop, double const &DemStructTotPop,
                                  double &GrowthStructSheathPop) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((Ic < 1)) {
                GrowthStructSheathPop = SupplyTot * (DemStructSheathPop /
                                                     DemStructTotPop);
            } else {
                GrowthStructSheathPop = DemStructSheathPop;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructSheathPop", URisocas);
    }
}


void RS_EvolGrowthStructRootPop(double const &NumPhase, double const &Ic, double const &SupplyTot, double const &DemStructRootPop, double const &DemStructTotPop,
                                double const &RootLignin,
                                double &GrowthStructRootPop) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((Ic < 1)) {
                GrowthStructRootPop = SupplyTot * (DemStructRootPop / DemStructTotPop);
            } else {
                GrowthStructRootPop = DemStructRootPop;
            }
        }

        GrowthStructRootPop = GrowthStructRootPop - (GrowthStructRootPop * (RootLignin / 100) * (1 / 1.7));

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructRootPop", URisocas);
    }
}


void RS_EvolGrowthStructINPop_V2_1(double const &NumPhase, double const &Ic, double const &SupplyTot, double const &DemStructInternodePop, double const &DemStructTotPop, double const &DemResInternodePop,
                                   double &GrowthStructInternodePop, double &GrowthResInternodePop) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((Ic < 1)) {
                GrowthStructInternodePop = SupplyTot * (DemStructInternodePop / DemStructTotPop);
                /*NEW G*/
                GrowthResInternodePop = SupplyTot * (DemResInternodePop / DemStructTotPop);
                /*/NEW G*/
            } else {
                GrowthStructInternodePop = DemStructInternodePop;

                /*NEW G*/
                GrowthResInternodePop = DemResInternodePop;
                /*/NEW G*/

            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructInternodePop_V2_1", URisocas);
    }
}


void RS_EvolGrowthStructPanPop(double const &NumPhase, double const &Ic, double const &SupplyTot, double const &DemStructPaniclePop, double const &DemStructTotPop,
                               double &GrowthStructPaniclePop) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            if ((Ic < 1)) {
                GrowthStructPaniclePop = SupplyTot * (DemStructPaniclePop /
                                                      DemStructTotPop);
            } else {
                GrowthStructPaniclePop = DemStructPaniclePop;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructPaniclePop", URisocas);
    }
}


void RS_Priority2GrowthPanStrctPop_V2_1(double const &PriorityPan, double const &DemStructPaniclePop, double const &NumPhase, double const &GrowthStructTotPop, double const &DemStructInternodePop, double const &DemStructTotPop, double const &DemStructLeafPop, double const &DemStructSheathPop, double const &DemStructRootPop, double const &DemResInternodePop,
                                        double &GrowthStructPaniclePop, double &GrowthStructInternodePop, double &GrowthStructLeafPop, double &GrowthStructSheathPop, double &GrowthStructRootPop, double &GrowthResInternodePop) {
    double GrowthPanDeficit;
    double GrowthStructPaniclePlus;

    /*try*/ {
        if ((GrowthStructPaniclePop < DemStructPaniclePop) /*NEW LB*/ && (NumPhase == 4)/*NEW LB*/) {
            GrowthPanDeficit = DemStructPaniclePop - GrowthStructPaniclePop;
            /*NEW LB*/
            GrowthStructPaniclePlus = min(PriorityPan * GrowthPanDeficit, GrowthStructTotPop - GrowthStructPaniclePop);
            /*/NEW LB*/
            GrowthStructPaniclePop = GrowthStructPaniclePop /*NEW LB*/ + GrowthStructPaniclePlus;
            GrowthStructInternodePop = GrowthStructInternodePop - GrowthStructPaniclePlus * (DemStructInternodePop / DemStructTotPop);
            GrowthStructLeafPop = GrowthStructLeafPop - GrowthStructPaniclePlus * (DemStructLeafPop / DemStructTotPop);
            GrowthStructSheathPop = GrowthStructSheathPop - GrowthStructPaniclePlus * (DemStructSheathPop / DemStructTotPop);
            GrowthStructRootPop = GrowthStructRootPop - GrowthStructPaniclePlus * (DemStructRootPop / DemStructTotPop);
            GrowthResInternodePop = GrowthResInternodePop - GrowthStructPaniclePlus * (DemResInternodePop / DemStructTotPop);
            /*/NEW LB*/


            /*DELETED LB*/
            /*GrowthStructInternodePop := max(0., GrowthStructInternodePop - PriorityPan
      * GrowthPanDeficit);*/
        }

    } /*catch (...)*/ {
        error_message("RS_Priority2GrowthPanStrctPop_V2_1", URisocas);
    }
}


void RS_EvolGrowthStructTot_V2_1(double const &NumPhase, double const &SupplyTot, double const &GrowthResInternodePop,
                                 double &GrowthStructTotPop, double &AssimSurplus, double &GrowthStructLeafPop,
                                 double &GrowthStructSheathPop, double &GrowthStructRootPop, double &GrowthStructInternodePop, double &GrowthStructPaniclePop, double &A_GrowthStructLeaf, double &A_GrowthStructTot, double &A_AssimSurplus) {
    /*try*/ {
        if (((NumPhase > 1) && (NumPhase < 5))) {
            GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop +
                                 GrowthStructRootPop +
                                 GrowthStructInternodePop + GrowthStructPaniclePop /*NEW P*/ + GrowthResInternodePop;

            A_GrowthStructTot = GrowthStructTotPop;
            AssimSurplus = max((SupplyTot - GrowthStructTotPop /*DELETED*//*- GrowthResInternodePop*/), 0.);
            A_AssimSurplus = max((SupplyTot - GrowthStructTotPop /*DELETED*//*- GrowthResInternodePop*/), 0.);
        }
        /*NEW LB*/
        else {
            GrowthStructLeafPop = 0;
            A_GrowthStructLeaf = GrowthStructLeafPop;

            GrowthStructSheathPop = 0;
            GrowthStructInternodePop = 0;
            GrowthStructRootPop = 0;
            GrowthStructPaniclePop = 0;
            GrowthStructTotPop = 0;
            A_GrowthStructTot = GrowthStructTotPop;
        }
        /*/NEW LB*/

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthStructTot_V2_1", URisocas);
    }
}


void RS_AddResToGrowthStructPop_V2_1(double const &NumPhase, double const &Ic, double const &/*PhaseStemElongation*/, double const &/*DryMatResInternodePop*/, double const &DemStructTotPop, double const &DemStructLeafPop, double const &DemStructSheathPop, double const &DemStructRootPop, double const &DemStructInternodePop, double const &DemStructPaniclePop, double const &/*RelMobiliInternodeMax*/, double const &GrowthResInternodePop,
                                     double &ResInternodeMobiliDayPot, double &GrowthStructDeficit, double &GrowthStructLeafPop, double &GrowthStructSheathPop, double &GrowthStructRootPop, double &GrowthStructInternodePop, double &GrowthStructPaniclePop, double &GrowthStructTotPop, double &ResInternodeMobiliDay, double &A_GrowthStructLeaf, double &A_GrowthStructTot, double &A_ResInternodeMobiliDay) {
    /*try*/ {
        if ((NumPhase > 1)) {


            //if (PhaseStemElongation = 1) then
            /*DELETED may 06*//*if (NumPhase > 2) then
      begin
      ResInternodeMobiliDayPot := RelMobiliInternodeMax * DryMatResInternodePop;
      GrowthStructDeficit := Max((DemStructTotPop - GrowthStructTotPop *//*NEW LB*//* - GrowthResInternodePop), 0);*/
            /*DELETED may 06*/ /*end;*/

            if (((Ic < 1) && (DemStructTotPop > 0))) {
                ResInternodeMobiliDay = min(ResInternodeMobiliDayPot, GrowthStructDeficit);

                A_ResInternodeMobiliDay = min(ResInternodeMobiliDayPot, GrowthStructDeficit);
                /*DELETED*/
                /*
        GrowthStructTotPop := GrowthStructLeafPop + GrowthStructSheathPop
        + GrowthStructRootPop + GrowthStructInternodePop +
        GrowthStructPaniclePop + GrowthResInternodePop;
        */
                /*/DELETED*/
                GrowthStructLeafPop = GrowthStructLeafPop + ResInternodeMobiliDay *
                                                                (DemStructLeafPop / DemStructTotPop);

                A_GrowthStructLeaf = GrowthStructLeafPop;

                GrowthStructSheathPop = GrowthStructSheathPop + ResInternodeMobiliDay *
                                                                    (DemStructSheathPop / DemStructTotPop);
                GrowthStructRootPop = GrowthStructRootPop + ResInternodeMobiliDay *
                                                                (DemStructRootPop / DemStructTotPop);
                GrowthStructInternodePop = GrowthStructInternodePop +
                                           ResInternodeMobiliDay * (DemStructInternodePop / DemStructTotPop);
                GrowthStructPaniclePop = GrowthStructPaniclePop + ResInternodeMobiliDay
                                                                      * (DemStructPaniclePop / DemStructTotPop);

                // The following is an update on total growth including mobilization from reserves. Storage does not benefit from mobilization so GrowthResInternodePop is unaltered since module 65, but is included in total growth
                GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop
                                     + GrowthStructRootPop + GrowthStructInternodePop +
                                     GrowthStructPaniclePop /*NEW P*/ + GrowthResInternodePop;

                A_GrowthStructTot = GrowthStructTotPop;

            }
        }

    } /*catch (...)*/ {
        error_message("RS_AddResToGrowthStructPop_V2_1 GrowthStrucTotPop : " + FloatToStr(GrowthStructTotPop), URisocas);
    }
}


void RS_EvolDemPanFilPopAndIcPFlow_V2_1(double const &NumPhase, double const &DryMatStructPaniclePop, double const &CoeffPanSinkPop, double const &SterilityTot, double const &DegresDuJourCor, double const &DegresNumPhase5, double const &SupplyTot, double const &Assim, double const &RespMaintTot, double const &StressCold,
                                        double &PanicleSinkPop, double &DemPanicleFillPop, double &AssimSurplus, double &Ic, double &A_AssimSurplus) {
    /*try*/ {
        if ((NumPhase == 5)) {
            PanicleSinkPop = DryMatStructPaniclePop * CoeffPanSinkPop * (1 - SterilityTot);
            DemPanicleFillPop = (DegresDuJourCor / DegresNumPhase5) * PanicleSinkPop
                                * sqrt(max(0.00001, StressCold));
            Ic = SupplyTot / max(DemPanicleFillPop, 0.0000001);
            if ((Ic <= 0)) {
                Ic = 0;
            }
        }
        if ((NumPhase == 6)) {
            Ic = Assim / RespMaintTot;
            if ((Ic >= 1)) {
                AssimSurplus = max(0., Assim - RespMaintTot);
                A_AssimSurplus = max(0., Assim - RespMaintTot);
            } else {
                AssimSurplus = 0.;
                A_AssimSurplus = 0.;
            }
            if ((Ic < 0)) {
                Ic = 0;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolDemPanFilPopAndIcPFlow_V2_1", URisocas);
    }
}


void RS_EvolPanicleFilPop_V2_1(double const &NumPhase, double const &Ic, double const &DryMatResInternodePop, double const &DemPanicleFilPop, double const &SupplyTot, double const &RelMobiliInternodeMax, double const &RespMaintTot, double const &Assim,
                               double &ResInternodeMobiliDayPot, double &AssimSurplus, double &PanicleFilDeficit, double &ResInternodeMobiliDay, double &PanicleFilPop, double &GrainYieldPop, double &A_AssimSurplus, double &A_ResInternodeMobiliDay) {

    /*try*/ {
        if ((NumPhase == 5)) {
            ResInternodeMobiliDayPot = RelMobiliInternodeMax * DryMatResInternodePop;
            if ((Ic > 1)) {
                PanicleFilPop = max(DemPanicleFilPop, 0.);
                PanicleFilDeficit = 0;
                AssimSurplus = SupplyTot - PanicleFilPop;
                A_AssimSurplus = SupplyTot - PanicleFilPop;
            } else {
                if ((Ic <= 1)) {
                    PanicleFilDeficit = max((DemPanicleFilPop - max(SupplyTot, 0.)), 0.);
                    ResInternodeMobiliDay = max(min(ResInternodeMobiliDayPot, 0.5 *
                                                                                  PanicleFilDeficit), 0.);
                    A_ResInternodeMobiliDay = max(min(ResInternodeMobiliDayPot, 0.5 *
                                                                                    PanicleFilDeficit), 0.);

                    PanicleFilPop = max((SupplyTot + ResInternodeMobiliDay), 0.);
                    AssimSurplus = 0;
                    A_AssimSurplus = 0;
                }
            }
            GrainYieldPop = GrainYieldPop + PanicleFilPop;
        } else {
            if ((NumPhase == 6)) {
                AssimSurplus = Assim - RespMaintTot;
                A_AssimSurplus = Assim - RespMaintTot;
                ResInternodeMobiliDay = min(max(0., RespMaintTot - Assim)
                                            , DryMatResInternodePop);
                A_ResInternodeMobiliDay = min(max(0., RespMaintTot - Assim)
                                              , DryMatResInternodePop);
                PanicleFilPop = 0;

            } else {
                if ((NumPhase > 6)) {
                    ResInternodeMobiliDay = 0;
                    A_ResInternodeMobiliDay = 0;
                }
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolPanicleFilPop_V2_1", URisocas);
    }
}

bool start = false;
void RS_EvolGrowthReserveInternode_V2_1(double const &NumPhase, double const &/*PhaseStemElongation*/, double const &DryMatStructInternodePop, double const &DryMatStructSheathPop, double const &CoeffResCapacityInternode, double const &AssimSurplus, double const &ResInternodeMobiliDay,
                                        double &ResCapacityInternodePop, double &IncreaseResInternodePop, double &DryMatResInternodePop, double &AssimNotUsed, double &AssimNotUsedCum, double &GrowthResInternodePop, double &DryMatResInternodePopOld, double &A_IncreaseResInternodePop) {
    /*try*/ {
        //if ((PhaseStemElongation = 1) or (NumPhase >= 5)) then
        if ((NumPhase >= 2)) {
            /*NEW LB*/
            DryMatResInternodePopOld = DryMatResInternodePop; // Needed to calculate reservesaccumulation for the day which happens in 2 steps
            /*/NEW LB*/

            ResCapacityInternodePop = (DryMatStructInternodePop + DryMatStructSheathPop) *
                                      CoeffResCapacityInternode;

            //growthView := GrowthResInternodePop;
            /*NEW G*/
            if(NumPhase < 5)
                DryMatResInternodePop = DryMatResInternodePop + GrowthResInternodePop;
            // Demand-driven growth of reserve pool
            /*/NEW G*/

            IncreaseResInternodePop = min(max(AssimSurplus, 0.), max((ResCapacityInternodePop - DryMatResInternodePop), 0.));

            A_IncreaseResInternodePop = min(max(AssimSurplus, 0.), max((ResCapacityInternodePop - DryMatResInternodePop), 0.));

            GrowthResInternodePop = IncreaseResInternodePop - ResInternodeMobiliDay;

            //showmessage(FloatToStr(DryMatResInternodePop)+' '+FloatToStr(IncreaseResInternodePop)+' '+FloatToStr(GrowthResInternodePop));

            DryMatResInternodePop = DryMatResInternodePop + GrowthResInternodePop;
            // Surplus- and mobilization-driven growth of reserve pool
            //growthView := GrowthResInternodePop;

            AssimNotUsed = max((AssimSurplus - IncreaseResInternodePop), 0.);
            AssimNotUsedCum = AssimNotUsedCum + AssimNotUsed;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthReserveInternode_V2_1", URisocas);
    }
}


void RS_EvolGrowthReserveInternode(double const &NumPhase, double const &/*PhaseStemElongation*/,
                                   double const &DryMatStructInternodePop, double const &DryMatStructSheathPop,
                                   double const &CoeffResCapacityInternode, double const &AssimSurplus,
                                   double const &ResInternodeMobiliDay, double &ResCapacityInternodePop,
                                   double &IncreaseResInternodePop, double &DryMatResInternodePop,
                                   double &AssimNotUsed, double &AssimNotUsedCum, double &GrowthResInternodePop) {
    /*try*/ {
        //if ((PhaseStemElongation = 1) or (NumPhase >= 5)) then
        if ((NumPhase >= 2)) {
            ResCapacityInternodePop = (DryMatStructInternodePop + DryMatStructSheathPop) *
                                      CoeffResCapacityInternode;
            IncreaseResInternodePop = min(max(AssimSurplus, 0.)
                                          , max((ResCapacityInternodePop - DryMatResInternodePop), 0.));
            GrowthResInternodePop = IncreaseResInternodePop - ResInternodeMobiliDay;
            DryMatResInternodePop = DryMatResInternodePop + GrowthResInternodePop;
            AssimNotUsed = max((AssimSurplus - IncreaseResInternodePop), 0.);
            AssimNotUsedCum = AssimNotUsedCum + AssimNotUsed;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthReserveInternode", URisocas);
    }
}

void RS_EvolGrowthTot_V2_1(double const &NumPhase, double const &GrowthStructLeafPop, double const &GrowthStructSheathPop,
                           double const &GrowthStructRootPop, double const &GrowthStructInternodePop, double const &GrowthStructPaniclePop,
                           double const &/*GrowthResInternodePop*/, double const &PanicleFilPop, double const &DryMatResInternodePop, double const &DryMatResInternodePopOld,
                           double &GrowthStructTotPop, double &GrowthDryMatPop, double &A_GrowthStructTot) {
    /*try*/ {
        if ((NumPhase < 5)) {
            GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop +
                                 GrowthStructRootPop + GrowthStructInternodePop +
                                 GrowthStructPaniclePop;

            A_GrowthStructTot = GrowthStructTotPop;

        } else {
            GrowthStructTotPop = 0;

            A_GrowthStructTot = GrowthStructTotPop;

        }
        if(NumPhase == 6) {
            A_GrowthStructTot = A_GrowthStructTot;
        }
        GrowthDryMatPop = GrowthStructTotPop + /*NEW LB*/ (DryMatResInternodePop - DryMatResInternodePopOld) /*/NEW LB*//*DELETED*/ /*GrowthResInternodePop*/ + PanicleFilPop;

    } /*catch (...)*/ {
        error_message("RS_EvolGrowthTot_V2_1", URisocas);
    }
}

void RS_ExcessAssimilToRoot_V2(double const &NumPhase, double const &ExcessAssimToRoot, double const &DryMatStructRootPop, double const &RootSystVolPop, double const &CoeffRootMassPerVolMax,
                               double &RootMassPerVol, double &GrowthStructRootPop, double &AssimNotUsed) {
    /*try*/ {
        if ((NumPhase > 1)) {
            RootMassPerVol = DryMatStructRootPop / RootSystVolPop;
        }
        if ((ExcessAssimToRoot == 1)) {
            if ((NumPhase < 5) && (NumPhase > 1) && (AssimNotUsed > 0)) {
                if ((RootMassPerVol < CoeffRootMassPerVolMax)) {
                    GrowthStructRootPop = GrowthStructRootPop + AssimNotUsed;
                    AssimNotUsed = 0;
                }
            }
        }

    } /*catch (...)*/ {
        error_message("RS_ExcessAssimilToRoot_V2", URisocas);
    }
}


void RS_EvolDryMatTot_V2_1(double const &NumPhase, double const &ChangePhase, double const &PlantsPerHill, double const &TxResGrain, double const &PoidsSecGrain, double const &Densite,
                           double const &GrowthStructLeafPop, double const &GrowthStructSheathPop, double const &GrowthStructRootPop, double const &GrowthStructInternodePop,
                           double const &GrowthStructPaniclePop, double const &/*GrowthStructTotPop*/, double const &/*GrowthResInternodePop*/, double const &GrainYieldPop,
                           double const &ResCapacityInternodePop, double const &CulmsPerPlant, double const &CoeffPanSinkPop, double const &SterilityTot, double const &DeadLeafDrywtPop,
                           double const &DryMatResInternodePopOld, double const &PanicleFilPop, double const &AssimNotUsedCum, double const &MobiliLeafDeath,
                           double const &RootLignin,

                           double &DryMatStructLeafPop, double &DryMatStructSheathPop, double &DryMatStructRootPop, double &DryMatStructInternodePop, double &DryMatStructPaniclePop,
                           double &DryMatStemPop, double &DryMatStructTotPop, double &DryMatResInternodePop, double &DryMatVegeTotPop, double &DryMatPanicleTotPop, double &DryMatAboveGroundPop,
                           double &DryMatTotPop, double &HarvestIndex, double &InternodeResStatus, double &PanicleNumPop, double &PanicleNumPlant, double &GrainYieldPanicle, double &SpikeNumPop,
                           double &SpikeNumPanicle, double &FertSpikeNumPop, double &GrainFillingStatus, double &RootShootRatio, double &DryMatAboveGroundTotPop, double &CumGrowthPop,
                           double &GrowthPop, double &CumCarbonUsedPop,
                           double &RootLigninPop
                           ) {
    /*try*/ {

        /*NEW LB*/
        CumGrowthPop = CumGrowthPop + GrowthStructLeafPop + GrowthStructSheathPop + GrowthStructInternodePop +
                       GrowthStructRootPop + GrowthStructPaniclePop + (DryMatResInternodePop - DryMatResInternodePopOld) + PanicleFilPop /*NEW R*/ - MobiliLeafDeath;

        GrowthPop = GrowthStructLeafPop + GrowthStructSheathPop + GrowthStructInternodePop + GrowthStructRootPop +
                    GrowthStructPaniclePop + (DryMatResInternodePop - DryMatResInternodePopOld) + PanicleFilPop /*NEW R*/ - MobiliLeafDeath;
        // Output Test variable for carbon balance (consider also AssimNotUsedCum)
        /*/NEW LB*/

        if (((NumPhase == 2) && (ChangePhase == 1))) {
            DryMatTotPop = Densite * PlantsPerHill * TxResGrain * PoidsSecGrain / 1000;
            DryMatStructLeafPop = DryMatTotPop * 0.5;
        } else {
            if ((NumPhase > 1)) {
                DryMatStructLeafPop = DryMatStructLeafPop + GrowthStructLeafPop;
                DryMatStructSheathPop = DryMatStructSheathPop + GrowthStructSheathPop;
                DryMatStructRootPop = DryMatStructRootPop + GrowthStructRootPop;
                RootLigninPop = DryMatStructRootPop * RootLignin / 100;
                DryMatStructInternodePop = DryMatStructInternodePop +
                                           GrowthStructInternodePop;
                DryMatStructPaniclePop = DryMatStructPaniclePop +
                                         GrowthStructPaniclePop;
                DryMatStemPop = DryMatStructSheathPop + DryMatStructInternodePop
                                + DryMatResInternodePop;
                DryMatStructTotPop = DryMatStructLeafPop + DryMatStructSheathPop +
                                     DryMatStructRootPop + DryMatStructInternodePop + DryMatStructPaniclePop;
                DryMatVegeTotPop = DryMatStemPop + DryMatStructLeafPop + DryMatStructRootPop /*DELETED LB*/ /*+ DryMatResInternodePop*/ + DeadLeafDrywtPop;/*DELETED LB*//*DryMatStructTotPop + DryMatResInternodePop -
          DryMatStructPaniclePop;*/
                DryMatPanicleTotPop = DryMatStructPaniclePop + GrainYieldPop;
                DryMatTotPop = DryMatVegeTotPop /*NEW LB*/ + DryMatPanicleTotPop/*/NEW LB*/;/*DELETED LB*/ /*+ GrainYieldPop*/ /*NEW Y*//*+ DryMatStructPaniclePop *//*/NEW Y*//*NEW G*//*+ DeadLeafDryWtPop*//*/NEW G*/ /*;*/
                DryMatAboveGroundPop = DryMatTotPop - DryMatStructRootPop /*NEW LB*/ - DeadLeafDrywtPop;
                /*NEW Y*/
                DryMatAboveGroundTotPop = DryMatAboveGroundPop + DeadLeafDrywtPop;
                /*/NEW Y*/
                /*NEW LB*/
                CumCarbonUsedPop = DryMatTotPop + AssimNotUsedCum; // This should be equal to CumSupplyTot!
                /*/NEW LB*/
                RootShootRatio = DryMatStructRootPop / DryMatAboveGroundPop;
                if ((ResCapacityInternodePop == 0)) {
                    InternodeResStatus = 0;
                } else {
                    InternodeResStatus = DryMatResInternodePop / ResCapacityInternodePop;
                }
            }

            if ((NumPhase > 4)) {
                HarvestIndex = GrainYieldPop / /*NEW LB*/DryMatAboveGroundTotPop; // This includes dead leaves
                PanicleNumPlant = CulmsPerPlant;
                PanicleNumPop = CulmsPerPlant * Densite * PlantsPerHill;
                GrainYieldPanicle = 1000 * GrainYieldPop / PanicleNumPop;
                SpikeNumPop = 1000 * DryMatStructPaniclePop * CoeffPanSinkPop / PoidsSecGrain;
                SpikeNumPanicle = SpikeNumPop / PanicleNumPop;
                FertSpikeNumPop = SpikeNumPop * (1 - SterilityTot);
                GrainFillingStatus = 1000 * (GrainYieldPop / max(FertSpikeNumPop, 0.00000001)) / PoidsSecGrain;
            }
        }

    } /*catch (...)*/
    {error_message("RS_EvolDryMatTot_V2_1 " /*+ E.message*/, URisocas);}

}


void RS_EvalLai_V2_1(double const &NumPhase, double const &ChangePhase, double const &DryMatStructLeafPop,
                     double const &sla, double const &SlaMax, double const &LeafLengthMax, double const &RelPotLeafLength,
                     double const &GrowthStructTotPop, double const &GrowthStructLeafPop, double const &DemStructLeafPop,
                     double &Lai, double &LastLeafLengthPot, double &LastLeafLength) {
    double CorrectedSla;

    /*try*/ {

        //showMessage(FloatToStr(NumPhase));
        if (((NumPhase == 2) && (ChangePhase == 1))) {
            CorrectedSla = SlaMax;
        } else {
            CorrectedSla = sla;

            /*NEW*/
            LastLeafLengthPot = RelPotLeafLength * LeafLengthMax;

            if (GrowthStructTotPop > 0) {
                LastLeafLength = LastLeafLengthPot * sqrt(max(GrowthStructLeafPop, 0.) / DemStructLeafPop);
            } else {
                LastLeafLength = 0;
            }
            /*/NEW*/

        }

        Lai = DryMatStructLeafPop * CorrectedSla;
    } /*catch (...)*/ {
        error_message("RS_EvalLai_V2_1", URisocas);
    }
}


void RS_EvalMaximumLai(double const &NumPhase, double const &ChangePhase, double const &Lai,
                       double &TempLai, double &MaximumLai) {
    /*try*/ {
        if ((Lai > TempLai)) {
            TempLai = Lai;
        }
        if ((NumPhase != 7)) {
            MaximumLai = 0;
        } else if ((NumPhase == 7) && (ChangePhase == 1)) {
            MaximumLai = TempLai;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalMaximumLai", URisocas);
    }
}


void RS_LeafRolling_V2_1(double const &NumPhase, double const &RollingBase, double const &RollingSens, double const &FTSW, double const &Eto,
                         double &KRolling) {
    /*try*/ {
        if ((NumPhase > 1)) {
            KRolling = RollingBase + (1 - RollingBase) * std::pow(FTSW, max(0.0000001
                                                                            , Eto * RollingSens));

            if (KRolling > 1)
                KRolling = 1;

        }

    } /*catch (...)*/ {
        error_message("RS_LeafRolling_V2_1", URisocas);
    }
}


void RS_EvalClumpAndLightInter_V2_1(double const &NumPhase, double const &KRolling, double const &Density,
                                    double const &PlantWidth, double const &/*PlantHeight*/, double const &Kdf,
                                    double const &Lai, double const &FractionPlantHeightSubmer,
                                    double &LIRkdf, double &LIRkdfcl, double &LTRkdf, double &LtrKdfcl) {
    double RolledLai;

    /*try*/ {
        if ((NumPhase > 1) && (PlantWidth > 0)) {
            RolledLai = Lai * KRolling * /*MODIFIED JUNE 20*//*Sqrt*/pow((1 - FractionPlantHeightSubmer), 0.3);
            LIRkdf = 1 - exp(-Kdf * RolledLai);
            LIRkdfcl = (1 - exp(-Kdf * RolledLai * 10000 / min(10000., Density * M_PI *
                                                                           std::pow(PlantWidth / 2000, 2)))) * (min(10000., Density * M_PI *
                                          std::pow(PlantWidth / 2000, 2)) / 10000);
            LTRkdf = 1 - LIRkdf;
            LtrKdfcl = 1 - LIRkdfcl;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalClumpingAndLightInter_V2_1", URisocas);
    }
}


void RS_EvalSlaMitch_V2_2( double const& SlaMax, double const& SlaMin, double const& AttenMitch, double const& SDJ,
                          double const& SDJLevee, double const& NumPhase, double const& DegresDuJour, double const& TOpt1,
                          double const& TBase, double const& TempSla, double const& DryMatStructLeafPop, double const& LeafDeathPop,
                          double const& GrowthStructLeafPop, double const& PAR, double const& PARCritSLA, double const& SLASwitch,
                          double & SlaMitch, double & SlaNew, double & sla, double & SlaMitchAdjusted)
{
    /*try*/ {
        if(  ( NumPhase > 1 ) )
        {
            SlaMitch = SlaMin + ( SlaMax - SlaMin ) * pow( AttenMitch , ( SDJ - SDJLevee ) );

            /* remove 11/01*/
            //SlaNew = SlaMin + (SlaMitch - SlaMin) * pow( min( DegresDuJour / (TOpt1 - TBase), 1. ), TempSla);
            //SlaNew +=  SlaNew * 0.8 * ( 1. - min ( PAR / PARCritSLA, 1. ));
            //// Increased SL for the day's new leaf mass if Par < 6 , at PAR = 1, increase is 50%
            //sla = ( ( sla * DryMatStructLeafPop ) + ( SlaNew * GrowthStructLeafPop ) ) / ( DryMatStructLeafPop + GrowthStructLeafPop );

            SlaMitchAdjusted = SlaMin + (SlaMitch - SlaMin) * pow(min(1., DegresDuJour / (TOpt1 - TBase)), TempSla) + (SlaMitch - SlaMin) * (1 - min(PAR / PARCritSLA, 1.));


            //The SLA of new leaf structural biomass is adjusted according to temperature (no effect if TempSla=0, increasing SLA at temperatures below TOpt1 if TempSla>0) and PAR (increasing SLA if PAR<ParCritSla).
            sla = max(SlaMin, ((DryMatStructLeafPop - GrowthStructLeafPop + LeafDeathPop) * sla + GrowthStructLeafPop*SlaMitchAdjusted - LeafDeathPop*SlaMax) / DryMatStructLeafPop);

            //The T- and PAR-sensitive SlaMitchAdjusted is attributed to new leaves, SlaMax is attributed to dying leaves and previous day’s Sla is attributed to pre-existing leaves.
            if (SLASwitch == 0) {
                sla = SlaMitch;
            }
            //Crop parameter SlaSwitch chooses between fixed SLA pattern and T- and PAR-sensitive SLA.

        }
        else
        {
            SlaMitch = 0;
            SlaNew = 0;
            sla = SlaMax;
        }

    } /*catch (...)*/ {error_message( "RS_EvalSlaMitch_V2_2" , URisocas );
    }
}

void RS_EvalSlaMitch(double const &SlaMax, double const &SlaMin, double const &AttenMitch, double const &SDJ,
                     double const &SDJLevee, double const &NumPhase, double const &DegresDuJour, double const &TOpt1,
                     double const &TBase, double const &TempSla, double const &DryMatStructLeafPop,
                     double const &GrowthStructLeafPop,
                     double &SlaMitch, double &SlaNew, double &sla) {
    /*try*/ {
        if ((NumPhase > 1)) {
            SlaMitch = SlaMin + (SlaMax - SlaMin) * std::pow(AttenMitch, (SDJ - SDJLevee));
            SlaNew = SlaMin + (SlaMitch - SlaMin) * std::pow(DegresDuJour / (TOpt1 - TBase), TempSla);

            sla = ((sla * DryMatStructLeafPop) + (SlaNew * GrowthStructLeafPop)) /
                  (DryMatStructLeafPop + GrowthStructLeafPop);
        } else {
            SlaMitch = 0;
            SlaNew = 0;
            sla = SlaMax;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalSlaMitch", URisocas);
    }
}


void RS_EvalRuiss_FloodDyna_V2(double const &NumPhase, double const &Rain, double const &SeuilRuiss,
                               double const &PourcRuiss, double const &BundHeight, double const &Irrigation,
                               double const &PlantHeight, double const &LifeSavingDrainage, double const &PlotDrainageDAF,
                               double const &VolMacropores, double const &SuilRuiss, double const &/*PercolationMax*/,
                               double const &DAF,
                               double &StockMacropores, double &FloodwaterDepth, double &EauDispo, double &Lr) {
    double CorrectedIrrigation;
    double CorrectedBundheight;

    /*try*/ {
        Lr = 0;
        CorrectedBundheight = BundHeight;
        // implement lifesaving drainage
        if ((LifeSavingDrainage == 1) &&
            (FloodwaterDepth > (0.5 * PlantHeight)) &&
            (PlantHeight > 0) &&
            (NumPhase > 1) &&
            (BundHeight > 0)) {
            CorrectedBundheight = 0.5 * PlantHeight;
            Lr = Lr + max(0., FloodwaterDepth - (0.5 * PlantHeight));
            FloodwaterDepth = min(FloodwaterDepth, (0.5 * PlantHeight));
            if ((FloodwaterDepth + StockMacropores > 0)) {
                EauDispo = FloodwaterDepth + StockMacropores;
            }
        }
        // implement terminal drainage
        if ((NumPhase > 4) && (NumPhase < 7) && (DAF > PlotDrainageDAF) &&
            (BundHeight > 0)) {
            CorrectedBundheight = 0;
            Lr = Lr + FloodwaterDepth;
            FloodwaterDepth = 0;
            if (((FloodwaterDepth + StockMacropores) > 0)) {
                EauDispo = StockMacropores;
            } else {
                EauDispo = Rain;
            }
        }
        // define corrected irrigation
        if ((Irrigation == NilValue)) {
            CorrectedIrrigation = 0;
        } else {
            CorrectedIrrigation = Irrigation;
        }
        // implement runoff and EauDispo under terminal drainage
        if ((CorrectedBundheight == 0) && (BundHeight != CorrectedBundheight)) {
            if (((StockMacropores + FloodwaterDepth) == 0)) {
                EauDispo = Rain + CorrectedIrrigation;
            } else {
                StockMacropores = StockMacropores + Rain + CorrectedIrrigation;
                Lr = Lr + max(0., StockMacropores - VolMacropores);
                StockMacropores = StockMacropores - max(0., StockMacropores -
                                                                VolMacropores);
                EauDispo = StockMacropores;
            }
        }
        // implement classical upland runoff (SARRAH)
        if ((BundHeight == 0)) {
            if ((Rain > SuilRuiss)) {
                Lr = Lr + (Rain + CorrectedIrrigation - SeuilRuiss) * PourcRuiss / 100;
                EauDispo = Rain + CorrectedIrrigation - Lr;
            } else {
                EauDispo = Rain + CorrectedIrrigation;
            }
        }
        // implement bunded-plot style water ponding and runoff, regular situation w/o drainage
        if ((CorrectedBundheight > 0)) {
            if (((StockMacropores + FloodwaterDepth) == 0)) {
                Lr = Lr + max((Rain + CorrectedIrrigation - BundHeight -
                               VolMacropores), 0.);
                EauDispo = min(Rain + CorrectedIrrigation, BundHeight + VolMacropores);
            } else {
                StockMacropores = StockMacropores + Rain + CorrectedIrrigation;
                FloodwaterDepth = FloodwaterDepth + max(0., StockMacropores -
                                                                VolMacropores);
                StockMacropores = min(VolMacropores, StockMacropores);
                Lr = Lr + max(0., FloodwaterDepth - CorrectedBundheight);
                FloodwaterDepth = min(FloodwaterDepth, CorrectedBundheight);
                EauDispo = StockMacropores + FloodwaterDepth;
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvalRuiss_FloodDyna_V2", URisocas);
    }
}


void RS_AutomaticIrrigation_V2_1(double const &NumPhase, double const &IrrigAuto, double const &IrrigAutoTarget, double const &BundHeight, double const &PlantHeight,
                                 double const &PlotDrainageDAF, double const &DAF, double const &VolMacropores, double const &VolRelMacropores, double const &Pluie, double const &IrrigAutoStop,
                                 double const &IrrigAutoResume, double const &ChangeNurseryStatus, double const &PercolationMax, double const &NbJas, double const &RuSurf, double const &Ru,
                                 double const &RootFront, double const &EpaisseurSurf, double const &EpaisseurProf, double const &FTSWIrrig, double const &Transplanting, double const &DurationNursery,
                                 double const &NurseryStatus, double const& Rain,
                                 double &Irrigation, double &Lr,
                                 double &FloodwaterDepth, double &IrrigAutoDay, double &IrrigTotDay, double &StockMacropores, double &EauDispo, double &RuRac, double &StockRac, double &FTSW) {
    double IrrigAutoTargetCor;
    double CorrectedIrrigation;
    double CorrectedBundheight;
    double StressPeriod;

    CorrectedBundheight = BundHeight;
    StressPeriod = 0;

    if ( Irrigation == NilValue ) {
        CorrectedIrrigation = 0;
    } else {
        CorrectedIrrigation = Irrigation;
    }

    if ((NumPhase > 4) && (NumPhase < 7) && (DAF > PlotDrainageDAF)) {
        CorrectedBundheight = 0;
    }

    if ((NbJas >= IrrigAutoStop) && (NbJas < IrrigAutoResume))
        StressPeriod = 1;
    else
        StressPeriod = 0;

    // Regular automatic irrigation in mm if FTSW drops below FtswIrrig
    if ( (NumPhase<7) && (DAF<=PlotDrainageDAF) && (IrrigAuto==1) && (NumPhase>0) && (CorrectedBundheight>0) && (FTSW<=FTSWIrrig) && (StressPeriod==0) ) {

        IrrigAutoTargetCor = min( (IrrigAutoTarget * BundHeight), (0.5 * PlantHeight) );

        //ADDED 30/06/2023
        if ( (Transplanting == 0 && NbJas >= 1) || ((Transplanting == 1) && (NbJas > (DurationNursery+1)) ) ) {
            IrrigAutoDay = max(0., IrrigAutoTargetCor - FloodwaterDepth + (VolMacropores - StockMacropores));
        } else {
            IrrigAutoDay = 0;
        }

    }

    // Provide initial water flush for infiltration under direct seeding, in mm
    if ( (NbJas == 1) && (Transplanting == 0) && (BundHeight>1) && (IrrigAuto==1) ) {
        IrrigAutoTargetCor = max(IrrigAutoTargetCor, BundHeight / 2);
        IrrigAutoDay = max(0., ( IrrigAutoTargetCor - FloodwaterDepth + (VolMacropores - StockMacropores) + RuSurf + PercolationMax) );
    }


    // Stop field irrigation while plants are in nursery
    if ( (Transplanting == 0) && (NurseryStatus == 0) ) {
        IrrigAutoDay = 0;
    }

    // Pre-irrigation at transplanting, in mm
    if ( (Transplanting == 1) && (ChangeNurseryStatus == 1) ) {
        IrrigAutoTargetCor = max(IrrigAutoTargetCor, BundHeight / 2);
        // dimension irrigation on day of transplanting, happens only on one day in the simulation
        IrrigAutoDay = max(0., (IrrigAutoTargetCor - FloodwaterDepth + (VolMacropores - StockMacropores) + RuSurf + PercolationMax));
    }

    if ( (NumPhase < 7) && (StressPeriod == 1) ) {
        IrrigAutoDay = 0;
        Irrigation = 0;
    }

    // Implement user-defined terminal plot drainage
    if ( (NumPhase > 4) && (NumPhase < 7) && (DAF > PlotDrainageDAF) ) {
        CorrectedBundheight = 0;
        IrrigAutoDay = 0;
        Lr = Lr + FloodwaterDepth;
        FloodwaterDepth = 0;
    }

    //  Implement user-defined stress period
    StressPeriod = 0;
    if ( (NbJas >= IrrigAutoStop) && (NbJas < IrrigAutoResume) ) {
        StressPeriod = 1;
        IrrigAutoDay = 0;
        CorrectedIrrigation = 0;
    } else {
        CorrectedIrrigation = Irrigation;
    }

    // all the irrigations calculated in this module must be cumulated in Module 93 EvalRUE for variables CumIrrig and CumWReceived
    // Update water reservoirs

    if ((StockMacropores + FloodwaterDepth) == 0) {
        EauDispo = Rain + CorrectedIrrigation + IrrigAutoDay;
    } else {
        FloodwaterDepth = FloodwaterDepth + IrrigAutoDay;
        // make sure Macropores is fully filled before floodwater can build up!
        if ( (VolMacropores > 0) && (StockMacropores < VolMacropores) && (FloodwaterDepth > 0) ) {
            StockMacropores = StockMacropores + FloodwaterDepth;
            FloodwaterDepth = max(0., StockMacropores - VolMacropores);
            StockMacropores = StockMacropores - FloodwaterDepth;
            RuRac = Ru * RootFront / 1000;
//            if (RootFront == 0)
//                RuRac = /*NEW JUNE 18*//*Ru*/ResUtil * ProfRacIni / 1000;
//            else
//                RuRac = /*NEW JUNE 18*//*Ru*/ResUtil * RootFront / 1000;
            StockRac = RuRac + StockMacropores * RootFront / (EpaisseurSurf + EpaisseurProf);
//            if (RootFront == 0)
//                StockRac = RuRac + StockMacropores * ProfRacIni / (EpaisseurSurf + EpaisseurProf);
//            else
//                StockRac = RuRac + StockMacropores * RootFront / (EpaisseurSurf + EpaisseurProf);
            FTSW = StockRac / RuRac;
        }

        EauDispo = StockMacropores + FloodwaterDepth; // must Rain + irrigations be added???
    }

    if ((NumPhase < 7) && (DAF <= PlotDrainageDAF) && (IrrigAuto == 1) && (NumPhase > 0) && (CorrectedBundheight == 0)) {
        FloodwaterDepth = 0;
        StockMacropores = 0;
        /*DELETED JUNE 18*//*StockMacropores := 0;*/
    }
    IrrigTotDay = CorrectedIrrigation + IrrigAutoDay;
}


void RS_EvolRempliResRFE_RDE_V2(double const &/*NumPhase*/, double const &/*RuSurf*/, double const &EauDispo, double const &RuRac, double const &CapaRFE, double const &CapaREvap, double const &CapaRDE, double const &StRuMax, double const &PercolationMax, double const &BundHeight, double const &/*EpaisseurSurf*/, double const &/*EpaisseurProf*/, double const &VolMacropores,
                                double &FloodwaterDepth, double &StockTotal, double &StockRac, double &Hum, double &StockSurface, double &Dr, double &ValRDE, double &ValRFE, double &ValRSurf, double &/*FloodWaterGain*/, double &StockMacropores) {
    double EauReste; double ValRSurfPrec; double EauTranspi;

    /*try*/ {
        Dr = 0;
        EauTranspi = 0;
        if ((StockMacropores + FloodwaterDepth == 0)) {
            EauReste = 0;
            ValRFE = ValRFE + EauDispo;
            if ((ValRFE > CapaRFE)) {
                EauReste = ValRFE - CapaRFE;
                ValRFE = CapaRFE;
            }
            ValRSurfPrec = ValRSurf;
            ValRSurf = ValRSurf + EauReste;
            if ((ValRSurfPrec < CapaREvap)) {
                EauTranspi = EauDispo - (min(CapaREvap, ValRSurf) - ValRSurfPrec);
            } else {
                EauTranspi = EauDispo;
            }
            if ((ValRSurf > (CapaREvap + CapaRDE))) {
                ValRSurf = CapaREvap + CapaRDE;
                ValRDE = CapaRDE;
            } else {
                if ((ValRSurf <= CapaREvap)) {
                    ValRDE = 0;
                } else {
                    ValRDE = ValRSurf - CapaREvap;
                }
            }
            StockSurface = ValRFE + ValRDE;
            StockTotal = StockTotal + EauTranspi;
            if ((StockTotal > StRuMax)) {
                Dr = StockTotal - StRuMax;
                StockTotal = StRuMax;
            } else {
                Dr = 0;
            }
            if (Hum < (CapaRFE + CapaRDE)) {
                Hum = StockSurface;
            } else {
                Hum = max(Hum, StockTotal);
            }
        }
        StockRac = min(StockRac + EauTranspi, RuRac);
        // Shifting non-percolating Dr back to macropores & floodwater if plot is bunded
        if ((BundHeight > 0)) {
            // Shifting non-percolating Dr to Floodwater
            StockMacropores = StockMacropores + max(0., Dr - PercolationMax);
            Dr = min(Dr, PercolationMax);
            if ((StockMacropores > VolMacropores)) {
                FloodwaterDepth = FloodwaterDepth + (StockMacropores - VolMacropores);
                StockMacropores = VolMacropores;
            }
            // Implementing Dr
            if ((FloodwaterDepth >= PercolationMax)) {
                Dr = PercolationMax;
                FloodwaterDepth = FloodwaterDepth - Dr;
                StockMacropores = VolMacropores;
            } else {
                if ((FloodwaterDepth < PercolationMax) && ((FloodwaterDepth +
                                                            StockMacropores) >= PercolationMax)) {
                    Dr = PercolationMax;
                    FloodwaterDepth = FloodwaterDepth - Dr;
                    StockMacropores = StockMacropores + FloodwaterDepth;
                    FloodwaterDepth = 0;
                } else {
                    Dr = min(PercolationMax, (FloodwaterDepth + StockMacropores + Dr));
                    FloodwaterDepth = 0;
                    StockMacropores = 0;
                }
            }
        }

    } /*catch (...)*/ {
        error_message("RS_EvolRempliResRFE_RDE_V2", URisocas);
    }
}


void RS_EvolWaterLoggingUpland_V2(double const &PercolationMax, double const &BundHeight, double const &VolMacropores,
                                  double &Dr, double &Lr, double &StockMacropores) {
    /*try*/ {
        if ((Dr > PercolationMax) && (BundHeight == 0)) {
            StockMacropores = StockMacropores + (Dr - PercolationMax);
            Lr = Lr + max(0., StockMacropores - VolMacropores);
            Dr = PercolationMax;
            StockMacropores = min(StockMacropores, VolMacropores);
        }

    } /*catch (...)*/ {
        error_message("RS_EvolWaterLoggingUpland_V2", URisocas);
    }
}


void RS_EvalStressWaterLogging_V2(double const &StockMacropores, double const &VolMacropores, double const &RootFront, double const &EpaisseurSurf, double const &EpaisseurProf, double const &WaterLoggingSens,
                                  double &FractionRootsLogged, double &CoeffStressLogging) {
    /*try*/ {
        if ((StockMacropores > 0) && (RootFront > 0)) {
            FractionRootsLogged = (max(0., RootFront - ((VolMacropores -
                                                         StockMacropores) / VolMacropores) * (EpaisseurSurf + EpaisseurProf))) /
                                  RootFront;
            CoeffStressLogging = 1 - (FractionRootsLogged * min(1.
                                                                , WaterLoggingSens));
        }

    } /*catch (...)*/ {
        error_message("RS_EvalStressWaterLogging_V2", URisocas);
    }
}



void RS_EvolRempliMacropores_V2(double const &/*NumPhase*/, double const &EpaisseurSurf, double const &EpaisseurProf, double const &ResUtil, double const &StockMacropores, double const &RootFront, double const &CapaRDE, double const &CapaRFE, double const &FloodwaterDepth,
                                double &StockTotal, double &Hum, double &StockSurface, double &StockRac, double &ValRDE, double &ValRFE, double &ValRSurf) {
    /*try*/ {
        if (((StockMacropores + FloodwaterDepth) > 0)) {
            StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
                         StockMacropores;
            Hum = StockTotal;
            StockSurface = EpaisseurSurf * ResUtil / 1000 + (EpaisseurSurf /
                                                             (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
            StockRac = RootFront * ResUtil / 1000 + (RootFront / (EpaisseurSurf +
                                                                  EpaisseurProf)) * StockMacropores;
            ValRDE = CapaRDE;
            ValRFE = CapaRFE;
            ValRSurf = EpaisseurSurf * ResUtil / 1000;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolRempliMacropores_V2", URisocas);
    }
}


void RS_EvolRurRFE_RDE_V2_1(double const &VitesseRacinaire, double const &Hum, double const &ResUtil, double const &/*StockSurface*/, double const &RuSurf, double const &ProfRacIni, double const &EpaisseurSurf, double const &EpaisseurProf, double const &ValRDE, double const &ValRFE, double const &NumPhase, double const &ChangePhase, double const &FloodwaterDepth, double const &StockMacropores, double const &RootFrontMax, double const &ChangeNurseryStatus, double const &Transplanting, double const &TransplantingDepth,
                            double &RuRac, double &StockRac, double &StockTotal, double &/*FloodWaterGain*/, double &RootFront) {
    double DeltaRur;

    /*try*/ {
        if ((NumPhase == 0)) {
            RuRac = 0;
            StockRac = 0;
        } else {
            if (((NumPhase == 1) && (ChangePhase == 1)))
            // les conditions de germination sont atteinte et nous sommes le jour même
            {
                RuRac = ResUtil * min(ProfRacIni, (EpaisseurSurf + EpaisseurProf)) /
                        1000;
                if ((ProfRacIni <= EpaisseurSurf)) {
                    StockRac = (ValRDE + ValRFE) * ProfRacIni / EpaisseurSurf;
                } else {
                    StockRac = StockTotal * min(ProfRacIni / (EpaisseurSurf +
                                                              EpaisseurProf), 1.);
                }
            } else {
                if ((Hum - StockMacropores - RuRac) < (VitesseRacinaire / 1000 * ResUtil)) {
                    DeltaRur = max(0., Hum - StockMacropores - RuRac);

                    /*NEW P*/
                    if ((RootFront >= (EpaisseurSurf + EpaisseurProf)) || (RootFront >= RootFrontMax))
                        DeltaRur = 0;
                    // limit root front progression to RootFrontMax and soil depth
                    /*/NEW P*/

                } else {
                    DeltaRur = VitesseRacinaire / 1000 * ResUtil;

                    /*NEW Y*/
                    if (/*NEW P*/(RootFront >= (EpaisseurSurf + EpaisseurProf)) || /*/NEW P*/(RootFront >= RootFrontMax))
                        DeltaRur = 0;
                    // limit root front progression to RootFrontMax and soil depth
                    /*/NEW Y*/

                }
                RuRac = RuRac + DeltaRur;
                if ((RuRac > RuSurf)) {
                    StockRac = StockRac + DeltaRur;
                } else {
                    StockRac = (ValRDE + ValRFE) * (RuRac / RuSurf);
                }
            }
        }
        // The following is needed to have the correct basis for calculating FTSW under
        // supersaturated soil condition (macropores filled)
        if ((NumPhase != 0)) {
            RootFront = ((1 / ResUtil) * RuRac) * 1000;

            /*NEW Y*/
            if ((ChangeNurseryStatus == 1) && (Transplanting == 1)) {
                RootFront = TransplantingDepth;

                if ((RootFront < 40))
                    RootFront = 40; else if ((RootFront > 200))
                    RootFront = 200;

                // Security: avoid aberrant values for transplanting depth

                // set new root front to depth of transplanting
                RuRac = RootFront * ResUtil / 1000;

            }
            /*/NEW Y*/


        }
        if (((StockMacropores + FloodwaterDepth) > 0)) {
            StockRac = RootFront * ResUtil / 1000 + (RootFront / (EpaisseurSurf +
                                                                  EpaisseurProf)) * StockMacropores;
            StockRac = min(StockRac, StockTotal);
        }

    } /*catch (...)*/ {
        error_message("RS_EvolRurRFE_RDE_V2_1", URisocas);
    }
}


void RS_PlantSubmergence_V2(double const &PlantHeight, double const &FloodwaterDepth,
                            double &FractionPlantHeightSubmer) {
    /*try*/ {
        FractionPlantHeightSubmer = min(max(0., FloodwaterDepth / max(PlantHeight
                                                                      , 0.1)), 1.);

    } /*catch (...)*/ {
        error_message("RS_PlantSubmergence_V2", URisocas);
    }
}

void RS_EvalRootFront(double const &NumPhase, double const &RuRac, double const &ResUtil,
                      double &RootFront) {
    /*try*/ {
        if ((NumPhase > 0)) {
            RootFront = ((1 / ResUtil) * RuRac) * 1000;
        }

    } /*catch (...)*/ {
        error_message("RS_EvalRootFront", URisocas);
    }
}


void RS_EvolPSPMVMD(double const &NumPhase, double const &ChangePhase, double const &SomDegresJourCor, double const &DegresDuJourCor, double const &SeuilPP, double const &PPCrit, double const &DureeDuJour, double const &PPExp,
                    double &SumPP, double &SeuilTempPhasePrec, double &SeuilTempPhaseSuivante) {
    double SDJPSP;
    /*Procedure speciale Vaksman Dingkuhn valable pour tous types de sensibilite
  photoperiodique et pour les varietes non photoperiodique. PPsens varie de 0,4
  a 1,2. Pour PPsens > 2,5 = variété non photoperiodique. SeuilPP = 13.5 PPcrit = 12
  SumPP est dans ce cas une variable quotidienne (et non un cumul) testee dans
  EvolPhenoPhotoperStress*/

    /*try*/ {
        if ((NumPhase == 3)) {
            if ((ChangePhase == 1)) {
                SumPP = 100; //valeur arbitraire d'initialisation >2.5
                SDJPSP = max(0.01, DegresDuJourCor);
            } else {
                SDJPSP = SomDegresJourCor - SeuilTempPhasePrec + max(0.01
                                                                     , DegresDuJourCor);
            }
            SumPP = std::pow((1000 / SDJPSP), PPExp) * max(0., (DureeDuJour - PPCrit)) /
                    (SeuilPP - PPCrit);
            SeuilTempPhaseSuivante = SeuilTempPhasePrec + SDJPSP;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolPSPMVMD", URisocas);
    }
}


void EvolSomDegresJour(double const &DegresDuJour, double const &NumPhase,
                       double &SommeDegresJour) {
    /*try*/ {
        if ((NumPhase >= 1)) {
            SommeDegresJour = SommeDegresJour + DegresDuJour;
        } else {
            SommeDegresJour = 0;
        }

    } /*catch (...)*/ {
        error_message("EvolSommeDegresJour | DegresDuJour: " + FloatToStr(DegresDuJour) +
                          " Phase n°" + FloatToStr(NumPhase) +
                          " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                          " SommeDegresJour: " + FloatToStr(SommeDegresJour), UMilBilanCarbone);
    }
}


void RS_EvolSomDegresJourCor(double const &DegresDuJourCor, double const &NumPhase,
                             double &SommeDegresJourCor) {
    /*try*/ {
        if ((NumPhase >= 1)) {   // on ne cumule qu'après la germination
            SommeDegresJourCor = SommeDegresJourCor + DegresDuJourCor;
        } else {
            SommeDegresJourCor = 0;
        }

    } /*catch (...)*/ {
        error_message("RS_EvolSomDegresJourCor", URisocas);
    }
}



// Modified 8/19/13
void RS_EvalRUE_V2_2( double const& NumPhase, double const& ChangePhase, double const& PARIntercepte, double const& DryMatTotPop,
                     double const& /*DeadLeafDrywtPop*/, double const& /*DryMatStructRootPop*/, double const& Tr, double const& Evap,
                     double const& Dr, double const& Lr, double const& SupplyTot, double const& AssimNotUsed, double const& Irrigation,
                     double const& IrrigAutoDay, double const& Pluie, double const& Assim, double const& AssimPot, double const& Conversion,
                     double const& NbJas, double const& Transplanting, double const& NurseryStatus, double const& Density, double const& DensityNursery,
                     double const& DryMatAboveGroundTotPop, double const& DryMatAboveGroundPop, double & RUE, double & CumPar, double & CumTr, double & CumEt, double & CumWUse,
                     double & CumWReceived, double & CumIrrig, double & CumDr, double & CumLr, double & TrEffInst, double & TrEff, double & WueEt,
                     double & WueTot, double & ConversionEff, double & RUEgreen)
{
    double CorrectedIrrigation;

    /*try*/ {
        if(  ( ( NumPhase < 1 ) || ( ( NumPhase == 1 ) && ( ChangePhase == 1 ) ) ) /*NEW G*/|| ( Density == DensityNursery )/*/NEW G*/ )
        {
            CumPar = 0;
            RUE = 0;
            CumTr = 0.00001;
            CumEt = 0.00001;
            CumWUse = 0;
            CumWReceived = 0;
            CumIrrig = 0;
            CumDr = 0;
            CumLr = 0;
        }

        else

        {
            /*NEW Y*/
            if(  ( Transplanting == 0 ) || ( NurseryStatus == 1 ) )
            {
                CumPar = CumPar + PARIntercepte;
                CumTr = CumTr + Tr;
                CumEt = CumEt + Tr + Evap;
                CumWUse = CumWUse + Tr + Evap + Dr + Lr;
            }
            /*/NEW Y*/


            if(  ( Irrigation == NilValue ) )
            {
                CorrectedIrrigation = 0;
            }
            else

            {
                CorrectedIrrigation = Irrigation;
            }

            /*NEW Y*/
            if(  ( Transplanting == 0 ) || ( NurseryStatus == 1 ) )
            {
                CumWReceived = CumWReceived + Pluie + CorrectedIrrigation + IrrigAutoDay;
                CumIrrig = CumIrrig + CorrectedIrrigation + IrrigAutoDay;
//                if (FirstDayIrrig > 0) {
//                    CumWReceived += FirstDayIrrig;
//                    CumIrrig += FirstDayIrrig;
//                    FirstDayIrrig = 0;
//                }
                CumDr = CumDr + Dr;
                CumLr = CumLr + Lr;
            }


            /*/NEW Y*/

            if(  ( AssimPot != 0 ) )
            {
                ConversionEff = Conversion * Assim / /*NEW JUNE*/ ( PARIntercepte * Conversion * 10 )/*AssimPot*/;
            }
            if(  ( ( Tr > 0 ) && ( NbJas > /*NEW G*/20/*/NEW G*/ ) && ( NumPhase > 1 ) ) )
            {
                TrEffInst = ( SupplyTot - AssimNotUsed ) / ( Tr * 10000 );
                TrEff = DryMatTotPop / ( CumTr * 10000 );
                WueEt = DryMatTotPop / ( CumEt * 10000 );
                WueTot = DryMatTotPop / ( CumWUse * 10000 );

                /*DELETED G*/
                //RUE := ((DryMatTotPop + DeadLeafDrywtPop - DryMatStructRootPop) / Max(CumPar, 0.00001)) / 10;
                //(Please delete this grey line and replace with the following one in green)
                /*/DELETED G*/

                /*NEW G*/
                RUE = ( DryMatAboveGroundTotPop / max( CumPar , 0.00001 ) ) / 10;
                /*/NEW G*/

                RUEgreen = ( DryMatAboveGroundPop / max( CumPar , 0.00001 ) ) / 10;

            }
        }

        //growthView := cumPar ;


    } /*catch (...)*/ {
        error_message( "RS_EvalRUE_V2_2" , URisocas );
    }
}

void RS_EvalRUE_V2_1(double const &NumPhase, double const &ChangePhase, double const &PARIntercepte, double const &DryMatTotPop,
                     double const &/*DeadLeafDrywtPop*/, double const &/*DryMatStructRootPop*/, double const &Tr, double const &Evap,
                     double const &Dr, double const &Lr, double const &SupplyTot, double const &AssimNotUsed, double const &Irrigation,
                     double const &IrrigAutoDay, double const &Pluie, double const &Assim, double const &AssimPot, double const &Conversion,
                     double const &NbJas, double const &Transplanting, double const &NurseryStatus, double const &Density, double const &DensityNursery,
                     double const &DryMatAboveGroundTotPop, double const &DryMatAboveGroundPop, double &RUE, double &CumPar, double &CumTr, double &CumEt, double &CumWUse,
                     double &CumWReceived, double &CumIrrig, double &CumDr, double &CumLr, double &TrEffInst, double &TrEff, double &WueEt,
                     double &WueTot, double &ConversionEff, double & RUEgreen) {
    double CorrectedIrrigation;

    /*try*/ {
        if (((NumPhase < 1) || ((NumPhase == 1) && (ChangePhase == 1))) /*NEW G*/ || (Density == DensityNursery)/*/NEW G*/) {
            CumPar = 0;
            RUE = 0;
            CumTr = 0;
            CumEt = 0.00001;
            CumWUse = 0.00001;
            CumWReceived = 0;
            CumIrrig = 0;
            CumDr = 0;
            CumLr = 0;
        }

        else

        {

            /*NEW Y*/
            if ((Transplanting == 0) || (NurseryStatus == 1)) {
                CumPar = CumPar + PARIntercepte;
                CumTr = CumTr + Tr;
                CumEt = CumEt + Tr + Evap;
                CumWUse = CumWUse + Tr + Evap + Dr + Lr;
            }
            /*/NEW Y*/

            if ((Irrigation == NilValue)) {
                CorrectedIrrigation = 0;
            } else {
                CorrectedIrrigation = Irrigation;
            }

            /*NEW Y*/
            if ((Transplanting == 0) || (NurseryStatus == 1)) {
                CumWReceived = CumWReceived + Pluie + CorrectedIrrigation + IrrigAutoDay;
                CumIrrig = CumIrrig + CorrectedIrrigation + IrrigAutoDay;
                CumDr = CumDr + Dr;
                CumLr = CumLr + Lr;
            }
            /*/NEW Y*/

            if ((AssimPot != 0)) {
                ConversionEff = Conversion * Assim / /*NEW JUNE*/ (PARIntercepte * Conversion * 10)/*AssimPot*/;
            }

            if (((Tr > 0) && (NbJas > 20) && (NumPhase > 1))) {
                TrEffInst = (SupplyTot - AssimNotUsed) / (Tr * 10000);
                TrEff = DryMatTotPop / (CumTr * 10000);
                WueEt = DryMatTotPop / (CumEt * 10000);
                WueTot = DryMatTotPop / (CumWUse * 10000);

                /*DELETED G*/
                //                RUE := ((DryMatTotPop + DeadLeafDrywtPop - DryMatStructRootPop) / Max(CumPar, 0.00001)) / 10;
                //(Please delete this grey line and replace with the following one in green)
                /*/DELETED G*/

                /*NEW G*/
                RUE = (DryMatAboveGroundTotPop / max(CumPar, 0.00001)) / 10;
                /*/NEW G*/
                RUEgreen = ( DryMatAboveGroundPop / max( CumPar , 0.00001 ) ) / 10;

            }
        }

        //growthView := cumPar ;


    } /*catch (...)*/ {
        error_message("RS_EvalRUE_V2_1", URisocas);
    }
}


/**********************************************/
/// LODGING
///
///
/**********************************************/

void EvalLodgingResistance(double const &NumPhase, double const &MatuProgress, double const &DryMatStructLeafPop,
                           double const &DryMatStemPop, double const &DeadLeafDrywtPop, double const &DryMatPanicleTotPop,
                           double const &PlantHeight, double const &CoeffLodging, double const &StemPorosity, double const &ApexHeight, double const &CulmsPop,
                           double &GrainMoisture, double &FreshMatPanicleTotPop, double &StemVigor, double &LodgingIndex,
                           double &FreshMatAbovegroundPop, double &LodgingResistance, double &LodgingResistance2, double &StemSurfMean,
                           double &StemDiaMean, double &StemDiaBase) {
    /*try*/ {
        if ( NumPhase > 4) {
            GrainMoisture = 0.7 - 0.5 * pow(MatuProgress, 2);
            //GrainMoisure equals 0.70 at flowering and decreases progressively to 0.20 (=0.7-0.5) during Matu1 and Matu2
            FreshMatPanicleTotPop = DryMatPanicleTotPop / (1-GrainMoisture) ;
            // FreshMatPanicleTotPop is an output variable!
            FreshMatAbovegroundPop  =  DryMatStructLeafPop / 0.3 + DryMatStemPop / 0.5 + DeadLeafDrywtPop / 0.7 + FreshMatPanicleTotPop;
            // FreshMatAbovegroundPop is an output variable! (it doesn’t consider drought effects yet);
            StemVigor = 1000000 * (DryMatStemPop / CulmsPop) / ApexHeight; // in grams per meter ;
            LodgingIndex = 0.0000001 * PlantHeight * FreshMatAbovegroundPop / StemVigor;
            //calculated in grams and meters units; LodgingIndex is an output variable
            LodgingResistance = CoeffLodging / LodgingIndex;
            //CoeffLodging is a new varietal parameter that depends on stem chemis/*try*/ and anatomy, traits we cannot simulate. Its value is empirical and can only be estimated if lodging has been observed in an experiment and compared with simulated LodgingIncidence. LodgingResistance is an output variable and will be used to calculate actual lodging on the basis of wind speed and rain in module EvalLodging_Incidence.


            //            // Estimation of stem thickness
            StemSurfMean = StemVigor * 2.5 / (1-StemPorosity);
            // StemPorosity is a new crop parameter expressing fraction air spaces in stem; default value is 0.67 (two-thirds); coefficient of 2.5 estimates water content (w/w); in mm2
            StemDiaMean = 2 * sqrt(StemSurfMean / M_PI); //in mm
            StemDiaBase = StemDiaMean * 1.3;
            //            // rough estimate of stem thickness distribution bottom to top; coefficient should be smaller in sorghum because it has less sheath around the culm at the base.
            LodgingResistance2 = LodgingResistance * StemDiaBase;
            //This is an alternative measure of LR that factors in stem base thickness (to be fitting using StemPorosity parameter). The StemPorosity parameter only effects stem thickness and nothing else in the simulation.

        }
    } /*catch (...)*/ {
        error_message("EvalLodgingResistance", URiz);
    }
}

void EvalLodgingIncidence(double const &NumPhase, double const &LodgingResistance, double const &Vt,
                          double const &Pluie,
                          double &LodgingDay, double &Lodging, double &LodgingPot) {
    /*try*/ {
        if ( NumPhase > 4) {
            LodgingDay = ((Vt * Vt) * (1+Pluie/100)) / LodgingResistance;
            //It may be necessary to introduce a Vt * Pluie threshold, to be studied
            LodgingPot = max(Lodging, LodgingDay);
            Lodging = min(LodgingPot, 100.);
            //Please don’t forget to initialize Lodging to zero!
        }
    } /*catch (...)*/ {
        error_message("EvalLodgingIncidence", URiz);
    }
}


void WetOrDryDirectSeeding(double const &DrySeedingSwitch, double const &Transplanting, double const &NumPhase, double const &StockSurface, double const &RuSurf,
                         double &GerminationStatus, double &DaysDrySowingToGermination, int &TriggerInitCulture) {
    // if (DrySeedingSwitch =0) then pre-germinated seed is sown on wet field; if (DrySeedingSwitch =1) then dry seed is sown, and germination is halted until topsoil is moist; default value should be zero.
//If (DrySeedingSwitch=1) and (Transplanting =0) and (NumPhase = 1) and (GerminationStatus :=0) then
//Begin
//	// There is enough water in surface soil layer for germination of dry seed:
//	If (StockSurface >= (0.9 * RuSurf)) then (GerminationStatus = 1);
//	// not enough water for germination of dry seed:
//	If (StockSurface < (0.9 * RuSurf)) then
//	Begin
//		GerminationStatus = 0;
//		DaysDrySowingToGermination := DaysDrySowingToGermination + 1;
//		// Output variable describing nb of days after sowing when germination happened; add to output list!
//  Go back to Module n°2 - RS_InitiationCulture (how do you program this?)
//// Module “InitiationCulture” is executed because seed is still too dry for germination
//End;
//End;

    if ( DrySeedingSwitch == 1 && Transplanting == 0 && NumPhase == 1 && GerminationStatus == 0) {
        if ( StockSurface >= (0.9 * RuSurf) ) {
            GerminationStatus = 1;
        }

        if ( StockSurface < (0.9 * RuSurf) ) {
            GerminationStatus = 0;
            DaysDrySowingToGermination = DaysDrySowingToGermination + 1;
            TriggerInitCulture = 1;
        }
    }

}


//##############################################################################
/// Ce module permet de suivre l'évolution des 5 derniers jours de Cstr afin de
/// moyenner la valeur des stress. Si la moyenne des Cstr est inférieure à
/// un seuil SorghumMortality, la culture décède (NumPhase=8)
/// Demande MD du 28/09/06
//##############################################################################

void SorghumMortality(double const &cstr, double const &SeuilCstrMortality, double &NumPhase,
                      std::array<double,5> &tabCstr, int &tabCstrIndiceCourant, int &NbJourCompte) {
    int i;
    double MoyenneCstr;

    /*try*/ {
        if ((NumPhase >= 2)) {
            NbJourCompte = NbJourCompte + 1;
            // gestion de l'indice...
            if (tabCstrIndiceCourant == 4) {
                tabCstrIndiceCourant = 0;
                tabCstr[ tabCstrIndiceCourant ] = cstr;
            } else {
                tabCstrIndiceCourant = tabCstrIndiceCourant + 1;
                tabCstr[ tabCstrIndiceCourant ] = cstr;
            }
            // gestion de la mortalité
            if ((NbJourCompte >= 5)) {
                // on peut moyenner...
                MoyenneCstr = 0;
                {
                    long i_end = 5 ;
                    for (i = 1 ; i < i_end ; ++i) {
                        MoyenneCstr = MoyenneCstr + tabCstr[ i ];
                    }
                }
                if ((MoyenneCstr / 5) <= SeuilCstrMortality) {
                    NumPhase = 7;
                }
            }
        }

    } /*catch (...)*/ {
        error_message("SorghumMortality", URiz);
    }
}


void RS_KeyResults_V2_1(double const &NumPhase, double const &CulmsPerPlant, double const &CulmsPerHill, double const &cstr, double const &FTSW, double const &Ic, double const &Lai, double const &GrainYieldPop, double const &DryMatAboveGroundPop, double const &DryMatResInternodePop, double const &DryMatTotPop, double const &GrainFillingStatus, double const &SterilityTot, double const &CumIrrig, double const &CumWUse,
                        double &CulmsPerPlantMax, double &CulmsPerHillMax, double &DurPhase1, double &DurPhase2, double &DurPhase3, double &DurPhase4, double &DurPhase5, double &DurPhase6, double &CumCstrPhase2, double &CumCstrPhase3, double &CumCstrPhase4, double &CumCstrPhase5, double &CumCstrPhase6, double &CumFTSWPhase2, double &CumFTSWPhase3, double &CumFTSWPhase4, double &CumFTSWPhase5, double &CumFTSWPhase6, double &CumIcPhase2, double &CumIcPhase3, double &CumIcPhase4, double &CumIcPhase5, double &CumIcPhase6, double &IcPhase2, double &IcPhase3, double &IcPhase4, double &IcPhase5, double &IcPhase6, double &FtswPhase2, double &FtswPhase3, double &FtswPhase4, double &FtswPhase5, double &FtswPhase6, double &CstrPhase2, double &CstrPhase3, double &CstrPhase4, double &CstrPhase5, double &CstrPhase6, double &DurGermFlow, double &DurGermMat, double &LaiFin, double &CulmsPerHillFin, double &CulmsPerPlantFin, double &GrainYieldPopFin, double &DryMatAboveGroundPopFin, double &ReservePopFin, double &DryMatTotPopFin, double &GrainFillingStatusFin, double &SterilityTotFin, double &CumIrrigFin, double &CumWUseFin) {
    /*try*/ {
        if ((NumPhase > 1) && (NumPhase < 7)) {
            CulmsPerPlantMax = max(CulmsPerPlant, CulmsPerPlantMax);
            CulmsPerHillMax = max(CulmsPerHill, CulmsPerHillMax);
        }
        if ((NumPhase == 1)) {
            DurPhase1 = DurPhase1 + 1;
        }
        if ((NumPhase == 2)) {
            DurPhase2 = DurPhase2 + 1;
            CumCstrPhase2 = CumCstrPhase2 + cstr;
            CumFTSWPhase2 = CumFTSWPhase2 + FTSW;
            CumIcPhase2 = CumIcPhase2 + Ic;
        }
        if ((NumPhase == 3)) {
            DurPhase3 = DurPhase3 + 1;
            CumCstrPhase3 = CumCstrPhase3 + cstr;
            CumFTSWPhase3 = CumFTSWPhase3 + FTSW;
            CumIcPhase3 = CumIcPhase3 + Ic;
        }
        if ((NumPhase == 4)) {
            DurPhase4 = DurPhase4 + 1;
            CumCstrPhase4 = CumCstrPhase4 + cstr;
            CumFTSWPhase4 = CumFTSWPhase4 + FTSW;
            CumIcPhase4 = CumIcPhase4 + Ic;
        }
        if ((NumPhase == 5)) {
            DurPhase5 = DurPhase5 + 1;
            CumCstrPhase5 = CumCstrPhase5 + cstr;
            CumFTSWPhase5 = CumFTSWPhase5 + FTSW;
            CumIcPhase5 = CumIcPhase5 + Ic;
        }
        if ((NumPhase == 6)) {
            DurPhase6 = DurPhase6 + 1;
            CumCstrPhase6 = CumCstrPhase6 + cstr;
            CumFTSWPhase6 = CumFTSWPhase6 + FTSW;
            CumIcPhase6 = CumIcPhase6 + Ic;
        }
        if ((NumPhase == 7)) {
            IcPhase2 = CumIcPhase2 / max(DurPhase2, 0.1);
            IcPhase3 = CumIcPhase3 / max(DurPhase3, 0.1);
            IcPhase4 = CumIcPhase4 / max(DurPhase4, 0.1);
            IcPhase5 = CumIcPhase5 / max(DurPhase5, 0.1);
            IcPhase6 = CumIcPhase6 / max(DurPhase6, 0.1);
            FtswPhase2 = CumFTSWPhase2 / max(DurPhase2, 0.1);
            FtswPhase3 = CumFTSWPhase3 / max(DurPhase3, 0.1);
            FtswPhase4 = CumFTSWPhase4 / max(DurPhase4, 0.1);
            FtswPhase5 = CumFTSWPhase5 / max(DurPhase5, 0.1);
            FtswPhase6 = CumFTSWPhase6 / max(DurPhase6, 0.1);
            CstrPhase2 = CumCstrPhase2 / max(DurPhase2, 0.1);
            CstrPhase3 = CumCstrPhase3 / max(DurPhase3, 0.1);
            CstrPhase4 = CumCstrPhase4 / max(DurPhase4, 0.1);
            CstrPhase5 = CumCstrPhase5 / max(DurPhase5, 0.1);
            CstrPhase6 = CumCstrPhase6 / max(DurPhase6, 0.1);
            DurGermFlow = DurPhase2 + DurPhase3 + DurPhase4;
            DurGermMat = DurPhase2 + DurPhase3 + DurPhase4 + DurPhase5 + DurPhase6;
            LaiFin = Lai;
            CulmsPerHillFin = CulmsPerHill;
            CulmsPerPlantFin = CulmsPerPlant;
            GrainYieldPopFin = GrainYieldPop;
            DryMatAboveGroundPopFin = DryMatAboveGroundPop;
            ReservePopFin = DryMatResInternodePop;

            /*NEW LB*/
            DryMatTotPopFin = DryMatTotPop;
            GrainFillingStatusFin = GrainFillingStatus;
            SterilityTotFin = SterilityTot;
            CumIrrigFin = CumIrrig;
            CumWUseFin = CumWUse;

            /*/NEW LB*/

        }

    } /*catch (...)*/ {
        error_message("RS_KeyResults_V2_1", URisocas);
    }
}

}



#endif // PROCESSES_H
