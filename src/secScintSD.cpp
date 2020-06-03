#include "secScintSD.hh"
#include "secAnalysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4Threading.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
#include <sstream>

secScintSD::secScintSD(const G4String& SDname, const std::vector<G4String> SDHCnameVect) :
    G4VSensitiveDetector(SDname),
    DecayFlagSiPM(false),
    DecayFlagScint(false),
    DecayEventID(0),
    DecayTime(0.),
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
    
}

G4bool secScintSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
//the return value of this function is currently researved and
//it may be used in the future update of Geant4 application.
    
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();
    const G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    auto AnalysisMgr = G4AnalysisManager::Instance();
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
            const G4double GlobalTime = step->GetPreStepPoint()->GetGlobalTime();
            if( VolumeCpyNb == 1 )
                {
                    auto pPhotonHitUp = new secScintHit();
                    pPhotonHitUp->SetPhotonEneg(aPhotonEneg).SetPhotonGlobalTime(GlobalTime);
                    pPhotonHCup->insert(pPhotonHitUp);
                    PhotonsGenUp++;
                    PhotonEnegUp += aPhotonEneg;
                    AnalysisMgr->FillH1(4, aPhotonEneg);
                }
            else if( VolumeCpyNb == 2 )
            {
                auto pPhotonHitDown = new secScintHit();
                pPhotonHitDown->SetPhotonEneg(aPhotonEneg).SetPhotonGlobalTime(GlobalTime);
                pPhotonHCdown->insert(pPhotonHitDown);
                PhotonsGenDown++;
                PhotonEnegDown += aPhotonEneg;
                AnalysisMgr->FillH1(5, aPhotonEneg);
            }
            //update the id
            //FormerID = step->GetTrack()->GetTrackID();
            step->GetTrack()->SetTrackStatus(fStopAndKill);
            return false;
        }
    }
//the case that a muon hit the scintillator
    else if( *ParticleNow == *G4MuonPlus::Definition() ||
             *ParticleNow == *G4MuonMinus::Definition() )
    {
        G4double aStepEdep = step->GetTotalEnergyDeposit();
        G4double GlobalTime = step->GetPreStepPoint()->GetGlobalTime();
        G4double MuonVelocity = step->GetPreStepPoint()->GetVelocity();
        
        if( VolumeCpyNb == 1 )
        {
            MuonEdepUp += aStepEdep;
            auto pMuonHitUp = new secScintHit();
            pMuonHitUp->SetMuonGlobalTime(GlobalTime).SetMuonVelocity(MuonVelocity);
            pMuonHCup->insert(pMuonHitUp);
        }
        else if( VolumeCpyNb == 2 )
        {
            MuonEdepDown += aStepEdep;
            auto pMuonHitDown = new secScintHit();
            pMuonHitDown->SetMuonGlobalTime(GlobalTime).SetMuonVelocity(MuonVelocity);
            pMuonHCdown->insert(pMuonHitDown);
            //step->GetPostStepPoint()->SetKineticEnergy(0.);	    
	}
    }
    else if( *ParticleNow == *G4NeutrinoMu::Definition() || 
	     *ParticleNow == *G4AntiNeutrinoMu::Definition() )
    {
	if( step->IsFirstStepInVolume() )
	{   
	    std::cout << "****Decayed!!****" << '\n';
	    DecayFlagScint = true;
	    DecayTime = step->GetPreStepPoint()->GetGlobalTime();
	    step->GetTrack()->SetTrackStatus(fStopAndKill);
	}
	/*
	if( VolumeCpyNb == 2 )
	{
            if( step->IsFirstStepInVolume() )
            {
		DecayFlagScint = true;
		DecayFlagSiPM  = true;
		DecayTime = step->GetPreStepPoint()->GetProperTime();
	    }
	}
	*/
    }
    
//Neutrino Generated, decay event!
    /*
    else if( *ParticleNow == *G4NeutrinoMu::Definition() ||
             *ParticleNow == *G4AntiNeutrinoMu::Definition() )
    {
        if( VolumeCpyNb == 2)
        {
            DecayFlagScint = true;
            DecayFlagSiPM  = true;
            DecayTime = step->GetPreStepPoint()->GetGlobalTime();
            step->GetTrack()->SetTrackStatus(fStopAndKill);
            //std::cout << "decayed!" << '\n';
        }
    }
    */

