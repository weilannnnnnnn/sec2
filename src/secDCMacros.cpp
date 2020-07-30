#include "secDCMacros.hh"
#include "secDetectorConstruction.hh"

#include "G4UIdirectory"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

secDCMacros::secDCMacros(secDetectorConstruction* secDC) :
    DetectorConstruction(secDC),
    LVNow(nullptr),
    PVNow(nullptr),
    IsOverlapCheck(true)
{
/*
    create commands for detector construction. ALL COMMAND BEGINS WITH /sec/DC
*/
    //command for toggling the overlap checking
    cmd_OverlapCheck = new G4UIcmdWithABool("/sec/DC/IsOverLapCheck", this);
    cmd_OverlapCheck->SetGuidance("Enable/Disable Detector Overlap Checking");
    cmd_OverlapCheck->SetParameterName("Flag", false); 
    cmd_OverlapCheck->SetDefaultValue(true);

    //command for specifing the logical volume
    cmd_SpecifyLV = new G4UIcmdWithAString("/sec/DC/LogicalVolume", this);
    cmd_SpecifyLV->SetGuidance("Specify the LogicalVolume to further set parmeter");
    cmd_SpecifyLV->SetParameterName("LogicalVolumeName", false);

    //command for specifing the logical volume
    cmd_SpecifyPV = new G4UIcmdWithAString("/sec/DC/PhysicalVolume", this);
    cmd_SpecifyPV->SetGuidance("Specify the Solid to further set parameter");
    cmd_SpecifyPV->SetParameterName("PhysicalVolumeName", false);

    //command for loading the data files for logical volume
    cmd_LoadFile = new G4UIcmdWithAString("/sec/DC/LoadFileForLV", this);
    cmd_LoadFile->SetGuidance("Specify the data file's name");
    cmd_LoadFile->SetParameterName("FileName", false);
    cmd_LoadFile->AvailableForStates(G4State_PreInit,G4State_Idle);

    //command for translating the Physical Volume
    cmd_PVPos = new G4UIcmdWith3VectorAndUnit("/sec/DC/Translation", this);
    cmd_PVPos->SetGuidance("Translate the Physical Volume to a new position");
    cmd_PVPos->SetParameterName("PosX", "PosY", "PosZ", false);

    //command for rotating the Physical Volume
    cmd_PVRotate = new G4UIcmdWith3Vector("/sec/DC/Rotation", this);
    cmd_PVRotate->SetGuidance("Rotate the Physical Volume according to Euler Angles (rad)");
    cmd_PVRotate->SetParameterName("alpha", "beta", "gamma", false);
}
secDCMacros::~secDCMacros()
{
    //delete the commands!!
    delete cmd_OverlapCheck;
    delete cmd_SpecifyLV;
    delete cmd_SpecifyPV;
    delete cmd_LoadFile;
    delete cmd_PVPos;
    delete cmd_PVRotate;
}

void secDCMacros::SetNewValue(G4UIcommand* cmd, G4String NewVal)
{
    if( cmd == cmd_OverlapCheck )
    {
        IsOverlapCheck = cmd->ConvertToBool(NewVal);
    }
    else if( cmd == cmd_SpecifyLV )
    {
        //not the store selling LVs! But the logical volume store!!
        auto LVStore = G4LogicalVolumeStore::GetInstance();
        LVNow = LVStore->GetVolume(NewVal);
        
        // if the volume doesn't exist, always set the volume "world_log"( world volume of the detector )
        if(LVNow == nullptr)
        {
            LVStore->GetVolume("world_log");
            std::cerr << "===========================================================\n"
                      << "                   Warning From sec2"
                      << "Logical volume called " << NewVal << " NOT FOUND!"
                      << "Probably wrong name, current Logical Volume is world volume"
                      << std::endl;
        }
    }
    else if( cmd == cmd_SpecifyPV )
    {
        auto PVStore = G4PhysicalVolumeStore::GetInstance();
        PVNow = PVStore->GetVolume(NewVal);

        if( PVNow == nullptr )
        {
            //always set the volume "world_phy" if the volume doesn't exist.
            PVStore->GetVolume("world_phy");
            if(PVNow == nullptr)
            {
                PVStore->GetVolume("world_phy");
                std::cerr << "===========================================================\n"
                          << "                   Warning From sec2"
                          << "Physical volume called " << NewVal << " NOT FOUND!"
                          << "Probably wrong name, current Physical Volume is world volume"
                          << std::endl;
            }
        }
    }
    else if ( cmd == cmd_LoadFile )
    {
        //unfinished!
        //register the data file in the detector construction
        DetectorConstruction->SetOpticalProperties(LVNow, NewVal);
    }
    else if( cmd == cmd_PVPos )
    {
        //Move the Physical Volume to a new place.
        PVNow->SetTranslation( cmd->ConvertTo3Vector(NewVal) );
        //inform the run manager
        InformRunMgr();
        DumpOverlapInfo(IsOverlapCheck, PVNow);
    }
    else if( cmd == cmd_PVRotate )
    {
        //rotate the Physical Volume accoring to the Euler Angle.
        G4ThreeVector EulerVect = cmd->ConvertTo3Vector(NewVal);
        G4RotationMatrix RotMat(EulerVect.x(), EulerVect.y(), EulerVect.z());
	PVNow->SetRotation( &RotMat );
        //inform the run manager
        InformRunMgr();
        DumpOverlapInfo(IsOverlapCheck, PVNow);
    }    
}

G4String secDCMacros::GetCurrentValue(G4UIcommand*)
{
    return "";
}
