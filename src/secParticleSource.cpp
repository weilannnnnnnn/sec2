#include "secParticleSource.hh"
#include "secRandGenFromFile.hh"
#include "secRandGenFromFx.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

#include "G4Event.hh"
#include "G4SystemOfUnit.hh"
#include "Randomize.hh"
#include "globals.hh"

#include <cmath>

class G4ThreeVector;
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
    if( G4UniformRand() > 1.2 / 2.2 )
    {
        ParticleDef = G4MuonPlus::Definition();
    }
    else
    {
        ParticleDef = G4MuonMinus::Definition();
    }

    //generate an energy value
    const G4double NormFact = 46.4488 * GeV; // normalization factor of energy
    G4double KineticEnergy = RandGenFile->Shoot(0, secVRandGen::CDF_TYPE) * NormFact - 105.6 * GeV;

    //generate position
    G4ThreeVector Centre(0, 0, -5.18 * m);

    const G4double Radius = 5 * sqrt(5) * m;
    G4double PosPhi = 2 * 3.141592653589793 * G4UniformRand();
    G4double PosTheta = RandGenFile->Shoot(1, secVRandGen::PDF_TYPE);

    G4ThreeVector DirVect; // direction vector
    //in spherical coordinate system
    DirVect.setMag(Radius);
    DirVect.setTheta(PosTheta);
    DirVect.setPhi(PosPhi);

    G4ThreeVector PosVect(0, 0, -5.18 * m); // position vector
    PosVect += DirVect; // set position
        
    auto vertex = new G4PrimaryVertex(PosVect, 0.);
    auto PrimaryParticle = new G4PrimaryParticle(ParticleDef);
    
    PrimaryParticle->SetKineticEnergy(KineticEnergy);
    PrimaryParticle->SetMass( ParticleDef->GetPDGMass() );
    PrimaryParticle->SetMomentumDirection( ParticleDef->GetPDGCharge() );

    vertex->SetPrimary( PrimaryParticle );

    Evt->AddPrimaryVertex( vertex );

}