/*
    another method of discovering the decay event
    const G4String DecayName = "Decay";
    else if( step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessTypeName() == DecayName )

    if a detailed process name is needed, u can invoke: GetProcessName() method, which is defined
    in "G4VProcess.hh", for instance:
    step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
*/
    return true;
    
}

void secScintSD::EndOfEvent(G4HCofThisEvent*)
{  
    
    if( !pMuonHCup->GetSize() || !pMuonHCdown->GetSize() )
    {
	std::cout << "****no in 1****" << '\n';
	return;
    }
    else if( !pPhotonHCup->GetSize() || !pPhotonHCdown->GetSize() )
    {
	std::cout << "++++no in 2++++" << '\n';
	return;
    }
    
    //fill histograms
    //Analysis Manager and Sensitive Detector Manager.
    auto AnalysisMgr = G4AnalysisManager::Instance();

    //fill the histograms
    AnalysisMgr->FillH1(0, PhotonsGenUp);
    AnalysisMgr->FillH1(1, PhotonsGenDown);

    AnalysisMgr->FillP1(0, MuonEdepUp, PhotonsGenUp);
    AnalysisMgr->FillP1(1, MuonEdepDown, PhotonsGenDown);
    

    //std::cout << "PhotonsGenUp = " << PhotonsGenUp << '\n';
    //std::cout << "PhotonsGenDown = " << PhotonsGenDown << '\n'; 
    //print the decay event
    if( DecayFlagScint )
    {   
        //reset the flag
        DecayFlagScint = false;
        DecayEventID++;
	//std::cout << "Decay Time " << DecayTime << '\n';	
        //PrintHC("ScintMuonDecayEventData", 
        //        pMuonHCup, pMuonHCdown, 
	//	{&secScintHit::GetMuonGlobalTime, &secScintHit::GetMuonVelocity});

        PrintHC("ScintPhotonDecayEventData.dat", 
                pPhotonHCup, pPhotonHCdown, 
		{&secScintHit::GetPhotonGlobalTime});
        
	std::ostringstream sstrm;
        sstrm << "DecayTime" << "_t" << G4Threading::G4GetThreadId();

        std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary );
        fstrm << DecayTime << '\n';

    }

    Reset();
    
}

void secScintSD::PrintHC(G4String FileName, secScintHitsCollection* pHC1, secScintHitsCollection* pHC2, std::initializer_list<secScintHit::DataGetter> GetterLst)
{
    //create files
    std::ostringstream StrStrm;

    StrStrm << FileName << "_t" << G4Threading::G4GetThreadId();
    
    //input data
    std::ofstream fStrm(StrStrm.str(), std::ofstream::app | std::ofstream::binary);
    assert( fStrm.is_open() );
/*
    fStrm << "DecayEventID_t" << G4Threading::G4GetThreadId() << ": " << DecayEventID << '\n';
    //upper Scint's data        
    fStrm << "Upper Scint" << '\n';

    for( size_t i = 0; i != pHC1->GetSize(); ++i )
    {
        for( auto pfunc = GetterLst.begin(); pfunc != GetterLst.end(); ++pfunc)
        {
            fStrm << ( ( (*pHC1)[i] ) ->* (*pfunc) )() << '\t';
        }
        fStrm << '\n' ;
    }
    //lower Scint's data
    fStrm << "Lower Scint" << '\n';
    fStrm << "Decay Time: " << '\n';
    fStrm << DecayTime << '\n';
*/
    for( size_t i = 0; i != pHC2->GetSize(); ++i )
    {
        for( auto pfunc = GetterLst.begin(); pfunc != GetterLst.end(); ++pfunc )
        {
	    G4double val = ( ( (*pHC2)[i] ) ->* (*pfunc) )();
            fStrm.write(reinterpret_cast<char *>( &val ), sizeof(val));
	    //fStrm <<       ( ( (*pHC2)[i] ) ->* (*pfunc) )() << '\t';
        }
        //fStrm << '\n';
    }
    fStrm.flush();
    //close files
    fStrm.close();
}
