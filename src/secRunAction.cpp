#include "secRunAction.hh"
#include "secAnalysis.hh"
#include "secParticleSource.hh"
#include "secSiPMSD.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "TFile.h"
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>

TFile* secSiPMSD::pFile = new TFile("secData.root", "RECREATE");

secRunAction::secRunAction(void) : 
 G4UserRunAction()
{
}

secRunAction::~secRunAction()
{
}

void secRunAction::BeginOfRunAction(const G4Run* )
{
    //ROOT file initialization
    if( IsMaster() )
    {
	secSiPMSD::pFile->mkdir("UpDecay");
	secSiPMSD::pFile->mkdir("DownDecay");

	secSiPMSD::pFile->mkdir("UpNoise");
	secSiPMSD::pFile->mkdir("DownNoise");

	secSiPMSD::pFile->mkdir("UpNorm");
	secSiPMSD::pFile->mkdir("DownNorm");
    }
}

void secRunAction::EndOfRunAction(const G4Run* )
{
    //use master thread to merge and close file.
    if( IsMaster() )
    {
        secSiPMSD::pFile->Close();
        GenerateNoiseTimeStamp("NoiseWaitTime.dat", 10000);
        //Merge the ascii file.
        MergeFile("DecayMuonWaitTime.dat");
        MergeFile("NormalMuonWaitTime.dat");
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

    if(  IsEmpty )
    {
        remove(FileName.c_str());
    }
}

void secRunAction::GenerateNoiseTimeStamp(G4String FileName, const size_t Num)
{
    std::ofstream ofstrm;

    ofstrm.open(FileName, std::ofstream::out | std::ofstream::binary);

    for( size_t i = 0; i != Num; ++i)
        ofstrm << secParticleSource::NoiseWaitTime() << '\n';
    ofstrm.close();
}
