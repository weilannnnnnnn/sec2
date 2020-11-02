#include "secRunMacro.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

secRunMacro::secRunMacro() :
    AlphaEneg(1*MeV),
    BetaAlphaRatio(1./3.),
    EventType(secParticleSource::Muons),
{
    cmd_AlphaEneg = new G4UIcmdWithADoubleAndUnit("/sec/Rand/AlphaEneg", this);
    cmd_AlphaEneg->SetGuidance("Specify the energy for alpha particle");
    cmd_AlphaEneg->SetParameterName("AlphaEneg", false);
    cmd_AlphaEneg->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_BetaAlphaRatio = new G4UIcmdWithADouble("/sec/Rand/BetaAlphaRatio", this);
    cmd_BetaAlphaRatio->SetGuidance("Specify the beta/alpha ratio");
    cmd_BetaAlphaRatio->SetParameterName("Beta/Alpha", false);
    cmd_BetaAlphaRatio->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_EventType = new G4UIcmdWithAString("/sec/Rand/EventType", this);
    cmd_EventType->SetGuidance("Specify the Event Type");
    cmd_EventType->SetParameterName("EventType", false);
    cmd_EventType->AvailableForStates(G4State_PreInit, G4State_Idle);
    cmd_EventType->SetCandidates("Muon NoiseBeta NoiseAll");
}

void secRunMacro::~secRunMacro()
{
    //delete the commands
    delete cmd_AlphaEneg;
    delete cmd_BetaAlphaRatio;
    delete cmd_EventType;
}

secRunMacro* secRunMacro::GetInstance()
{
    static secRunMacro Instance;
    return &Instance;
}

void secRunMacro::SetNewValue(G4UIcommand* cmd, G4String NewVal)
{
    if (cmd == cmd_AlphaEneg)
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
G4String secRunMacro::GetCurrentValue(G4UIcommand* cmd)
{
    return "";
}