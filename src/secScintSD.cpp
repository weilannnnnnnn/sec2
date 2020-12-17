#include "secSiPMSD.hh"
#include "secScintSD.hh"
#include "secParticleSource.hh"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Threading.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "tools/histo/h1d"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <atomic>
#include <mutex>

std::mutex mtx_ScintSD;

TFile* secScintSD::pFile = nullptr;
TTree* secScintSD::UpNoiseTree    = nullptr;
TTree* secScintSD::DownNoiseTree  = nullptr;
TTree* secScintSD::UpDecayTree    = nullptr;
TTree* secScintSD::DownDecayTree  = nullptr;    
TTree* secScintSD::UpNormalTree   = nullptr;
TTree* secScintSD::DownNormalTree = nullptr;

secScintSD::secScintSD(const G4String& SDname, const std::vector<G4String> SDHCnameVect) :
    G4VSensitiveDetector(SDname),
    NoiseIdx(0),
    MuonTimeStamp(0.),
    IsMuonTimeStampGened(false),
    DecayFlagSiPM(false),
    EventIsKept(false),
    DecayEventID(0),
    PhotonsGenUp(0),
    PhotonsGenDown(0),
    PhotonEnegUp(0.),
    PhotonEnegDown(0.),
    MuonEdepUp(0.),
    MuonEdepDown(0.),
    FormerID(-1),
    pPhotonHCup(nullptr),
    pPhotonHCdown(nullptr),
    pMuonHCup(nullptr),
    pMuonHCdown(nullptr)
{
    //the arrive time for the first event
    for(G4String str : SDHCnameVect)
    {
        collectionName.insert(str);
    }
}

secScintSD::~secScintSD()
{

}
void secScintSD::Initialize(G4HCofThisEvent* HC)
{    
    auto SDmgr = G4SDManager::GetSDMpointer();
    pPhotonHCup   = new secScintHitsCollection(SensitiveDetectorName, collectionName[0]);
    pPhotonHCdown = new secScintHitsCollection(SensitiveDetectorName, collectionName[1]);
    pMuonHCup     = new secScintHitsCollection(SensitiveDetectorName, collectionName[2]);
    pMuonHCdown   = new secScintHitsCollection(SensitiveDetectorName, collectionName[3]);

    G4int HCID0 = SDmgr->GetCollectionID(collectionName[0]);
    G4int HCID1 = SDmgr->GetCollectionID(collectionName[1]);
    G4int HCID2 = SDmgr->GetCollectionID(collectionName[2]);
    G4int HCID3 = SDmgr->GetCollectionID(collectionName[3]);

    HC->AddHitsCollection(HCID0, pPhotonHCup);
    HC->AddHitsCollection(HCID1, pPhotonHCdown);
    HC->AddHitsCollection(HCID2, pMuonHCup);
    HC->AddHitsCollection(HCID3, pMuonHCdown);

    static thread_local bool IsWaitTimeRead = false;
    static std::once_flag IsInit;
    std::call_once(IsInit, InitDataFile); 
    //read in the noise wait time file.
    if( secParticleSource::Muons == secParticleSource::GetEventType() && !IsWaitTimeRead)
    {
        IsWaitTimeRead = true;
        NoiseWaitTimeVect.push_back(-INFINITY);
      //===================//
        mtx_ScintSD.lock();//using locks here!
      //===================//
        TTree* NoiseTree = secScintSD::UpNoiseTree;
        G4double WaitTime = 0;
        NoiseTree->SetBranchAddress("TimeStamp", &WaitTime);
        const size_t SizeBr = NoiseTree->GetEntries();
        for(size_t i = 0; i != SizeBr; ++i)
        {
            NoiseTree->GetEntry(i);
            NoiseWaitTimeVect.push_back(WaitTime);
        }
      //=====================//
        mtx_ScintSD.unlock();//unlocked!
      //=====================//
        std::sort(NoiseWaitTimeVect.begin()+1, NoiseWaitTimeVect.end());
        NoiseWaitTimeVect.push_back(INFINITY);
    }
}

G4bool secScintSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
//the return value of this function is currently researved and
//it may be used in the future update of Geant4 application.
    
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();
    const G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    //the case that a opticalphoton generated in the scintillator
/*
    Notice: 
            When optical photons Generated in the Scintillator hits the aluminum foil, 
        which means the boundary process has happened, and G4 considered that the optical
        photon has entered the foil(PostStepPoint) and will be reflected in the Scintillator
        in the next step or will be absorbed by the foil.
            As a result, the IsFirstStepInVolume() method will not work! We must compare the TrackID to 
        indentify whether the particle has been counted or not.
*/
    if( *ParticleNow == *G4OpticalPhoton::Definition() )
    {
        if( step->GetTrack()->GetTrackID() != FormerID )
        {
            const G4double aPhotonEneg = step->GetTrack()->GetKineticEnergy();
            const G4double GlobalTime = step->GetPostStepPoint()->GetGlobalTime();
            if( VolumeCpyNb == 1 )
            {
                auto pPhotonHitUp = new secScintHit();
                pPhotonHitUp->SetEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
                pPhotonHCup->insert(pPhotonHitUp);
                PhotonsGenUp++;
                PhotonEnegUp += aPhotonEneg;
            }
            else if( VolumeCpyNb == 2 )
            {
                auto pPhotonHitDown = new secScintHit();
                pPhotonHitDown->SetEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
                pPhotonHCdown->insert(pPhotonHitDown);
                PhotonsGenDown++;
                PhotonEnegDown += aPhotonEneg;
            }
            //update the id
            FormerID = step->GetTrack()->GetTrackID();
            //step->GetTrack()->SetTrackStatus(fStopAndKill);
            return false;
        }
    }
