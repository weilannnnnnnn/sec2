#include "secTrackingAction.hh"
#include "secEventAction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

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

void secTrackingAction::PostUserTrackingAction(const G4Track*)
{

}
