#include "secSiPMSD.hh"
#include "secScintSD.hh"
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
        // initialization of the wait time stamp generators
        (void) secParticleSource::GenNoiseWaitTime(0, false, false);
        (void) secParticleSource::MuonWaitTimeMT(-1);
	}
}

void secRunAction::EndOfRunAction(const G4Run* )
{
    //use master thread to merge and close file.
    if( IsMaster() )
    {
        secScintSD::pFile->cd();
        secScintSD::UpNoiseTree->Write("", TObject::kOverwrite);
        secScintSD::DownNoiseTree->Write("", TObject::kOverwrite);
		    secScintSD::UpDecayTree->Write("", TObject::kOverwrite);	
		    secScintSD::DownDecayTree->Write("", TObject::kOverwrite);
		    secScintSD::UpNormalTree->Write("", TObject::kOverwrite);
		    secScintSD::DownNormalTree->Write("", TObject::kOverwrite);
		    secScintSD::pFile->Close();
        MergeFile("NumberOfNoise.dat");
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

}
