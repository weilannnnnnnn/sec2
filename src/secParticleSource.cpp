#include "secParticleSource.hh"
#include "secRandGenFromFile.hh"
#include "secRandGenFromFx.hh"
#include "secRandMacro.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Geantino.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

#include <cmath>
#include <fstream>
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
    /* 
    std::ofstream ofstrm;
    ofstrm.open("SampleFile.txt");

    for( G4int i = 0; i != 10000; ++i )
    {
        ofstrm << RandGenFile->Shoot(0, secVRandGen::CDF_TYPE) << ' ' << RandGenFile->Shoot(0, secVRandGen::PDF_TYPE) << '\n';
    }
    */
    //generate an energy value
    G4double KineticEnergy = RandGenFile->Shoot(0, secVRandGen::CDF_TYPE) * GeV;
    //std::cout << "Energy = " << KineticEnergy / 1000.<< " GeV" << std::endl;     
    //generate position
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
    
    auto vertex = new G4PrimaryVertex( /*G4ThreeVector(0, 0, 0)*/PosVect, 0. );
    auto PrimaryParticle = new G4PrimaryParticle( ParticleDef );
    
    PrimaryParticle->SetKineticEnergy( KineticEnergy );
    PrimaryParticle->SetMomentumDirection( /*G4ThreeVector(0, 0, 1)*/DirVect.unit() );
    PrimaryParticle->SetMass( ParticleDef->GetPDGMass() );
    PrimaryParticle->SetCharge( ParticleDef->GetPDGCharge() );

    vertex->SetPrimary( PrimaryParticle );

    Evt->AddPrimaryVertex( vertex );

}
