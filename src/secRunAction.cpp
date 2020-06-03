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
                          100, 0, 80000);
    AnalysisMgr->CreateH1("PhotonsGenDown",
                          "Generated Photons in Second Scintillator",
                          100, 0, 80000);
                          
    AnalysisMgr->CreateH1("PhotonRecvUp",
                          "Received Photons in First SiPM",
                          100, 0, 40000);
    AnalysisMgr->CreateH1("PhotonsRecvDown",
                          "Received Photons in Second SiPM",
                          100, 0, 40000);

    AnalysisMgr->CreateH1("PhotonEnegDistUp",
		          "Energy Distribution of Photons In upper Scintillator",
			  100, 0.*eV, 10.*eV);
    AnalysisMgr->CreateH1("PhotonEnegDistDown",
		          "Energy Distribution of Photons In lower Scintillator",
			  100, 0.*eV, 10.*eV);
    
    AnalysisMgr->CreateH1("MuonEnegDist",
		          "Muon Energy Distribution",
			  100, 0.1*GeV, 1000*GeV);


    AnalysisMgr->CreateP1("LightOutputUp",
                          "The Light Output in the First Scintillator",
                          30, 1., 10.,
                          0., 500000);
    
    AnalysisMgr->CreateP1("LightOutPutDown",
                          "The Light Output in the second Scintillator",
                          30, 1., 10.,
                          0., 500000);
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

