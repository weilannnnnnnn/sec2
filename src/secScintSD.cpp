#include "secScintSD.hh"
#include "secAnalysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "globals.hh"
#include <iostream>

secScintSD::secScintSD(const G4String& SDname, const std::vector<G4String> SDHCnameVect) :
    G4VSensitiveDetector(SDname),
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
    G4double ParticlePosZ = step->GetPreStepPoint()->GetPosition().getZ();
    //G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    auto AnalysisMgr = G4AnalysisManager::Instance();
    //the case that a opticalphoton generated in the scintillator
    
    if( *ParticleNow == *G4NeutrinoMu::Definition() || *ParticleNow == *G4AntiNeutrinoMu::Definition() )
    {
	if( ParticlePosZ < 0 )
	    std::cout << "Decayed!" << '\n';
    }
    
    if( *ParticleNow == *G4OpticalPhoton::Definition() )
    {
	 if( step->GetTrack()->GetTrackID() != FormerID )
	 {
	     const G4double aPhotonEneg = step->GetPreStepPoint()->GetKineticEnergy();
	     if( ParticlePosZ > 0 )
             {
                 PhotonsGenUp++;
                 PhotonEnegUp += aPhotonEneg;
	       	 AnalysisMgr->FillH1(4, aPhotonEneg);
	     }
             else
             {
                 PhotonsGenDown++;
                 PhotonEnegDown += aPhotonEneg;
	   	 AnalysisMgr->FillH1(5, aPhotonEneg);
             }
	     //step->GetTrack()->SetTrackStatus(fStopAndKill);
    	     FormerID = step->GetTrack()->GetTrackID();
	     return false;
	 }
    }
    //the case that a muon hit the scintillator
    else if( *ParticleNow == *G4MuonPlus::Definition() ||
             *ParticleNow == *G4MuonMinus::Definition() )
    {
        G4double MuonEneg = step->GetTotalEnergyDeposit();
        if( ParticlePosZ > 0 )
        {
            MuonEdepUp += MuonEneg;
        }
        else
        {
            MuonEdepDown += MuonEneg;
        }
    }
    return true;
    
}

void secScintSD::EndOfEvent(G4HCofThisEvent*)
{   
    
        std::cout << 1.*CLHEP::ns << '\n'; 
        //create new hits
        auto PhotonHitUp   = new secScintHit();
        auto PhotonHitDown = new secScintHit();
        auto MuonHitUp     = new secScintHit();
        auto MuonHitDown   = new secScintHit();
        
        //set the parameters of hits
        PhotonHitUp->SetGenPhotons(PhotonsGenUp).SetPhotonEneg(PhotonEnegUp);
        PhotonHitDown->SetGenPhotons(PhotonsGenDown).SetPhotonEneg(PhotonEnegDown);

        MuonHitUp->SetMuonEdep(MuonEdepUp);
        MuonHitDown->SetMuonEdep(MuonEdepDown);
        
        //save the hits
        pPhotonHCup->insert(PhotonHitUp);
        pPhotonHCdown->insert(PhotonHitDown);
        pMuonHCup->insert(MuonHitUp);
        pMuonHCdown->insert(MuonHitDown);

        //fill histograms
        //Analysis Manager and Sensitive Detector Manager.
        auto AnalysisMgr = G4AnalysisManager::Instance();

        PhotonsGenUp   = ((*pPhotonHCup)  [ pPhotonHCup->GetSize()-1 ])  ->GetGenPhotons();
        PhotonsGenDown = ((*pPhotonHCdown)[ pPhotonHCdown->GetSize()-1 ])->GetGenPhotons();
        PhotonEnegUp   = ((*pPhotonHCup)  [ pPhotonHCup->GetSize()-1 ])  ->GetPhotonEneg();
        PhotonEnegDown = ((*pPhotonHCdown)[ pPhotonHCdown->GetSize()-1 ])->GetPhotonEneg();
        MuonEdepUp     = ((*pMuonHCup)    [ pMuonHCup->GetSize()-1 ])    ->GetMuonEdep();
        MuonEdepDown   = ((*pMuonHCdown)  [ pMuonHCdown->GetSize()-1 ])  ->GetMuonEdep();
        
        /*	
	std::cout << '\n' << "PhotonsGenUp = " << PhotonsGenUp << '\n';
	std::cout << "PhotonsGenDown = " <<PhotonsGenDown << '\n';
	std::cout << "PhotonEnegUp = " << PhotonEnegUp << '\n';
	std::cout << "PhotonEnegDown = " << PhotonEnegDown << '\n';
	std::cout << "MuonEdepUp = " << MuonEdepUp << '\n';
	std::cout << "MuonEdepDown = " << MuonEdepDown << '\n' << '\n';
        */

	//fill the histograms
        AnalysisMgr->FillH1(0, PhotonsGenUp);
        AnalysisMgr->FillH1(1, PhotonsGenDown);

        AnalysisMgr->FillP1(0, MuonEdepUp, PhotonsGenUp);
        AnalysisMgr->FillP1(1, MuonEdepDown, PhotonsGenDown);
    
        Reset();
    
}
