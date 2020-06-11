#ifndef secDCMacros_hh
#define secDCMacros_hh

#include "secMacros.hh"


/*
=======================================================
class discription:
    this class manages the macro commands which configure
    the detector construction.

    Commands begin with /sec/DC
=======================================================
*/

class secDetectorConstruction;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;

class secDCMacros : public secMacros
{
    public:
        //ctor and dtor
        secDCMacros();
        virtual ~secDCMacros();

    private:

        secDetectorConstruction* DetectorConstruction;
        G4UIcmdWithABool* cmd_OverLapCheck;
        G4UIcmdWithAString* cmd_LoadFile;
        G4UIcmdWith3VectorAndUnit* cmd_VolumeSize;
        G4UIcmdWith3VectorAndUnit* cmd_VolumePos;
};
#endif
