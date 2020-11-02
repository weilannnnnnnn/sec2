#include "secTrackingAction.hh"
#include "secEventAction.hh"
#include "secScintSD.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4SDManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

secTrackingAction::secTrackingAction(secEventAction* eventAction):
    G4UserTrackingAction(),
    EventAction(eventAction)
{
    pScintSD = (secScintSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector("ScintSD");
}

secTrackingAction::~secTrackingAction()
{

}

void secTrackingAction::PreUserTrackingAction(const G4Track*)
{

}

void secTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    if( 0 == aTrack->GetParentID() ) //muon track
    {
        if( !pScintSD->IsKeptEvent() )
            G4RunManager::GetRunManager()->AbortEvent();
    }
}
