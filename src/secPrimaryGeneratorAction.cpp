#include "secPrimaryGeneratorAction.hh"
#include "secRandGenFromFile.hh"

#include "G4GeneralParticleSource.hh"
//#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "Randomize.hh"
#include <iostream>
#include <fstream>
#define PI 3.14159256359

//ctor
secPrimaryGeneratorAction::secPrimaryGeneratorAction(void) :
    G4VUserPrimaryGeneratorAction(),
    pMuonGun(nullptr)
{
    pMuonGun = new G4GeneralParticleSource();
}

//dtor
secPrimaryGeneratorAction::~secPrimaryGeneratorAction()
{
    delete pMuonGun;
}

void secPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
    //the configuration of the source is achieved in macro file run.mac!
    
    pMuonGun -> GeneratePrimaryVertex(anEvent);
}
