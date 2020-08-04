#include "secPrimaryGeneratorAction.hh"
#include "secParticleSource.hh"
#include "G4GeneralParticleSource.hh"
#include "secParticleSource.hh"
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
    pMuonGun = new secParticleSource();
}

//dtor
secPrimaryGeneratorAction::~secPrimaryGeneratorAction()
{
    delete pMuonGun;
}

void secPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
    pMuonGun -> GeneratePrimaryVertex(anEvent);
}
