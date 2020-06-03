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

    void AddPhotonsGenUp(void)  { PhotonsGenUp++;  }  //Generated photons
    void AddPhotonsRecvUp(void) { PhotonsRecvUp++; }  // received photons

    void AddPhotonsGenDown(void)  { PhotonsGenDown++;  }
    void AddPhotonsRecvDown(void) { PhotonsRecvDown++; }
    void Reset(void);
private:
    G4int PhotonsGenUp;
    G4int PhotonsRecvUp;
    G4int PhotonsGenDown;
    G4int PhotonsRecvDown;
    
    G4double EfficiencyUp; // recv / gen
    G4double EfficiencyDown;
};

inline void secEventAction::Reset(void)
{
    PhotonsGenUp = 0;
    PhotonsRecvUp = 0;
    PhotonsGenDown = 0;
    PhotonsRecvDown = 0;
    EfficiencyUp = 0.;
    EfficiencyDown = 0.;
}

#endif
