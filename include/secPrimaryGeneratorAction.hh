#ifndef secPrimaryGeneratorAction_hh
#define secPrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;
//class G4ParticleGun;

class secPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    secPrimaryGeneratorAction(void);
    virtual ~secPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);


private:

    G4GeneralParticleSource* pMuonGun;

};

#endif
