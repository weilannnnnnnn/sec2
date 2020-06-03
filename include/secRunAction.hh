#ifndef secRunAction_HH
#define secRunAction_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class secRunAction : public G4UserRunAction
{
public:

    secRunAction(void);
    virtual ~secRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    void MergeFile(G4String FileName);
};

#endif
