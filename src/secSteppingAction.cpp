#include "secSteppingAction.hh"
#include "secEventAction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4NeutrinoMu.hh"

class G4OpticalPhoton;
class G4ParticleDefinition;

//constructor
secSteppingAction::secSteppingAction(secEventAction* eventAction) :
    G4UserSteppingAction(),
    EventAction(eventAction)
{

}

//destructor
secSteppingAction::~secSteppingAction()
{/*nothing here*/}


void secSteppingAction::UserSteppingAction(const G4Step*)
{/*
    const G4VPhysicalVolume* VolumeNow = step->GetPreStepPoint()->GetPhysicalVolume();
    const G4ParticleDefinition* ParticleNow = step->GetTrack()->GetParticleDefinition();

    static const G4VPhysicalVolume* SiPMPV1 = G4PhysicalVolumeStore::GetInstance()->GetVolume("SiPM1");
    static const G4VPhysicalVolume* SiPMPV2 = G4PhysicalVolumeStore::GetInstance()->GetVolume("SiPM2");
    
    if( VolumeNow == SiPMPV1 )
    {
        if( *ParticleNow == *G4OpticalPhoton::Definition() )
        {   //case that photons reach the surface of the upper SiPM
            //kill the photons reached the SiPM1!
	        EventAction->AddPhotonsRecvUp();
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
    }
    else if( VolumeNow == SiPMPV2 )
    {
        if( *ParticleNow == *G4OpticalPhoton::Definition() )
        {   //case that the photons reach the surface of the lower SiPM
            //kill the photons reached the SiPM2!
	        EventAction->AddPhotonsRecvDown();
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
    }
*/
}
