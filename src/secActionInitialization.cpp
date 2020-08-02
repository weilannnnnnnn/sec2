#include "secActionInitialization.hh"
#include "secPrimaryGeneratorAction.hh"
#include "secRunAction.hh"
#include "secEventAction.hh"
#include "secTrackingAction.hh"
#include "secSteppingAction.hh"

//ctor
secActionInitialization::secActionInitialization(void) :
    G4VUserActionInitialization()
{/*nothing here*/}

//dtor
secActionInitialization::~secActionInitialization()
{/*nothing here*/}


void secActionInitialization::BuildForMaster(void) const
{
    secRunAction* runAction = new secRunAction;
    SetUserAction(runAction);
}

void secActionInitialization::Build(void) const
{
    //primary generator
    auto pGenerator = new secPrimaryGeneratorAction();
    SetUserAction(pGenerator);

    //RunAction
    auto runAction = new secRunAction();
    SetUserAction(runAction);

    //EventAction
    auto eventAction = new secEventAction();
    SetUserAction(eventAction);

    //TrackingAction
    auto trackingAction = new secTrackingAction(eventAction);
    SetUserAction(trackingAction);

    //SteppingAction
    SetUserAction(new secSteppingAction(eventAction));
}
