#include "secDCMacros.hh"
#include "secDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"

secDCMacros::secDCMacros(secDetectorConstruction* secDC) :
    DetectorConstruction(secDC),
    LVNow(nullptr),
    PVNow(nullptr)
    IsOverLapCheck(true)
{
/*
    create commands for detector construction. ALL COMMAND BEGINS WITH /sec/DC
*/
    //command for toggling the overlap checking
    cmd_OverLapCheck = new G4UIcmdWithABool("/sec/DC/IsOverLapCheck", this);
    cmd_OverLapCheck->SetGuidence("Enable/Disable Detector Overlap Checking");
    cmd_OverLapCheck->SetParameterName("Flag", false); 
    cmd_OverLapCheck->SetDefaultValue(true);

    //command for specifing the logical volume
    cmd_SpecifyLV = new G4UIcmdWithAString("/sec/DC/LogicalVolume", this);
    cmd_SpecifyLV->SetGuidence("Specify the LogicalVolume to further set parmeter");
    cmd_SpecifyLV->SetParameterName("LogicalVolumeName");

    //command for specifing the logical volume
    cmd_SpecifyPV = new G4UIcmdWithAString("/sec/DC/PhysicalVolume", this);
    cmd_SpecifyPV->SetGuidence("Specify the Solid to further set parameter");
    cmd_SpecifyPV->SetGuidence("PhysicalVolumeName");

    //command for loading the data files for logical volume
    cmd_FileName = new G4UIcmdWithAString("/sec/DC/LoadFileForLV");
    cmd_FileName->SetGuidence("Specify the data file's name");
    cmd_FileName->SetParameterName("FileName");

    //command for changing Physical Volume
    cmd_VolumeSize = new G4UIcmdWith3VectorAndUnit("/sec/DC/SetVolumeSize");
    cmd_VolumeSize->SetGuidence("Set The Geometry, three parameters correspond to G4's");
    cmd_VolumeSize->SetParameterName("Par1", "Par2", "Par3", false);

}
secDCMacros::~secDCMacros()
{
    //delete the commands!!
    delete cmd_OverLapCheck;
    delete cmd_SpecifyLV;
    delete cmd_SpecifyPV;
    delete cmd_FileName;
    delete cmd_VolumeSize;
}

void secDCMacros::SetNewValue(G4UIcommand* cmd, G4String NewValue)
{
    if( cmd == cmd_OverLapCheck )
    {
        IsOverLapCheck = cmd->ConvertToBool(NewValue);
    }
    else if( cmd == cmd_SpecifyLV )
    {
        //not the store selling LVs! But the logical volume store!!
        auto LVStore = G4LogicalVolumeStore::GetInstance();
        LVNow = LVStore->GetVolume(NewValue);
        
        // if the volume doesn't exist, always set the volume "world_log"( world volume of the detector )
        if(LVNow = nullptr)
        {
            LVStore->GetVolume("world_log");
        }
    }
    else if( cmd == cmd_SpecifyPV )
    {
        auto PVStore = G4PhysicalVolume::GetInstance();
        PVNow = PVStore->GetVolume(NewValue);

        if( PVNow = nullptr )
        {
            //always set the volume "world_phy" if the volume doesn't exist.
            PVStore->GetVolume("world_phy");
        }
    }
    else if ( cmd == cmd_FileName )
    {
        //unfinished!
        //register the data file in the detector construction
    }
    else if( cmd == cmd_VolumeSize )
    {
        //unfinished! 
        //need to parameterize the detectore construction

        //if the Physical is changed by user, an overlap checking procedure will be invoke
        //when the toggle is set true
        if( IsOverLapCheck == true )
        {
            if( PVNow->CheckOverLaps() )
            {
                //overlap happened! error message!!
            }
        }
    }
    
}