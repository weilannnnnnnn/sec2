#ifndef secMacros_hh
#define secMacros_hh
#include "G4UIcommand.hh"
#include "globals.hh"
/*
====================================================
class description:
    this class is used to manage the UI commands
    (macros) used by sec detector. Currently, 
    It will be drived by secDCMacroManager
    (DC means detector construction) and 
    secSDMacroManager. (SD means sensitive detector)

    All macro commands defined by sec begin with: /sec
===================================================    
*/
class G4UIdirectory;

class secMacros : public G4UImanager
{
    public:
        //ctor and dtor
        secMacros();
        virtual ~secMacros();

    private:
        G4UIdirectory* CmdBaseDir;
};

#endif
