#ifndef secScintillation_hh
#define secScintillation_hh

#include "G4Scintillation.hh"
#include "G4ProcessType.hh"
#include "G4ForceCondition.hh"
#include "globals.hh"

/*
    This class is dreved from G4Scintillation in order to disable 
    the at rest process of G4Scintillation
*/

class secScintillation : public G4Scintillation
{
    public:
        //ctor and dtor
        secScintillation(const G4String& ProcName = "secScint", 
                         G4ProcessType ProcType = fElectromagnetic);
        virtual ~secScintillation();
    
    private:
        secScintillation(const secScintillation& rhs) = delete;
        secScintillation& operator=(const secScintillation& rhs) = delete;
    
    public:
        virtual G4double GetMeanLifeTime(G4Track* aTrack, 
                                         G4ForceCondition* Condition) override;

};
#endif
 