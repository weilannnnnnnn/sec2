#include "secPrimaryGeneratorAction.hh"
#include "secAnalysis.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSPosDistribution.hh"

#include "G4RunManager.hh"

#include "G4ParticleGun.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iostream>
#define PI 3.14159256359

//ctor
secPrimaryGeneratorAction::secPrimaryGeneratorAction(void) :
    G4VUserPrimaryGeneratorAction(),
    pMuonGun(nullptr),
    mu_p(nullptr),
    mu_n(nullptr)
{
    pMuonGun = new G4ParticleGun();
    //pMuonGun = new G4GeneralParticleSource();
    /*
    PosMgr = new G4SPSPosDistribution();
    EnegMgr = new G4SPSEneDistribution();
    AngMgr = new G4SPSAngDistribution();
    */
    mu_p = G4MuonPlus::Definition();
    mu_n = G4MuonMinus::Definition();
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
    
    //G4double Eneg = ( 0.1 + (1000. - 0.1) * G4UniformRand() ) * GeV;
    G4double Eneg = 150.*MeV;
    pMuonGun->SetParticleEnergy(Eneg);

    G4double PosX = G4UniformRand() * 5. * cm;
    G4double PosY = G4UniformRand() * 5. * cm;
    G4ThreeVector PosVect(PosX, PosY, 3.5 * cm);
    pMuonGun->SetParticlePosition( PosVect );

    G4double Theta = PI - G4UniformRand() * PI / 2.;
    G4double Phi  = G4UniformRand() * 2 * PI;
    G4ThreeVector MomentumVect(1, 0, 0);
    MomentumVect.setTheta(Theta);
    MomentumVect.setPhi(Phi);
    pMuonGun->SetParticleMomentumDirection( MomentumVect );
    
    //auto AnalysisMgr = G4AnalysisManager::Instance();
    //AnalysisMgr->FillH1(6, Eneg);
    
    
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
