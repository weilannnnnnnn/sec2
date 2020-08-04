#include "secRandMacro.hh"
#include "secRandGenFromFile.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "secRandGenFromFile.hh"

secRandMacro::secRandMacro(secRandGenFromFile* gen) : 
    Generator(gen),
    FuncType(secVRandGen::PDF_TYPE)
{

    secRanddir = new G4UIdirectory("/sec/Rand");

    cmd_FuncType = new G4UIcmdWithAString("/sec/Rand/FuncType", this);
    cmd_FuncType->SetGuidance("Specify the distribution function type", this);
    cmd_FuncType->SetParameterName("FileType", false);
    cmd_FuncType->AvailableForState(G4State_PreInit, G4State_Idle);
    cmd_FuncType->SetCandidates("PDF CDF");

    cmd_FileName = new G4UIcmdWithAString("/sec/Rand/FileName", this);
    cmd_FileName->SetGuidance("Specify the file name of PDF / CDF");
    cmd_FileName->SetParameterName("FileName", false);
    cmd_FileName->AvailableForStates(G4State_PreInit, G4State_Idle);

    
}

void secRandMacro::SetNewValue(G4UIcommand* cmd, G4String NewVal)
{
    if( cmd == cmd_FuncType )
    {
        //specify file type( PDF or CDF )
        const G4String PDF_typename = "PDF";
        const G4String CDF_typename = "CDF"; 
        if( NewVal == PDF_typename )
        {
            FuncType = secVRandGen::PDF_TYPE;
        }
        else if( NewVal == CDF_typename )
        {
            FuncType = secVRandGen::CDF_TYPE;   
        }

    }
    else if( cmd == cmd_FileName )
    {
        //specify the file name.
        Generator->LoadFile(NewVal, FuncType);
    }    
}

void secRandMacro::GetCurrentValue(G4UIcommand*)
{

}
secRandMacro::~secRandMacro()
{
    delete secRanddir;
    delete cmd_FileName;
    delete cmd_FuncType;
}