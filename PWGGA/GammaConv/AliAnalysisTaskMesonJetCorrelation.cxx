/**************************************************************************
 * Copyright(c) 1998-2020, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: Joshua Koenig <joshua.konig@cern.ch>                                        *
 * Version 1.0                                                            *
 *                                                                        *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
// Class used to do analysis for light neutral mesons inside Jets
//----------------------------------------------------------------
//////////////////////////////////////////////////////////////////

#include "AliAnalysisTaskMesonJetCorrelation.h"

ClassImp(AliAnalysisTaskMesonJetCorrelation)

  //________________________________________________________________________
  AliAnalysisTaskMesonJetCorrelation::AliAnalysisTaskMesonJetCorrelation() : AliAnalysisTaskSE(),
                                                                             fV0Reader(nullptr),
                                                                             fV0ReaderName("V0ReaderV1"),
                                                                             fReaderGammas(nullptr),
                                                                             fCaloTriggerHelperName(""),
                                                                             fCorrTaskSetting(""),
                                                                             fInputEvent(nullptr),
                                                                             fMCEvent(nullptr),
                                                                             fAODMCTrackArray(nullptr),
                                                                             // cut folders
                                                                             fCutFolder(nullptr),
                                                                             fESDList(nullptr),
                                                                             fMCList(nullptr),
                                                                             fTrueList(nullptr),
                                                                             fJetList(nullptr),
                                                                             fTrueJetList(nullptr),
                                                                             fOutputContainer(nullptr),
                                                                             fEventCutArray(nullptr),
                                                                             fConvCutArray(nullptr),
                                                                             fClusterCutArray(nullptr),
                                                                             fMesonCutArray(nullptr),
                                                                             // photon and cluster candidates
                                                                             fGammaCandidates({}),
                                                                             fClusterCandidates({}),
                                                                             // Event mixing and background related
                                                                             fEventMix(nullptr),
                                                                             fRandom(),
                                                                             fGenPhaseSpace(),
                                                                             // Jet related
                                                                             fConvJetReader(nullptr),
                                                                             fAddNameConvJet(""),
                                                                             fHighestJetVector(),
                                                                             fMaxPtJet(0),
                                                                             fOutlierJetReader(nullptr),
                                                                             // global settings
                                                                             fMesonPDGCode(111),
                                                                             fOtherMesonsPDGCodes({111, 221, 331}),
                                                                             fiCut(0),
                                                                             fIsMC(0),
                                                                             fnCuts(0),
                                                                             fWeightJetJetMC(0),
                                                                             fDoLightOutput(0),
                                                                             fDoMesonQA(0),
                                                                             fDoPhotonQA(0),
                                                                             fDoClusterQA(0),
                                                                             fDoJetQA(0),
                                                                             fIsHeavyIon(0),
                                                                             fIsCalo(false),
                                                                             fIsConv(false),
                                                                             fIsConvCalo(false),
                                                                             fIsFromDesiredHeader(false),
                                                                             fDoMaterialBudgetWeightingOfGammasForTrueMesons(false),
                                                                             fEventPlaneAngle(0),
                                                                             fTrackMatcherRunningMode(0),
                                                                             fUseThNForResponse(true),
                                                                             fEnableSortForClusMC(true),
                                                                             // aod relabeling
                                                                             fMCEventPos(nullptr),
                                                                             fMCEventNeg(nullptr),
                                                                             fESDArrayPos(nullptr),
                                                                             fESDArrayNeg(nullptr),
                                                                             // binnings
                                                                             fVecBinsMesonInvMass({}),
                                                                             fVecBinsPhotonPt({}),
                                                                             fVecBinsClusterPt({}),
                                                                             fVecBinsMesonPt({}),
                                                                             fVecBinsJetPt({}),
                                                                             fVecBinsFragment({}),
                                                                             vecEquidistFromMinus05({}),
                                                                             // Jet vectors
                                                                             fVectorJetPt({}),
                                                                             fVectorJetPx({}),
                                                                             fVectorJetPy({}),
                                                                             fVectorJetPz({}),
                                                                             fVectorJetEta({}),
                                                                             fVectorJetPhi({}),
                                                                             fVectorJetArea({}),
                                                                             fTrueVectorJetPt({}),
                                                                             fTrueVectorJetPx({}),
                                                                             fTrueVectorJetPy({}),
                                                                             fTrueVectorJetPz({}),
                                                                             fTrueVectorJetEta({}),
                                                                             fTrueVectorJetPhi({}),
                                                                             fTrueVectorJetPartonID({}),
                                                                             fTrueVectorJetPartonPt({}),
                                                                             fVectorJetEtaPerp({}),
                                                                             fVectorJetPhiPerp({}),
                                                                             MapRecJetsTrueJets(),
                                                                             // response matrix
                                                                             fRespMatrixHandlerMesonPt({}),
                                                                             fRespMatrixHandlerFrag({}),
                                                                             fRespMatrixHandlerFragTrueJets({}),
                                                                             fRespMatrixHandlerMesonInvMass({}),
                                                                             fRespMatrixHandlerMesonInvMassVsZ({}),
                                                                             fRespMatrixHandlerMesonBackInvMassVsZ({}),
                                                                             fRespMatrixHandlerMesonBackInvMassVsPt({}),
                                                                             fRespMatrixHandlerMesonInvMassPerpCone({}),
                                                                             fRespMatrixHandlerMesonInvMassVsZPerpCone({}),
                                                                             // basic Histograms
                                                                             fHistoNEvents({}),
                                                                             fHistoNEventsWOWeight({}),
                                                                             fHistoNGoodESDTracks({}),
                                                                             fHistoNGoodESDTracksEvtWithJets({}),
                                                                             fHistoVertexZ({}),
                                                                             // JJ related histograms
                                                                             fProfileJetJetXSection({}),
                                                                             fHistoJetJetNTrials({}),
                                                                             fHistoPtHardJJWeight({}),
                                                                             // cluster histograms
                                                                             fHistoClusterPt({}),
                                                                             fHistoClusterE({}),
                                                                             fHistoClusterPtInJet({}),
                                                                             fHistoClusterEInJet({}),
                                                                             fHistoClusterPtPerpCone({}),
                                                                             // conversion histos
                                                                             fHistoConvGammaPt({}),
                                                                             fHistoConvGammaPtInJet({}),
                                                                             fHistoConvGammaPtPerpCone({}),
                                                                             // inv mass Histograms
                                                                             fHistoInvMassVsPt({}),
                                                                             fHistoInvMassVsPt_Incl({}),
                                                                             fHistoJetPtVsFrag({}),
                                                                             fHistoInvMassVsPtMassCut({}),
                                                                             fHistoInvMassVsPtMassCutSB({}),
                                                                             fHistoInvMassVsPtBack({}),
                                                                             fHistoInvMassVsPtPerpCone({}),
                                                                             fHistoJetPtVsFragPerpCone({}),
                                                                             // jet related Histograms
                                                                             fHistoEventwJets({}),
                                                                             fHistoNJets({}),
                                                                             fHistoPtJet({}),
                                                                             fHistoJetEta({}),
                                                                             fHistoJetPhi({}),
                                                                             fHistoJetArea({}),
                                                                             fHistoTruevsRecJetPt({}),
                                                                             fHistoTruevsRecJetPtForTrueJets({}),
                                                                             fHistoTrueJetPtVsPartonPt({}),
                                                                             fHistoMatchedPtJet({}),
                                                                             fHistoUnMatchedPtJet({}),
                                                                             fHistoTruePtJet({}),
                                                                             fHistoTrueMatchedPtJet({}),
                                                                             fHistoTrueUnMatchedPtJet({}),
                                                                             // true meon histograms
                                                                             fRespMatrixHandlerTrueMesonInvMassVsPt({}),
                                                                             fRespMatrixHandlerTrueMesonInvMassVsZ({}),
                                                                             fRespMatrixHandlerTrueOtherMesonInvMassVsPt({}),
                                                                             fRespMatrixHandlerTrueOtherMesonInvMassVsZ({}),
                                                                             fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt({}),
                                                                             fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ({}),
                                                                             fHistoTrueMesonInvMassVsTruePt({}),
                                                                             fHistoTruePrimaryMesonInvMassPt({}),
                                                                             fHistoTrueSecondaryMesonInvMassPt({}),
                                                                             fHistoTrueMesonJetPtVsTruePt({}),
                                                                             fHistoTrueMesonJetPtVsTrueZ({}),
                                                                             fHistoTrueMesonJetPtVsRecPt({}),
                                                                             fHistoTrueMesonJetPtVsRecZ({}),
                                                                             fHistoTrueSecMesonJetPtVsRecPt({}),
                                                                             fHistoTrueSecMesonJetPtVsRecZ({}),
                                                                             fHistoTrueMesonInTrueJet_JetPtVsTruePt({}),
                                                                             fHistoTrueMesonInTrueJet_JetPtVsTrueZ({}),
                                                                             fHistoMesonResponse({}),
                                                                             // true meson double counting
                                                                             fMesonDoubleCount({}),
                                                                             fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount({}),
                                                                             fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount({}),
                                                                             // true conv. photons
                                                                             fHistoTrueConvGammaPt({}),
                                                                             fHistoTruePrimaryConvGammaPt({}),
                                                                             fHistoTruePrimaryConvGammaESDPtMCPt({}),
                                                                             // true cluster histograms
                                                                             fHistoTrueClusGammaPt({}),
                                                                             // MC generated histograms
                                                                             fHistoMCGammaPtNotTriggered({}),
                                                                             fHistoMCGammaPtNoVertex({}),
                                                                             fHistoMCAllGammaPt({}),
                                                                             fHistoMCDecayGammaPi0Pt({}),
                                                                             fHistoMCDecayGammaRhoPt({}),
                                                                             fHistoMCDecayGammaEtaPt({}),
                                                                             fHistoMCDecayGammaOmegaPt({}),
                                                                             fHistoMCDecayGammaEtapPt({}),
                                                                             fHistoMCDecayGammaPhiPt({}),
                                                                             fHistoMCDecayGammaSigmaPt({}),
                                                                             fHistoMCPrimaryPtvsSource({}),
                                                                             fHistoMCMesonPtNotTriggered({}),
                                                                             fHistoMCMesonPtNoVertex({}),
                                                                             fHistoMCMesonPt({}),
                                                                             fHistoMCMesonWOEvtWeightPt({}),
                                                                             fHistoMCMesonInAccPt({}),
                                                                             fHistoMCMesonInAccPtNotTriggered({}),
                                                                             fHistoMCMesonWOWeightInAccPt({}),
                                                                             fHistoMCMesonWOEvtWeightInAccPt({}),
                                                                             fHistoMCSecMesonPtvsSource({}),
                                                                             fHistoMCSecMesonSource({}),
                                                                             fHistoMCSecMesonInAccPtvsSource({}),
                                                                             // MC gen jet-meson corr
                                                                             fHistoMCJetPtVsMesonPt({}),
                                                                             fHistoMCJetPtVsMesonPtInAcc({}),
                                                                             fHistoMCJetPtVsFrag({}),
                                                                             fHistoMCJetPtVsFragInAcc({}),
                                                                             fHistoMCJetPtVsFrag_Sec({}),
                                                                             fHistoMCRecJetPtVsFrag({}),
                                                                             fHistoMCRecJetPtVsMesonPt({}),
                                                                             fHistoMCJetPtVsMesonPt_Sec({}),
                                                                             fHistoMCPartonPtVsFrag({}),
                                                                             fHistoMCJetPtVsFragTrueParton({}),
                                                                             fHistoMCPartonPtVsFragTrueParton({})
{
}

//________________________________________________________________________
AliAnalysisTaskMesonJetCorrelation::AliAnalysisTaskMesonJetCorrelation(const char* name) : AliAnalysisTaskSE(name),
                                                                                           fV0Reader(nullptr),
                                                                                           fV0ReaderName("V0ReaderV1"),
                                                                                           fReaderGammas(nullptr),
                                                                                           fCaloTriggerHelperName(""),
                                                                                           fCorrTaskSetting(""),
                                                                                           fInputEvent(nullptr),
                                                                                           fMCEvent(nullptr),
                                                                                           fAODMCTrackArray(nullptr),
                                                                                           // cut folders
                                                                                           fCutFolder(nullptr),
                                                                                           fESDList(nullptr),
                                                                                           fMCList(nullptr),
                                                                                           fTrueList(nullptr),
                                                                                           fJetList(nullptr),
                                                                                           fTrueJetList(nullptr),
                                                                                           fOutputContainer(nullptr),
                                                                                           fEventCutArray(nullptr),
                                                                                           fConvCutArray(nullptr),
                                                                                           fClusterCutArray(nullptr),
                                                                                           fMesonCutArray(nullptr),
                                                                                           // photon and cluster candidates
                                                                                           fGammaCandidates({}),
                                                                                           fClusterCandidates({}),
                                                                                           // Event mixing and background related
                                                                                           fEventMix(nullptr),
                                                                                           fRandom(),
                                                                                           fGenPhaseSpace(),
                                                                                           // Jet related
                                                                                           fConvJetReader(nullptr),
                                                                                           fAddNameConvJet(""),
                                                                                           fHighestJetVector(),
                                                                                           fMaxPtJet(0),
                                                                                           fOutlierJetReader(nullptr),
                                                                                           // global settings
                                                                                           fMesonPDGCode(111),
                                                                                           fOtherMesonsPDGCodes({111, 221, 331}),
                                                                                           fiCut(0),
                                                                                           fIsMC(0),
                                                                                           fnCuts(0),
                                                                                           fWeightJetJetMC(0),
                                                                                           fDoLightOutput(0),
                                                                                           fDoMesonQA(0),
                                                                                           fDoPhotonQA(0),
                                                                                           fDoClusterQA(0),
                                                                                           fDoJetQA(0),
                                                                                           fIsHeavyIon(0),
                                                                                           fIsCalo(false),
                                                                                           fIsConv(false),
                                                                                           fIsConvCalo(false),
                                                                                           fIsFromDesiredHeader(false),
                                                                                           fDoMaterialBudgetWeightingOfGammasForTrueMesons(false),
                                                                                           fEventPlaneAngle(0),
                                                                                           fTrackMatcherRunningMode(0),
                                                                                           fUseThNForResponse(true),
                                                                                           fEnableSortForClusMC(true),
                                                                                           // aod relabeling
                                                                                           fMCEventPos(nullptr),
                                                                                           fMCEventNeg(nullptr),
                                                                                           fESDArrayPos(nullptr),
                                                                                           fESDArrayNeg(nullptr),
                                                                                           // binnings
                                                                                           fVecBinsMesonInvMass({}),
                                                                                           fVecBinsPhotonPt({}),
                                                                                           fVecBinsClusterPt({}),
                                                                                           fVecBinsMesonPt({}),
                                                                                           fVecBinsJetPt({}),
                                                                                           fVecBinsFragment({}),
                                                                                           vecEquidistFromMinus05({}),
                                                                                           // Jet vectors
                                                                                           fVectorJetPt({}),
                                                                                           fVectorJetPx({}),
                                                                                           fVectorJetPy({}),
                                                                                           fVectorJetPz({}),
                                                                                           fVectorJetEta({}),
                                                                                           fVectorJetPhi({}),
                                                                                           fVectorJetArea({}),
                                                                                           fTrueVectorJetPt({}),
                                                                                           fTrueVectorJetPx({}),
                                                                                           fTrueVectorJetPy({}),
                                                                                           fTrueVectorJetPz({}),
                                                                                           fTrueVectorJetEta({}),
                                                                                           fTrueVectorJetPhi({}),
                                                                                           fTrueVectorJetPartonID({}),
                                                                                           fTrueVectorJetPartonPt({}),
                                                                                           fVectorJetEtaPerp({}),
                                                                                           fVectorJetPhiPerp({}),
                                                                                           MapRecJetsTrueJets(),
                                                                                           // response matrix
                                                                                           fRespMatrixHandlerMesonPt({}),
                                                                                           fRespMatrixHandlerFrag({}),
                                                                                           fRespMatrixHandlerFragTrueJets({}),
                                                                                           fRespMatrixHandlerMesonInvMass({}),
                                                                                           fRespMatrixHandlerMesonInvMassVsZ({}),
                                                                                           fRespMatrixHandlerMesonBackInvMassVsZ({}),
                                                                                           fRespMatrixHandlerMesonBackInvMassVsPt({}),
                                                                                           fRespMatrixHandlerMesonInvMassPerpCone({}),
                                                                                           fRespMatrixHandlerMesonInvMassVsZPerpCone({}),
                                                                                           // basic Histograms
                                                                                           fHistoNEvents({}),
                                                                                           fHistoNEventsWOWeight({}),
                                                                                           fHistoNGoodESDTracks({}),
                                                                                           fHistoNGoodESDTracksEvtWithJets({}),
                                                                                           fHistoVertexZ({}),
                                                                                           // JJ related histograms
                                                                                           fProfileJetJetXSection({}),
                                                                                           fHistoJetJetNTrials({}),
                                                                                           fHistoPtHardJJWeight({}),
                                                                                           // cluster histograms
                                                                                           fHistoClusterPt({}),
                                                                                           fHistoClusterE({}),
                                                                                           fHistoClusterPtInJet({}),
                                                                                           fHistoClusterEInJet({}),
                                                                                           fHistoClusterPtPerpCone({}),
                                                                                           // conversion histos
                                                                                           fHistoConvGammaPt({}),
                                                                                           fHistoConvGammaPtInJet({}),
                                                                                           fHistoConvGammaPtPerpCone({}),
                                                                                           // inv mass Histograms
                                                                                           fHistoInvMassVsPt({}),
                                                                                           fHistoInvMassVsPt_Incl({}),
                                                                                           fHistoJetPtVsFrag({}),
                                                                                           fHistoInvMassVsPtMassCut({}),
                                                                                           fHistoInvMassVsPtMassCutSB({}),
                                                                                           fHistoInvMassVsPtBack({}),
                                                                                           fHistoInvMassVsPtPerpCone({}),
                                                                                           fHistoJetPtVsFragPerpCone({}),
                                                                                           // jet related Histograms
                                                                                           fHistoEventwJets({}),
                                                                                           fHistoNJets({}),
                                                                                           fHistoPtJet({}),
                                                                                           fHistoJetEta({}),
                                                                                           fHistoJetPhi({}),
                                                                                           fHistoJetArea({}),
                                                                                           fHistoTruevsRecJetPt({}),
                                                                                           fHistoTruevsRecJetPtForTrueJets({}),
                                                                                           fHistoTrueJetPtVsPartonPt({}),
                                                                                           fHistoMatchedPtJet({}),
                                                                                           fHistoUnMatchedPtJet({}),
                                                                                           fHistoTruePtJet({}),
                                                                                           fHistoTrueMatchedPtJet({}),
                                                                                           fHistoTrueUnMatchedPtJet({}),
                                                                                           // true meon histograms
                                                                                           fRespMatrixHandlerTrueMesonInvMassVsPt({}),
                                                                                           fRespMatrixHandlerTrueMesonInvMassVsZ({}),
                                                                                           fRespMatrixHandlerTrueOtherMesonInvMassVsPt({}),
                                                                                           fRespMatrixHandlerTrueOtherMesonInvMassVsZ({}),
                                                                                           fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt({}),
                                                                                           fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ({}),
                                                                                           fHistoTrueMesonInvMassVsTruePt({}),
                                                                                           fHistoTruePrimaryMesonInvMassPt({}),
                                                                                           fHistoTrueSecondaryMesonInvMassPt({}),
                                                                                           fHistoTrueMesonJetPtVsTruePt({}),
                                                                                           fHistoTrueMesonJetPtVsTrueZ({}),
                                                                                           fHistoTrueMesonJetPtVsRecPt({}),
                                                                                           fHistoTrueMesonJetPtVsRecZ({}),
                                                                                           fHistoTrueSecMesonJetPtVsRecPt({}),
                                                                                           fHistoTrueSecMesonJetPtVsRecZ({}),
                                                                                           fHistoTrueMesonInTrueJet_JetPtVsTruePt({}),
                                                                                           fHistoTrueMesonInTrueJet_JetPtVsTrueZ({}),
                                                                                           fHistoMesonResponse({}),
                                                                                           // true meson double counting
                                                                                           fMesonDoubleCount({}),
                                                                                           fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount({}),
                                                                                           fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount({}),
                                                                                           // true conv. photons
                                                                                           fHistoTrueConvGammaPt({}),
                                                                                           fHistoTruePrimaryConvGammaPt({}),
                                                                                           fHistoTruePrimaryConvGammaESDPtMCPt({}),
                                                                                           // true cluster histograms
                                                                                           fHistoTrueClusGammaPt({}),
                                                                                           // MC generated histograms
                                                                                           fHistoMCGammaPtNotTriggered({}),
                                                                                           fHistoMCGammaPtNoVertex({}),
                                                                                           fHistoMCAllGammaPt({}),
                                                                                           fHistoMCDecayGammaPi0Pt({}),
                                                                                           fHistoMCDecayGammaRhoPt({}),
                                                                                           fHistoMCDecayGammaEtaPt({}),
                                                                                           fHistoMCDecayGammaOmegaPt({}),
                                                                                           fHistoMCDecayGammaEtapPt({}),
                                                                                           fHistoMCDecayGammaPhiPt({}),
                                                                                           fHistoMCDecayGammaSigmaPt({}),
                                                                                           fHistoMCPrimaryPtvsSource({}),
                                                                                           fHistoMCMesonPtNotTriggered({}),
                                                                                           fHistoMCMesonPtNoVertex({}),
                                                                                           fHistoMCMesonPt({}),
                                                                                           fHistoMCMesonWOEvtWeightPt({}),
                                                                                           fHistoMCMesonInAccPt({}),
                                                                                           fHistoMCMesonInAccPtNotTriggered({}),
                                                                                           fHistoMCMesonWOWeightInAccPt({}),
                                                                                           fHistoMCMesonWOEvtWeightInAccPt({}),
                                                                                           fHistoMCSecMesonPtvsSource({}),
                                                                                           fHistoMCSecMesonSource({}),
                                                                                           fHistoMCSecMesonInAccPtvsSource({}),
                                                                                           // MC gen jet-meson corr
                                                                                           fHistoMCJetPtVsMesonPt({}),
                                                                                           fHistoMCJetPtVsMesonPtInAcc({}),
                                                                                           fHistoMCJetPtVsFrag({}),
                                                                                           fHistoMCJetPtVsFragInAcc({}),
                                                                                           fHistoMCJetPtVsFrag_Sec({}),
                                                                                           fHistoMCRecJetPtVsFrag({}),
                                                                                           fHistoMCRecJetPtVsMesonPt({}),
                                                                                           fHistoMCJetPtVsMesonPt_Sec({}),
                                                                                           fHistoMCPartonPtVsFrag({}),
                                                                                           fHistoMCJetPtVsFragTrueParton({}),
                                                                                           fHistoMCPartonPtVsFragTrueParton({})
{
  // Define output slots here
  DefineOutput(1, TList::Class());
  DefineOutput(2, TTree::Class());
}

//________________________________________________________________________
AliAnalysisTaskMesonJetCorrelation::~AliAnalysisTaskMesonJetCorrelation()
{
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::Terminate(const Option_t*)
{
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskMesonJetCorrelation::Notify()
{
  for (Int_t iCut = 0; iCut < fnCuts; iCut++) {
    if (((AliConvEventCuts*)fEventCutArray->At(iCut))->GetPeriodEnum() == AliConvEventCuts::kNoPeriod && ((AliConvEventCuts*)fV0Reader->GetEventCuts())->GetPeriodEnum() != AliConvEventCuts::kNoPeriod) {
      ((AliConvEventCuts*)fEventCutArray->At(iCut))->SetPeriodEnumExplicit(((AliConvEventCuts*)fV0Reader->GetEventCuts())->GetPeriodEnum());
    } else if (((AliConvEventCuts*)fEventCutArray->At(iCut))->GetPeriodEnum() == AliConvEventCuts::kNoPeriod) {
      ((AliConvEventCuts*)fEventCutArray->At(iCut))->SetPeriodEnum(fV0Reader->GetPeriodName());
    }
  }
  return true;
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::UserCreateOutputObjects()
{
  // cout << "in UserCreateOutputObjects" << endl;

  MakeBinning();

  // Create top level output list
  if (fOutputContainer != NULL) {
    delete fOutputContainer;
    fOutputContainer = NULL;
  }
  if (fOutputContainer == NULL) {
    fOutputContainer = new TList();
    fOutputContainer->SetOwner(kTRUE);
  }

  // Initialize V0 reader
  fV0Reader = (AliV0ReaderV1*)AliAnalysisManager::GetAnalysisManager()->GetTask(fV0ReaderName.Data());
  if (!fV0Reader) {
    printf("Error: No V0 Reader");
    return;
  } // GetV0Reader

  fConvJetReader = (AliAnalysisTaskConvJet*)AliAnalysisManager::GetAnalysisManager()->GetTask(Form("AliAnalysisTaskConvJet%s", fAddNameConvJet.EqualTo("") == true ? "" : Form("_%s",fAddNameConvJet.Data())));
  if (!fConvJetReader) {
    printf(Form("ERROR: No AliAnalysisTaskConvJet%s\n", fAddNameConvJet.EqualTo("") == true ? "" : Form("_%s",fAddNameConvJet.Data())));
    return;
  } // GetV0Reader

  fEventMix = new EventMixPoolMesonJets();

  // initialize lists
  fCutFolder = new TList*[fnCuts];
  fESDList = new TList*[fnCuts];
  fTrueList = new TList*[fnCuts];
  fMCList = new TList*[fnCuts];
  fJetList = new TList*[fnCuts];
  fTrueJetList = new TList*[fnCuts];

  // initialize lists of histograms
  fHistoNEvents.resize(fnCuts);
  if (fIsMC > 1) {
    fHistoNEventsWOWeight.resize(fnCuts);
  }
  fHistoNGoodESDTracks.resize(fnCuts);
  fHistoNGoodESDTracksEvtWithJets.resize(fnCuts);
  fHistoVertexZ.resize(fnCuts);

  if (fIsMC == 2) {
    fProfileJetJetXSection.resize(fnCuts);
    fHistoJetJetNTrials.resize(fnCuts);
    fHistoPtHardJJWeight.resize(fnCuts);
  }

  // cluster histograms
  if (!fIsConv) {
    fHistoClusterPt.resize(fnCuts);
    fHistoClusterE.resize(fnCuts);
    fHistoClusterPtInJet.resize(fnCuts);
    fHistoClusterEInJet.resize(fnCuts);
    fHistoClusterPtPerpCone.resize(fnCuts);
  }
  if (!fIsCalo) {
    // conversion histos
    fHistoConvGammaPt.resize(fnCuts);
    fHistoConvGammaPtInJet.resize(fnCuts);
    fHistoConvGammaPtPerpCone.resize(fnCuts);
  }

  //----------------------
  // MC gen histograms
  //----------------------
  if (fIsMC) {
    fHistoMCGammaPtNotTriggered.resize(fnCuts);
    fHistoMCGammaPtNoVertex.resize(fnCuts);
    fHistoMCAllGammaPt.resize(fnCuts);
    fHistoMCDecayGammaPi0Pt.resize(fnCuts);
    fHistoMCDecayGammaRhoPt.resize(fnCuts);
    fHistoMCDecayGammaEtaPt.resize(fnCuts);
    fHistoMCDecayGammaOmegaPt.resize(fnCuts);
    fHistoMCDecayGammaEtapPt.resize(fnCuts);
    fHistoMCDecayGammaPhiPt.resize(fnCuts);
    fHistoMCDecayGammaSigmaPt.resize(fnCuts);
    fHistoMCPrimaryPtvsSource.resize(fnCuts);
    fHistoMCMesonPtNotTriggered.resize(fnCuts);
    fHistoMCMesonPtNoVertex.resize(fnCuts);
    fHistoMCMesonPt.resize(fnCuts);
    fHistoMCMesonWOEvtWeightPt.resize(fnCuts);
    fHistoMCMesonInAccPt.resize(fnCuts);
    fHistoMCMesonInAccPtNotTriggered.resize(fnCuts);
    fHistoMCMesonWOEvtWeightInAccPt.resize(fnCuts);
    fHistoMCSecMesonPtvsSource.resize(fnCuts);
    fHistoMCSecMesonSource.resize(fnCuts);
    fHistoMCSecMesonInAccPtvsSource.resize(fnCuts);

    fHistoMCJetPtVsMesonPt.resize(fnCuts);
    fHistoMCJetPtVsMesonPtInAcc.resize(fnCuts);
    fHistoMCJetPtVsFrag.resize(fnCuts);
    fHistoMCJetPtVsFragInAcc.resize(fnCuts);
    fHistoMCJetPtVsFrag_Sec.resize(fnCuts);
    fHistoMCRecJetPtVsFrag.resize(fnCuts);
    fHistoMCRecJetPtVsMesonPt.resize(fnCuts);
    fHistoMCJetPtVsMesonPt_Sec.resize(fnCuts);
    fHistoMCPartonPtVsFrag.resize(fnCuts);
    fHistoMCJetPtVsFragTrueParton.resize(fnCuts);
    fHistoMCPartonPtVsFragTrueParton.resize(fnCuts);
  }

  //----------------------
  // true conversion photon histograms
  //----------------------
  if (fIsMC) {
    fHistoTrueConvGammaPt.resize(fnCuts);
    fHistoTruePrimaryConvGammaPt.resize(fnCuts);
    fHistoTruePrimaryConvGammaESDPtMCPt.resize(fnCuts);
  }

  //----------------------
  // true cluster histograms
  //----------------------
  if (fIsMC && !fIsConv) {
    fHistoTrueClusGammaPt.resize(fnCuts);
  }

  //----------------------
  // jet histogram
  //----------------------
  fHistoEventwJets.resize(fnCuts);
  fHistoNJets.resize(fnCuts);
  fHistoPtJet.resize(fnCuts);
  fHistoJetEta.resize(fnCuts);
  fHistoJetPhi.resize(fnCuts);
  fHistoJetArea.resize(fnCuts);
  if (fIsMC) {
    fHistoTruevsRecJetPt.resize(fnCuts);
    if(!fDoLightOutput){
      fHistoTruevsRecJetPtForTrueJets.resize(fnCuts);
    }
    fHistoTrueJetPtVsPartonPt.resize(fnCuts);
    fHistoMatchedPtJet.resize(fnCuts);
    fHistoUnMatchedPtJet.resize(fnCuts);
    fHistoTruePtJet.resize(fnCuts);
    fHistoTrueMatchedPtJet.resize(fnCuts);
    fHistoTrueUnMatchedPtJet.resize(fnCuts);
  }

  //----------------------
  // inv mass histograms
  //----------------------
  fHistoInvMassVsPt.resize(fnCuts);
  fHistoInvMassVsPt_Incl.resize(fnCuts);

  fHistoInvMassVsPtMassCut.resize(fnCuts);
  fHistoInvMassVsPtMassCutSB.resize(fnCuts);

  if (fIsMC) {
    fRespMatrixHandlerTrueMesonInvMassVsPt.resize(fnCuts);
    fRespMatrixHandlerTrueMesonInvMassVsZ.resize(fnCuts);
    fRespMatrixHandlerTrueOtherMesonInvMassVsPt.resize(fnCuts);
    fRespMatrixHandlerTrueOtherMesonInvMassVsZ.resize(fnCuts);
    fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt.resize(fnCuts);
    fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ.resize(fnCuts);
    if(fDoMesonQA > 0){
      fHistoTrueMesonInvMassVsTruePt.resize(fnCuts);
      fHistoTruePrimaryMesonInvMassPt.resize(fnCuts);
      fHistoTrueSecondaryMesonInvMassPt.resize(fnCuts);
    }

    fHistoTrueMesonJetPtVsTruePt.resize(fnCuts);
    fHistoTrueMesonJetPtVsTrueZ.resize(fnCuts);

    fHistoTrueMesonJetPtVsRecPt.resize(fnCuts);
    fHistoTrueMesonJetPtVsRecZ.resize(fnCuts);

    if(fDoMesonQA>0){
      fHistoTrueSecMesonJetPtVsRecPt.resize(fnCuts);
      fHistoTrueSecMesonJetPtVsRecZ.resize(fnCuts);

      fHistoTrueMesonInTrueJet_JetPtVsTruePt.resize(fnCuts);
      fHistoTrueMesonInTrueJet_JetPtVsTrueZ.resize(fnCuts);
    }

    fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount.resize(fnCuts);
    fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount.resize(fnCuts);
  }

  if (fDoMesonQA) {
    fHistoMesonResponse.resize(fnCuts);
  }

  // perpendicular cone
  fHistoInvMassVsPtPerpCone.resize(fnCuts);

  // inv mass background
  fHistoInvMassVsPtBack.resize(fnCuts);

  //----------------------
  // fragmentation histos
  //----------------------
  fHistoJetPtVsFrag.resize(fnCuts);

  // perpendicular cone
  fHistoJetPtVsFragPerpCone.resize(fnCuts);

  //----------------------
  // response matrix
  //----------------------
  fRespMatrixHandlerMesonInvMassVsZ.resize(fnCuts);
  fRespMatrixHandlerMesonInvMass.resize(fnCuts);
  fRespMatrixHandlerMesonBackInvMassVsZ.resize(fnCuts);
  fRespMatrixHandlerMesonBackInvMassVsPt.resize(fnCuts);
  // perpendicular cone
  fRespMatrixHandlerMesonInvMassVsZPerpCone.resize(fnCuts);
  fRespMatrixHandlerMesonInvMassPerpCone.resize(fnCuts);
  if (fIsMC) {
    fRespMatrixHandlerMesonPt.resize(fnCuts);
    fRespMatrixHandlerFrag.resize(fnCuts);
    if(!fDoLightOutput) {
      fRespMatrixHandlerFragTrueJets.resize(fnCuts);
    }
  }

  for (int iCut = 0; iCut < fnCuts; ++iCut) {
    TString cutstringEvent = ((AliConvEventCuts*)fEventCutArray->At(iCut))->GetCutNumber();
    TString cutstringCalo = (fIsCalo == true || fIsConvCalo == true) ? ((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))->GetCutNumber() : "";
    TString cutstringMeson = ((AliConversionMesonCuts*)fMesonCutArray->At(iCut))->GetCutNumber();
    TString cutstringConv = (fIsConv == true || fIsConvCalo == true) ? ((AliConversionPhotonCuts*)fConvCutArray->At(iCut))->GetCutNumber() : "";

    TString cutString = "";
    if (fIsConv)
      cutString = Form("%s_%s_%s", cutstringEvent.Data(), cutstringConv.Data(), cutstringMeson.Data());
    else if (fIsCalo)
      cutString = Form("%s_%s_%s", cutstringEvent.Data(), cutstringCalo.Data(), cutstringMeson.Data());
    else if (fIsConvCalo)
      cutString = Form("%s_%s_%s_%s", cutstringEvent.Data(), cutstringConv.Data(), cutstringCalo.Data(), cutstringMeson.Data());

    fCutFolder[iCut] = new TList();
    fCutFolder[iCut]->SetName(Form("Cut Number %s", cutString.Data()));
    fCutFolder[iCut]->SetOwner(kTRUE);
    fOutputContainer->Add(fCutFolder[iCut]);

    fESDList[iCut] = new TList();
    fESDList[iCut]->SetName(Form("%s ESD histograms", cutString.Data()));
    fESDList[iCut]->SetOwner(kTRUE);
    fCutFolder[iCut]->Add(fESDList[iCut]);

    fJetList[iCut] = new TList();
    fJetList[iCut]->SetName(Form("%s Jet histograms", cutString.Data()));
    fJetList[iCut]->SetOwner(kTRUE);
    fCutFolder[iCut]->Add(fJetList[iCut]);

    if (fIsMC) {
      fTrueJetList[iCut] = new TList();
      fTrueJetList[iCut]->SetName(Form("%s True Jet histograms", cutString.Data()));
      fTrueJetList[iCut]->SetOwner(kTRUE);
      fCutFolder[iCut]->Add(fTrueJetList[iCut]);

      fTrueList[iCut] = new TList();
      fTrueList[iCut]->SetName(Form("%s True histograms", cutString.Data()));
      fTrueList[iCut]->SetOwner(kTRUE);
      fCutFolder[iCut]->Add(fTrueList[iCut]);

      fMCList[iCut] = new TList();
      fMCList[iCut]->SetName(Form("%s MC histograms", cutString.Data()));
      fMCList[iCut]->SetOwner(kTRUE);
      fCutFolder[iCut]->Add(fMCList[iCut]);
    }

    fHistoNEvents[iCut] = new TH1F("NEvents", "NEvents", 15, -0.5, 14.5);
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(1, "Accepted");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(2, "Centrality");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(3, "Miss. MC or inc. ev.");
    if (((AliConvEventCuts*)fEventCutArray->At(iCut))->IsSpecialTrigger() > 1) {
      TString TriggerNames = "Not Trigger: ";
      TriggerNames = TriggerNames + ((AliConvEventCuts*)fEventCutArray->At(iCut))->GetSpecialTriggerName();
      fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(4, TriggerNames.Data());
    } else {
      fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(4, "Trigger");
    }
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(5, "Vertex Z");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(6, "Cont. Vertex");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(7, "SPD Pile-Up");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(8, "no SDD");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(9, "no V0AND");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(10, "EMCAL/TPC problems");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(11, "rejectedForJetJetMC");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(12, "SPD hits vs tracklet");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(13, "Out-of-Bunch pileup Past-Future");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(14, "Pileup V0M-TPCout Tracks");
    fHistoNEvents[iCut]->GetXaxis()->SetBinLabel(15, "Sphericity");
    fESDList[iCut]->Add(fHistoNEvents[iCut]);
    if (fIsMC > 1) {
      fHistoNEventsWOWeight[iCut] = new TH1F("NEventsWOWeight", "NEventsWOWeight", 15, -0.5, 14.5);
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(1, "Accepted");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(2, "Centrality");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(3, "Miss. MC or inc. ev.");
      if (((AliConvEventCuts*)fEventCutArray->At(iCut))->IsSpecialTrigger() > 1) {
        TString TriggerNames = "Not Trigger: ";
        TriggerNames = TriggerNames + ((AliConvEventCuts*)fEventCutArray->At(iCut))->GetSpecialTriggerName();
        fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(4, TriggerNames.Data());
      } else {
        fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(4, "Trigger");
      }
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(5, "Vertex Z");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(6, "Cont. Vertex");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(7, "Pile-Up");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(8, "no SDD");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(9, "no V0AND");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(10, "EMCAL problem");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(11, "rejectedForJetJetMC");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(12, "SPD hits vs tracklet");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(13, "Out-of-Bunch pileup Past-Future");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(14, "Pileup V0M-TPCout Tracks");
      fHistoNEventsWOWeight[iCut]->GetXaxis()->SetBinLabel(15, "Sphericity");
      fESDList[iCut]->Add(fHistoNEventsWOWeight[iCut]);
    }

    if (fIsHeavyIon == 1) {
      fHistoNGoodESDTracks[iCut] = new TH1F("GoodESDTracks", "GoodESDTracks", 4000, -0.5, 3999.5);
      fHistoNGoodESDTracksEvtWithJets[iCut] = new TH1F("GoodESDTracksEvtsWithJets", "GoodESDTracksEvtsWithJets", 4000, -0.5, 3999.5);
    } else if (fIsHeavyIon == 2) {
      fHistoNGoodESDTracks[iCut] = new TH1F("GoodESDTracks", "GoodESDTracks", 400, -0.5, 399.5);
      fHistoNGoodESDTracksEvtWithJets[iCut] = new TH1F("GoodESDTracksEvtsWithJets", "GoodESDTracksEvtsWithJets", 400, -0.5, 399.5);
    } else {
      fHistoNGoodESDTracks[iCut] = new TH1F("GoodESDTracks", "GoodESDTracks", 200, -0.5, 199.5);
      fHistoNGoodESDTracksEvtWithJets[iCut] = new TH1F("GoodESDTracksEvtsWithJets", "GoodESDTracksEvtsWithJets", 200, -0.5, 199.5);
    }
    fHistoNGoodESDTracks[iCut]->GetXaxis()->SetTitle("#primary tracks");
    fESDList[iCut]->Add(fHistoNGoodESDTracks[iCut]);
    fHistoNGoodESDTracksEvtWithJets[iCut]->GetXaxis()->SetTitle("#primary tracks");
    fESDList[iCut]->Add(fHistoNGoodESDTracksEvtWithJets[iCut]);

    fHistoVertexZ[iCut] = new TH1F("VertexZ", "VertexZ", 220, -11, 11);
    fHistoVertexZ[iCut]->GetXaxis()->SetTitle("Z_{vtx} (cm)");
    fESDList[iCut]->Add(fHistoVertexZ[iCut]);

    // JJ-MC related
    if (fIsMC == 2) {
      fProfileJetJetXSection[iCut] = new TProfile("XSection", "XSection", 1, -0.5, 0.5);
      fESDList[iCut]->Add(fProfileJetJetXSection[iCut]);
      fHistoJetJetNTrials[iCut] = new TH1F("NTrials", "#sum{NTrials}", 1, 0, 1);
      fHistoJetJetNTrials[iCut]->GetXaxis()->SetBinLabel(1, "#sum{NTrials}");
      fESDList[iCut]->Add(fHistoJetJetNTrials[iCut]);
      fHistoPtHardJJWeight[iCut] = new TH2F("fHistoPtHardJJWeight", "fHistoPtHardJJWeight", 100, 0, 200, 30, 0, 30);
      fESDList[iCut]->Add(fHistoPtHardJJWeight[iCut]);
    }

    // cluster related histograms
    if (!fIsConv) {
      fHistoClusterPt[iCut] = new TH1F("ClusGamma_Pt", "ClusGamma_Pt", fVecBinsClusterPt.size() - 1, fVecBinsClusterPt.data());
      fHistoClusterPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoClusterPt[iCut]);

      fHistoClusterE[iCut] = new TH1F("ClusGamma_E", "ClusGamma_E", fVecBinsClusterPt.size() - 1, fVecBinsClusterPt.data());
      fHistoClusterE[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoClusterE[iCut]);

      fHistoClusterPtInJet[iCut] = new TH1F("ClusGamma_InJet_Pt", "ClusGamma_InJet_Pt", fVecBinsClusterPt.size() - 1, fVecBinsClusterPt.data());
      fHistoClusterPtInJet[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoClusterPtInJet[iCut]);

      fHistoClusterEInJet[iCut] = new TH1F("ClusGamma_InJet_E", "ClusGamma_InJet_E", fVecBinsClusterPt.size() - 1, fVecBinsClusterPt.data());
      fHistoClusterEInJet[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoClusterEInJet[iCut]);

      fHistoClusterPtPerpCone[iCut] = new TH1F("ClusGamma_Pt_PerpCone", "ClusGamma_Pt_PerpCone", fVecBinsClusterPt.size() - 1, fVecBinsClusterPt.data());
      fHistoClusterPtPerpCone[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoClusterPtPerpCone[iCut]);
    }

    // conversion related histograms
    if (!fIsCalo) {
      fHistoConvGammaPt[iCut] = new TH1F("ConvGamma_Pt", "ConvGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoConvGammaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoConvGammaPt[iCut]);

      fHistoConvGammaPtInJet[iCut] = new TH1F("ConvGamma_InJet_Pt", "ConvGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoConvGammaPtInJet[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoConvGammaPtInJet[iCut]);

      fHistoConvGammaPtPerpCone[iCut] = new TH1F("ConvGamma_PerpCone_Pt", "ConvGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoConvGammaPtPerpCone[iCut]->SetXTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoConvGammaPtPerpCone[iCut]);
    }

    // MC histograms
    if (fIsMC) {
      fHistoMCGammaPtNotTriggered[iCut] = new TH1F("MC_AllGammaNotTriggered_Pt", "MC_AllGammaNotTriggered_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCGammaPtNotTriggered[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCGammaPtNotTriggered[iCut]);
      fHistoMCGammaPtNoVertex[iCut] = new TH1F("MC_AllGammaNoVertex_Pt", "MC_AllGammaNoVertex_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCGammaPtNoVertex[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCGammaPtNoVertex[iCut]);

      fHistoMCAllGammaPt[iCut] = new TH1F("MC_AllGamma_Pt", "MC_AllGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCAllGammaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCAllGammaPt[iCut]);

      fHistoMCDecayGammaPi0Pt[iCut] = new TH1F("MC_DecayGammaPi0_Pt", "MC_DecayGammaPi0_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaPi0Pt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaPi0Pt[iCut]);
      fHistoMCDecayGammaRhoPt[iCut] = new TH1F("MC_DecayGammaRho_Pt", "MC_DecayGammaRho_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaRhoPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaRhoPt[iCut]);
      fHistoMCDecayGammaEtaPt[iCut] = new TH1F("MC_DecayGammaEta_Pt", "MC_DecayGammaEta_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaEtaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaEtaPt[iCut]);
      fHistoMCDecayGammaOmegaPt[iCut] = new TH1F("MC_DecayGammaOmega_Pt", "MC_DecayGammaOmmega_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaOmegaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaOmegaPt[iCut]);
      fHistoMCDecayGammaEtapPt[iCut] = new TH1F("MC_DecayGammaEtap_Pt", "MC_DecayGammaEtap_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaEtapPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaEtapPt[iCut]);
      fHistoMCDecayGammaPhiPt[iCut] = new TH1F("MC_DecayGammaPhi_Pt", "MC_DecayGammaPhi_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaPhiPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaPhiPt[iCut]);
      fHistoMCDecayGammaSigmaPt[iCut] = new TH1F("MC_DecayGammaSigma_Pt", "MC_DecayGammaSigma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCDecayGammaSigmaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCDecayGammaSigmaPt[iCut]);

      fHistoMCPrimaryPtvsSource[iCut] = new TH2F("MC_Primary_Pt_Source", "MC_Primary_Pt_Source", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data(), 7, vecEquidistFromMinus05.data());
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(1, "Pi+");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(2, "Pi-");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(3, "K+");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(4, "K-");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(5, "K0s");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(6, "K0l");
      fHistoMCPrimaryPtvsSource[iCut]->GetYaxis()->SetBinLabel(7, "Lambda");
      fHistoMCPrimaryPtvsSource[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCPrimaryPtvsSource[iCut]->SetYTitle("particle");
      fMCList[iCut]->Add(fHistoMCPrimaryPtvsSource[iCut]);

      fHistoMCMesonPt[iCut] = new TH1F("MC_Pi0_Pt", "MC_Pi0_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCMesonPt[iCut]->Sumw2();
      fMCList[iCut]->Add(fHistoMCMesonPt[iCut]);
      fHistoMCMesonPtNotTriggered[iCut] = new TH1F("MC_Pi0_Pt_NotTriggered", "MC_Pi0_Pt_NotTriggered", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonPtNotTriggered[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCMesonPtNotTriggered[iCut]->Sumw2();
      fMCList[iCut]->Add(fHistoMCMesonPtNotTriggered[iCut]);
      fHistoMCMesonPtNoVertex[iCut] = new TH1F("MC_Pi0_Pt_NoVertex", "MC_Pi0_Pt_NoVertex", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonPtNoVertex[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCMesonPtNoVertex[iCut]->Sumw2();
      fMCList[iCut]->Add(fHistoMCMesonPtNoVertex[iCut]);

      fHistoMCMesonWOEvtWeightPt[iCut] = new TH1F("MC_Pi0_WOEventWeights_Pt", "MC_Pi0_WOEventWeights_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonWOEvtWeightPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCMesonWOEvtWeightPt[iCut]);

      fHistoMCMesonInAccPt[iCut] = new TH1F("MC_Pi0InAcc_Pt", "MC_Pi0InAcc_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonInAccPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCMesonInAccPt[iCut]->Sumw2();
      fMCList[iCut]->Add(fHistoMCMesonInAccPt[iCut]);
      fHistoMCMesonInAccPtNotTriggered[iCut] = new TH1F("MC_Pi0InAcc_Pt_NotTriggered", "MC_Pi0InAcc_Pt_NotTriggered", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoMCMesonInAccPtNotTriggered[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCMesonInAccPtNotTriggered[iCut]->Sumw2();
      fMCList[iCut]->Add(fHistoMCMesonInAccPtNotTriggered[iCut]);

      if (fIsMC > 1) {
        fHistoMCMesonWOEvtWeightInAccPt[iCut] = new TH1F("MC_Pi0WOEvtWeightInAcc_Pt", "MC_Pi0WOEvtWeightInAcc_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
        fHistoMCMesonWOEvtWeightInAccPt[iCut]->SetXTitle("p_{T} (GeV/c)");
        fMCList[iCut]->Add(fHistoMCMesonWOEvtWeightInAccPt[iCut]);
      }

      fHistoMCSecMesonPtvsSource[iCut] = new TH2F("MC_SecPi0_Pt_Source", "MC_SecPi0_Pt_Source", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data(), 16, vecEquidistFromMinus05.data());
      fHistoMCSecMesonPtvsSource[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCSecMesonPtvsSource[iCut]->SetYTitle("source");
      fMCList[iCut]->Add(fHistoMCSecMesonPtvsSource[iCut]);

      fHistoMCSecMesonSource[iCut] = new TH1F("MC_SecPi0_Source", "MC_SecPi0_Source", 5000, 0., 5000);
      fHistoMCSecMesonSource[iCut]->SetYTitle("source PDG");
      fMCList[iCut]->Add(fHistoMCSecMesonSource[iCut]);

      fHistoMCSecMesonInAccPtvsSource[iCut] = new TH2F("MC_SecPi0InAcc_Pt_Source", "MC_SecPi0InAcc_Pt_Source", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data(), 16, vecEquidistFromMinus05.data());
      fHistoMCSecMesonInAccPtvsSource[iCut]->SetXTitle("p_{T} (GeV/c)");
      fHistoMCSecMesonInAccPtvsSource[iCut]->SetYTitle("source");
      fMCList[iCut]->Add(fHistoMCSecMesonInAccPtvsSource[iCut]);

      // jet pt vs meson pt
      fHistoMCJetPtVsMesonPt[iCut] = new TH2F("MC_MesonPt_JetPt", "MC_MesonPt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsMesonPt[iCut]->SetXTitle("p_{T, meson} (true)");
      fHistoMCJetPtVsMesonPt[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsMesonPt[iCut]);

      fHistoMCJetPtVsMesonPtInAcc[iCut] = new TH2F("MC_MesonPt_JetPt_InAcc", "MC_MesonPt_JetPt_InAcc", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsMesonPtInAcc[iCut]->SetXTitle("p_{T, meson} (true)");
      fHistoMCJetPtVsMesonPtInAcc[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsMesonPtInAcc[iCut]);

      fHistoMCRecJetPtVsMesonPt[iCut] = new TH2F("MC_MesonPt_JetPt_InRecJet", "MC_MesonPt_JetPt_InRecJet", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCRecJetPtVsMesonPt[iCut]->SetXTitle("z (true)");
      fHistoMCRecJetPtVsMesonPt[iCut]->SetYTitle("p_{T, true, rec. Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCRecJetPtVsMesonPt[iCut]);

      // jet pt vs fragmentation z
      fHistoMCJetPtVsFrag[iCut] = new TH2F("MC_Frag_JetPt", "MC_Frag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsFrag[iCut]->SetXTitle("z (true)");
      fHistoMCJetPtVsFrag[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsFrag[iCut]);

      fHistoMCJetPtVsFragInAcc[iCut] = new TH2F("MC_Frag_JetPt_InAcc", "MC_Frag_JetPt_InAcc", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsFragInAcc[iCut]->SetXTitle("z (true)");
      fHistoMCJetPtVsFragInAcc[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsFragInAcc[iCut]);

      fHistoMCJetPtVsFrag_Sec[iCut] = new TH2F("MC_Sec_Frag_JetPt", "MC_Sec_Frag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsFrag_Sec[iCut]->SetXTitle("z (true)");
      fHistoMCJetPtVsFrag_Sec[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsFrag_Sec[iCut]);

      fHistoMCRecJetPtVsFrag[iCut] = new TH2F("MC_Frag_JetPt_InRecJet", "MC_Frag_JetPt_InRecJet", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCRecJetPtVsFrag[iCut]->SetXTitle("z (true)");
      fHistoMCRecJetPtVsFrag[iCut]->SetYTitle("p_{T, true, rec. Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCRecJetPtVsFrag[iCut]);

      fHistoMCJetPtVsMesonPt_Sec[iCut] = new TH2F("MC_Sec_MesonPt_JetPt", "MC_Sec_MesonPt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsMesonPt_Sec[iCut]->SetXTitle("p_{T, true} (true)");
      fHistoMCJetPtVsMesonPt_Sec[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsMesonPt_Sec[iCut]);

      fHistoMCPartonPtVsFrag[iCut] = new TH2F("MC_Frag_PartonPt", "MC_Frag_PartonPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCPartonPtVsFrag[iCut]->SetXTitle("z (true)");
      fHistoMCPartonPtVsFrag[iCut]->SetYTitle("p_{T, parton, jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCPartonPtVsFrag[iCut]);

      fHistoMCJetPtVsFragTrueParton[iCut] = new TH2F("MC_Frag_JetPt_TrueParton", "MC_Frag_JetPt_TrueParton", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCJetPtVsFragTrueParton[iCut]->SetXTitle("z (true)");
      fHistoMCJetPtVsFragTrueParton[iCut]->SetYTitle("p_{T, true, Jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCJetPtVsFragTrueParton[iCut]);

      fHistoMCPartonPtVsFragTrueParton[iCut] = new TH2F("MC_Frag_PartonPt_TrueParton", "MC_Frag_PartonPt_TrueParton", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoMCPartonPtVsFragTrueParton[iCut]->SetXTitle("z (true)");
      fHistoMCPartonPtVsFragTrueParton[iCut]->SetYTitle("p_{T, parton, jet} (GeV/c)");
      fMCList[iCut]->Add(fHistoMCPartonPtVsFragTrueParton[iCut]);
    }

    // conversion photons
    if (fIsMC && !fIsCalo) {
      fHistoTrueConvGammaPt[iCut] = new TH1F("ESD_TrueConvGamma_Pt", "ESD_TrueConvGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fTrueList[iCut]->Add(fHistoTrueConvGammaPt[iCut]);
      if (!fDoLightOutput) {
        fHistoTruePrimaryConvGammaPt[iCut] = new TH1F("ESD_TruePrimaryConvGamma_Pt", "ESD_TruePrimaryConvGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
        fTrueList[iCut]->Add(fHistoTruePrimaryConvGammaPt[iCut]);
        fHistoTruePrimaryConvGammaESDPtMCPt[iCut] = new TH2F("ESD_TruePrimaryConvGammaESD_PtMCPt", "ESD_TruePrimaryConvGammaESD_PtMCPt",
                                                             fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data(), fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
        fTrueList[iCut]->Add(fHistoTruePrimaryConvGammaESDPtMCPt[iCut]);
      }
    }

    if (fIsMC && !fIsConv) {
      fHistoTrueClusGammaPt[iCut] = new TH1F("TrueClusGamma_Pt", "ESD_TrueClusGamma_Pt", fVecBinsPhotonPt.size() - 1, fVecBinsPhotonPt.data());
      fHistoTrueClusGammaPt[iCut]->SetXTitle("p_{T} (GeV/c)");
      fTrueList[iCut]->Add(fHistoTrueClusGammaPt[iCut]);
    }

    // jet histograms
    fHistoEventwJets[iCut] = new TH1F("NEvents_with_Jets", "NEvents_with_Jets", 4, -0.5, 3.5);
    fJetList[iCut]->Add(fHistoEventwJets[iCut]);

    fHistoNJets[iCut] = new TH1F("NJets", "NJets", 20, -0.5, 19.5);
    fJetList[iCut]->Add(fHistoNJets[iCut]);

    fHistoPtJet[iCut] = new TH1F("JetPt", "JetPt", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
    fJetList[iCut]->Add(fHistoPtJet[iCut]);
    fHistoJetEta[iCut] = new TH1F("JetEta", "JetEta", 100, -1, 1);
    fJetList[iCut]->Add(fHistoJetEta[iCut]);
    fHistoJetPhi[iCut] = new TH1F("JetPhi", "JetPhi", 70, 0, 7);
    fJetList[iCut]->Add(fHistoJetPhi[iCut]);
    fHistoJetArea[iCut] = new TH1F("JetArea", "JetArea", 50, 0, 1);
    fJetList[iCut]->Add(fHistoJetArea[iCut]);

    if (fIsMC) {
      fHistoTruevsRecJetPt[iCut] = new TH2F("True_JetPt_vs_Rec_JetPt", "True_JetPt_vs_Rec_JetPt", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoTruevsRecJetPt[iCut]);
      if(!fDoLightOutput){
        fHistoTruevsRecJetPtForTrueJets[iCut] = new TH2F("True_JetPt_vs_Rec_JetPt_ForTrueJets", "True_JetPt_vs_Rec_JetPt_ForTrueJets", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
        fTrueJetList[iCut]->Add(fHistoTruevsRecJetPtForTrueJets[iCut]);
      }
      fHistoTrueJetPtVsPartonPt[iCut] = new TH2F("True_JetPt_vs_Parton_Pt", "True_JetPt_vs_Parton_Pt", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoTrueJetPtVsPartonPt[iCut]);

      fHistoMatchedPtJet[iCut] = new TH1F("JetPt_MatchedToTrue", "JetPt_MatchedToTrue", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoMatchedPtJet[iCut]);
      fHistoUnMatchedPtJet[iCut] = new TH1F("JetPt_NotMatchedToTrue", "JetPt_NotMatchedToTrue", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoUnMatchedPtJet[iCut]);
      fHistoTruePtJet[iCut] = new TH1F("TrueJetPt", "TrueJetPt", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoTruePtJet[iCut]);
      fHistoTrueMatchedPtJet[iCut] = new TH1F("TrueJetPt_MatchedToRec", "TrueJetPt_MatchedToRec", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoTrueMatchedPtJet[iCut]);
      fHistoTrueUnMatchedPtJet[iCut] = new TH1F("TrueJetPt_NotMatchedToRec", "TrueJetPt_NotMatchedToRec", fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fTrueJetList[iCut]->Add(fHistoTrueUnMatchedPtJet[iCut]);
    }

    // inv mass histograms
    fHistoInvMassVsPt[iCut] = new TH2F("ESD_Mother_InvMass_Pt", "ESD_Mother_InvMass_Pt", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
    fHistoInvMassVsPt[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
    fHistoInvMassVsPt[iCut]->SetYTitle("p_{T} (GeV/c)");
    fESDList[iCut]->Add(fHistoInvMassVsPt[iCut]);

    // in perpendicular cone

    fHistoInvMassVsPtPerpCone[iCut] = new TH2F("ESD_Mother_InvMass_Pt_PerpCone", "ESD_Mother_InvMass_Pt_PerpCone", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
    fHistoInvMassVsPtPerpCone[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
    fHistoInvMassVsPtPerpCone[iCut]->SetYTitle("p_{T} (GeV/c)");
    fESDList[iCut]->Add(fHistoInvMassVsPtPerpCone[iCut]);

    if (fIsMC) {
      // true mesons inv mass vs pT
      fRespMatrixHandlerTrueMesonInvMassVsPt[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsPt[iCut]->GetTHnSparse("ESD_TrueMother_InvMass_Pt_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsPt[iCut]->GetTH2("ESD_TrueMother_InvMass_Pt_JetPt"));

      fRespMatrixHandlerTrueMesonInvMassVsZ[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsZ[iCut]->GetTHnSparse("ESD_TrueMother_InvMass_Z_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsZ[iCut]->GetTH2("ESD_TrueMother_InvMass_Z_JetPt"));

      // true other mesons inv mass vs pT
      fRespMatrixHandlerTrueOtherMesonInvMassVsPt[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueOtherMesonInvMassVsPt[iCut]->GetTHnSparse("ESD_TrueOtherMother_InvMass_Pt_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueOtherMesonInvMassVsPt[iCut]->GetTH2("ESD_TrueOtherMother_InvMass_Pt_JetPt"));

      fRespMatrixHandlerTrueOtherMesonInvMassVsZ[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueOtherMesonInvMassVsZ[iCut]->GetTHnSparse("ESD_TrueOtherMother_InvMass_Z_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueOtherMesonInvMassVsZ[iCut]->GetTH2("ESD_TrueOtherMother_InvMass_Z_JetPt"));

      // secondary mesons inv mass vs pT
      fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt[iCut]->GetTHnSparse("ESD_TrueSecondaryMother_InvMass_Pt_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt[iCut]->GetTH2("ESD_TrueSecondaryMother_InvMass_Pt_JetPt"));

      fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ[iCut]->GetTHnSparse("ESD_TrueSecondaryMother_InvMass_Z_JetPt"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ[iCut]->GetTH2("ESD_TrueSecondaryMother_InvMass_Z_JetPt"));

      if(fDoMesonQA > 0){
        fHistoTrueMesonInvMassVsTruePt[iCut] = new TH2F("ESD_TrueMother_InvMass_TruePt", "ESD_TrueMother_InvMass_TruePt", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
        fHistoTrueMesonInvMassVsTruePt[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
        fHistoTrueMesonInvMassVsTruePt[iCut]->SetYTitle("p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueMesonInvMassVsTruePt[iCut]);

        fHistoTruePrimaryMesonInvMassPt[iCut] = new TH2F("ESD_TruePrimaryMother_InvMass_Pt", "ESD_TruePrimaryMother_InvMass_Pt", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
        fHistoTruePrimaryMesonInvMassPt[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
        fHistoTruePrimaryMesonInvMassPt[iCut]->SetYTitle("p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTruePrimaryMesonInvMassPt[iCut]);

        fHistoTrueSecondaryMesonInvMassPt[iCut] = new TH2F("ESD_TrueSecondaryMother_InvMass_Pt", "ESD_TrueSecondaryMother_InvMass_Pt", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
        fHistoTrueSecondaryMesonInvMassPt[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
        fHistoTrueSecondaryMesonInvMassPt[iCut]->SetYTitle("p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueSecondaryMesonInvMassPt[iCut]);
      }

      fHistoTrueMesonJetPtVsTruePt[iCut] = new TH2F("ESD_TrueMeson_TruePt_JetPt", "ESD_TrueMeson_TruePt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoTrueMesonJetPtVsTruePt[iCut]->SetXTitle("meson p_{T} (GeV/c)");
      fHistoTrueMesonJetPtVsTruePt[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
      fTrueList[iCut]->Add(fHistoTrueMesonJetPtVsTruePt[iCut]);

      fHistoTrueMesonJetPtVsTrueZ[iCut] = new TH2F("ESD_TrueMeson_TrueFrag_JetPt", "ESD_TrueMeson_TrueFrag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoTrueMesonJetPtVsTrueZ[iCut]->SetXTitle("Z");
      fHistoTrueMesonJetPtVsTrueZ[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
      fTrueList[iCut]->Add(fHistoTrueMesonJetPtVsTrueZ[iCut]);

      fHistoTrueMesonJetPtVsRecPt[iCut] = new TH2F("ESD_TrueMeson_RecPt_JetPt", "ESD_TrueMeson_RecPt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoTrueMesonJetPtVsRecPt[iCut]->SetXTitle("meson p_{T} (GeV/c)");
      fHistoTrueMesonJetPtVsRecPt[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
      fTrueList[iCut]->Add(fHistoTrueMesonJetPtVsRecPt[iCut]);

      fHistoTrueMesonJetPtVsRecZ[iCut] = new TH2F("ESD_TrueMeson_RecFrag_JetPt", "ESD_TrueMeson_RecFrag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
      fHistoTrueMesonJetPtVsRecZ[iCut]->SetXTitle("Z");
      fHistoTrueMesonJetPtVsRecZ[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
      fTrueList[iCut]->Add(fHistoTrueMesonJetPtVsRecZ[iCut]);

      if(fDoMesonQA > 0){
        fHistoTrueSecMesonJetPtVsRecPt[iCut] = new TH2F("ESD_TrueSecMeson_RecPt_JetPt", "ESD_TrueSecMeson_RecPt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
        fHistoTrueSecMesonJetPtVsRecPt[iCut]->SetXTitle("Pt");
        fHistoTrueSecMesonJetPtVsRecPt[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueSecMesonJetPtVsRecPt[iCut]);

        fHistoTrueSecMesonJetPtVsRecZ[iCut] = new TH2F("ESD_TrueSecMeson_RecFrag_JetPt", "ESD_TrueMeson_RecFrag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
        fHistoTrueSecMesonJetPtVsRecZ[iCut]->SetXTitle("Z");
        fHistoTrueSecMesonJetPtVsRecZ[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueSecMesonJetPtVsRecZ[iCut]);

        fHistoTrueMesonInTrueJet_JetPtVsTruePt[iCut] = new TH2F("ESD_TrueMeson_InTrueJets_TruePt_JetPt", "ESD_TrueMeson_InTrueJets_TruePt_JetPt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
        fHistoTrueMesonInTrueJet_JetPtVsTruePt[iCut]->SetXTitle("meson p_{T} (GeV/c)");
        fHistoTrueMesonInTrueJet_JetPtVsTruePt[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueMesonInTrueJet_JetPtVsTruePt[iCut]);

        fHistoTrueMesonInTrueJet_JetPtVsTrueZ[iCut] = new TH2F("ESD_TrueMeson_InTrueJets_TrueFrag_JetPt", "ESD_TrueMeson_InTrueJets_TrueFrag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
        fHistoTrueMesonInTrueJet_JetPtVsTrueZ[iCut]->SetXTitle("meson z_{T}");
        fHistoTrueMesonInTrueJet_JetPtVsTrueZ[iCut]->SetYTitle("Jet p_{T} (GeV/c)");
        fTrueList[iCut]->Add(fHistoTrueMesonInTrueJet_JetPtVsTrueZ[iCut]);

        fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
        if (fUseThNForResponse)
          fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount[iCut]->GetTHnSparse("ESD_TrueMother_InvMass_Pt_JetPt_DoubleCount"));
        else
          fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount[iCut]->GetTH2("ESD_TrueMother_InvMass_Pt_JetPt_DoubleCount"));

        fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
        if (fUseThNForResponse)
          fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount[iCut]->GetTHnSparse("ESD_TrueMother_InvMass_Z_JetPt_DoubleCount"));
        else
          fTrueList[iCut]->Add(fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount[iCut]->GetTH2("ESD_TrueMother_InvMass_Z_JetPt_DoubleCount"));

        fHistoMesonResponse[iCut] = new TH2F("ESD_TrueMeson_RecPt_TruePt", "ESD_TrueMeson_RecPt_TruePt", fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
        fHistoMesonResponse[iCut]->SetXTitle("p_{T, rec} (GeV/c)");
        fHistoMesonResponse[iCut]->SetYTitle("p_{T, true} (GeV/c)");
        fTrueList[iCut]->Add(fHistoMesonResponse[iCut]);
      }
    }

    if (fDoMesonQA > 0) {
      fHistoInvMassVsPt_Incl[iCut] = new TH2F("ESD_Mother_InvMass_Pt_Incl", "ESD_Mother_InvMass_Pt_Incl", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
      fHistoInvMassVsPt_Incl[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
      fHistoInvMassVsPt_Incl[iCut]->SetYTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoInvMassVsPt_Incl[iCut]);

      fHistoInvMassVsPtMassCut[iCut] = new TH2F("ESD_Mother_InvMass_Pt_MassCut", "ESD_Mother_InvMass_Pt_MassCut", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
      fHistoInvMassVsPtMassCut[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
      fHistoInvMassVsPtMassCut[iCut]->SetYTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoInvMassVsPtMassCut[iCut]);

      fHistoInvMassVsPtMassCutSB[iCut] = new TH2F("ESD_Mother_InvMass_Pt_MassCutSB", "ESD_Mother_InvMass_Pt_MassCutSB", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
      fHistoInvMassVsPtMassCutSB[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
      fHistoInvMassVsPtMassCutSB[iCut]->SetYTitle("p_{T} (GeV/c)");
      fESDList[iCut]->Add(fHistoInvMassVsPtMassCutSB[iCut]);
    }

    fHistoInvMassVsPtBack[iCut] = new TH2F("ESD_Back_InvMass_Pt", "ESD_Back_InvMass_Pt", fVecBinsMesonInvMass.size() - 1, fVecBinsMesonInvMass.data(), fVecBinsMesonPt.size() - 1, fVecBinsMesonPt.data());
    fHistoInvMassVsPtBack[iCut]->SetXTitle("M_{#gamma#gamma} (GeV/c^{2})");
    fHistoInvMassVsPtBack[iCut]->SetYTitle("p_{T} (GeV/c)");
    fESDList[iCut]->Add(fHistoInvMassVsPtBack[iCut]);

    fHistoJetPtVsFrag[iCut] = new TH2F("ESD_Frag_JetPt", "ESD_Frag_JetPt", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
    fHistoJetPtVsFrag[iCut]->SetXTitle("z");
    fHistoJetPtVsFrag[iCut]->SetYTitle("p_{T, Jet} (GeV/c)");
    fESDList[iCut]->Add(fHistoJetPtVsFrag[iCut]);

    // perpendicular cone
    fHistoJetPtVsFragPerpCone[iCut] = new TH2F("ESD_Frag_JetPt_PerpCone", "ESD_Frag_JetPt_PerpCone", fVecBinsFragment.size() - 1, fVecBinsFragment.data(), fVecBinsJetPt.size() - 1, fVecBinsJetPt.data());
    fHistoJetPtVsFragPerpCone[iCut]->SetXTitle("z");
    fHistoJetPtVsFragPerpCone[iCut]->SetYTitle("p_{T, Jet} (GeV/c)");
    fESDList[iCut]->Add(fHistoJetPtVsFragPerpCone[iCut]);

    fRespMatrixHandlerMesonInvMass[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, fVecBinsJetPt, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMass[iCut]->GetTHnSparse("InvMassVsPt_JetPt"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMass[iCut]->GetTH2("InvMassVsPt_JetPt"));

    fRespMatrixHandlerMesonInvMassVsZ[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassVsZ[iCut]->GetTHnSparse("InvMassVsZ_JetPt"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassVsZ[iCut]->GetTH2("InvMassVsZ_JetPt"));

    fRespMatrixHandlerMesonBackInvMassVsZ[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonBackInvMassVsZ[iCut]->GetTHnSparse("InvMassVsZ_Background_JetPt"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonBackInvMassVsZ[iCut]->GetTH2("InvMassVsZ_Background_JetPt"));

    fRespMatrixHandlerMesonBackInvMassVsPt[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonBackInvMassVsPt[iCut]->GetTHnSparse("InvMassVsPt_Background_JetPt"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonBackInvMassVsPt[iCut]->GetTH2("InvMassVsPt_Background_JetPt"));

    // perpendicular cone
    fRespMatrixHandlerMesonInvMassVsZPerpCone[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsFragment, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassVsZPerpCone[iCut]->GetTHnSparse("InvMassVsZ_JetPt_PerpCone"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassVsZPerpCone[iCut]->GetTH2("InvMassVsZ_JetPt_PerpCone"));

    fRespMatrixHandlerMesonInvMassPerpCone[iCut] = new MatrixHandler4D(fVecBinsMesonInvMass, fVecBinsMesonPt, fVecBinsJetPt, {0, 1}, fUseThNForResponse);
    if (fUseThNForResponse)
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassPerpCone[iCut]->GetTHnSparse("InvMassVsPt_JetPt_PerpCone"));
    else
      fESDList[iCut]->Add(fRespMatrixHandlerMesonInvMassPerpCone[iCut]->GetTH2("InvMassVsPt_JetPt_PerpCone"));

    if (fIsMC) {
      fRespMatrixHandlerMesonPt[iCut] = new MatrixHandler4D(fVecBinsMesonPt, fVecBinsMesonPt, fVecBinsJetPt, fVecBinsJetPt, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerMesonPt[iCut]->GetTHnSparse("MesonPt_JetPt_TrueVsRec"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerMesonPt[iCut]->GetTH2("MesonPt_JetPt_TrueVsRec"));

      fRespMatrixHandlerFrag[iCut] = new MatrixHandler4D(fVecBinsFragment, fVecBinsFragment, fVecBinsJetPt, fVecBinsJetPt, fUseThNForResponse);
      if (fUseThNForResponse)
        fTrueList[iCut]->Add(fRespMatrixHandlerFrag[iCut]->GetTHnSparse("Frag_JetPt_TrueVsRec"));
      else
        fTrueList[iCut]->Add(fRespMatrixHandlerFrag[iCut]->GetTH2("Frag_JetPt_TrueVsRec"));

      if(!fDoLightOutput){
        fRespMatrixHandlerFragTrueJets[iCut] = new MatrixHandler4D(fVecBinsFragment, fVecBinsFragment, fVecBinsJetPt, fVecBinsJetPt, fUseThNForResponse);
        if (fUseThNForResponse)
          fTrueList[iCut]->Add(fRespMatrixHandlerFragTrueJets[iCut]->GetTHnSparse("Frag_JetPt_TrueVsRec_ForTrueJets"));
        else
          fTrueList[iCut]->Add(fRespMatrixHandlerFragTrueJets[iCut]->GetTH2("Frag_JetPt_TrueVsRec_ForTrueJets"));
      }
    }

    // Call Sumw2 forall histograms in list
    if (fIsMC) {
      CallSumw2ForLists(fESDList[iCut]);
      CallSumw2ForLists(fTrueList[iCut]);
      CallSumw2ForLists(fMCList[iCut]);
      CallSumw2ForLists(fJetList[iCut]);
      CallSumw2ForLists(fTrueJetList[iCut]);
    }

  } // end cut loop

  if (fIsCalo || fIsConvCalo) {
    // Add the track matcher (has to be initialized in the AddTask!)
    for (int iMatcherTask = 0; iMatcherTask < 5; iMatcherTask++) {
      AliCaloTrackMatcher* temp = 0x0;
      if (!fCorrTaskSetting.CompareTo("")) {
        temp = (AliCaloTrackMatcher*)(AliAnalysisManager::GetAnalysisManager()->GetTask(Form("CaloTrackMatcher_%i_%i", iMatcherTask, fTrackMatcherRunningMode)));
      } else {
        temp = (AliCaloTrackMatcher*)(AliAnalysisManager::GetAnalysisManager()->GetTask(Form("CaloTrackMatcher_%i_%i_%s", iMatcherTask, fTrackMatcherRunningMode, fCorrTaskSetting.Data())));
      }
      if (temp)
        fOutputContainer->Add(temp->GetCaloTrackMatcherHistograms());
    }
  }

  // now add all the cut lists if needed
  for (int iCut = 0; iCut < fnCuts; iCut++) {
    // if(!((AliConvEventCuts*)fEventCutArray->At(iCut))) continue;
    if (((AliConvEventCuts*)fEventCutArray->At(iCut))->GetCutHistograms()) {
      fCutFolder[iCut]->Add(((AliConvEventCuts*)fEventCutArray->At(iCut))->GetCutHistograms());
    }
    if (fIsCalo || fIsConvCalo) {
      // if(!((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))) continue;
      if (((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))->GetCutHistograms()) {
        fCutFolder[iCut]->Add(((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))->GetCutHistograms());
      }
    }

    if (fIsConv || fIsConvCalo) {
      // if(!((AliConversionPhotonCuts*)fConvCutArray->At(iCut))) continue;
      if (((AliConversionPhotonCuts*)fConvCutArray->At(iCut))->GetCutHistograms()) {
        fCutFolder[iCut]->Add(((AliConversionPhotonCuts*)fConvCutArray->At(iCut))->GetCutHistograms());
      }
    }
    // if(fSetPlotHistsExtQA){
    //   if(!((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))) continue;
    //   if(((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))->GetExtQAHistograms()){
    //     fCutFolder[iCut]->Add(((AliCaloPhotonCuts*)fClusterCutArray->At(iCut))->GetExtQAHistograms());
    //   }
    // }
    // if(!((AliConversionMesonCuts*)fMesonCutArray->At(iCut))) continue;
    if (((AliConversionMesonCuts*)fMesonCutArray->At(iCut))->GetCutHistograms()) {
      fCutFolder[iCut]->Add(((AliConversionMesonCuts*)fMesonCutArray->At(iCut))->GetCutHistograms());
    }
  }

  OpenFile(1);
  PostData(1, fOutputContainer);
}

void AliAnalysisTaskMesonJetCorrelation::MakeBinning()
{
  // cout << " In Make Binning" << endl;

  double epsilon = 1e-6;

  if (fMesonPDGCode == 111) { // pi0
    double valInvMass = 0;
    for (int i = 0; i <= 1000; ++i) {
      fVecBinsMesonInvMass.push_back(valInvMass);
      if (valInvMass < 0.3 - epsilon)
        valInvMass += 0.004;
      else
        break;
    }
  } else if (fMesonPDGCode == 221) { // eta
    double valInvMass = 0.3;
    for (int i = 0; i <= 1000; ++i) {
      fVecBinsMesonInvMass.push_back(valInvMass);
      if (valInvMass >= 0.3 - epsilon && valInvMass < 0.8 - epsilon)
        valInvMass += 0.004;
      else
        break;
    }
  } else { // this should not really happen
    double valInvMass = 0;
    for (int i = 0; i <= 1000; ++i) {
      fVecBinsMesonInvMass.push_back(valInvMass);
      if (valInvMass < 0.3 - epsilon)
        valInvMass += 0.004;
      else
        break;
    }
  }
  //---------------------------
  // Photon/single part pt Binning
  //---------------------------
  double valGammaPt = 0;
  for (int i = 0; i < 1000; ++i) {
    fVecBinsPhotonPt.push_back(valGammaPt);
    if (valGammaPt < 1.0 - epsilon)
      valGammaPt += 0.1;
    else if (valGammaPt < 5 - epsilon)
      valGammaPt += 0.2;
    else if (valGammaPt < 10 - epsilon)
      valGammaPt += 0.5;
    else if (valGammaPt < 50 - epsilon)
      valGammaPt += 1;
    else if (valGammaPt < 100 - epsilon)
      valGammaPt += 5;
    else
      break;
  }
  //---------------------------
  // Cluster/single part pt Binning
  //---------------------------
  double valClusterPt = 0;
  for (int i = 0; i < 1000; ++i) {
    fVecBinsClusterPt.push_back(valClusterPt);
    if (valClusterPt < 1.0 - epsilon)
      valClusterPt += 0.1;
    else if (valClusterPt < 5 - epsilon)
      valClusterPt += 0.2;
    else if (valClusterPt < 10 - epsilon)
      valClusterPt += 0.5;
    else if (valClusterPt < 50 - epsilon)
      valClusterPt += 1;
    else if (valClusterPt < 100 - epsilon)
      valClusterPt += 5;
    else if (valClusterPt < 200 - epsilon)
      valClusterPt += 10;
    else
      break;
  }
  //---------------------------
  // Meson pt Binning
  //---------------------------
  double valMesonPt = 0;
  for (int i = 0; i < 1000; ++i) {
    fVecBinsMesonPt.push_back(valMesonPt);
    if (valMesonPt < 5 - epsilon)
      valMesonPt += 0.2;
    else if (valMesonPt < 10 - epsilon)
      valMesonPt += 0.5;
    else if (valMesonPt < 20 - epsilon)
      valMesonPt += 1;
    else if (valMesonPt < 50 - epsilon)
      valMesonPt += 5;
    else
      break;
  }
  //---------------------------
  // Jet pt Binning
  //---------------------------
  double valJetPt = 0;
  for (int i = 0; i < 1000; ++i) {
    fVecBinsJetPt.push_back(valJetPt);
    if (valJetPt < 10 - epsilon)
      valJetPt += 10;
    else if (valJetPt < 50.0 - epsilon)
      valJetPt += 5;
    else if (valJetPt < 100 - epsilon)
      valJetPt += 10;
    else if (valJetPt < 200 - epsilon)
      valJetPt += 20;
    else if (valJetPt < 500 - epsilon)
      valJetPt += 50;
    else
      break;
  }
  //---------------------------
  // Fragmentation Binning
  //---------------------------
  double valZ = 0;
  for (int i = 0; i < 1000; ++i) {
    fVecBinsFragment.push_back(valZ);
    if (valZ < 0.1 - epsilon)
      valZ += 0.01;
    else if (valZ < 0.2 - epsilon)
      valZ += 0.02;
    else if (valZ < 1 - epsilon)
      valZ += 0.05;
    else if (valZ < 1.2 - epsilon)
      valZ += 0.1;
    else
      break;
  }

  //---------------------------
  // Equidistant binning starting at -0.5
  //---------------------------
  for (int i = 0; i < 1000; ++i) {
    vecEquidistFromMinus05.push_back(i - 0.5);
  }
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::CallSumw2ForLists(TList* l)
{
  if (fIsMC > 1) {
    TIter iter(l->MakeIterator());
    while (TObject* obj = iter()) {
      TString className = obj->ClassName();
      if (className.Contains("TH1")) {
        static_cast<TH1*>(obj)->Sumw2();
      } else if (className.Contains("TH2")) {
        static_cast<TH2*>(obj)->Sumw2();
      } else if (className.Contains("THnSparse")) {
        static_cast<THnSparse*>(obj)->Sumw2();
      }
    }
  }
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::InitJets()
{
  fVectorJetPt = fConvJetReader->GetVectorJetPt();
  fVectorJetPx = fConvJetReader->GetVectorJetPx();
  fVectorJetPy = fConvJetReader->GetVectorJetPy();
  fVectorJetPz = fConvJetReader->GetVectorJetPz();
  fVectorJetEta = fConvJetReader->GetVectorJetEta();
  fVectorJetPhi = fConvJetReader->GetVectorJetPhi();
  fVectorJetArea = fConvJetReader->GetVectorJetArea();

  fVectorJetEtaPerp = fConvJetReader->GetVectorJetEta();
  for (auto& eta : fVectorJetEtaPerp) {
    eta *= -1;
  }
  fVectorJetPhiPerp = fConvJetReader->GetVectorJetPhi();
  for (auto& phi : fVectorJetPhiPerp) {
    phi += 0.5 * TMath::Pi();
    if (phi > 2 * TMath::Pi()) {
      phi -= TMath::Pi();
    }
  }

  if (fIsMC > 0) {
    if (fIsMC) {
      if (!fAODMCTrackArray)
        fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
      fConvJetReader->FindPartonsJet(fAODMCTrackArray);
    }
    fTrueVectorJetPx = fConvJetReader->GetTrueVectorJetPx();
    fTrueVectorJetPy = fConvJetReader->GetTrueVectorJetPy();
    fTrueVectorJetPz = fConvJetReader->GetTrueVectorJetPz();
    fTrueVectorJetPt = fConvJetReader->GetTrueVectorJetPt();
    fTrueVectorJetEta = fConvJetReader->GetTrueVectorJetEta();
    fTrueVectorJetPhi = fConvJetReader->GetTrueVectorJetPhi();
    fTrueVectorJetPartonID = fConvJetReader->GetTrueVectorJetParton();
    fTrueVectorJetPartonPt = fConvJetReader->GetTrueVectorJetPartonPt();
  }
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessJets()
{

  // clear map before next event
  MapRecJetsTrueJets.clear();

  fHistoNJets[fiCut]->Fill(fConvJetReader->GetNJets(), fWeightJetJetMC);

  if (fConvJetReader->GetTrueNJets() > 0) {
    fHistoEventwJets[fiCut]->Fill(1., fWeightJetJetMC); // event has true jet
    if (fConvJetReader->GetNJets() > 0) {
      fHistoEventwJets[fiCut]->Fill(2., fWeightJetJetMC); // event has true jet and rec. jet
    }
  }

  if (fConvJetReader->GetNJets() > 0) {

    fHistoEventwJets[fiCut]->Fill(0., fWeightJetJetMC); // event has jet
    fHighestJetVector.SetXYZ(0, 0, 0);
    fMaxPtJet = 0.;

    for (int i = 0; i < fConvJetReader->GetNJets(); i++) {
      fHistoPtJet[fiCut]->Fill(fVectorJetPt.at(i), fWeightJetJetMC);
      fHistoJetEta[fiCut]->Fill(fVectorJetEta.at(i), fWeightJetJetMC);
      fHistoJetPhi[fiCut]->Fill(fVectorJetPhi.at(i), fWeightJetJetMC);
      fHistoJetArea[fiCut]->Fill(fVectorJetArea.at(i), fWeightJetJetMC);

      if (fIsMC > 0 && fConvJetReader->GetNJets() > 0 && fConvJetReader->GetTrueNJets() > 0) {
        Double_t min = 100;
        int match = -1;
        for (int j = 0; j < fConvJetReader->GetTrueNJets(); j++) {
          Double_t R_jetjet;
          Double_t DeltaEta = fVectorJetEta.at(i) - fTrueVectorJetEta.at(j);
          Double_t DeltaPhi = abs(fVectorJetPhi.at(i) - fTrueVectorJetPhi.at(j));
          if (DeltaPhi > M_PI) {
            DeltaPhi = 2 * M_PI - DeltaPhi;
          }
          R_jetjet = TMath::Sqrt(pow((DeltaEta), 2) + pow((DeltaPhi), 2));
          if (R_jetjet < min) {
            min = R_jetjet;
            match = j;
          }
        }
        if (match >= 0) {
          MapRecJetsTrueJets[i] = match; // store matched jet indices in map
          fHistoTruevsRecJetPt[fiCut]->Fill(fVectorJetPt.at(i), fTrueVectorJetPt.at(match), fWeightJetJetMC);
          fHistoTrueJetPtVsPartonPt[fiCut]->Fill(fTrueVectorJetPt.at(match), fTrueVectorJetPartonPt.at(match), fWeightJetJetMC);
          fHistoMatchedPtJet[fiCut]->Fill(fVectorJetPt.at(i), fWeightJetJetMC);
        } else {
          fHistoUnMatchedPtJet[fiCut]->Fill(fVectorJetPt.at(i), fWeightJetJetMC);
          // Fill response matrix in case that no corresponding true jet was found
          if(!fDoLightOutput){
            fHistoTruevsRecJetPtForTrueJets[fiCut]->Fill(fVectorJetPt.at(i), 0.5, fWeightJetJetMC);
          }
        }
      }
      if (fVectorJetPt.at(i) > fMaxPtJet) {
        fMaxPtJet = fVectorJetPt.at(i);
        fHighestJetVector.SetXYZ(fVectorJetPx[i], fVectorJetPy[i], fVectorJetPz[i]);
      }
    }
  }

  // Process True Jets
  if (fIsMC) {
    for (int i = 0; i < fConvJetReader->GetTrueNJets(); i++) {
      fHistoTruePtJet[fiCut]->Fill(fTrueVectorJetPt.at(i), fWeightJetJetMC);

      // lambda to find the index of the rec. jet corresponding to the true jet
      auto findResults = std::find_if(std::begin(MapRecJetsTrueJets), std::end(MapRecJetsTrueJets), [&](const std::pair<int, int>& pair) {
        return pair.second == i;
      });
      if (findResults != std::end(MapRecJetsTrueJets)) {
        fHistoTrueMatchedPtJet[fiCut]->Fill(fTrueVectorJetPt.at(i), fWeightJetJetMC);
        // Fill response matrix in case that pair of true and rec jets are found
        if(!fDoLightOutput){
          if (findResults->first < static_cast<int>(fVectorJetPt.size())) { // probably not needed for safety
            fHistoTruevsRecJetPtForTrueJets[fiCut]->Fill(fVectorJetPt.at(findResults->first), fTrueVectorJetPt.at(i), fWeightJetJetMC);
          }
        }
      } else {
        fHistoTrueUnMatchedPtJet[fiCut]->Fill(fTrueVectorJetPt.at(i), fWeightJetJetMC);
        // Fill response matrix in case that no corresponding rec jet was found
        if(!fDoLightOutput){
          fHistoTruevsRecJetPtForTrueJets[fiCut]->Fill(0.5, fTrueVectorJetPt.at(i), fWeightJetJetMC);
        }
      }
    }
  }
}

//_____________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::UserExec(Option_t*)
{
  //
  // Called for each event
  //
  fInputEvent = InputEvent();

  if (fIsMC > 0)
    fMCEvent = MCEvent();

  int eventQuality = ((AliConvEventCuts*)fV0Reader->GetEventCuts())->GetEventQuality();
  if (fInputEvent->IsIncompleteDAQ() == kTRUE)
    eventQuality = 2; // incomplete event
  // Event Not Accepted due to MC event missing or wrong trigger for V0ReaderV1 or because it is incomplete => abort processing of this event/file
  if (eventQuality == 2 || eventQuality == 3) {
    for (int iCut = 0; iCut < fnCuts; iCut++) {
      fHistoNEvents[iCut]->Fill(eventQuality, fWeightJetJetMC);
      if (fIsMC > 1)
        fHistoNEventsWOWeight[iCut]->Fill(eventQuality);
    }
    return;
  }

  if (fIsConv || fIsConvCalo) {
    fReaderGammas = fV0Reader->GetReconstructedGammas(); // Gammas from default Cut
  }

  if (fIsMC > 0 && fInputEvent->IsA() == AliAODEvent::Class() && !(fV0Reader->AreAODsRelabeled())) {
    RelabelAODPhotonCandidates(kTRUE); // In case of AODMC relabeling MC
    fV0Reader->RelabelAODs(kTRUE);
  }

  // Get Event Plane Angle
  AliEventplane* EventPlane = fInputEvent->GetEventplane();
  if (fIsHeavyIon == 1)
    fEventPlaneAngle = EventPlane->GetEventplane("V0", fInputEvent, 2);
  else
    fEventPlaneAngle = 0.0;

  for (int iCut = 0; iCut < fnCuts; iCut++) {
    fiCut = iCut;
    Bool_t isRunningEMCALrelAna = kFALSE;
    if (fIsCalo || fIsConvCalo) {
      if (((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetClusterType() == 1)
        isRunningEMCALrelAna = kTRUE;
    }

    int eventNotAccepted = ((AliConvEventCuts*)fEventCutArray->At(iCut))->IsEventAcceptedByCut(fV0Reader->GetEventCuts(), fInputEvent, fMCEvent, fIsHeavyIon, isRunningEMCALrelAna);
    if (fIsMC == 2) {
      Float_t xsection = -1.;
      Float_t ntrials = -1.;
      ((AliConvEventCuts*)fEventCutArray->At(iCut))->GetXSectionAndNTrials(fMCEvent, xsection, ntrials, fInputEvent);
      if ((xsection == -1.) || (ntrials == -1.))
        AliFatal(Form("ERROR: GetXSectionAndNTrials returned invalid xsection/ntrials, periodName from V0Reader: '%s'", fV0Reader->GetPeriodName().Data()));
      fProfileJetJetXSection[iCut]->Fill(0., xsection);
      fHistoJetJetNTrials[iCut]->Fill(1, ntrials);
    }

    fWeightJetJetMC = 1;
    if (fIsMC > 0) {
      Float_t maxjetpt = -1.;
      Float_t pthard = -1;
      if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetUseJetFinderForOutliers())
        maxjetpt = fOutlierJetReader->GetMaxJetPt();
      Bool_t isMCJet = ((AliConvEventCuts*)fEventCutArray->At(iCut))->IsJetJetMCEventAccepted(fMCEvent, fWeightJetJetMC, pthard, fInputEvent, maxjetpt);
      if (isMCJet && (fIsMC == 2)) {
        fHistoPtHardJJWeight[iCut]->Fill(pthard, fWeightJetJetMC);
      }
      if (fIsMC == 3) {
        Double_t weightMult = ((AliConvEventCuts*)fEventCutArray->At(iCut))->GetWeightForMultiplicity(fV0Reader->GetNumberOfPrimaryTracks());
        fWeightJetJetMC = fWeightJetJetMC * weightMult;
      }

      if (!isMCJet) {
        fHistoNEvents[iCut]->Fill(10, fWeightJetJetMC);
        if (fIsMC > 1)
          fHistoNEventsWOWeight[iCut]->Fill(10);
        continue;
      }
    }

    // Jets need to be initialized before the ProcessMCParticles because they are needed in ProcessAODMCParticles
    if(fLocalDebugFlag) {printf("InitJets\n");}
    InitJets();

    // reset double counting vector
    fMesonDoubleCount.clear();

    if (eventNotAccepted != 0 && eventQuality != 4) {
      fHistoNEvents[iCut]->Fill(eventNotAccepted, fWeightJetJetMC); // Check Centrality, PileUp, SDD and V0AND --> Not Accepted => eventQuality = 1
      if (fIsMC > 1)
        fHistoNEventsWOWeight[iCut]->Fill(eventNotAccepted);

      if(fLocalDebugFlag) {printf("ProcessAODMCParticles\n");}
      if (fIsMC > 0) {
        if (eventNotAccepted == 3) { // Event rejected due to wrong trigger, MC particles still have to be processed
          ProcessAODMCParticles(1);
        }
        if (eventNotAccepted != 1) { // exclude centrality/multiplicity selection from MC particles processing
          ProcessAODMCParticles(2);
        }
      }
      continue;
    }
    if(fLocalDebugFlag) {printf("ProcessAODMCParticles\n");}
    if (eventQuality != 0) { // Event Not Accepted
      fHistoNEvents[iCut]->Fill(eventQuality, fWeightJetJetMC);
      if (fIsMC > 1)
        fHistoNEventsWOWeight[iCut]->Fill(eventQuality); // Should be 0 here

      if (fIsMC > 0) {
        if (eventQuality != 4) { // 4 = event outside of +-10cm, we dont want to count these events
          ProcessAODMCParticles(2);
        }
      }
      continue;
    }

    // from here on we accepted the event!
    fHistoNEvents[iCut]->Fill(eventQuality, fWeightJetJetMC); // Should be 0 here
    if (fIsMC > 1)
      fHistoNEventsWOWeight[iCut]->Fill(eventQuality); // Should be 0 here

    fHistoNGoodESDTracks[iCut]->Fill(fV0Reader->GetNumberOfPrimaryTracks(), fWeightJetJetMC);
    if (fConvJetReader->GetNJets() > 0) {
      fHistoNGoodESDTracksEvtWithJets[iCut]->Fill(fV0Reader->GetNumberOfPrimaryTracks(), fWeightJetJetMC);
    }
    fHistoVertexZ[iCut]->Fill(fInputEvent->GetPrimaryVertex()->GetZ(), fWeightJetJetMC);

    if (fIsMC > 0) {
      ProcessAODMCParticles(0);
    }

    if(fLocalDebugFlag) {printf("ProcessJets\n");}
    ProcessJets();
    if (fIsConvCalo || fIsCalo) {
      if(fLocalDebugFlag) {printf("ProcessJets\n");}
      ProcessClusters(); // process calo clusters
    }
    if (fIsConvCalo || fIsConv) {
      if(fLocalDebugFlag) {printf("ProcessJets\n");}
      ProcessPhotonCandidates(); // Process this cuts gammas
    }

    if(fLocalDebugFlag) {printf("ProcessJets\n");}
    CalculateMesonCandidates();

    if(fLocalDebugFlag) {printf("ProcessJets\n");}
    CalculateBackground();

    if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->DoGammaSwappForBg()) {
      if(fLocalDebugFlag) {printf("UpdateEventMixData\n");}
      UpdateEventMixData();
    }
  }

  if (fIsMC > 0 && fInputEvent->IsA() == AliAODEvent::Class() && !(fV0Reader->AreAODsRelabeled())) {
    RelabelAODPhotonCandidates(kFALSE); // Back to ESDMC Label
    fV0Reader->RelabelAODs(kFALSE);
  }
  PostData(1, fOutputContainer);

} // end userExec

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessClusters()
{
  // cout << "in Process clusters" << endl;

  // clear cluster candidates
  for (unsigned int i = 0; i < fClusterCandidates.size(); ++i) {
    if (fClusterCandidates[i])
      delete fClusterCandidates[i];
    fClusterCandidates[i] = nullptr;
  }
  fClusterCandidates.clear();

  int nclus = 0;
  TClonesArray* arrClustersProcess = NULL;
  // fNCurrentClusterBasic             = 0;
  if (!fCorrTaskSetting.CompareTo("")) {
    nclus = fInputEvent->GetNumberOfCaloClusters();
  } else {
    arrClustersProcess = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(Form("%sClustersBranch", fCorrTaskSetting.Data())));
    if (!arrClustersProcess) {
      AliFatal(Form("%sClustersBranch was not found in AliAnalysisTaskGammaCalo! Check the correction framework settings!", fCorrTaskSetting.Data()));
    }
    nclus = arrClustersProcess->GetEntries();
  }

  if (nclus == 0)
    return;
  int NClusinJets = 0;

  // match tracks to clusters
  ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->MatchTracksToClusters(fInputEvent, fWeightJetJetMC, kTRUE, fMCEvent);

  for (Long_t i = 0; i < nclus; i++) {
    Double_t tempClusterWeight = fWeightJetJetMC;
    // Double_t tempPhotonWeight = fWeightJetJetMC;
    AliVCluster* clus = NULL;
    if (fInputEvent->IsA() == AliESDEvent::Class()) {
      if (arrClustersProcess)
        clus = new AliESDCaloCluster(*(AliESDCaloCluster*)arrClustersProcess->At(i));
      else
        clus = new AliESDCaloCluster(*(AliESDCaloCluster*)fInputEvent->GetCaloCluster(i));
    } else if (fInputEvent->IsA() == AliAODEvent::Class()) {
      if (arrClustersProcess)
        clus = new AliAODCaloCluster(*(AliAODCaloCluster*)arrClustersProcess->At(i));
      else
        clus = new AliAODCaloCluster(*(AliAODCaloCluster*)fInputEvent->GetCaloCluster(i));
    }
    if (!clus)
      continue;

    // Set the jetjet weight to 1 in case the cluster orignated from the minimum bias header
    if (fIsMC > 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() == 4) {
      if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(clus->GetLabelAt(0), fMCEvent, fInputEvent) == 2) {
        tempClusterWeight = 1;
      }
    }

    // apply cluster cuts
    if (!((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelected(clus, fInputEvent, fMCEvent, fIsMC, tempClusterWeight, i)) {
      // if (((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetIsAcceptedForBasicCounting())fNCurrentClusterBasic++;
      delete clus;
      continue;
    }

    // vertex
    Double_t vertex[3] = {0};
    InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);

    // TLorentzvector with cluster
    TLorentzVector clusterVector;
    clus->GetMomentum(clusterVector, vertex);

    TLorentzVector* tmpvec = new TLorentzVector();
    tmpvec->SetPxPyPzE(clusterVector.Px(), clusterVector.Py(), clusterVector.Pz(), clusterVector.E());

    // convert to AODConversionPhoton
    AliAODConversionPhoton* PhotonCandidate = new AliAODConversionPhoton(tmpvec);
    if (!PhotonCandidate) {
      delete clus;
      delete tmpvec;
      continue;
    }

    // Flag Photon as CaloPhoton
    PhotonCandidate->SetIsCaloPhoton(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetClusterType());
    PhotonCandidate->SetCaloClusterRef(i);
    PhotonCandidate->SetLeadingCellID(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->FindLargestCellInCluster(clus, fInputEvent));
    // get MC label
    if (fIsMC > 0) {
      int* mclabelsCluster = clus->GetLabels();
      PhotonCandidate->SetNCaloPhotonMCLabels(clus->GetNLabels());
      if (clus->GetNLabels() > 0) {
        for (int k = 0; k < (int)clus->GetNLabels(); k++) {
          PhotonCandidate->SetCaloPhotonMCLabel(k, mclabelsCluster[k]);
        } // end of label loop
      }
    }

    fHistoClusterPt[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
    fHistoClusterE[fiCut]->Fill(PhotonCandidate->E(), fWeightJetJetMC);

    float clusPos[3] = {0, 0, 0};
    clus->GetPosition(clusPos);
    TVector3 clusterVectorJets(clusPos[0], clusPos[1], clusPos[2]);
    double etaCluster = clusterVectorJets.Eta();
    double phiCluster = clusterVectorJets.Phi();

    if (fConvJetReader->GetNJets() > 0) {
      double RJetPi0Cand;
      int matchedJet = -1;
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), etaCluster, phiCluster, matchedJet, RJetPi0Cand)) {
        NClusinJets++;
        // fClusterEtaPhiJets[fiCut]->Fill(phiCluster, etaCluster);
        fHistoClusterPtInJet[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
        fHistoClusterEInJet[fiCut]->Fill(PhotonCandidate->E(), fWeightJetJetMC);
      }

      double RJetPi0CandPerp = 0;
      int matchedJetPerp = -1;
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhiPerp, fConvJetReader->Get_Jet_Radius(), etaCluster, phiCluster, matchedJetPerp, RJetPi0CandPerp)) {
        fHistoClusterPtPerpCone[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
      }
    }

    fIsFromDesiredHeader = kTRUE;
    // bool fIsOverlappingWithOtherHeader = kFALSE;
    // bool fAllowOverlapHeaders = true;
    // test whether largest contribution to cluster orginates in added signals
    if (fIsMC > 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() > 0) {
      // Set the jetjet weight to 1 in case the photon candidate orignated from the minimum bias header
      // if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetCaloPhotonMCLabel(0), fMCEvent, fInputEvent) == 2 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() == 4)
      //   tempPhotonWeight = 1;
      // if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetCaloPhotonMCLabel(0), fMCEvent, fInputEvent) == 0)
      // fIsFromDesiredHeader = kFALSE;
      // if (clus->GetNLabels() > 1) {
      // int* mclabelsCluster = clus->GetLabels();
      // for (int l = 1; l < (int)clus->GetNLabels(); l++) {
      //   if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(mclabelsCluster[l], fMCEvent, fInputEvent, false) == 0)
      //     fIsOverlappingWithOtherHeader = kTRUE;
      // }
      // }
    }

    if (fIsMC > 0) {
      ProcessTrueClusterCandidatesAOD(PhotonCandidate);
    }
    // if ( (fIsFromDesiredHeader && !fIsOverlappingWithOtherHeader && !fAllowOverlapHeaders) || (fIsFromDesiredHeader && fAllowOverlapHeaders) ){
    fClusterCandidates.push_back(PhotonCandidate);
    // }

    delete clus;
    delete tmpvec;
  } // end cluster loop
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessPhotonCandidates()
{
  // cout << "ProcessPhotonCandidates" << endl;
  // clear photon candidates
  fGammaCandidates.clear();

  if (((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetDoElecDeDxPostCalibration()) {
    if (!(((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->LoadElecDeDxPostCalibration(fInputEvent->GetRunNumber()))) {
      AliFatal(Form("ERROR: LoadElecDeDxPostCalibration returned kFALSE for %d despite being requested!", fInputEvent->GetRunNumber()));
    }
  }

  Double_t magField = fInputEvent->GetMagneticField();
  int nV0 = 0;
  TList* GammaCandidatesStepOne = new TList();
  TList* GammaCandidatesStepTwo = new TList();
  // Loop over Photon Candidates allocated by ReaderV1

  for (int i = 0; i < fReaderGammas->GetEntriesFast(); i++) {
    AliAODConversionPhoton* PhotonCandidate = (AliAODConversionPhoton*)fReaderGammas->At(i);
    if (!PhotonCandidate)
      continue;
    fIsFromDesiredHeader = kTRUE;

    Double_t weightMatBudgetGamma = 1.;
    if (fDoMaterialBudgetWeightingOfGammasForTrueMesons && ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetWeightsInitialized()) {
      weightMatBudgetGamma = ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetCorrectingWeightForTrueGamma(PhotonCandidate, magField);
    }

    // if(fIsMC>0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 0){
    //   int isPosFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelPositive(), fMCEvent, fInputEvent);
    //   if(isPosFromMBHeader == 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 3) continue;
    //   int isNegFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelNegative(), fMCEvent, fInputEvent);
    //   if(isNegFromMBHeader == 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 3) continue;
    //   if( (isNegFromMBHeader+isPosFromMBHeader) != 4) fIsFromDesiredHeader = kFALSE;
    // }

    if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->PhotonIsSelected(PhotonCandidate, fInputEvent))
      continue;
    if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->InPlaneOutOfPlaneCut(PhotonCandidate->GetPhotonPhi(), fEventPlaneAngle))
      continue;
    if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseElecSharingCut() &&
        !((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseToCloseV0sCut()) {
      fGammaCandidates.push_back(PhotonCandidate); // if no second loop is required add to events good gammas

      if (fIsFromDesiredHeader) {
        fHistoConvGammaPt[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
        if (fConvJetReader->GetNJets() > 0) {
          double RJetPi0Cand;
          int matchedJet = -1;
          if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJet, RJetPi0Cand)) {
            fHistoConvGammaPtInJet[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
          }
          double RJetPi0CandPerp = 0;
          int matchedJetPerp = -1;
          if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhiPerp, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJetPerp, RJetPi0CandPerp)) {
            fHistoConvGammaPtPerpCone[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
          }
        }
      }
      if (fIsMC > 0) {
        ProcessTruePhotonCandidatesAOD(PhotonCandidate);
      }

    } else if (((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseElecSharingCut()) { // if Shared Electron cut is enabled, Fill array, add to step one
      ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->FillElectonLabelArray(PhotonCandidate, nV0);
      nV0++;
      GammaCandidatesStepOne->Add(PhotonCandidate);
    } else if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseElecSharingCut() &&
               ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseToCloseV0sCut()) { // shared electron is disabled, step one not needed -> step two
      GammaCandidatesStepTwo->Add(PhotonCandidate);
    }
  }
  if (((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseElecSharingCut()) {

    for (int i = 0; i < GammaCandidatesStepOne->GetEntries(); i++) {

      AliAODConversionPhoton* PhotonCandidate = (AliAODConversionPhoton*)GammaCandidatesStepOne->At(i);
      if (!PhotonCandidate)
        continue;
      fIsFromDesiredHeader = kTRUE;

      Double_t weightMatBudgetGamma = 1.;
      if (fDoMaterialBudgetWeightingOfGammasForTrueMesons && ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetWeightsInitialized()) {
        weightMatBudgetGamma = ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetCorrectingWeightForTrueGamma(PhotonCandidate, magField);
      }

      if (fMCEvent && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 0) {
        int isPosFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelPositive(), fMCEvent, fInputEvent);
        int isNegFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelNegative(), fMCEvent, fInputEvent);
        if ((isNegFromMBHeader + isPosFromMBHeader) != 4)
          fIsFromDesiredHeader = kFALSE;
      }

      if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->RejectSharedElectronV0s(PhotonCandidate, i, GammaCandidatesStepOne->GetEntries()))
        continue;
      if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseToCloseV0sCut()) { // To Colse v0s cut diabled, step two not needed
        fGammaCandidates.push_back(PhotonCandidate);
        if (fIsFromDesiredHeader) {
          fHistoConvGammaPt[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
          if (fConvJetReader->GetNJets() > 0) {
            double RJetPi0Cand;
            int matchedJet = -1;
            if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJet, RJetPi0Cand)) {
              fHistoConvGammaPtInJet[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
            }
            double RJetPi0CandPerp = 0;
            int matchedJetPerp = -1;
            if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhiPerp, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJetPerp, RJetPi0CandPerp)) {
              fHistoConvGammaPtPerpCone[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
            }
          }
        }
        if (fIsMC > 0) {
          ProcessTruePhotonCandidatesAOD(PhotonCandidate);
        }
      } else
        GammaCandidatesStepTwo->Add(PhotonCandidate); // Close v0s cut enabled -> add to list two
    }
  }
  if (((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->UseToCloseV0sCut()) {
    for (int i = 0; i < GammaCandidatesStepTwo->GetEntries(); i++) {

      AliAODConversionPhoton* PhotonCandidate = (AliAODConversionPhoton*)GammaCandidatesStepTwo->At(i);

      if (!PhotonCandidate)
        continue;
      fIsFromDesiredHeader = kTRUE;

      Double_t weightMatBudgetGamma = 1.;
      if (fDoMaterialBudgetWeightingOfGammasForTrueMesons && ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetWeightsInitialized()) {
        weightMatBudgetGamma = ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetCorrectingWeightForTrueGamma(PhotonCandidate, magField);
      }

      if (fMCEvent && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 0) {
        int isPosFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelPositive(), fMCEvent, fInputEvent);
        int isNegFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(PhotonCandidate->GetMCLabelNegative(), fMCEvent, fInputEvent);
        if ((isNegFromMBHeader + isPosFromMBHeader) != 4)
          fIsFromDesiredHeader = kFALSE;
      }
      if (!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->RejectToCloseV0s(PhotonCandidate, GammaCandidatesStepTwo, i))
        continue;
      fGammaCandidates.push_back(PhotonCandidate); // Add gamma to current cut TList
      if (fIsFromDesiredHeader) {
        fHistoConvGammaPt[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
        if (fConvJetReader->GetNJets() > 0) {
          double RJetPi0Cand;
          int matchedJet = -1;
          if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJet, RJetPi0Cand)) {
            fHistoConvGammaPtInJet[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
          }
          double RJetPi0CandPerp = 0;
          int matchedJetPerp = -1;
          if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhiPerp, fConvJetReader->Get_Jet_Radius(), PhotonCandidate->Eta(), PhotonCandidate->Phi(), matchedJetPerp, RJetPi0CandPerp)) {
            fHistoConvGammaPtPerpCone[fiCut]->Fill(PhotonCandidate->Pt(), fWeightJetJetMC);
          }
        }
      }
      if (fIsMC > 0) {
        ProcessTruePhotonCandidatesAOD(PhotonCandidate);
      }
    }
  }

  delete GammaCandidatesStepOne;
  GammaCandidatesStepOne = 0x0;
  delete GammaCandidatesStepTwo;
  GammaCandidatesStepTwo = 0x0;
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::CalculateMesonCandidates()
{
  // PCM-Calo case
  if (fIsConvCalo) {
    if (fGammaCandidates.size() > 0) {

      TClonesArray* arrClustersMesonCand = NULL;
      if (fCorrTaskSetting.CompareTo(""))
        arrClustersMesonCand = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(Form("%sClustersBranch", fCorrTaskSetting.Data())));

      for (unsigned int firstGammaIndex = 0; firstGammaIndex < fGammaCandidates.size(); firstGammaIndex++) {
        AliAODConversionPhoton* gamma0 = dynamic_cast<AliAODConversionPhoton*>(fGammaCandidates[firstGammaIndex]);
        if (gamma0 == NULL)
          continue;

        for (unsigned int secondGammaIndex = 0; secondGammaIndex < fClusterCandidates.size(); secondGammaIndex++) {
          AliAODConversionPhoton* gamma1 = dynamic_cast<AliAODConversionPhoton*>(fClusterCandidates[secondGammaIndex]);
          if (gamma1 == NULL)
            continue;

          // match conv to clusters
          if (gamma1->GetIsCaloPhoton() > 0) {
            AliVCluster* cluster = NULL;
            if (arrClustersMesonCand) {
              cluster = new AliAODCaloCluster(*(AliAODCaloCluster*)arrClustersMesonCand->At(gamma1->GetCaloClusterRef()));
            } else {
              cluster = fInputEvent->GetCaloCluster(gamma1->GetCaloClusterRef());
            }

            bool matched = ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->MatchConvPhotonToCluster(gamma0, cluster, fInputEvent, fWeightJetJetMC);

            if (!matched) {
              FillMesonHistograms(gamma0, gamma1, firstGammaIndex, secondGammaIndex);
            }
            if (arrClustersMesonCand && cluster)
              delete cluster;
          }
        }
      }
    }

    // Pure Calo case
  } else if (fIsCalo) {
    for (unsigned int firstGammaIndex = 0; firstGammaIndex < fClusterCandidates.size(); firstGammaIndex++) {
      AliAODConversionPhoton* gamma0 = dynamic_cast<AliAODConversionPhoton*>(fClusterCandidates[firstGammaIndex]);
      if (gamma0 == NULL)
        continue;

      for (unsigned int secondGammaIndex = firstGammaIndex + 1; secondGammaIndex < fClusterCandidates.size(); secondGammaIndex++) {
        AliAODConversionPhoton* gamma1 = dynamic_cast<AliAODConversionPhoton*>(fClusterCandidates[secondGammaIndex]);
        if (gamma1 == NULL)
          continue;

        FillMesonHistograms(gamma0, gamma1, firstGammaIndex, secondGammaIndex);
      }
    }

    // Pure Conv case
  } else if (fIsConv) {
    for (unsigned int firstGammaIndex = 0; firstGammaIndex < fGammaCandidates.size(); firstGammaIndex++) {
      AliAODConversionPhoton* gamma0 = dynamic_cast<AliAODConversionPhoton*>(fGammaCandidates[firstGammaIndex]);
      if (gamma0 == NULL)
        continue;

      for (unsigned int secondGammaIndex = firstGammaIndex + 1; secondGammaIndex < fGammaCandidates.size(); secondGammaIndex++) {
        AliAODConversionPhoton* gamma1 = dynamic_cast<AliAODConversionPhoton*>(fGammaCandidates[secondGammaIndex]);
        if (gamma1 == NULL)
          continue;

        FillMesonHistograms(gamma0, gamma1, firstGammaIndex, secondGammaIndex);
      }
    }
  }
}

void AliAnalysisTaskMesonJetCorrelation::FillMesonHistograms(AliAODConversionPhoton* gamma0, AliAODConversionPhoton* gamma1, int firstGammaIndex, int secondGammaIndex)
{
  AliAODConversionMother* pi0cand = new AliAODConversionMother(gamma0, gamma1);
  pi0cand->SetLabels(firstGammaIndex, secondGammaIndex);

  if ((((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(pi0cand, kTRUE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), gamma0->GetLeadingCellID(), gamma1->GetLeadingCellID(), gamma0->GetIsCaloPhoton(), gamma1->GetIsCaloPhoton()))) {

    if (fDoMesonQA > 0) {
      fHistoInvMassVsPt_Incl[fiCut]->Fill(pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);
    }
    double RJetPi0Cand = 0;
    int matchedJet = -1;
    float ptJet = 0;
    if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), pi0cand->Eta(), pi0cand->Phi(), matchedJet, RJetPi0Cand)) {
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->DoJetAnalysis()) {
        ptJet = fVectorJetPt[matchedJet];
      }

      // Fill Inv Mass Histograms
      fHistoInvMassVsPt[fiCut]->Fill(pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);

      // Fill the inv. mass histograms for all jet pTs
      fRespMatrixHandlerMesonInvMass[fiCut]->Fill(ptJet, 0.5, pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);

      // Fill Z histograms
      float z = GetFrag(pi0cand, matchedJet, false);

      fRespMatrixHandlerMesonInvMassVsZ[fiCut]->Fill(ptJet, 0.5, pi0cand->M(), z, fWeightJetJetMC);     // Inv Mass vs. Z in Jet Pt_rec bins. Needed to subtract background in the Z-distribution
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelectedByMassCut(pi0cand, 0)) { // nominal mass range
        fHistoJetPtVsFrag[fiCut]->Fill(z, ptJet, fWeightJetJetMC);
        if (fDoMesonQA > 0) {
          fHistoInvMassVsPtMassCut[fiCut]->Fill(pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);
        }
      }

      if (fIsMC > 0) {
        ProcessTrueMesonCandidatesAOD(pi0cand, gamma0, gamma1, matchedJet, RJetPi0Cand);
      }

    } // end IsParticleInJet
    // check if particle is in jet transverse to original jet.
    // Needed for underlying event study
    double RJetPi0CandPerp = 0;
    int matchedJetPerp = -1;
    if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhiPerp, fConvJetReader->Get_Jet_Radius(), pi0cand->Eta(), pi0cand->Phi(), matchedJetPerp, RJetPi0CandPerp)) {

      fHistoInvMassVsPtPerpCone[fiCut]->Fill(pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);

      // Fill the inv. mass histograms for all jet pTs
      fRespMatrixHandlerMesonInvMassPerpCone[fiCut]->Fill(ptJet, 0.5, pi0cand->M(), pi0cand->Pt(), fWeightJetJetMC);

      // Fill Z histograms
      float z = GetFrag(pi0cand, matchedJet, false);

      fRespMatrixHandlerMesonInvMassVsZPerpCone[fiCut]->Fill(ptJet, 0.5, pi0cand->M(), z, fWeightJetJetMC); // Inv Mass vs. Z in Jet Pt_rec bins. Needed to subtract background in the Z-distribution
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelectedByMassCut(pi0cand, 0)) {     // nominal mass range
        fHistoJetPtVsFragPerpCone[fiCut]->Fill(z, ptJet, fWeightJetJetMC);
      }

    } // end isInJet in perpendicular direction
    if (fIsMC) {
      // check if meson is in true jet
      double RJetPi0CandTrue = 0;
      int matchedJetTrue = -1;
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fTrueVectorJetEta, fTrueVectorJetPhi, fConvJetReader->Get_Jet_Radius(), pi0cand->Eta(), pi0cand->Phi(), matchedJetTrue, RJetPi0CandTrue)) {
        ProcessTrueMesonCandidatesInTrueJetsAOD(pi0cand, gamma0, gamma1, matchedJetTrue, RJetPi0CandTrue);
      }
    }
  } // end mesonIsSelected
  if (pi0cand)
    delete pi0cand;
}

//________________________________________________________________________
float AliAnalysisTaskMesonJetCorrelation::GetFrag(AliAODConversionMother* Pi0Candidate, const int matchedJet, int isTrueJet)
{
  if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->DoJetAnalysis()) {
    return 0;
  }
  if (matchedJet < 0) {
    return 0;
  }
  float z = 0;
  if (isTrueJet == 1) {
    z = Pi0Candidate->Pt() / fTrueVectorJetPt[matchedJet];
  } else if (isTrueJet == 2) {
    z = Pi0Candidate->Pt() / fTrueVectorJetPartonPt[matchedJet];
  } else {
    z = Pi0Candidate->Pt() / fVectorJetPt[matchedJet];
  }
  return z;
}

//________________________________________________________________________
float AliAnalysisTaskMesonJetCorrelation::GetFrag(AliAODMCParticle* Pi0Candidate, const int matchedJet, int isTrueJet)
{
  if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->DoJetAnalysis()) {
    return 0;
  }
  if (matchedJet < 0) {
    return 0;
  }
  float z = 0;
  if (isTrueJet == 1) {
    z = Pi0Candidate->Pt() / fTrueVectorJetPt[matchedJet];
  } else if (isTrueJet == 2) {
    z = Pi0Candidate->Pt() / fTrueVectorJetPartonPt[matchedJet];
  } else {
    z = Pi0Candidate->Pt() / fVectorJetPt[matchedJet];
  }
  return z;
}

//________________________________________________________________________
/// \brief function to steer the background calculation. Either mixed events or rotation is called
void AliAnalysisTaskMesonJetCorrelation::CalculateBackground()
{
  // cout << "CalculateBackground" << endl;
  if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->DoGammaSwappForBg()) {
    CalculateBackgroundSwapp();
  } else {
    CalculateBackgroundMix();
  }
}

//________________________________________________________________________
/// \brief Calculate Mixed event distribution. This depends on the mixing class EventMixPoolMesonJets. This new mixing technique shifts the events such that the axis of the leading Jet overlaps and as such, the mixing should be more realistic
void AliAnalysisTaskMesonJetCorrelation::CalculateBackgroundMix()
{
  // cout << "CalculateBackgroundMix" << endl;
  // Mix just within jets or shift event such that jet axes match
  if (fIsCalo) {
    for (unsigned int iCurrent = 0; iCurrent < fClusterCandidates.size(); iCurrent++) {
      AliAODConversionPhoton* currentEventGamma = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent));

      for (unsigned int iMix = 0; iMix < fEventMix->GetNBGEvents(fMaxPtJet); iMix++) {

        std::vector<std::shared_ptr<AliAODConversionPhoton>> vecGammaMix = fEventMix->getPhotonsRotated(iMix, fMaxPtJet, fHighestJetVector, true);
        for (const auto& gammaMix : vecGammaMix) {
          int cellID = ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetCaloCellIdFromEtaPhi(gammaMix->GetPhotonEta(), static_cast<double>((gammaMix->GetPhotonPhi() < 0) ? gammaMix->GetPhotonPhi() + TMath::Pi() * 2. : gammaMix->GetPhotonPhi()));
          if (!(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->CheckDistanceToBadChannelSwapping(cellID, fInputEvent, ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetDistanceToBorderForBg()))) {
            std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(gammaMix.get(), ((AliAODConversionPhoton*)currentEventGamma)));

            if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), cellID, ((AliAODConversionPhoton*)currentEventGamma)->GetLeadingCellID())) {
              // Fill histograms here
              FillInvMassBackHistograms(backgroundCandidate.get());
            }
          }
        }
      }
    }
  } else if (fIsConvCalo) { // for now take the rotated clusters
    for (unsigned int iCurrent = 0; iCurrent < fGammaCandidates.size(); iCurrent++) {
      AliAODConversionPhoton* currentEventGamma = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent));

      for (unsigned int iMix = 0; iMix < fEventMix->GetNBGEvents(fMaxPtJet); iMix++) {

        std::vector<std::shared_ptr<AliAODConversionPhoton>> vecGammaMix = fEventMix->getPhotonsRotated(iMix, fMaxPtJet, fHighestJetVector, true);
        for (const auto& gammaMix : vecGammaMix) {
          int cellID = ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetCaloCellIdFromEtaPhi(gammaMix->GetPhotonEta(), static_cast<double>((gammaMix->GetPhotonPhi() < 0) ? gammaMix->GetPhotonPhi() + TMath::Pi() * 2. : gammaMix->GetPhotonPhi()));
          if (!(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->CheckDistanceToBadChannelSwapping(cellID, fInputEvent, ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetDistanceToBorderForBg()))) {

            std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(gammaMix.get(), ((AliAODConversionPhoton*)currentEventGamma)));

            if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), cellID, -1)) {
              // Fill histograms here
              FillInvMassBackHistograms(backgroundCandidate.get());
            }
          }
        }
      }
    }
  } else if (fIsConv) {
    for (unsigned int iCurrent = 0; iCurrent < fGammaCandidates.size(); iCurrent++) {
      AliAODConversionPhoton* currentEventGamma = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent));

      for (unsigned int iMix = 0; iMix < fEventMix->GetNBGEvents(fMaxPtJet); iMix++) {

        std::vector<std::shared_ptr<AliAODConversionPhoton>> vecGammaMix = fEventMix->getPhotonsRotated(iMix, fMaxPtJet, fHighestJetVector, false);
        for (const auto& gammaMix : vecGammaMix) {

          if (fabs(gammaMix->Eta()) > ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetEtaCut()) {
            std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(gammaMix.get(), ((AliAODConversionPhoton*)currentEventGamma)));

            if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), -1, -1)) {
              // Fill histograms here
              FillInvMassBackHistograms(backgroundCandidate.get());
            }
          }
        }
      }
    }
  }
}

//________________________________________________________________________
/// \brief Calculate the rotation background and fill background histograms (Inv mass vs. pT) (Details of steering implemented in AliConversionMesonCuts)
void AliAnalysisTaskMesonJetCorrelation::CalculateBackgroundSwapp()
{
  // cout << "CalculateBackgroundSwapp" << endl;

  std::vector<std::array<Double_t, 2>> vSwappingInvMassPT;
  std::vector<std::array<Double_t, 2>> vSwappingInvMassPTAlphaCut;
  vSwappingInvMassPT.clear();
  vSwappingInvMassPTAlphaCut.clear();
  vSwappingInvMassPT.resize(0);
  vSwappingInvMassPTAlphaCut.resize(0);

  // curcial requirement is that the event has at least 3 cluster candidates
  if (fIsCalo) {
    // needs at least three clusters
    if (fClusterCandidates.size() <= 2)
      return;
    for (unsigned int iCurrent1 = 0; iCurrent1 < fClusterCandidates.size(); iCurrent1++) {

      AliAODConversionPhoton* currentEventGoodV0Temp1 = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent1));

      for (unsigned int iCurrent2 = iCurrent1 + 1; iCurrent2 < fClusterCandidates.size(); iCurrent2++) {
        AliAODConversionPhoton* currentEventGoodV0Temp2 = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent2));

        std::array<std::unique_ptr<AliAODConversionPhoton>, 2> swappedGammas = GetGammasSwapped(currentEventGoodV0Temp1, currentEventGoodV0Temp2);
        if (!swappedGammas[0].get())
          continue;

        std::array<int, 2> cellIDRotatedPhoton{
          ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetCaloCellIdFromEtaPhi(swappedGammas[0]->GetPhotonEta(), static_cast<double>((swappedGammas[0]->GetPhotonPhi() < 0) ? swappedGammas[0]->GetPhotonPhi() + TMath::Pi() * 2. : swappedGammas[0]->GetPhotonPhi())),
          ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetCaloCellIdFromEtaPhi(swappedGammas[1]->GetPhotonEta(), static_cast<double>((swappedGammas[1]->GetPhotonPhi() < 0) ? swappedGammas[1]->GetPhotonPhi() + TMath::Pi() * 2. : swappedGammas[1]->GetPhotonPhi()))};

        for (unsigned int iCurrent3 = 0; iCurrent3 < fClusterCandidates.size(); iCurrent3++) {
          AliAODConversionPhoton* currentEventGoodV0Temp3 = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent3));

          for (unsigned int iSwapped = 0; iSwapped < swappedGammas.size(); ++iSwapped) {
            if (iCurrent3 == iCurrent1 || iCurrent3 == iCurrent2) {
              continue;
            }
            std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(swappedGammas[iSwapped].get(), ((AliAODConversionPhoton*)currentEventGoodV0Temp3)));
            if (!(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->CheckDistanceToBadChannelSwapping(cellIDRotatedPhoton[iSwapped], fInputEvent, ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetDistanceToBorderForBg())) && swappedGammas[iSwapped]->P() > ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetMinClusterEnergy()) {
              if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), cellIDRotatedPhoton[iSwapped], ((AliAODConversionPhoton*)currentEventGoodV0Temp3)->GetLeadingCellID())) {
                // Fill histograms here
                FillInvMassBackHistograms(backgroundCandidate.get());
              }
            }
          }
        }
      }
    }
  } else if (fIsConvCalo) {
    // gamma1 is cluster, gamma2 is V0
    for (unsigned int iCurrent1 = 0; iCurrent1 < fClusterCandidates.size(); iCurrent1++) {
      AliAODConversionPhoton* currentEventGoodV0Temp1 = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent1));

      for (unsigned int iCurrent2 = iCurrent1 + 1; iCurrent2 < fGammaCandidates.size(); iCurrent2++) {
        AliAODConversionPhoton* currentEventGoodV0Temp2 = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent2));

        std::array<std::unique_ptr<AliAODConversionPhoton>, 2> swappedGammas = GetGammasSwapped(currentEventGoodV0Temp1, currentEventGoodV0Temp2);
        if (!swappedGammas[0].get())
          continue;

        int cellIDRotatedPhoton = ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetCaloCellIdFromEtaPhi(swappedGammas[0]->GetPhotonEta(), static_cast<double>((swappedGammas[0]->GetPhotonPhi() < 0) ? swappedGammas[0]->GetPhotonPhi() + TMath::Pi() * 2. : swappedGammas[0]->GetPhotonPhi()));

        for (unsigned int iCurrent3 = 0; iCurrent3 < fClusterCandidates.size(); iCurrent3++) {
          AliAODConversionPhoton* currentEventGoodV0Temp3 = (AliAODConversionPhoton*)(fClusterCandidates.at(iCurrent3));

          for (unsigned int iSwapped = 0; iSwapped < swappedGammas.size(); ++iSwapped) {
            if (iCurrent3 == iCurrent1) {
              continue;
            }

            if (swappedGammas[1]->Eta() < ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetEtaCut()) {
              std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(swappedGammas[1].get(), ((AliAODConversionPhoton*)currentEventGoodV0Temp3)));

              if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), -1, ((AliAODConversionPhoton*)currentEventGoodV0Temp3)->GetLeadingCellID())) {
                // Fill histograms here
                FillInvMassBackHistograms(backgroundCandidate.get());
              }
            }
          }
          for (unsigned int iCurrent3 = 0; iCurrent3 < fGammaCandidates.size(); iCurrent3++) {
            AliAODConversionPhoton* currentEventGoodV0Temp3 = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent3));

            for (unsigned int iSwapped = 0; iSwapped < swappedGammas.size(); ++iSwapped) {
              if (iCurrent3 == iCurrent2) {
                continue;
              }

              std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(swappedGammas[0].get(), ((AliAODConversionPhoton*)currentEventGoodV0Temp3)));

              if (!(((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->CheckDistanceToBadChannelSwapping(cellIDRotatedPhoton, fInputEvent, ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetDistanceToBorderForBg())) && swappedGammas[0]->P() > ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->GetMinClusterEnergy()) {
                if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), cellIDRotatedPhoton, -1)) {
                  // Fill histograms here
                  FillInvMassBackHistograms(backgroundCandidate.get());
                }
              }
            }
          }
        }
      }
    }
  } else if (fIsConv) {
    // needs at least three photons
    if (fGammaCandidates.size() <= 2)
      return;
    for (unsigned int iCurrent1 = 0; iCurrent1 < fGammaCandidates.size(); iCurrent1++) {
      AliAODConversionPhoton* currentEventGoodV0Temp1 = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent1));

      for (unsigned int iCurrent2 = iCurrent1 + 1; iCurrent2 < fGammaCandidates.size(); iCurrent2++) {
        AliAODConversionPhoton* currentEventGoodV0Temp2 = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent2));

        std::array<std::unique_ptr<AliAODConversionPhoton>, 2> swappedGammas = GetGammasSwapped(currentEventGoodV0Temp1, currentEventGoodV0Temp2);

        if (!swappedGammas[0].get())
          continue;

        for (unsigned int iCurrent3 = 0; iCurrent3 < fGammaCandidates.size(); iCurrent3++) {
          AliAODConversionPhoton* currentEventGoodV0Temp3 = (AliAODConversionPhoton*)(fGammaCandidates.at(iCurrent3));

          for (unsigned int iSwapped = 0; iSwapped < swappedGammas.size(); ++iSwapped) {
            if (iCurrent3 == iCurrent1 || iCurrent3 == iCurrent2) {
              continue;
            }

            std::unique_ptr<AliAODConversionMother> backgroundCandidate(std::make_unique<AliAODConversionMother>(swappedGammas[iSwapped].get(), ((AliAODConversionPhoton*)currentEventGoodV0Temp3)));
            if (swappedGammas[iSwapped]->Eta() < ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetEtaCut()) {
              if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelected(backgroundCandidate.get(), kFALSE, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift(), -1, -1)) {
                // Fill histograms here
                FillInvMassBackHistograms(backgroundCandidate.get());
              }
            }
          }
        }
      }
    }
  }
}

//__________________________________________________________________________
/// \brief function to fill all histograms related to the meson background (estimated with rotation, mixed evt etc.)
/// \param backgroundCandidate particle reconstructed from two photons containing the mass, pT etc.
void AliAnalysisTaskMesonJetCorrelation::FillInvMassBackHistograms(AliAODConversionMother* backgroundCandidate)
{

  int matchedJet = -1;
  double RJetPi0Cand = 0;
  if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), backgroundCandidate->Eta(), backgroundCandidate->Phi(), matchedJet, RJetPi0Cand)) {
    return;
  }

  //_______ Standard Inv Mass vs. pT background histo _______________
  fHistoInvMassVsPtBack[fiCut]->Fill(backgroundCandidate->M(), backgroundCandidate->Pt(), fWeightJetJetMC);

  //_______ Fill Inv Mass vs. Z for purity correction _______________
  float ptJet = (matchedJet < 0) ? 0 : fVectorJetPt[matchedJet];
  float z = GetFrag(backgroundCandidate, matchedJet, false);
  fRespMatrixHandlerMesonBackInvMassVsPt[fiCut]->Fill(ptJet, 0.5, backgroundCandidate->M(), backgroundCandidate->Pt(), fWeightJetJetMC); // Inv Mass vs. meson Pt in Jet Pt_rec bins. Needed to subtract background in the Pt-distribution
  fRespMatrixHandlerMesonBackInvMassVsZ[fiCut]->Fill(ptJet, 0.5, backgroundCandidate->M(), z, fWeightJetJetMC);                          // Inv Mass vs. Z in Jet Pt_rec bins. Needed to subtract background in the Z-distribution
}

//__________________________________________________________________________
/// \brief Function to rotate two photons around their combined 4-momentum axis. Main function for the rotation background. Details of the swapping are steered via AliConversionMesonCuts
/// \param currentEventGoodV0Temp1 original photon 1 (not rotated)
/// \param currentEventGoodV0Temp2 original photon 2 (not rotated)
std::array<std::unique_ptr<AliAODConversionPhoton>, 2> AliAnalysisTaskMesonJetCorrelation::GetGammasSwapped(AliAODConversionPhoton* currentEventGoodV0Temp1, AliAODConversionPhoton* currentEventGoodV0Temp2)
{

  Double_t rotationAngle = TMath::Pi() / 2.0; // 0.78539816339; // rotaion angle 90°

  // Needed for TGenPhaseSpace
  TVector3 tvEtaPhigamma1, tvEtaPhigamma2, tvEtaPhigamma1Decay, tvEtaPhigamma2Decay, tvNormBeforeDecay, tvNormAfterDecay;
  Float_t asymBeforeDecay = 0.;
  Float_t asymAfterDecay = 0.;
  Double_t massGamma[2] = {0, 0};

  TLorentzVector lvRotationPhoton1; // photon candidates which get rotated
  TLorentzVector lvRotationPhoton2; // photon candidates which get rotated
  TVector3 lvRotationPion;          // reconstructed mother particle from the two photons

  lvRotationPhoton1.SetX(currentEventGoodV0Temp1->Px());
  lvRotationPhoton1.SetY(currentEventGoodV0Temp1->Py());
  lvRotationPhoton1.SetZ(currentEventGoodV0Temp1->Pz());
  lvRotationPhoton1.SetE(currentEventGoodV0Temp1->E());

  lvRotationPhoton2.SetX(currentEventGoodV0Temp2->Px());
  lvRotationPhoton2.SetY(currentEventGoodV0Temp2->Py());
  lvRotationPhoton2.SetZ(currentEventGoodV0Temp2->Pz());
  lvRotationPhoton2.SetE(currentEventGoodV0Temp2->E());

  lvRotationPion = (lvRotationPhoton1 + lvRotationPhoton2).Vect();

  // rotate both photons around the momentum vector of their hypothetical mother particle
  if ((((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 0 || ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 1)) {
    if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 0)
      rotationAngle = TMath::Pi() / 2.0;                                                        // rotate by 90 degree
    else if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 1) { // rotate by random angle between
      Double_t temp = (fRandom.Rndm() < 0.5) ? 0 : TMath::Pi();
      rotationAngle = temp + TMath::Pi() / 3.0 + fRandom.Rndm() * TMath::Pi() / 3.0;
    }
    lvRotationPhoton1.Rotate(rotationAngle, lvRotationPion);
    lvRotationPhoton2.Rotate(rotationAngle, lvRotationPion);
  } else if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() >= 10) { // generate new decay with TGenPhaseSpace
    if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 11) {
      tvEtaPhigamma1 = lvRotationPhoton1.Vect();
      tvEtaPhigamma2 = lvRotationPhoton2.Vect();
      tvNormBeforeDecay = tvEtaPhigamma1.Cross(tvEtaPhigamma2);
      asymBeforeDecay = fabs((lvRotationPhoton1.E() - lvRotationPhoton2.E()) / (lvRotationPhoton1.E() + lvRotationPhoton2.E()));
    }

    TLorentzVector lvRotationMother = lvRotationPhoton1 + lvRotationPhoton2;
    fGenPhaseSpace.SetDecay(lvRotationMother, 2, massGamma);
    fGenPhaseSpace.Generate();
    lvRotationPhoton1 = *fGenPhaseSpace.GetDecay(0);
    lvRotationPhoton2 = *fGenPhaseSpace.GetDecay(1);

    if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GammaSwappMethodBg() == 11) {
      tvEtaPhigamma1Decay = lvRotationPhoton1.Vect();
      tvEtaPhigamma2Decay = lvRotationPhoton2.Vect();
      tvNormAfterDecay = tvEtaPhigamma1Decay.Cross(tvEtaPhigamma2Decay); // norm vector to decay plane
      asymAfterDecay = fabs((lvRotationPhoton1.E() - lvRotationPhoton2.E()) / (lvRotationPhoton1.E() + lvRotationPhoton2.E()));
      // check if decay is nearly the same as original decay: if yes continue with next decay
      if ((tvNormAfterDecay.Angle(tvNormBeforeDecay) < 20 * TMath::Pi() / 180. || tvNormAfterDecay.Angle(tvNormBeforeDecay) > 340 * TMath::Pi() / 180.) && (fabs(asymBeforeDecay - asymAfterDecay) < 0.05)) {
        std::array<std::unique_ptr<AliAODConversionPhoton>, 2> gammasNULL{nullptr, nullptr};
        return gammasNULL;
      }
    }
  }
  std::array<std::unique_ptr<AliAODConversionPhoton>, 2> gammas{std::make_unique<AliAODConversionPhoton>(&lvRotationPhoton1), std::make_unique<AliAODConversionPhoton>(&lvRotationPhoton2)};
  return gammas;
}

bool AliAnalysisTaskMesonJetCorrelation::MCParticleIsSelected(AliAODMCParticle* particle1, AliAODMCParticle* particle2, bool checkConversion)
{
  if (fIsCalo) {
    if (MCParticleIsSelected(particle1, false, false) && MCParticleIsSelected(particle2, false, false)) {
      return true;
    }
  } else if (fIsConv) {
    if (MCParticleIsSelected(particle1, true, checkConversion) && MCParticleIsSelected(particle2, true, checkConversion)) {
      return true;
    }
  } else if (fIsConvCalo) {
    if (MCParticleIsSelected(particle1, false, false) && MCParticleIsSelected(particle2, true, checkConversion)) {
      return true;
    } else if (MCParticleIsSelected(particle2, false, false) && MCParticleIsSelected(particle1, true, checkConversion)) {
      return true;
    }
  }
  return false;
}

bool AliAnalysisTaskMesonJetCorrelation::MCParticleIsSelected(AliAODMCParticle* particle, bool isConv, bool checkConversion)
{
  if (isConv) {
    return ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->PhotonIsSelectedAODMC(particle, fAODMCTrackArray, checkConversion);
  } else {
    return ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelectedAODMC(particle, fAODMCTrackArray);
  }
}

// bool AliAnalysisTaskMesonJetCorrelation::IsParticleFromPartonFrag(AliAODMCParticle* particle, int idParton){
//   if (((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 111 && ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 221) {
//         tmpGammaMotherlabel = ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetMother();
//       } else
// }

// ToDo: Fill histograms for jet stuff
//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessAODMCParticles(int isCurrentEventSelected)
{

  const AliVVertex* primVtxMC = fMCEvent->GetPrimaryVertex();
  Double_t mcProdVtxX = primVtxMC->GetX();
  Double_t mcProdVtxY = primVtxMC->GetY();
  Double_t mcProdVtxZ = primVtxMC->GetZ();

  if (!fAODMCTrackArray)
    fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
  if (fAODMCTrackArray == NULL)
    return;

  // Check if MC generated particles should be filled for this event using the selected trigger
  if (!((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsMCTriggerSelected(fInputEvent, fMCEvent)) {
    return;
  }

  // Loop over all primary MC particle
  for (Long_t i = 0; i < fAODMCTrackArray->GetEntriesFast(); i++) {

    AliAODMCParticle* particle = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(i));
    if (!particle)
      continue;

    int matchedJet = -1;
    double RJetPi0Cand = 0;
    if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fTrueVectorJetEta, fTrueVectorJetPhi, fConvJetReader->Get_Jet_Radius(), particle->Eta(), particle->Phi(), matchedJet, RJetPi0Cand)) {
      matchedJet = -1;
    }
    int matchedRecJet = -1;
    double RRecJetPi0Cand = 0;
    if (!((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->IsParticleInJet(fVectorJetEta, fVectorJetPhi, fConvJetReader->Get_Jet_Radius(), particle->Eta(), particle->Phi(), matchedRecJet, RRecJetPi0Cand)) {
      matchedRecJet = -1;
    }

    if (matchedJet < 0 && matchedRecJet < 0) {
      continue;
    }

    Bool_t isPrimary = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsConversionPrimaryAOD(fInputEvent, particle, mcProdVtxX, mcProdVtxY, mcProdVtxZ);
    if (isPrimary) {

      int isMCFromMBHeader = -1;
      if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 0) {
        isMCFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(i, fMCEvent, fInputEvent, true);
        if (isMCFromMBHeader == 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 3)
          continue;
      }
      // if(!((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->InPlaneOutOfPlaneCut(particle->Phi(),fEventPlaneAngle,kFALSE)) continue;

      // check photons
      if (matchedJet >= 0 && MCParticleIsSelected(particle, fIsConv, false)) { // here we state that this is a conversion, however this also works for calo photons on generator level!
        if (isCurrentEventSelected == 1) {
          fHistoMCGammaPtNotTriggered[fiCut]->Fill(particle->Pt(), fWeightJetJetMC); // All MC Gamma
        } else if (isCurrentEventSelected == 2) {
          fHistoMCGammaPtNoVertex[fiCut]->Fill(particle->Pt(), fWeightJetJetMC); // All MC Gamma
        }
        fHistoMCAllGammaPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC); // All MC Gamma

        if (!fDoLightOutput) {
          if (particle->GetMother() > -1) { // Meson Decay Gamma
            switch ((static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetMother())))->GetPdgCode()) {
              case 111: // Pi0
                fHistoMCDecayGammaPi0Pt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 113: // Rho0
                fHistoMCDecayGammaRhoPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 221: // Eta
                fHistoMCDecayGammaEtaPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 223: // Omega
                fHistoMCDecayGammaOmegaPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 331: // Eta'
                fHistoMCDecayGammaEtapPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 333: // Phi
                fHistoMCDecayGammaPhiPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
              case 3212: // Sigma
                fHistoMCDecayGammaSigmaPt[fiCut]->Fill(particle->Pt(), fWeightJetJetMC);
                break;
            }
          }
        }
      } // end if photons
      // check for photon conversions
      // if(((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->PhotonIsSelectedAODMC(particle,fAODMCTrackArray,kTRUE)){
      // for(int daughterIndex=particle->GetDaughterLabel(0);daughterIndex<=particle->GetDaughterLabel(1);daughterIndex++){
      //   AliAODMCParticle *tmpDaughter = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(daughterIndex));
      //   if(!tmpDaughter) continue;
      // }
      // if(!fDoLightOutput) fHistoMCConvGammaPt[fiCut]->Fill(particle->Pt(),fWeightJetJetMC);
      // }

      Double_t mesonY = 1.e30;
      Double_t ratio = 0;
      if (particle->E() != TMath::Abs(particle->Pz())) {
        ratio = (particle->E() + particle->Pz()) / (particle->E() - particle->Pz());
      }
      if (!(ratio <= 0)) {
        mesonY = particle->Y() - ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift();
      }

      if (!fDoLightOutput && matchedJet >= 0) {
        if ((mesonY > ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetRapidityCutValueMin()) && (mesonY < ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetRapidityCutValueMax())) {
          if (particle->GetPdgCode() == 211) { // positve pions
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 0., fWeightJetJetMC);
          } else if (particle->GetPdgCode() == -211) { // negative pions
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 1., fWeightJetJetMC);
          } else if (particle->GetPdgCode() == 321) { // positve kaons
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 2., fWeightJetJetMC);
          } else if (particle->GetPdgCode() == -321) { // negative kaons
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 3., fWeightJetJetMC);
          } else if (TMath::Abs(particle->GetPdgCode()) == 310) { // K0s
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 4., fWeightJetJetMC);
          } else if (TMath::Abs(particle->GetPdgCode()) == 130) { // K0l
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 5., fWeightJetJetMC);
          } else if (TMath::Abs(particle->GetPdgCode()) == 3122) { // Lambda/ AntiLambda
            fHistoMCPrimaryPtvsSource[fiCut]->Fill(particle->Pt(), 6., fWeightJetJetMC);
          }
        }
      }
      // check neutral mesons
      if (particle->GetPdgCode() == fMesonPDGCode) {

        if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))
              ->MesonIsSelectedAODMC(particle, fAODMCTrackArray, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift())) {

          AliAODMCParticle* daughter0 = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetDaughterLabel(0)));
          AliAODMCParticle* daughter1 = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetDaughterLabel(1)));
          Float_t weighted = 1;
          if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(i, fMCEvent, fInputEvent)) {
            if (particle->Pt() > 0.005) {
              weighted = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetWeightForMeson(i, 0x0, fInputEvent);
            }
          }

          // Double_t mesonY = 1.e30;
          // Double_t ratio = 0;
          if (particle->E() != TMath::Abs(particle->Pz())) {
            ratio = (particle->E() + particle->Pz()) / (particle->E() - particle->Pz());
          }
          // if (!(ratio <= 0)) {
          //   mesonY = particle->Y() - ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift();
          // }

          if (matchedJet >= 0) {
            if (isCurrentEventSelected == 1)
              fHistoMCMesonPtNotTriggered[fiCut]->Fill(particle->Pt(), weighted * fWeightJetJetMC); // All MC Pi0 in not triggered collisions
            else if (isCurrentEventSelected == 2)
              fHistoMCMesonPtNoVertex[fiCut]->Fill(particle->Pt(), weighted * fWeightJetJetMC); // All MC Pi0 in not triggered collisions
            fHistoMCMesonPt[fiCut]->Fill(particle->Pt(), weighted * fWeightJetJetMC);
            if (fIsMC > 1)
              fHistoMCMesonWOEvtWeightPt[fiCut]->Fill(particle->Pt());
          }

          //------------------------
          // Fill histograms for meson fragmentation
          //------------------------
          float z_jet = -1;
          float z_parton = -1;
          if (matchedJet >= 0) {
            z_jet = GetFrag(particle, matchedJet, 1);
            z_parton = GetFrag(particle, matchedJet, 2);

            fHistoMCJetPtVsFrag[fiCut]->Fill(z_jet, fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
            fHistoMCJetPtVsMesonPt[fiCut]->Fill(particle->Pt(), fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
            fHistoMCPartonPtVsFrag[fiCut]->Fill(z_parton, fTrueVectorJetPartonPt[matchedJet], weighted * fWeightJetJetMC);

            if (IsParticleFromPartonFrag(particle, fTrueVectorJetPartonID[matchedJet])) {
              fHistoMCJetPtVsFragTrueParton[fiCut]->Fill(z_jet, fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
              fHistoMCPartonPtVsFragTrueParton[fiCut]->Fill(z_parton, fTrueVectorJetPartonPt[matchedJet], weighted * fWeightJetJetMC);
            }
          }
          // inside rec. jet but fill true jet quantities
          if (matchedRecJet >= 0 && matchedJet >= 0) {
            fHistoMCRecJetPtVsFrag[fiCut]->Fill(z_jet, fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
            fHistoMCRecJetPtVsMesonPt[fiCut]->Fill(particle->Pt(), fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
          }
          // Check the acceptance for both gammas
          if (MCParticleIsSelected(daughter0, daughter1, false)) {
            if (CheckAcceptance(daughter0, daughter1)) {
              if (matchedJet >= 0) {
                fHistoMCMesonInAccPt[fiCut]->Fill(particle->Pt(), weighted * fWeightJetJetMC); // MC Pi0 with gamma in acc
                fHistoMCJetPtVsFragInAcc[fiCut]->Fill(z_jet, fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
                fHistoMCJetPtVsMesonPtInAcc[fiCut]->Fill(particle->Pt(), fTrueVectorJetPt[matchedJet], weighted * fWeightJetJetMC);
                if (isCurrentEventSelected == 1)
                  fHistoMCMesonInAccPtNotTriggered[fiCut]->Fill(particle->Pt(), weighted * fWeightJetJetMC); // MC Pi0 with gamma in acc for not triggered events
                if (fIsMC > 1)
                  fHistoMCMesonWOEvtWeightInAccPt[fiCut]->Fill(particle->Pt()); // MC Pi0 with gamma in acc wo any weight
              }
            }
          }
        }
      }
    } else {
      // fill secondary histograms
      int isMCFromMBHeader = -1;
      if (((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 0) {
        isMCFromMBHeader = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(i, fMCEvent, fInputEvent);
        if (isMCFromMBHeader == 0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() != 3)
          continue;
      }
      if (((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->MesonIsSelectedAODMC(particle, fAODMCTrackArray, ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetEtaShift())) {
        AliAODMCParticle* daughter0 = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetDaughterLabel(0)));
        AliAODMCParticle* daughter1 = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetDaughterLabel(1)));
        AliAODMCParticle* mother = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(particle->GetMother()));
        int pdgCode = mother->GetPdgCode();
        int source = -1;
        if (particle->GetPdgCode() == fMesonPDGCode) {
          source = ((AliConversionMesonCuts*)fMesonCutArray->At(fiCut))->GetSourceClassification(fMesonPDGCode, pdgCode);
          fHistoMCSecMesonPtvsSource[fiCut]->Fill(particle->Pt(), source, fWeightJetJetMC); // All MC Pi0
          fHistoMCSecMesonSource[fiCut]->Fill(pdgCode, fWeightJetJetMC);

          //------------------------
          // Fill histograms for meson fragmentation
          //------------------------
          if (matchedJet >= 0) {
            float z_jet = GetFrag(particle, matchedJet, 1);
            fHistoMCJetPtVsFrag_Sec[fiCut]->Fill(z_jet, fTrueVectorJetPt[matchedJet], fWeightJetJetMC);
            fHistoMCJetPtVsMesonPt_Sec[fiCut]->Fill(particle->Pt(), fTrueVectorJetPt[matchedJet], fWeightJetJetMC);
          }

          if (MCParticleIsSelected(daughter0, daughter1, false)) {
            if (CheckAcceptance(daughter0, daughter1)) {
              fHistoMCSecMesonInAccPtvsSource[fiCut]->Fill(particle->Pt(), source, fWeightJetJetMC); // All MC Pi0
            }
          }
        }
      }
    }
  }
}

//__________________________________________________________________________________________________________
bool AliAnalysisTaskMesonJetCorrelation::IsParticleFromPartonFrag(AliAODMCParticle* particle, int idParton)
{
  int motherLabel = particle->GetMother();

  for (int i = 0; i < 50; ++i) {
    if (motherLabel == idParton) {
      return true;
    }
    auto tmpPart = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(motherLabel));
    if (!tmpPart) {
      return false;
    }
    if (tmpPart->GetPdgCode() == 90) {
      return false;
    }
    motherLabel = tmpPart->GetMother();
    if (motherLabel < 0) {
      return false;
    }
  }
  return false;
}

//__________________________________________________________________________________________________________
bool AliAnalysisTaskMesonJetCorrelation::CheckAcceptance(AliAODMCParticle* gamma0, AliAODMCParticle* gamma1)
{
  if (fIsCalo) {
    if (((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelectedAODMC(gamma0, fAODMCTrackArray) &&
        ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelectedAODMC(gamma1, fAODMCTrackArray)) {
      return true;
    } else {
      return false;
    }
  } else if (fIsConvCalo) {
    if (((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelectedAODMC(gamma0, fAODMCTrackArray) ||
        ((AliCaloPhotonCuts*)fClusterCutArray->At(fiCut))->ClusterIsSelectedAODMC(gamma1, fAODMCTrackArray)) {
      return true;
    } else {
      return false;
    }
  } else if (fIsConv) {
    return true;
  }
  return false;
}

//________________________________________________________________________
int AliAnalysisTaskMesonJetCorrelation::GetPhotonMotherLabel(AliAODConversionPhoton* gammaCand, int& convertedPhotonLabel, bool isCaloPhoton)
{

  int gammaMotherLabel = -1;

  if (isCaloPhoton) {
    int gammaMCLabel = gammaCand->GetCaloPhotonMCLabel(0); // get most probable MC label
    int tmpGammaMotherlabel = -1;
    convertedPhotonLabel = -1; // default is -1

    // check if gamma0 originates from pi0 decay
    AliAODMCParticle* gammaMC = 0x0;
    if (gammaMCLabel != -1) { // Gamma is Combinatorial; MC Particles don't belong to the same Mother
      // Daughters Gamma 0
      gammaMC = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gammaMCLabel));

      if (gammaCand->IsLargestComponentPhoton() || gammaCand->IsLargestComponentElectron()) { // largest component is electro magnetic

        tmpGammaMotherlabel = gammaMC->GetMother();
        // get mother of interest (pi0 or eta)
        if (gammaCand->IsLargestComponentPhoton()) { // for photons its the direct mother
          gammaMotherLabel = gammaMC->GetMother();
        } else if (gammaCand->IsLargestComponentElectron()) { // for electrons its either the direct mother or for conversions the grandmother
          if (gammaCand->IsConversion()) {
            convertedPhotonLabel = gammaMC->GetMother();
            AliAODMCParticle* gammaGrandMotherMC = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gammaMC->GetMother()));
            gammaMotherLabel = gammaGrandMotherMC->GetMother();
          } else
            gammaMotherLabel = gammaMC->GetMother();
        }
      }
    }

    int SaftyLoopCounter = 0;
    while (tmpGammaMotherlabel > 0 && SaftyLoopCounter < 100) {
      SaftyLoopCounter++;
      if (((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 111 && ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 221) {
        tmpGammaMotherlabel = ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetMother();
      } else {
        gammaMotherLabel = tmpGammaMotherlabel;
        break;
      }
    }
  } else {
    AliAODMCParticle* positiveMC = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gammaCand->GetMCLabelPositive()));
    AliAODMCParticle* negativeMC = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gammaCand->GetMCLabelNegative()));

    Int_t gamma0MCLabel = -1;
    if (!positiveMC || !negativeMC) {
      return -1;
    }

    if (gammaCand->IsTrueConvertedPhoton()) {
      gamma0MCLabel = positiveMC->GetMother();
      AliAODMCParticle* gammaMC0 = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gamma0MCLabel));
      gammaMotherLabel = gammaMC0->GetMother();
    }

    Int_t tmpGammaMotherlabel = gammaMotherLabel;
    Int_t SaftyLoopCounter = 0;
    while (tmpGammaMotherlabel > 0 && SaftyLoopCounter < 100) {
      SaftyLoopCounter++;
      if (((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 111 && ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetPdgCode() != 221) {
        tmpGammaMotherlabel = ((AliAODMCParticle*)fAODMCTrackArray->At(tmpGammaMotherlabel))->GetMother();
      } else {
        gammaMotherLabel = tmpGammaMotherlabel;
        break;
      }
    }
  }
  return gammaMotherLabel;
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessTrueClusterCandidatesAOD(AliAODConversionPhoton* TruePhotonCandidate)
{

  Double_t tempPhotonWeight = fWeightJetJetMC;
  AliAODMCParticle* Photon = NULL;
  if (!fAODMCTrackArray)
    fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
  if (fAODMCTrackArray) {
    if (TruePhotonCandidate->GetIsCaloPhoton() == 0)
      AliFatal("CaloPhotonFlag has not been set task will abort");
    if (TruePhotonCandidate->GetNCaloPhotonMCLabels() > 0)
      Photon = (AliAODMCParticle*)fAODMCTrackArray->At(TruePhotonCandidate->GetCaloPhotonMCLabel(0));
    else
      return;
  } else {
    AliInfo("AODMCTrackArray could not be loaded");
    return;
  }

  if (Photon == NULL) {
    return;
  }
  // int pdgCodeParticle = Photon->GetPdgCode();
  TruePhotonCandidate->SetCaloPhotonMCFlagsAOD(fAODMCTrackArray, fEnableSortForClusMC);

  // Set the jetjet weight to 1 in case the cluster orignated from the minimum bias header
  // if (fIsMC>0 && ((AliConvEventCuts*)fEventCutArray->At(fiCut))->GetSignalRejection() == 4){
  //   if ( ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsParticleFromBGEvent(TruePhotonCandidate->GetCaloPhotonMCLabel(0), fMCEvent, fInputEvent) == 2) tempPhotonWeight = 1;
  // }

  // True Photon
  if (TruePhotonCandidate->IsLargestComponentPhoton() || (TruePhotonCandidate->IsLargestComponentElectron() && TruePhotonCandidate->IsConversion())) {
    fHistoTrueClusGammaPt[fiCut]->Fill(TruePhotonCandidate->Pt(), tempPhotonWeight);
  }
}

/// \brief check if particle is true meson
/// \param Pi0Candidate meson candidate to be checked
/// \param TrueGammaCandidate0 cluster in case of calo and convCalo, conversion photon in case of conv
/// \param TrueGammaCandidate1 cluster in case of calo, conversion photon in case of conv-calo and conv
/// \param matchedJet index of matched Jet
/// \param RJetPi0Cand distance of Jet axis to meson
//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessTrueMesonCandidatesAOD(AliAODConversionMother* Pi0Candidate, AliAODConversionPhoton* TrueGammaCandidate0, AliAODConversionPhoton* TrueGammaCandidate1, const int matchedJet, const float RJetPi0Cand)
{
  const AliVVertex* primVtxMC = fMCEvent->GetPrimaryVertex();
  double mcProdVtxX = primVtxMC->GetX();
  double mcProdVtxY = primVtxMC->GetY();
  double mcProdVtxZ = primVtxMC->GetZ();

  // Process True Mesons
  if (!fAODMCTrackArray)
    fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
  if (fAODMCTrackArray == NULL)
    return;

  bool isTrueParticle = false;
  bool isTrueOtherParticle = false; // this is true if the particle we are looking for is a pi0 but the particle we find is an eta etc. This is needed as the mesons overlapp in the inv. mass histograms

  // check if meson originates from the same meson
  int convertedPhotonLabel0, convertedPhotonLabel1;                                                                            // to check if two conversion clusters come from the same mother
  int gamma0MotherLabel = GetPhotonMotherLabel(TrueGammaCandidate0, convertedPhotonLabel0, (fIsCalo == false) ? false : true); // calo photon for calo
  int gamma1MotherLabel = GetPhotonMotherLabel(TrueGammaCandidate1, convertedPhotonLabel1, (fIsConv == true) ? false : true);  // calo photon for calo and convcalo

  AliAODMCParticle* trueMesonCand = nullptr;
  if (gamma0MotherLabel >= 0 && gamma0MotherLabel == gamma1MotherLabel) {
    trueMesonCand = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gamma1MotherLabel));
    if (trueMesonCand->GetPdgCode() == fMesonPDGCode) {
      isTrueParticle = true;
    } else if (std::find(fOtherMesonsPDGCodes.begin(), fOtherMesonsPDGCodes.end(), fMesonPDGCode) != fOtherMesonsPDGCodes.end()) {
      isTrueOtherParticle = true;
    }
  }

  // return if particle is neither the particle we are looking for or another meson we are interested in
  if (!isTrueOtherParticle && !isTrueParticle)
    return;

  // Define most important variables here
  float mesonPtRec = Pi0Candidate->Pt();
  float mesonPtTrue = trueMesonCand->Pt();
  float mesonMassRec = Pi0Candidate->M();
  float jetPtRec = (matchedJet < 0) ? 0 : fVectorJetPt[matchedJet];
  int indexTrueJet = (matchedJet < 0 || MapRecJetsTrueJets.count(matchedJet) == 0) ? -1 : MapRecJetsTrueJets[matchedJet];
  float jetPtTrue = (indexTrueJet < 0) ? 0 : fTrueVectorJetPt[indexTrueJet];

  float z_rec = GetFrag(Pi0Candidate, matchedJet, false);
  float z_rec_trueJet = GetFrag(Pi0Candidate, indexTrueJet, true);
  float z_true = GetFrag(trueMesonCand, indexTrueJet, true);

  // fill all other mesons (eta and eta prime in case pi0 is selected etc.)
  if (isTrueOtherParticle) {
    fRespMatrixHandlerTrueOtherMesonInvMassVsPt[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), z_rec, fWeightJetJetMC);
    fRespMatrixHandlerTrueOtherMesonInvMassVsZ[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), z_rec, fWeightJetJetMC);
    return;
  }

  if (!isTrueParticle)
    return;

  // fill all true mesons (primary + secondaries)
  fRespMatrixHandlerTrueMesonInvMassVsPt[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), Pi0Candidate->Pt(), fWeightJetJetMC);
  fRespMatrixHandlerTrueMesonInvMassVsZ[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), z_rec, fWeightJetJetMC);

  if(fDoMesonQA > 0){
    fHistoTrueMesonInvMassVsTruePt[fiCut]->Fill(Pi0Candidate->M(), trueMesonCand->Pt(), fWeightJetJetMC);

    if (std::find(fMesonDoubleCount.begin(), fMesonDoubleCount.end(), gamma0MotherLabel) != fMesonDoubleCount.end()) {
      fRespMatrixHandlerTrueMesonInvMassVsPtDoubleCount[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), Pi0Candidate->Pt(), fWeightJetJetMC);
      fRespMatrixHandlerTrueMesonInvMassVsZDoubleCount[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), z_rec, fWeightJetJetMC);
    } else {
      fMesonDoubleCount.push_back(gamma0MotherLabel);
    }
  }

  // fill all primary true mesons
  bool isPrimary = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsConversionPrimaryAOD(fInputEvent, static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gamma0MotherLabel)), mcProdVtxX, mcProdVtxY, mcProdVtxZ);

  if (isPrimary) {
    if (fDoMesonQA) {
      fHistoTruePrimaryMesonInvMassPt[fiCut]->Fill(mesonMassRec, mesonPtRec, fWeightJetJetMC);
     // meson response matrix (no Jet included here). Only use that for x-checks as the response is probably jet pt dependent
      fHistoMesonResponse[fiCut]->Fill(mesonPtRec, mesonPtTrue, fWeightJetJetMC);
    }

    fHistoTrueMesonJetPtVsTrueZ[fiCut]->Fill(z_true, jetPtTrue, fWeightJetJetMC);
    fHistoTrueMesonJetPtVsTruePt[fiCut]->Fill(trueMesonCand->Pt(), jetPtTrue, fWeightJetJetMC);

    fHistoTrueMesonJetPtVsRecZ[fiCut]->Fill(z_rec, jetPtRec, fWeightJetJetMC);
    fHistoTrueMesonJetPtVsRecPt[fiCut]->Fill(Pi0Candidate->Pt(), jetPtRec, fWeightJetJetMC);

    fRespMatrixHandlerFrag[fiCut]->Fill(jetPtRec, jetPtTrue, z_rec, z_true, fWeightJetJetMC);
    // response matrix filled with only true jet information. 
    if(!fDoLightOutput){
      fRespMatrixHandlerFragTrueJets[fiCut]->Fill(jetPtRec, jetPtTrue, z_rec_trueJet, z_true, fWeightJetJetMC);
    }

    fRespMatrixHandlerMesonInvMass[fiCut]->Fill(jetPtRec, jetPtTrue, Pi0Candidate->M(), Pi0Candidate->Pt(), fWeightJetJetMC);

    // fill 4d response matrix
    fRespMatrixHandlerMesonPt[fiCut]->Fill(jetPtRec, jetPtTrue, mesonPtRec, mesonPtTrue, fWeightJetJetMC);
  } else { // fill all secondary mesons

    if(fDoMesonQA>0){
      fHistoTrueSecMesonJetPtVsRecZ[fiCut]->Fill(z_rec, jetPtRec, fWeightJetJetMC);
      fHistoTrueSecMesonJetPtVsRecPt[fiCut]->Fill(Pi0Candidate->Pt(), jetPtRec, fWeightJetJetMC);
    }

    if(fDoMesonQA > 0){
      fHistoTrueSecondaryMesonInvMassPt[fiCut]->Fill(Pi0Candidate->M(), mesonPtRec, fWeightJetJetMC);
    }
    fRespMatrixHandlerTrueSecondaryMesonInvMassVsPt[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), Pi0Candidate->Pt(), fWeightJetJetMC);
    fRespMatrixHandlerTrueSecondaryMesonInvMassVsZ[fiCut]->Fill(jetPtRec, 0.5, Pi0Candidate->M(), z_rec, fWeightJetJetMC);
  }
}

/// \brief check if particle is true meson for inside true jets
/// \param Pi0Candidate meson candidate to be checked
/// \param TrueGammaCandidate0 cluster in case of calo and convCalo, conversion photon in case of conv
/// \param TrueGammaCandidate1 cluster in case of calo, conversion photon in case of conv-calo and conv
/// \param matchedJet index of matched true Jet
/// \param RJetPi0Cand distance of Jet axis to meson
//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessTrueMesonCandidatesInTrueJetsAOD(AliAODConversionMother* Pi0Candidate, AliAODConversionPhoton* TrueGammaCandidate0, AliAODConversionPhoton* TrueGammaCandidate1, const int matchedJet, const float RJetPi0Cand)
{

  // Process True Mesons
  if (!fAODMCTrackArray)
    fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
  if (fAODMCTrackArray == NULL)
    return;

  bool isTrueParticle = false;

  // check if meson originates from the same meson
  int convertedPhotonLabel0, convertedPhotonLabel1;                                                                            // to check if two conversion clusters come from the same mother
  int gamma0MotherLabel = GetPhotonMotherLabel(TrueGammaCandidate0, convertedPhotonLabel0, (fIsCalo == false) ? false : true); // calo photon for calo
  int gamma1MotherLabel = GetPhotonMotherLabel(TrueGammaCandidate1, convertedPhotonLabel1, (fIsConv == true) ? false : true);  // calo photon for calo and convcalo

  AliAODMCParticle* trueMesonCand = nullptr;
  if (gamma0MotherLabel >= 0 && gamma0MotherLabel == gamma1MotherLabel) {
    trueMesonCand = static_cast<AliAODMCParticle*>(fAODMCTrackArray->At(gamma1MotherLabel));
    if (trueMesonCand->GetPdgCode() == fMesonPDGCode) {
      isTrueParticle = true;
    }
  }

  // return if particle is neither the particle we are looking for or another meson we are interested in
  if (!isTrueParticle)
    return;

  // Define most important variables here
  float mesonPtTrue = trueMesonCand->Pt();
  float jetPtTrue = fTrueVectorJetPt[matchedJet];
  float z_true = GetFrag(trueMesonCand, matchedJet, true);

  if(fDoMesonQA>0){
    fHistoTrueMesonInTrueJet_JetPtVsTrueZ[fiCut]->Fill(z_true, jetPtTrue, fWeightJetJetMC);
    fHistoTrueMesonInTrueJet_JetPtVsTruePt[fiCut]->Fill(mesonPtTrue, jetPtTrue, fWeightJetJetMC);
  }
}

//_____________________________________________________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::ProcessTruePhotonCandidatesAOD(AliAODConversionPhoton* TruePhotonCandidate)
{

  const AliVVertex* primVtxMC = fMCEvent->GetPrimaryVertex();
  Double_t mcProdVtxX = primVtxMC->GetX();
  Double_t mcProdVtxY = primVtxMC->GetY();
  Double_t mcProdVtxZ = primVtxMC->GetZ();

  Double_t magField = fInputEvent->GetMagneticField();

  if (!fAODMCTrackArray)
    fAODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
  if (fAODMCTrackArray == NULL)
    return;
  AliAODMCParticle* posDaughter = (AliAODMCParticle*)fAODMCTrackArray->At(TruePhotonCandidate->GetMCLabelPositive());
  AliAODMCParticle* negDaughter = (AliAODMCParticle*)fAODMCTrackArray->At(TruePhotonCandidate->GetMCLabelNegative());

  if (posDaughter == NULL || negDaughter == NULL)
    return; // cant find electon/positron
  int pdgCode[2] = {TMath::Abs(posDaughter->GetPdgCode()), TMath::Abs(negDaughter->GetPdgCode())};

  if (pdgCode[0] != 11 || pdgCode[1] != 11) {
    return; // One Particle is not a electron
  }

  if (posDaughter->GetPdgCode() == negDaughter->GetPdgCode()) {
    return; // Same Charge
  }

  AliAODMCParticle* Photon = (AliAODMCParticle*)fAODMCTrackArray->At(posDaughter->GetMother());
  if (Photon->GetPdgCode() != 22) {
    return; // Mother is no Photon
  }

  if (((posDaughter->GetMCProcessCode())) != 5 || ((negDaughter->GetMCProcessCode())) != 5) {
    return; // check if the daughters come from a conversion
  }

  if (!fIsFromDesiredHeader) {
    return;
  }

  // True Photon
  Double_t weightMatBudgetGamma = 1.;
  if (fDoMaterialBudgetWeightingOfGammasForTrueMesons && ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetWeightsInitialized()) {
    weightMatBudgetGamma = ((AliConversionPhotonCuts*)fConvCutArray->At(fiCut))->GetMaterialBudgetCorrectingWeightForTrueGamma(TruePhotonCandidate, magField);
  }

  fHistoTrueConvGammaPt[fiCut]->Fill(TruePhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);

  Bool_t isPrimary = ((AliConvEventCuts*)fEventCutArray->At(fiCut))->IsConversionPrimaryAOD(fInputEvent, Photon, mcProdVtxX, mcProdVtxY, mcProdVtxZ);
  if (isPrimary) {
    // Count just primary MC Gammas as true --> For Ratio esdtruegamma / mcconvgamma
    if (!fDoLightOutput) {
      fHistoTruePrimaryConvGammaPt[fiCut]->Fill(TruePhotonCandidate->Pt(), fWeightJetJetMC * weightMatBudgetGamma);
      fHistoTruePrimaryConvGammaESDPtMCPt[fiCut]->Fill(TruePhotonCandidate->Pt(), Photon->Pt(), fWeightJetJetMC * weightMatBudgetGamma); // Allways Filled
    }
  }
  TruePhotonCandidate->SetIsTrueConvertedPhoton();
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::UpdateEventMixData()
{
  if (fIsCalo) {
    if (fClusterCandidates.size() > 1) {
      std::shared_ptr<EventWithJetAxis> tmpEvt = std::make_shared<EventWithJetAxis>(fClusterCandidates, true, fHighestJetVector);
      fEventMix->AddEvent(tmpEvt, fMaxPtJet);
    }
  } else if (fIsConvCalo) {
    if (fClusterCandidates.size() > 0 && fGammaCandidates.size() > 0) {
      std::shared_ptr<EventWithJetAxis> tmpEvt = std::make_shared<EventWithJetAxis>(fClusterCandidates, fGammaCandidates, fHighestJetVector);
      fEventMix->AddEvent(tmpEvt, fMaxPtJet);
    }
  } else if (fIsConv) {
    if (fGammaCandidates.size() > 1) {
      std::shared_ptr<EventWithJetAxis> tmpEvt = std::make_shared<EventWithJetAxis>(fGammaCandidates, false, fHighestJetVector);
      fEventMix->AddEvent(tmpEvt, fMaxPtJet);
    }
  }
}

//________________________________________________________________________
void AliAnalysisTaskMesonJetCorrelation::RelabelAODPhotonCandidates(Bool_t mode)
{

  // Relabeling For AOD Event
  // ESDiD -> AODiD
  // MCLabel -> AODMCLabel

  if (mode) {
    fMCEventPos = new int[fReaderGammas->GetEntries()];
    fMCEventNeg = new int[fReaderGammas->GetEntries()];
    fESDArrayPos = new int[fReaderGammas->GetEntries()];
    fESDArrayNeg = new int[fReaderGammas->GetEntries()];
  }

  for (int iGamma = 0; iGamma < fReaderGammas->GetEntries(); iGamma++) {
    AliAODConversionPhoton* PhotonCandidate = (AliAODConversionPhoton*)fReaderGammas->At(iGamma);
    if (!PhotonCandidate)
      continue;
    if (!mode) { // Back to ESD Labels
      PhotonCandidate->SetMCLabelPositive(fMCEventPos[iGamma]);
      PhotonCandidate->SetMCLabelNegative(fMCEventNeg[iGamma]);
      PhotonCandidate->SetLabelPositive(fESDArrayPos[iGamma]);
      PhotonCandidate->SetLabelNegative(fESDArrayNeg[iGamma]);
      continue;
    }
    fMCEventPos[iGamma] = PhotonCandidate->GetMCLabelPositive();
    fMCEventNeg[iGamma] = PhotonCandidate->GetMCLabelNegative();
    fESDArrayPos[iGamma] = PhotonCandidate->GetTrackLabelPositive();
    fESDArrayNeg[iGamma] = PhotonCandidate->GetTrackLabelNegative();

    Bool_t AODLabelPos = kFALSE;
    Bool_t AODLabelNeg = kFALSE;

    for (int i = 0; i < fInputEvent->GetNumberOfTracks(); i++) {
      AliAODTrack* tempDaughter = static_cast<AliAODTrack*>(fInputEvent->GetTrack(i));
      if (!AODLabelPos) {
        if (tempDaughter->GetID() == PhotonCandidate->GetTrackLabelPositive()) {
          PhotonCandidate->SetMCLabelPositive(TMath::Abs(tempDaughter->GetLabel()));
          PhotonCandidate->SetLabelPositive(i);
          AODLabelPos = kTRUE;
        }
      }
      if (!AODLabelNeg) {
        if (tempDaughter->GetID() == PhotonCandidate->GetTrackLabelNegative()) {
          PhotonCandidate->SetMCLabelNegative(TMath::Abs(tempDaughter->GetLabel()));
          PhotonCandidate->SetLabelNegative(i);
          AODLabelNeg = kTRUE;
        }
      }
      if (AODLabelNeg && AODLabelPos) {
        break;
      }
    }
    if (!AODLabelPos || !AODLabelNeg) {
      cout << "WARNING!!! AOD TRACKS NOT FOUND FOR" << endl;
      if (!AODLabelNeg) {
        PhotonCandidate->SetMCLabelNegative(-999999);
        PhotonCandidate->SetLabelNegative(-999999);
      }
      if (!AODLabelPos) {
        PhotonCandidate->SetMCLabelPositive(-999999);
        PhotonCandidate->SetLabelPositive(-999999);
      }
    }
  }

  if (!mode) {
    delete[] fMCEventPos;
    delete[] fMCEventNeg;
    delete[] fESDArrayPos;
    delete[] fESDArrayNeg;
  }
}
