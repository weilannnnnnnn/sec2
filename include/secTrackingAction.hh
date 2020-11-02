#ifndef secTrackingAction_hh
#define secTrackingAction_hh
#include "G4UserTrackingAction.hh"

class G4Track;
class secEventAction;
class secScintSD;
class G4VPhysicalVolume;

class secTrackingAction : public G4UserTrackingAction
{
    public:
        secTrackingAction(secEventAction* eventAction);
        virtual ~secTrackingAction();

        virtual void PreUserTrackingAction    (const G4Track* track);
        virtual void PostUserTrackingAction   (const G4Track* track);

    private:
        secEventAction* EventAction;
        secScintSD* pScintSD;
};
#endif
