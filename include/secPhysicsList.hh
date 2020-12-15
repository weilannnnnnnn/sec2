#ifndef secPhysicsList_hh
#define secPhysicsList_hh

#include "G4VModularPhysicsList.hh"

class secOpticalPhysics;
class G4DecayPhysics;
class G4EmStandardPhysics;
class G4EmPenelopePhysics;
class G4SpinDecayPhysics;
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
};

#endif
