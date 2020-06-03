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

void secTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    auto ParticleNow = track->GetParticleDefinition();
    auto VolumeNow = track->GetVolume();

    static const G4VPhysicalVolume* ScintPV1 = G4PhysicalVolumeStore::GetInstance()->GetVolume("sci_phy1");
    static const G4VPhysicalVolume* ScintPV2 = G4PhysicalVolumeStore::GetInstance()->GetVolume("sci_phy2");
    
    if( *ParticleNow == *G4OpticalPhoton::Definition() )
    {
        if( VolumeNow == ScintPV1 )
        {//photons generated in the upper scintillator
            EventAction->AddPhotonsGenUp();
        }
        else if( VolumeNow == ScintPV2 )
        {//photons generated in the lower scintillator
            EventAction->AddPhotonsGenDown();
	}
    }
}

void secTrackingAction::PostUserTrackingAction(const G4Track*)
{

}
