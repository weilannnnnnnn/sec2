#ifndef secRunAction_HH
#define secRunAction_HH

#include "G4UserRunAction.hh"
#include "secAnalysis.hh"
#include "globals.hh"

class G4Run;

/*========================================================
class desctiption:

    The RunAction in sec detector, which manages the ASCII
file and CERN ROOT file's input and output
==========================================================*/

class secRunAction : public G4UserRunAction
{
public:

    secRunAction(void);
    virtual ~secRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    void MergeFile(G4String FileName);
    void GenerateNoiseTimeStamp(G4String FileName, const size_t Num);

};

#endif
