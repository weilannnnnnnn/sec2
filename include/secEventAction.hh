#ifndef secEventAction_hh
#define secEventAction_hh

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class secEventAction : public G4UserEventAction
{
public:
    secEventAction(void);
    virtual~secEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    void Reset(void);
    void AddPhotonsRecvUp(void) { PhotonsRecvUp++; }  // received photons
    
    void AddPhotonsRecvDown(void) { PhotonsRecvDown++; }

private:
    G4int PhotonsRecvUp;
    G4int PhotonsRecvDown;

};
inline void secEventAction::Reset(void)
{
    PhotonsRecvUp = 0;
    PhotonsRecvDown = 0;
}


#endif
