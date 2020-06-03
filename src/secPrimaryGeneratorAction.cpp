#include "secPrimaryGeneratorAction.hh"

#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSPosDistribution.hh"

#include "G4RunManager.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#define PI 3.14159256359

//ctor
secPrimaryGeneratorAction::secPrimaryGeneratorAction(void) :
    G4VUserPrimaryGeneratorAction(),
    pMuonGun(nullptr),
    mu_p(nullptr),
    mu_n(nullptr)
{
    pMuonGun = new G4GeneralParticleSource();//one particle at a time
    /*
    PosMgr = new G4SPSPosDistribution();
    EnegMgr = new G4SPSEneDistribution();
    AngMgr = new G4SPSAngDistribution();
    */
    auto table = G4ParticleTable::GetParticleTable();
    mu_p = table -> FindParticle("mu+");
    mu_n = table -> FindParticle("mu-");
}

//dtor
secPrimaryGeneratorAction::~secPrimaryGeneratorAction()
{
    delete pMuonGun;
    /*
    delete PosMgr;
    delete EnegMgr;
    delete AngMgr;
    */
}

void secPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
    	
    //charge ratio
    if(G4UniformRand() > 1.2 / 2.2)
    {
        pMuonGun -> SetParticleDefinition(mu_p);
    }
    else
    {
        pMuonGun -> SetParticleDefinition(mu_n);
    }
    /*
    pMuonGun->SetParticleEnergy(10*GeV);
    pMuonGun->SetParticlePosition(G4ThreeVector(0., 0., 10*cm));
    pMuonGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
    */
    /*
    //Position distribution
    //auto PosMgr = new G4SPSPosDistribution();
    PosMgr->SetPosDisType("Volume");
    PosMgr->SetPosDisShape("Sphere");
    PosMgr->SetRadius(10*cm);
    PosMgr->SetCentreCoords(G4ThreeVector());
    pMuonGun->SetParticlePosition(PosMgr->GenerateOne());
    

    //Energy distribution
    //G4SPSEneDistribution EnegMgr;
    EnegMgr->SetEnergyDisType("Pow");
    EnegMgr->SetAlpha(-2.75);
    EnegMgr->SetEmin(100 * MeV);
    EnegMgr->SetEmax(1000 * GeV);
    pMuonGun->SetParticleEnergy(EnegMgr->GenerateOne(mu_p));
    */
    /*
    //Angular distribution
    //G4SPSAngDistribution AngMgr;
    AngMgr->SetMinTheta(PI / 2.);
    AngMgr->SetMaxTheta(PI);
    AngMgr->SetAngDistType("cos");
    pMuonGun->SetParticleMomentumDirection(AngMgr->GenerateOne());
    */   
    pMuonGun -> GeneratePrimaryVertex(anEvent);
}