//the case that a muon hit the scintillator
    else if( *ParticleNow == *G4MuonPlus::Definition() ||
             *ParticleNow == *G4MuonMinus::Definition() )
    {
        if( !IsMuonTimeStampGened )
        {
            IsMuonTimeStampGened = true;
            MuonTimeStamp = secParticleSource::MuonWaitTime();
            NoiseIdx = GetCoupledIdx(MuonTimeStamp);
            if( -1 != NoiseIdx )
                EventIsKept = true;
        }
        G4double aStepEdep = step->GetTotalEnergyDeposit();
        G4double GlobalTime = step->GetPostStepPoint()->GetGlobalTime();
        G4double MuonVelocity = step->GetPreStepPoint()->GetVelocity();
        
        if( VolumeCpyNb == 1 )
        {
            //get energy disposit
            MuonEdepUp += aStepEdep;
            //save to hits collection
            auto pMuonHitUp = new secScintHit();
            pMuonHitUp->SetGlobalTime(GlobalTime).SetVelocity(MuonVelocity);
            pMuonHCup->insert(pMuonHitUp);
        }
        else if( VolumeCpyNb == 2 )
        {
            //get energy deposit
            MuonEdepDown += aStepEdep;
            //save to hits collection
            auto pMuonHitDown = new secScintHit();
            pMuonHitDown->SetGlobalTime(GlobalTime).SetVelocity(MuonVelocity);
            pMuonHCdown->insert(pMuonHitDown);

            if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == G4ProcessType::fDecay )
            {
                DecayFlagSiPM  = true;//is a decay event!!
                EventIsKept = true;
            }
        }	
    }
    return true;   
}

void secScintSD::EndOfEvent(G4HCofThisEvent*)
{  
    Reset();
}


//print histogram version
void secScintSD::PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter, 
                         unsigned int nbins, G4double Xmin, G4double Xmax)
{
    //fill histogram
    tools::histo::h1d hist("aHist", nbins, Xmin, Xmax);
    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {   
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        hist.fill(val);
    }

    //create files
    std::ostringstream sstrm;
    sstrm << FileName << "_t" << G4Threading::G4GetThreadId();
    std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary);
    
    if( !fstrm.is_open() )
    {
        std::cerr << "***Unable to open file: " << FileName << " ***" << '\n';
        std::cerr << "***The generated data will not be recorded!***" << '\n';
    }

    //assert( fstrm.is_open() );
    fstrm << "Decay Event ID_t" << G4Threading::G4GetThreadId() << "= " << DecayEventID << '\n';
    
    //write file
    const std::vector<unsigned int>& entries = hist.bins_entries();
    std::vector<G4double> edges;
    edges.push_back(-1.);
    const G4double binw = (Xmax - Xmin) / nbins;
    for( size_t i = 0; i != nbins + 1; ++i )
    {
	const G4double edge = Xmin + i * binw;
        edges.push_back( edge );
    }
    //std::cout << "entries size = " << entries.size() << " edges size = " << edges.size() << '\n';
    size_t sz = entries.size() <= edges.size() ? entries.size() : edges.size();
    
    for( size_t i = 0; i != sz; ++i )
    {
	
        if( entries[i] == 0 )
	{
            continue;
	}

        fstrm << edges[i] << '\t' << entries[i] << '\n';
    }

    fstrm.flush();
    //close files
    fstrm.close();
}

//direct print HC version
void secScintSD::PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter)
{
    //create files
    std::ostringstream sstrm;
    sstrm << FileName << "_t" << G4Threading::G4GetThreadId();
    std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary);
    
    if( !fstrm.is_open() )
    {
        std::cerr << "***Unable to open file: " << FileName << " ***" << '\n';
        std::cerr << "***The generated data will not be recorded!***" << '\n';
    }

    //assert( fstrm.is_open() );
    fstrm << "Decay Event ID_t" << G4Threading::G4GetThreadId() << "= " << DecayEventID << '\n';

    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        fstrm << val << '\n';
    }    

    fstrm.flush();
    //close files
    fstrm.close();

}

//print single double value version
void secScintSD::PrintData(G4String FileName, G4double val)
{
    	std::ostringstream sstrm;
        sstrm << FileName << "_t" << G4Threading::G4GetThreadId();

        std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary );
        fstrm << val << '\n';
}

