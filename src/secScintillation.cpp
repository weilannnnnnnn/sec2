#include "secScintillation.hh"

secScintillation::secScintillation(const G4String& ProcName = "secScint", 
                                   G4ProcessType ProcType = fElectromagnetic) :
    G4Scintillation(ProcName, ProcType)
{

}
secScintillation::~secScintillation()
{

}

G4double secScintillation::GetMeanLifeTime(G4Track* aTrack, G4ForceCondition* Condition)
{
    //in order to disable the at rest process of scintillation
    *Condition = InActivated;
    return DBL_MAX;
}