#include "secTrackingAction.hh"
#include "secEventAction.hh"
#include "secScintSD.hh"
#include "secSiPMSD.hh"
#include "secParticleSource.hh"
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
	static secParticleSource::secSourceGenType EventType  = secParticleSource::GetEventType();
    if( 1 == aTrack->GetTrackID() && EventType == secParticleSource::Muons ) //muon track
    {   
        if( !pScintSD->IsKeptEvent() )
		{    
            G4MTRunManager::GetRunManager()->AbortEvent();
		}
		else
			std::cout << "Event Saved!" << std::endl;
    }
}
