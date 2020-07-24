#ifndef secDCMacros_hh
#define secDCMacros_hh

#include "secMacros.hh"

#ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
#else
    #include "G4RunManager.hh"
#endif

#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include <iostream>
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
class G4VPhysicalVolume;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;

class secDCMacros : public secMacros
{
    public:
        //ctor and dtor
        secDCMacros(secDetectorConstruction* secDC);
        virtual ~secDCMacros();

        virtual void SetNewValue(G4UIcommand* cmd, G4String NewVal) override;
        virtual G4String GetNewValue(G4UIcommand* command) override;
    private:

        secDetectorConstruction* DetectorConstruction;
        
        //user specified Logical Volume
        G4LogicalVolume* LVNow;
        //user specified Physical Volume
        G4VPhysicalVolume* PVNow;

        G4bool IsOverlapCheck;
        G4UIdirectory* secDCdir;
        G4UIcmdWithABool*          cmd_OverLapCheck;
        G4UIcmdWithAString*        cmd_SpecifyLV;
        G4UIcmdWithAString*        cmd_SpecifyPV;
        G4UIcmdWithAString*        cmd_LoadFile;
        G4UIcmdWith3VectorAndUnit* cmd_PVPos;
        G4UIcmdWith3Vector*        cmd_PVRotate;

        //to warn user if overlap happened after changing the geometry
        void DumpOverlapInfo(G4bool IsOverLap, G4VPhysicalVolume* PVnow);

        //to inform the run manager that the geometry has been changed!
        void InformRunMgr();
};

inline void secDCMacros::DumpOverlapInfo(G4bool IsOverLapCheck, G4VPhysicalVolume* PVNow)
{
    //if overlapping happened, throw a warning
    if( IsOverLapCheck && PVNow->CheckOverLaps() )
    {
        std::cerr << "===========================================================\n"
                  << "                   Warning From sec2"
                  << "Overlap happened when changing " << PVNow->GetName() << std::endl;
    }
}

inline void secDCMacros::InformRunMgr()
{
    #ifdef G4MULTITHREADED
        auto RunMgr = G4MTRunManager::GetRunManager();
    #else
        auto RunMgr = G4RunManager::GetRunManager();
    #endif
    RunMgr->GeometryHasBeenModified();
}
#endif
