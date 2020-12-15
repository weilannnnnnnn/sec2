#include "secPhysicsList.hh"

#include "secOpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//ctor
secPhysicsList::secPhysicsList(void) : 
G4VModularPhysicsList()
{
    auto pOptical = OpticalPhysics_init();
    RegisterPhysics( pOptical );

    //G4SpinDecayPhysics depends on G4DecayPhysics.
    RegisterPhysics( new G4DecayPhysics() );
    RegisterPhysics( new G4SpinDecayPhysics() );
    RegisterPhysics( new G4EmStandardPhysics() );
    RegisterPhysics( new G4EmPenelopePhysics() );
    RegisterPhysics( new G4StepLimiterPhysics() );
}

//dtor
secPhysicsList::~secPhysicsList()
{
    
}

void secPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
}


secOpticalPhysics* secPhysicsList::OpticalPhysics_init()
{
    auto pOptical = new secOpticalPhysics;

    pOptical->SetTrackSecondariesFirst(kScintillation, false);
    pOptical->SetTrackSecondariesFirst(kCerenkov,      false);
    pOptical->SetTrackSecondariesFirst(kWLS,           false);

//Scintillation
    pOptical->SetScintillationYieldFactor(1.);
    pOptical->SetScintillationExcitationRatio(0.);

//Cerenkov light
    pOptical->SetMaxNumPhotonsPerStep(50);

//wave length shifting (WLS)
    pOptical->SetWLSTimeProfile("delta");

    return pOptical;
}
