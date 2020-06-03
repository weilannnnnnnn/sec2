#ifndef secPhysicsList_hh
#define secPhysicsList_hh

#include "G4VModularPhysicsList.hh"

class secOpticalPhysics;
class G4DecayPhysics;
class G4EmStandardPhysics;
class G4HadronElasticPhysics;
class G4IonPhysics;
class G4StepLimiterPhysics;

class secPhysicsList : public G4VModularPhysicsList
{
    public:
        //ctor
        secPhysicsList();
        //dtor
        virtual ~secPhysicsList();

        virtual void SetCuts();
        
    private:
        secOpticalPhysics*       OpticalPhysics_init(void);
        G4DecayPhysics*          DecayPhysics_init(void);
        G4EmStandardPhysics*     EmPhysics_init(void);
        G4HadronElasticPhysics*  HadronElasticPhysics_init(void);
        G4IonPhysics*            IonPhysics_init(void);
        G4StepLimiterPhysics*    StepLimiter_init(void);
};

#endif
