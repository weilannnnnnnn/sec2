#ifndef secPrimaryGeneratorAction_hh
#define secPrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

class G4GeneralParticleSource;
class G4Event;
class G4ParticleDefinition;
class G4ParticleGun;

class secPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    secPrimaryGeneratorAction(void);
    virtual ~secPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);


private:

    G4ParticleGun* pMuonGun;
    //G4GeneralParticleSource* pMuonGun;
    /*
    G4SPSPosDistribution* PosMgr;
    G4SPSEneDistribution* EnegMgr;
    G4SPSAngDistribution* AngMgr;
    */
    G4ParticleDefinition* mu_p;
    G4ParticleDefinition* mu_n;

};

#endif
