#include "secSiPMSD.hh"
#include "secRunAction.hh"
#include "secAnalysis.hh"
#include "secParticleSource.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "secParticleSource.hh"
#include "globals.hh"
#include "TFile.h"
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>

//create the file and initialize the trees
TFile* secSiPMSD::pFile = new TFile("secData.root", "UPDATE");
TTree* secSiPMSD::UpNoiseTree    = nullptr;
TTree* secSiPMSD::DownNoiseTree  = nullptr;
TTree* secSiPMSD::UpDecayTree    = nullptr;
TTree* secSiPMSD::DownDecayTree  = nullptr;    
TTree* secSiPMSD::UpNormalTree   = nullptr;
TTree* secSiPMSD::DownNormalTree = nullptr;

secRunAction::secRunAction(void) : 
 G4UserRunAction()
{	

}
secRunAction::~secRunAction()
{
}

void secRunAction::BeginOfRunAction(const G4Run* )
{
	if( IsMaster() )
	{
        (void) secParticleSource::GenNoiseWaitTime(0, false, false);
        //ROOT file initialization
        if( secParticleSource::GetEventType() != secParticleSource::Muons )
        {
            InitTrees();
        }
        else
        {
            secSiPMSD::UpNoiseTree    = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("UpNoise"));
            secSiPMSD::DownNoiseTree  = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("DownNoise"));
            secSiPMSD::UpDecayTree    = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("UpDecay"));
            secSiPMSD::DownDecayTree  = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("DownDecay"));  
            secSiPMSD::UpNormalTree   = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("UpNormal"));
            secSiPMSD::DownNormalTree = dynamic_cast<TTree*> (secSiPMSD::pFile->Get("DownNormal"));
            std::cout << "UpNoiseTreeAddr = " << secSiPMSD::UpNoiseTree << std::endl;
            if( secSiPMSD::UpNoiseTree == nullptr )
            {
                InitTrees();
            }
        }
	}
}

void secRunAction::EndOfRunAction(const G4Run* )
{
    //use master thread to merge and close file.
    if( IsMaster() )
    {
        secSiPMSD::UpNoiseTree->Write();
        secSiPMSD::DownNoiseTree->Write();
		secSiPMSD::UpDecayTree->Write();	
		secSiPMSD::DownDecayTree->Write();
		secSiPMSD::UpNormalTree->Write();
		secSiPMSD::DownNormalTree->Write();
		secSiPMSD::pFile->Close();
		MergeFile("NoiseWaitTime.txt");
    }
}

void secRunAction::MergeFile(G4String FileName)
{
    std::fstream FinalDataStrm;
    std::ifstream TempDataStrm;
    std::ostringstream sstrm;

    //open data files
    FinalDataStrm.open(FileName, std::ofstream::app | std::ofstream::binary);

    for(G4int i = 0; i != G4Threading::GetNumberOfRunningWorkerThreads(); ++i)
    {
        sstrm <<  FileName << "_t" << i;
        TempDataStrm.open(sstrm.str(), std::ifstream::in | std::ifstream::binary);
        //copy file
        FinalDataStrm << TempDataStrm.rdbuf();
        FinalDataStrm.flush();
        //close and delete the temporary file
        TempDataStrm.close();
        remove(sstrm.str().c_str());
        sstrm.str("");
    }
    G4bool IsEmpty = FinalDataStrm.peek() == EOF;
    FinalDataStrm.close();

    //if(  IsEmpty )
    //{
        //remove(FileName.c_str());
    //}
}

void secRunAction::InitTrees()
{
    secSiPMSD::pFile->cd();
    secSiPMSD::UpNoiseTree    = new TTree("UpNoise", "Up Noise Response Tree");
    secSiPMSD::DownNoiseTree  = new TTree("DownNoise", "Down Noise Response Tree");
    secSiPMSD::UpDecayTree    = new TTree("UpDecay", "Up Decay Response Tree");
    secSiPMSD::DownDecayTree  = new TTree("DownDecay", "Down Decay Response Tree");    
    secSiPMSD::UpNormalTree   = new TTree("UpNormal", "Up Normal Event Response Tree");
    secSiPMSD::DownNormalTree = new TTree("DownNormal", "Down Normal Event Response");

    secSiPMSD::UpNoiseTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");		
    secSiPMSD::UpNoiseTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::UpNoiseTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D");

    secSiPMSD::DownNoiseTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secSiPMSD::DownNoiseTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::DownNoiseTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D");

    secSiPMSD::UpDecayTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secSiPMSD::UpDecayTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::UpDecayTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D"); //in second.


    secSiPMSD::DownDecayTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secSiPMSD::DownDecayTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::DownDecayTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D"); //in second.

    
    secSiPMSD::UpNormalTree->Branch("ArraySize",     (unsigned*) nullptr, "ArraySize/i");
    secSiPMSD::UpNormalTree->Branch("Entries",       (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::UpNormalTree->Branch("TimeStamp",     (double*) nullptr, "TimeStamp/D"); //in second.
    secSiPMSD::UpNormalTree->Branch("Coupled index", (unsigned*) nullptr, "idx/i");


    secSiPMSD::DownNormalTree->Branch("ArraySize",     (unsigned*) nullptr, "ArraySize/i");
    secSiPMSD::DownNormalTree->Branch("Entries",       (unsigned*) nullptr, "Entries[ArraySize]/i");
    secSiPMSD::DownNormalTree->Branch("TimeStamp",     (double*) nullptr, "TimeStamp/D"); //in second.
    secSiPMSD::DownNormalTree->Branch("Coupled index", (unsigned*) nullptr, "idx/i");   
}
