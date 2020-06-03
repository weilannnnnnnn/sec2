#include "secScintillation.hh"

secScintillation::secScintillation(const G4String& ProcName, 
                                   G4ProcessType ProcType) :
    G4Scintillation(ProcName, ProcType)
{

}
secScintillation::~secScintillation()
{

}

G4double secScintillation::GetMeanLifeTime(const G4Track&, G4ForceCondition* Condition)
{
    //in order to disable the at rest process of scintillation
    *Condition = InActivated;
    return DBL_MAX;
}
