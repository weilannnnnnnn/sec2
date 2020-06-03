#include "secRunAction.hh"
#include "secAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
secRunAction::secRunAction(void) : 
 G4UserRunAction()
{
//using root format for saving the histograms
    auto AnalysisMgr = G4AnalysisManager::Instance();
    AnalysisMgr->SetVerboseLevel(1);

//create histograms
    AnalysisMgr->CreateH1("PhotonsGenUp", 
                          "Generated Photons in First Scintillator", 
                          10000, 0, 50000);
    AnalysisMgr->CreateH1("PhotonsGenDown",
                          "Generated Photons in Second Scintillator",
                          10000, 0, 50000);
                          
    AnalysisMgr->CreateH1("PhotonRecvUp",
                          "Received Photons in First SiPM",
                          10000, 0, 50000);
    AnalysisMgr->CreateH1("PhotonsRecvDown",
                          "Received Photons in Second SiPM",
                          10000, 0, 50000);

    AnalysisMgr->CreateH1("CollectEfficiencyUp",
                          "Photon Collection Efficiency in First Layer",
                          100, 0., 1.);
    AnalysisMgr->CreateH1("CollectEfficiencyDown",
                          "Photon Collection Efficiency in Second Layer",
                          100, 0., 1.);
}

secRunAction::~secRunAction()
{
    delete G4AnalysisManager::Instance();
}

void secRunAction::BeginOfRunAction(const G4Run* )
{
    auto AnalysisMgr = G4AnalysisManager::Instance();
    const G4String FileName = "secHist";
    AnalysisMgr->OpenFile(FileName);
}

void secRunAction::EndOfRunAction(const G4Run* )
{
    auto AnalysisMgr = G4AnalysisManager::Instance();

    AnalysisMgr->Write();
    AnalysisMgr->CloseFile();
}

