#include "secSiPMSD.hh"
#include "secScintSD.hh"
#include "secAnalysis.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Threading.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicalVolumeStore.hh"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
class G4VPhysicalVolume;

secSiPMSD::secSiPMSD(const G4String &SDname, const std::vector<G4String> SDHCnameVect, secScintSD* pSD) : 
    G4VSensitiveDetector(SDname),
    DecayEventID(0),
    pScintSD(pSD),
    pHCup(nullptr),
    pHCdown(nullptr)
{
/* register the names of the HCs to G4VSensitiveDetector object so
    that the HCids which are assigned by G4 Kenel could be assessed by
    G4SDManager object */
    for(G4String str : SDHCnameVect)
    {
        collectionName.insert(str);
    }
}

secSiPMSD::~secSiPMSD()
{

}

void secSiPMSD::Initialize(G4HCofThisEvent* HC)
{
    //SensitiveDetectorName is a string defined in G4VSensitiveDetector.hh
    //collectionName is a STL vector defined in G4CollectionVector.hh

    //create HCs
    auto SDmgr = G4SDManager::GetSDMpointer();
    pHCup   = new secSiPMHitsCollection(SensitiveDetectorName, collectionName[0]);
    pHCdown = new secSiPMHitsCollection(SensitiveDetectorName, collectionName[1]);

    //Get collection IDs from the G4SDManager object, and register the created HCs to G4HCofThisEvent object.
    G4int HCIDup   = SDmgr->GetCollectionID(collectionName[0]);
    G4int HCIDdown = SDmgr->GetCollectionID(collectionName[1]);

    HC->AddHitsCollection(HCIDup, pHCup);
    HC->AddHitsCollection(HCIDdown, pHCdown);
}

G4bool secSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory* )
{
//the return value of this function is currently researved and
//it may be used in the future update of Geant4 application.
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();    
    if( *ParticleNow != *G4OpticalPhoton::Definition() )
    {
        //not an optical photon, return, don't generate hit
        return false;
    }

//the case that an optical photon hits one of the SiPM
//the copy number of the UPPER SiPM is 1 and that of the lower one is 2!!
    const G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4double aPhotonEneg = step->GetPreStepPoint()->GetKineticEnergy(); //MeV
    G4double GlobalTime  = step->GetTrack()->GetGlobalTime();
    if( VolumeCpyNb == 1 )
    {
        //upper SiPM!!
        auto newHitUp = new secSiPMHit();
        (*newHitUp).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCup->insert(newHitUp);
    }
    else if( VolumeCpyNb == 2 )
    {
        //lower SiPM!!
        auto newHitDown = new secSiPMHit();
        (*newHitDown).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCdown->insert(newHitDown);
    }
    else
    {
        //do nothing.
    }
    //the photons is absorbed by the SiPM, so the track must be killed
    step->GetTrack()->SetTrackStatus(fStopAndKill);
    return true;
}

void secSiPMSD::EndOfEvent(G4HCofThisEvent*)
{
    
    if( !(pHCup->GetSize()) || !(pHCdown->GetSize()) )
    {
        //ignore the event that didn't trigger both of the SiPMs!
	return;
    }
 
    else//fill histograms
    {
        auto AnalysisMgr = G4AnalysisManager::Instance();
        //get number of photons and fill
        AnalysisMgr->FillH1(2, pHCup->GetSize());
        AnalysisMgr->FillH1(3, pHCdown->GetSize());
        for( size_t i = 0; i != pHCup->GetSize(); ++i )
        {
            AnalysisMgr->FillH1(6, ((*pHCup)[i])->GetPhotonEneg() );
        }
        for( size_t i = 0; i != pHCdown->GetSize(); ++i )
        {
            AnalysisMgr->FillH1(7, ((*pHCdown)[i])->GetPhotonEneg() );
        }
    }
    
    //digitization and fill the histograms
    //create the Decay Event's files
    if( IsADecayEvent() )
    {
	//std::cout << "decayed!!" << '\n';
        ResetDecayFlag();
    //create files
        std::ostringstream StrStrm;

        StrStrm << "DecayEventData" << "_t" << G4Threading::G4GetThreadId();
        
    //input data
        std::ofstream fStrm(StrStrm.str(), std::ofstream::app);
        assert( fStrm.is_open() );
        
        fStrm << "DecayEventID_t" << G4Threading::G4GetThreadId() << ": " << ++DecayEventID << '\n';
        //upper SiPM's data        
        fStrm << "Upper SiPM" << '\n';
        fStrm << "Global time" << '\t' << "Photon Energy" <<'\n';
        for( size_t i = 0; i != pHCup->GetSize(); ++i )
        {
            fStrm << ( (*pHCup)[i] )->GetGlobalTime() << '\t' 
		  << ( (*pHCup)[i] )->GetPhotonEneg() << '\n';
        }
        //lower SiPM's data
        fStrm << "Lower SiPM" << '\n';
        fStrm << "Global time" << '\t' << "Photon Energy" <<'\n';
        for( size_t i = 0; i != pHCdown->GetSize(); ++i )
        {
            fStrm << ( (*pHCdown)[i] )->GetGlobalTime() << '\t' 
		  << ( (*pHCdown)[i] )->GetPhotonEneg() << '\n';
        }
    //close files
        fStrm.close();
    }
}

G4bool secSiPMSD::IsADecayEvent()
{
    return pScintSD->aDecayEvent;    
}
void secSiPMSD::ResetDecayFlag()
{
    pScintSD->aDecayEvent = false;
}
