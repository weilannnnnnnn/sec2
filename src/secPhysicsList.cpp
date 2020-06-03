#include "secPhysicsList.hh"

#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"


//ctor
secPhysicsList::secPhysicsList(void) : 
G4VModularPhysicsList()
{
    auto pOptical = OpticalPhysics_init();
    auto pDecay   = DecayPhysics_init();
    auto pEm      = EmPhysics_init();
    auto pHadron  = HadronElasticPhysics_init();
    auto pIon     = IonPhysics_init();

    RegisterPhysics( pOptical );
    RegisterPhysics( pDecay );
    RegisterPhysics( pEm );
    RegisterPhysics( pHadron );
    RegisterPhysics( pIon );
}

//dtor
secPhysicsList::~secPhysicsList()
{
    
}

void secPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
}

inline
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
    pOptical->SetMaxNumPhotonsPerStep(300);

//wave length shifting (WLS)
    pOptical->SetWLSTimeProfile("delta");

//Boundary( Rayleigh/Mie Scattering)

    return pOptical;
}
inline
G4DecayPhysics* secPhysicsList::DecayPhysics_init(void)
{
    auto pDecay = new G4DecayPhysics;

    return pDecay;
}
inline
G4EmStandardPhysics* secPhysicsList::EmPhysics_init(void)
{
    auto pEm = new G4EmStandardPhysics;

    return pEm;
}
inline
G4HadronElasticPhysics* secPhysicsList::HadronElasticPhysics_init(void)
{
    auto pHadron = new G4HadronElasticPhysics;

    return pHadron;
}
inline
G4IonPhysics* secPhysicsList::IonPhysics_init(void)
{
    auto pIon = new G4IonPhysics;

    return pIon;
}
