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

#include "G4VProcess.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4NeutrinoMu.hh"
#include "G4Threading.hh"
#include <iostream>
#include <fstream>
#include <sstream>

//constructor
secSteppingAction::secSteppingAction(secEventAction* eventAction) :
    G4UserSteppingAction(),
    EventAction(eventAction)
{

}

//destructor
secSteppingAction::~secSteppingAction()
{/*nothing here*/}


void secSteppingAction::UserSteppingAction(const G4Step* step )
{
/*  
    //print the decay Event
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();
    if( *ParticleNow == *G4MuonPlus::Definition() ||
        *ParticleNow == *G4MuonMinus::Definition() )
    {
	if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == G4ProcessType::fDecay )
	{
	    std::ostringstream sstrm;
	    sstrm << "DecayTime.dat" << "_t" << G4Threading::G4GetThreadId();

	    std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary );
	    fstrm << step->GetPostStepPoint()->GetGlobalTime() << '\n';

            //std::cout << "DecayTime = " << step->GetPostStepPoint()->GetGlobalTime() << '\n';
	    //step->GetTrack()->SetTrackStatus(fStopAndKill);
	}
    }
*/
}

