#ifndef secDCMacros_hh
#define secDCMacros_hh

#include "secMacros.hh"
#include "globals.hh"

/*
=======================================================
class discription:
    this class manages the macro commands which configure
    the detector construction.

    Commands begin with /sec/DC
=======================================================
*/
class secDetectorConstruction;
class G4LogicalVolume;
class G4PVPlacement;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;

class secDCMacros : public secMacros
{
    public:
        //ctor and dtor
        secDCMacros(secDetectorConstruction* secDC);
        virtual ~secDCMacros();

        virtual void SetNewValue(G4UIcommand* command, G4String newValues) override;
        virtual G4String GetNewValue(G4UIcommand* command) override;
    private:

        secDetectorConstruction* DetectorConstruction;
        G4LogicalVolume* LVNow;
        G4PVPlacement* PVNow;
        G4bool IsOverlapCheck;

        G4UIdirectory* secDCdir;
        G4UIcmdWithABool* cmd_OverLapCheck;
        G4UIcmdWithAString* cmd_SpecifyLV;
        G4UIcmdWithAString* cmd_SpecifyPV;
        G4UIcmdWithAString* cmd_FileName;
        G4UIcmdWith3VectorAndUnit* cmd_VolumeSize;
        G4UIcmdWith3VectorAndUnit* cmd_VolumePos;
};
#endif
