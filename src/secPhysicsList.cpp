#include "secPhysicsList.hh"

#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//ctor
secPhysicsList::secPhysicsList(void) : 
G4VModularPhysicsList()
{
    auto pOptical = OpticalPhysics_init();
    auto pDecay   = DecayPhysics_init();
    auto pEm      = EmPhysics_init();
    auto pHadron  = HadronElasticPhysics_init();
    auto pIon     = IonPhysics_init();
    auto pLimiter = StepLimiter_init();

    RegisterPhysics( pOptical );
    RegisterPhysics( pDecay );
    RegisterPhysics( pEm );
    RegisterPhysics( pHadron );
    RegisterPhysics( pIon );
    RegisterPhysics( pLimiter );
}

//dtor
secPhysicsList::~secPhysicsList()
{
    
}

void secPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
    
    //set the production cuts of muons in the down scintillator
    auto pRegion = G4RegionStore::GetInstance()->GetRegion("sci_reg2");
    SetParticleCuts(0., G4MuonPlus::Definition(), pRegion);
    SetParticleCuts(0., G4MuonMinus::Definition(), pRegion); 
}


G4OpticalPhysics* secPhysicsList::OpticalPhysics_init()
{
    auto pOptical = new G4OpticalPhysics;

    pOptical->SetTrackSecondariesFirst(kScintillation, true);
    pOptical->SetTrackSecondariesFirst(kCerenkov,      true);
    pOptical->SetTrackSecondariesFirst(kWLS,           true);

//Scintillation
    pOptical->SetScintillationYieldFactor(1.);
    pOptical->SetScintillationExcitationRatio(0.);

//Cerenkov light
    pOptical->SetMaxNumPhotonsPerStep(50);

//wave length shifting (WLS)
    pOptical->SetWLSTimeProfile("delta");

    return pOptical;
}

G4DecayPhysics* secPhysicsList::DecayPhysics_init(void)
{
    auto pDecay = new G4DecayPhysics;

    return pDecay;
}

G4EmStandardPhysics* secPhysicsList::EmPhysics_init(void)
{
    auto pEm = new G4EmStandardPhysics;

    return pEm;
}

G4HadronElasticPhysics* secPhysicsList::HadronElasticPhysics_init(void)
{
    auto pHadron = new G4HadronElasticPhysics;

    return pHadron;
}

G4IonPhysics* secPhysicsList::IonPhysics_init(void)
{
    auto pIon = new G4IonPhysics;

    return pIon;
}

G4StepLimiterPhysics* secPhysicsList::StepLimiter_init(void)
{
    auto pLimiter = new G4StepLimiterPhysics;

    return pLimiter;
}
