#include "secRandMacro.hh"
#include "secRandGenFromFile.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"

secRandMacro::secRandMacro(secRandGenFromFile* gen) : 
    Generator(gen),
    AlphaEneg(1*MeV),
    BetaAlphaRatio(1./3.),
    EventType(secParticleSource::Muons),
    FuncType(secVRandGen::PDF_TYPE)
{

    secRanddir = new G4UIdirectory("/sec/Rand");

    cmd_FuncType = new G4UIcmdWithAString("/sec/Rand/FuncType", this);
    cmd_FuncType->SetGuidance("Specify the distribution function type");
    cmd_FuncType->SetParameterName("FileType", false);
    cmd_FuncType->AvailableForStates(G4State_PreInit, G4State_Idle);
    cmd_FuncType->SetCandidates("PDF CDF");

    cmd_FileName = new G4UIcmdWithAString("/sec/Rand/FileName", this);
    cmd_FileName->SetGuidance("Specify the file name of PDF / CDF");
    cmd_FileName->SetParameterName("FileName", false);
    cmd_FileName->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_AlphaEneg = new G4UIcmdWithADoubleAndUnit("/sec/Rand/AlphaEneg", this);
    cmd_AlphaEneg->SetGuidance("Specify the energy for alpha particle");
    cmd_AlphaEneg->SetParameterName("AlphaEneg", false);
    cmd_AlphaEneg->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_BetaAlphaRatio = new G4UIcmdWithAdouble("/sec/Rand/BetaAlphaRatio", this);
    cmd_BetaAlphaRatio->SetGuidance("Specify the beta/alpha ratio");
    cmd_BetaAlphaRatio->SetParameterName("Beta/Alpha", false);
    cmd_BetaAlphaRatio->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_EventType = new G4UIcmdWithAString("/sec/Rand/EventType", this);
    cmd_EventType->SetGuidance("Specify the Event Type");
    cmd_EventType->SetParameterName("EventType", false);
    cmd_EventType->AvailableForStates(G4State_PreInit, G4State_Idle);
    cmd_EventType->SetCandidates("Muon NoiseBeta NoiseAll");

}

secRandMacro::~secRandMacro()
{
    delete secRanddir;
    delete cmd_FileName;
    delete cmd_FuncType;
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
    else if (cmd == cmd_AlphaEneg)
    {
        AlphaEneg = cmd->GetNewDoubleValue(NewVal);
    }
    else if (cmd == cmd_BetaAlphaRatio)
    {
        BetaAlphaRatio = cmd->GetNewDoubleValue(NewVal);
    }
    else if (cmd == cmd_EventType)
    {
        const G4double MuonName = "Muon";
        const G4double BetaName = "NoiseBeta";
        const G4double NoiseAllName = "NoiseAll";
        if( NewVal == MuonName )
        {
            EventType = secParticleSource::Muons;
        }
        else if( NewVal == BetaName )
        {
            EventType = secParticleSource::NoiseBeta;
        }
        else if( NewVal == NoiseAllName )
        {
            EventType = secParticleSource::NoiseAll;
        }

    }
}

G4String secRandMacro::GetCurrentValue(G4UIcommand*)
{
    return "";
}
