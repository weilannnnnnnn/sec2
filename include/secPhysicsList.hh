#ifndef secPhysicsList_hh
#define secPhysicsList_hh

#include "G4VModularPhysicsList.hh"

class G4OpticalPhysics;
class G4DecayPhysics;
class G4EmStandardPhysics;
class G4HadronElasticPhysics;
class G4IonPhysics;

class secPhysicsList : public G4VModularPhysicsList
{
    public:
        //ctor
        secPhysicsList();
        //dtor
        virtual ~secPhysicsList();

        virtual void SetCuts();
        
    private:
        G4OpticalPhysics*       OpticalPhysics_init(void);
        G4DecayPhysics*         DecayPhysics_init(void);
        G4EmStandardPhysics*    EmPhysics_init(void);
        G4HadronElasticPhysics* HadronElasticPhysics_init(void);
        G4IonPhysics*           IonPhysics_init(void);
};

#endif