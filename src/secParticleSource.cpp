#include "secParticleSource.hh"
#include "secRandGenFromFile.hh"
#include "secRandGenFromFx.hh"
#include "secRandMacro.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"
#include "G4Geantino.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

#include <iostream>
#include <cmath>
#include <fstream>
#include <atomic>

class G4PrimaryVertex;
class G4PrimaryParticle;

secParticleSource::secParticleSource()  
{
    //random generators
    RandGenFile = secRandGenFromFile::GetInstance();
    RandGenFx   = secRandGenFromFx::GetInstance();

}

secParticleSource::~secParticleSource()
{
}

void secParticleSource::GeneratePrimaryVertex(G4Event* Evt)
{
    //No generation option, which will be added in the future.
    //GenMuons(Evt);
    GenNoiseBeta(Evt);
}

void secParticleSource::GenMuons(G4Event* Evt)
{
    G4ParticleDefinition* ParticleDef = nullptr;

    //determine particle definition
    //the charge ratio of mu+ and mu- is about 1.2 : 1
    //ParticleDef = G4Geantino::Definition();
   
    if( G4UniformRand() > 1.2 / 2.2 )
    {
        ParticleDef = G4MuonPlus::Definition();
    }
    else
    {
        ParticleDef = G4MuonMinus::Definition();
    }

    //generate an energy value
    G4double KineticEnergy = RandGenFile->Shoot(0, secVRandGen::CDF_TYPE) * GeV;
    //std::cout << "Energy = " << KineticEnergy / 1000.<< " GeV" << std::endl;     
    //generate position
    G4double WaitTime = MuonWaitTime(); 
    const G4double Radius = 2 * sqrt(5) * m;
    const G4double PosPhi = 2 * 3.141592653589793 * G4UniformRand();
    const G4double PosTheta = RandGenFile->Shoot(0, secVRandGen::PDF_TYPE);
 
    G4ThreeVector DirVect(0, 0, -1); // direction vector
    //in spherical coordinate system
    DirVect.setMag( Radius );
    DirVect.setTheta( PosTheta );
    DirVect.setPhi( PosPhi );

    G4ThreeVector PosVect(0, 0, 6*m - Radius); // position vector
    PosVect += DirVect; // set position
    DirVect = - DirVect;
    
    auto vertex = new G4PrimaryVertex( /*G4ThreeVector(0, 0, 0)*/PosVect, WaitTime );
    auto PrimaryParticle = new G4PrimaryParticle( ParticleDef );
    
    PrimaryParticle->SetKineticEnergy( KineticEnergy );
    PrimaryParticle->SetMomentumDirection( /*G4ThreeVector(0, 0,WaitTime 1)*/DirVect.unit() );
    PrimaryParticle->SetMass( ParticleDef->GetPDGMass() );
    PrimaryParticle->SetCharge( ParticleDef->GetPDGCharge() );

    vertex->SetPrimary( PrimaryParticle );

    Evt->AddPrimaryVertex( vertex );
}

void secParticleSource::GenNoiseBeta(G4Event* Evt)
{
    G4ParticleDefinition* ParDef = G4Electron::Definition();
    //generate energy sample.
    G4double Eneg = RandGenFile->Shoot(2, secVRandGen::PDF_TYPE) * MeV;

    //generate direction
    G4ThreeVector DirVect(0, 1, 0);
    G4ThreeVector PosVect(0, 1, 0);
    
    G4double Theta = G4UniformRand() * 3.141592653589793;
    G4double Phi   = G4UniformRand() * 3.141592653589793 * 2.;
    DirVect.setTheta( Theta );
    DirVect.setPhi( Phi );
    
    //generate position
    //G4double X = G4UniformRand() * 20. * m - 10. * m;
    G4double X = G4UniformRand() * m - 0.5 * m;
    //G4double Y = G4UniformRand() * 20. * m - 10. * m;
    G4double Y = G4UniformRand() * m - 0.5 * m;
    //G4double Z = G4UniformRand() * 12. * m - 6. * m;
    G4double Z = G4UniformRand() * m - 0.5*m + 1.528 * m;

    PosVect.setX( X );
    PosVect.setY( Y );
    PosVect.setZ( Z );

    auto vertex = new G4PrimaryVertex(PosVect, 0.);
    auto PriPar = new G4PrimaryParticle( ParDef );

    PriPar->SetKineticEnergy( Eneg );
    PriPar->SetMomentumDirection( DirVect.unit() );
    PriPar->SetMass( ParDef->GetPDGMass() );
    PriPar->SetCharge( ParDef->GetPDGCharge() );

    vertex->SetPrimary( PriPar );
    Evt->AddPrimaryVertex( vertex );
}

G4double secParticleSource::MuonWaitTime()
{
    //generate muons events' wait time.
    static std::atomic<G4double> MuonWaitTime(0.);
    
    return ( MuonWaitTime = MuonWaitTime + CLHEP::RandExponential::shoot(0.5)*s );
}

G4double secParticleSource::NoiseWaitTime()
{
    //generate noise particle's time stamp.
    static std::atomic_flag IsInit(ATOMIC_FLAG_INIT);
    static G4double* NoiseWaitTimeArr = nullptr;
    static size_t NoiseIdx = 0;
    //if the time stamp hasn't been generated, initialize.
    if( ! IsInit.test_and_set() )
    {
        const size_t NoiseNum = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
        NoiseWaitTimeArr = new G4double[NoiseNum]; // last until the program ends.
        const G4double NoiseInten = 100; // Becquerel, Bq
        for( size_t i = 0; i != NoiseNum; ++i )
        {
            const G4double time =  CLHEP::RandFlat::shoot(0, NoiseNum / NoiseInten);
            NoiseWaitTimeArr[i] = time;
        }
    }
    return ( NoiseWaitTimeArr[NoiseIdx++] );
}
