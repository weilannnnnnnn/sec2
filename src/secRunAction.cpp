#include "secRunAction.hh"
#include "secAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
secRunAction::secRunAction(void) : 
 G4UserRunAction()
{
//using root format for saving the histograms
    auto AnalysisMgr = G4AnalysisManager::Instance();
    AnalysisMgr->SetVerboseLevel(1);

//create histograms
//---------------------------------------------------------------------
    AnalysisMgr->CreateH1("PhotonsGenUp", 
                          "Generated Photons in First Scintillator", 
                          100, 0, 80000);
    AnalysisMgr->CreateH1("PhotonsGenDown",
                          "Generated Photons in Second Scintillator",
                          100, 0, 80000);
//---------------------------------------------------------------------                        
    AnalysisMgr->CreateH1("PhotonRecvUp",
                          "Received Photons in First SiPM",
                          100, 0, 40000);
    AnalysisMgr->CreateH1("PhotonsRecvDown",
                          "Received Photons in Second SiPM",
                          100, 0, 40000);
//---------------------------------------------------------------------
    AnalysisMgr->CreateH1("GenPhotonEnegDistUp",
		                  "Energy Distribution of Generated Photons In upper Scintillator",
			              100, 0.*eV, 10.*eV);
    AnalysisMgr->CreateH1("GenPhotonEnegDistDown",
		                  "Energy Distribution of Generated Photons In lower Scintillator",
			              100, 0.*eV, 10.*eV);
//----------------------------------------------------------------------
    AnalysisMgr->CreateH1("RecvPhotonEnegDistUp",
                          "Energy Distribution of Recived Photons In upper Scintillator",
                          100, 0.*eV, 10.*eV);
    AnalysisMgr->CreateH1("RecvPhotonEnegDistDown",
                          "Energy Distribution of Recived Photons In lower Scintillator",
                          100, 0.*eV, 10.*eV);
//-----------------------------------------------------------------------
    AnalysisMgr->CreateP1("LightOutputUp",
                          "The Light Output in the First Scintillator",
                          30, 1.*MeV, 10.*MeV,
                          0., 500000);
    AnalysisMgr->CreateP1("LightOutPutDown",
                          "The Light Output in the second Scintillator",
                          30, 1., 10.,
                          0., 500000);
//------------------------------------------------------------------------                          
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
    //save and close the histograms
    auto AnalysisMgr = G4AnalysisManager::Instance();

    AnalysisMgr->Write();
    AnalysisMgr->CloseFile();
    //merge the decay data files
    
    if( !isMaster )
    {
        //only use one thread to merge the file.
        return;
    }
    else
    {
        //MergeFile("ScintMuonDecayEventData");
        MergeFile("ScintPhotonDecayEventData.dat");
        MergeFile("SiPMDecayEventData");
        MergeFile("DecayTime");
    }
    
}

void secRunAction::MergeFile(G4String FileName)
{
    std::ofstream FinalDataStrm;
    std::ifstream TempDataStrm;
    std::ostringstream sstrm;

    //open data files
    FinalDataStrm.open(FileName, std::ofstream::app | std::ofstream::binary);

    for(G4int i = 0; i != G4Threading::GetNumberOfRunningWorkerThreads(); ++i)
    {
        sstrm <<  FileName << "_t" << i;
        TempDataStrm.open(sstrm.str(), std::ifstream::in | std::ifstream::binary);
        
        if(TempDataStrm.eof())
        {  
            TempDataStrm.close(); 
            remove(sstrm.str().c_str());
            sstrm.str("");
            continue;
        }
        //copy file
        FinalDataStrm << TempDataStrm.rdbuf();
        FinalDataStrm.flush();
        //close and delete the temporary file
        TempDataStrm.close();
        remove(sstrm.str().c_str());
        sstrm.str("");
    }
    FinalDataStrm.close();
}

