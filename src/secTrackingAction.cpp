#include "secTrackingAction.hh"
#include "secEventAction.hh"
#include "secSiPMSD.hh"
#include "secScintSD.hh"

//#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
//#include "G4WorkerRunManager.hh"
#include "G4SDManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include <iostream>

secTrackingAction::secTrackingAction(secEventAction* eventAction):
    G4UserTrackingAction(),
    EventAction(eventAction)
{
}

secTrackingAction::~secTrackingAction()
{

}

void secTrackingAction::PreUserTrackingAction(const G4Track*)
{

}

void secTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
	
    static thread_local secScintSD* pScintSD = (secScintSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector("ScintSD");
	if( 1 == aTrack->GetTrackID() ) //muon track
    {   
        if( !pScintSD->IsKeptEvent() )
			G4MTRunManager::GetRunManager()->AbortEvent();
    }
}
