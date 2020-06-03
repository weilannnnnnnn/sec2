#ifndef secSteppingAction_hh
#define secSteppingAction_hh
#include "G4UserSteppingAction.hh"

class secEventAction;
class G4PhysicalVolumeStore;
class G4VPhysicalVolume;

class secSteppingAction : public G4UserSteppingAction
{
public:
    secSteppingAction(secEventAction* eventAction);
    virtual ~secSteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

private:
    secEventAction* EventAction;//current event

//-------------------------------------------------------------------
};

#endif // secSteppingAction.hh