G4int secScintSD::GetCoupledIdx(G4double MuonTS)
{
        /*
            if the a single normal muon event is coupled with a noise event, than this
            normal muon event will be saved. 
        */
        const G4double FrontTimeWindow = 100*ns;
        const G4double BackTimeWindow = 20000*ns;
        G4bool IsCoupled = false;
        const size_t sz = NoiseWaitTimeVect.size();
        static thread_local unsigned idx = 0;

        //locate the closest noise wait time
	while( MuonTS - NoiseWaitTimeVect[idx] > 0 )
        {
            ++idx;
            if( idx == sz )
            {
                break;
            }
        }
        //after this loop, the EventWait time lies in: NoiseWaitTime[idx-1], EventWaitTime, NoiseWaitTime[idx]
        //or at the very beginning or the very end of the NoiseWaitTimeVect.
        //if the normal event is close enough to a noise event, than this normal event will be saved.
        if( idx == 0 )
        {
            if( NoiseWaitTimeVect[idx] - MuonTS <= BackTimeWindow )
                IsCoupled = true;
        }
        else if( idx == sz )
        {
            if( MuonTS - NoiseWaitTimeVect[sz-1] <= FrontTimeWindow )
            {
                IsCoupled = true;
            }
        }
        else if ( MuonTS - NoiseWaitTimeVect[idx-1] <= FrontTimeWindow ||
                  NoiseWaitTimeVect[idx] - MuonTS <= BackTimeWindow )
        {
                IsCoupled = true;
        }

        if( !IsCoupled )
            return -1;
        
        return idx;
}

void secScintSD::InitDataFile()
{
    //ROOT file initialization

	if( secParticleSource::GetEventType() != secParticleSource::Muons)
	{
	    //noise event first, create the trees and branches
        secScintSD::pFile = new TFile("secData.root", "RECREATE");
	    InitTrees();
	}
	else if( secParticleSource::GetEventType() == secParticleSource::Muons)
	{
	    //second muon events, read trees from disk!
        secScintSD::pFile = new TFile("secData.root", "UPDATE");
	    ReadTrees();
	    if( secScintSD::UpNoiseTree == nullptr )
	    {
		//first noise event, create trees.
		InitTrees();
	    }
	}

} 

void secScintSD::InitTrees()
{
    secScintSD::UpNoiseTree    = new TTree("UpNoise", "Up Noise Response Tree");
    secScintSD::DownNoiseTree  = new TTree("DownNoise", "Down Noise Response Tree");
    secScintSD::UpDecayTree    = new TTree("UpDecay", "Up Decay Response Tree");
    secScintSD::DownDecayTree  = new TTree("DownDecay", "Down Decay Response Tree");    
    secScintSD::UpNormalTree   = new TTree("UpNormal", "Up Normal Event Response Tree");
    secScintSD::DownNormalTree = new TTree("DownNormal", "Down Normal Event Response");

    secScintSD::UpNoiseTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");		
    secScintSD::UpNoiseTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::UpNoiseTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D");
    
    secScintSD::DownNoiseTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secScintSD::DownNoiseTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::DownNoiseTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D");

    secScintSD::UpDecayTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secScintSD::UpDecayTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::UpDecayTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D"); //in second.


    secScintSD::DownDecayTree->Branch("ArraySize", (unsigned*) nullptr, "ArraySize/i");
    secScintSD::DownDecayTree->Branch("Entries",   (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::DownDecayTree->Branch("TimeStamp", (double*) nullptr, "TimeStamp/D"); //in second.

    
    secScintSD::UpNormalTree->Branch("ArraySize",     (unsigned*) nullptr, "ArraySize/i");
    secScintSD::UpNormalTree->Branch("Entries",       (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::UpNormalTree->Branch("TimeStamp",     (double*) nullptr, "TimeStamp/D"); //in second.
    secScintSD::UpNormalTree->Branch("Coupled index", (unsigned*) nullptr, "idx/i");


    secScintSD::DownNormalTree->Branch("ArraySize",     (unsigned*) nullptr, "ArraySize/i");
    secScintSD::DownNormalTree->Branch("Entries",       (unsigned*) nullptr, "Entries[ArraySize]/i");
    secScintSD::DownNormalTree->Branch("TimeStamp",     (double*) nullptr, "TimeStamp/D"); //in second.
    secScintSD::DownNormalTree->Branch("Coupled index", (unsigned*) nullptr, "idx/i");  
}

void secScintSD::ReadTrees()
{
    secScintSD::UpNoiseTree    = (TTree*) (secScintSD::pFile->Get("UpNoise"));
    secScintSD::DownNoiseTree  = (TTree*) (secScintSD::pFile->Get("DownNoise"));
    secScintSD::UpDecayTree    = (TTree*) (secScintSD::pFile->Get("UpDecay"));
    secScintSD::DownDecayTree  = (TTree*) (secScintSD::pFile->Get("DownDecay"));  
    secScintSD::UpNormalTree   = (TTree*) (secScintSD::pFile->Get("UpNormal"));
    secScintSD::DownNormalTree = (TTree*) (secScintSD::pFile->Get("DownNormal"));
}
