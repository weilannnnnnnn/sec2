#include "secParticleSource.hh"
#include "secRandGenFromFile.hh"
#include "secRandGenFromFx.hh"
#include "secRandMacro.hh"
#include "secSourceMacro.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"
#include "G4Alpha.hh"
#include "G4Geantino.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "Randomize.hh"
#include "globals.hh"

#include <iostream>
#include <cmath>
#include <fstream>
#include <atomic>

class G4PrimaryVertex;
class G4PrimaryParticle;
secParticleSource::secSourceGenType secParticleSource::GenTypeNow = secParticleSource::secSourceGenType::Muons;

secParticleSource::secParticleSource():
    AlphaEneg(1*MeV),
    BetaAlphaRatio(1./3.),
    SrcCentre(G4ThreeVector(0.,0.,0.)),
    SrcSize(G4ThreeVector(0.,0.,0.))
{
    //random generators
    RandGenFile = secRandGenFromFile::GetInstance();
    RandGenFx   = secRandGenFromFx::GetInstance();
    //initialize noise WaitTime list. 
    srcMac = new secSourceMacro(this);
}

secParticleSource::~secParticleSource()
{
}

void secParticleSource::GeneratePrimaryVertex(G4Event* Evt)
{
    GenAnEvent(Evt, secParticleSource::GenTypeNow);
}

void secParticleSource::GenAnEvent(G4Event* pEvt, secSourceGenType GenType)
{
    switch( GenType )
    {
        case Muons : 
        {
            GenMuons(pEvt);
	    break;
        }
        case NoiseBeta :
        {
            GenNoiseBeta(pEvt);
	    break;
        }
        case NoiseAll :
        {
            //unfinished
            GenNoiseAll(pEvt, AlphaEneg, BetaAlphaRatio);
	    break;
        }
        default :
        {
            GenMuons(pEvt);
        }
    }
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
    PrimaryParticle->SetMomentumDirection( /*G4ThreeVector(0, 0,WaitTime 1)*/DirVect.unit() );
    PrimaryParticle->SetMass( ParticleDef->GetPDGMass() );
    PrimaryParticle->SetCharge( ParticleDef->GetPDGCharge() );

    vertex->SetPrimary( PrimaryParticle );

    Evt->AddPrimaryVertex( vertex );
}
G4ThreeVector secParticleSource::GenNoisePos()
{
    const G4double X = (2 * G4UniformRand() - 1) * SrcSize.x() + SrcCentre.x();
    const G4double Y = (2 * G4UniformRand() - 1) * SrcSize.y() + SrcCentre.y();
    const G4double Z = (2 * G4UniformRand() - 1) * SrcSize.z() + SrcCentre.z();

    return G4ThreeVector(X, Y, Z);
}
void secParticleSource::GenNoiseAlpha(G4Event* Evt, G4double Eneg)
{
    auto ParDef = G4Alpha::Definition();
    //generate direction
    G4ThreeVector DirVect(0, 1, 0);
    G4ThreeVector PosVect = GenNoisePos();
    
    G4double Theta = G4UniformRand() * 3.141592653589793;
    G4double Phi   = G4UniformRand() * 3.141592653589793 * 2.;
    DirVect.setTheta( Theta );
    DirVect.setPhi( Phi );

    auto vertex = new G4PrimaryVertex( PosVect, 0. );
	auto PriPar = new G4PrimaryParticle( ParDef );

    PriPar->SetKineticEnergy( Eneg );
    PriPar->SetMomentumDirection( DirVect.unit() );
    PriPar->SetMass( ParDef->GetPDGMass() );
    PriPar->SetCharge( ParDef->GetPDGCharge() );

    vertex->SetPrimary( PriPar );
    Evt->AddPrimaryVertex( vertex );

}

void secParticleSource::GenNoiseBeta(G4Event* Evt)
{
    //update the noise wait time list
    const G4int ThreadID = G4Threading::G4GetThreadId();
    (void) GenNoiseWaitTime(ThreadID, true, true, -1.);
    G4ParticleDefinition* ParDef = G4Electron::Definition();
    //generate energy sample.
    G4double Eneg = RandGenFile->Shoot(1, secVRandGen::PDF_TYPE) * MeV;

    //generate direction
    G4ThreeVector DirVect(0, 1, 0);
    G4ThreeVector PosVect = GenNoisePos();
    
    G4double Theta = G4UniformRand() * 3.141592653589793;
    G4double Phi   = G4UniformRand() * 3.141592653589793 * 2.;
    DirVect.setTheta( Theta );
    DirVect.setPhi( Phi );
    
    auto vertex = new G4PrimaryVertex( PosVect, 0. );
	auto PriPar = new G4PrimaryParticle( ParDef );

    PriPar->SetKineticEnergy( Eneg );
    PriPar->SetMomentumDirection( DirVect.unit() );
    PriPar->SetMass( ParDef->GetPDGMass() );
    PriPar->SetCharge( ParDef->GetPDGCharge() );

    vertex->SetPrimary( PriPar );
    Evt->AddPrimaryVertex( vertex );
}

void secParticleSource::GenNoiseAll(G4Event* Evt, G4double Eneg, G4double BARatio)
{
    if( G4UniformRand() < 1. / (1. + BARatio) )
    {
        GenNoiseBeta(Evt);
    }
    else
    {
        GenNoiseAlpha(Evt, Eneg);
    }    
}

G4double secParticleSource::MuonWaitTime()
{
    //generate muons events' wait time.
    static std::atomic<G4double> MuonWaitTime(0.);
    
    return ( MuonWaitTime = MuonWaitTime + CLHEP::RandExponential::shoot(1./385.04)*s );
}

G4double secParticleSource::GenNoiseWaitTime( G4int ThreadID, G4bool IsInit, 
                                              G4bool IsUpdate, G4double Inten )
{
    static const size_t EventNum = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
    static const size_t ThreadNum = G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads();
	static G4double* LocalWaitTimePtr = new G4double[ThreadNum]; // save the wait time of each thread.
    NoiseIntensity = Inten;
    //initialization part, invoked in secRunAction::BeginOfRunAction()
    if( !IsInit )
        return -1.;
        //initialization completed.

    if( IsUpdate )
    {
        //update the global wait time pointer. Invoked in GenNoiseBeta()
        LocalWaitTimePtr[ThreadID] = CLHEP::RandFlat::shoot(0., EventNum / NoiseIntensity);
        return -1;
    }
    //get the noise time stamp of this event, invoked in secSiPMSD::ProcessHits()
    return ( LocalWaitTimePtr[ThreadID]*s );
}
