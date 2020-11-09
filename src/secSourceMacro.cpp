#include "secSourceMacro.hh"
#include "secParticleSource.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

secSourceMacro::secSourceMacro(secParticleSource* secSrc) :
    ptrSrc(secSrc),
    AlphaEneg(1*MeV),
    BetaAlphaRatio(1./3.),
    EventType(secParticleSource::Muons),
{
    cmd_AlphaEneg = new G4UIcmdWithADoubleAndUnit("/sec/Source/AlphaEneg", this);
    cmd_AlphaEneg->SetGuidance("Specify the energy for alpha particle");
    cmd_AlphaEneg->SetParameterName("AlphaEneg", false);
    cmd_AlphaEneg->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_BetaAlphaRatio = new G4UIcmdWithADouble("/sec/Source/BetaAlphaRatio", this);
    cmd_BetaAlphaRatio->SetGuidance("Specify the beta/alpha ratio");
    cmd_BetaAlphaRatio->SetParameterName("Beta/Alpha", false);
    cmd_BetaAlphaRatio->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_EventType = new G4UIcmdWithAString("/sec/Source/EventType", this);
    cmd_EventType->SetGuidance("Specify the Event Type");
    cmd_EventType->SetParameterName("EventType", false);
    cmd_EventType->AvailableForStates(G4State_PreInit, G4State_Idle);
    cmd_EventType->SetCandidates("Muon NoiseBeta NoiseAll");

    cmd_SourceCentre = new G4UIcmdWith3VectorAndUnit("/sec/Source/NoiseCentre");
    cmd_SourceCentre->SetGuidance("Specify the centre of the noise source");
    cmd_SourceCentre->SetParameterName("SourceCentre", false);
    cmd_SourceCentre->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_SourceSize = new G4UIcmdWith3VectorAndUnit("/sec/Source/NoiseSourceSize");
    cmd_SourceSize->SetGuidance("Specify the half size of the noise source");
    cmd_SourceSize->SetParameterName("SourceSize", false);
    cmd_SourceSize->AvailableForStates(G4State_PreInit, G4State_Idle);

}

void secSourceMacro::~secSourceMacro()
{
    //delete the commands
    delete cmd_AlphaEneg;
    delete cmd_BetaAlphaRatio;
    delete cmd_EventType;
    delete cmd_SourceCentre;
    delete cmd_SourceSize;

}

void secSourceMacro::SetNewValue(G4UIcommand* cmd, G4String NewVal)
{
    if (cmd == cmd_AlphaEneg)
    {
        ptrSrc->AlphaEneg = cmd->GetNewDoubleValue(NewVal);
    }
    else if (cmd == cmd_BetaAlphaRatio)
    {
        ptrSrc->BetaAlphaRatio = cmd->GetNewDoubleValue(NewVal);
    }
    else if (cmd == cmd_EventType)
    {
        const G4double MuonName = "Muon";
        const G4double BetaName = "NoiseBeta";
        const G4double NoiseAllName = "NoiseAll";
        secParticleSource::secSourceGenType EventType = 0;
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
        secParticleSource::secSourceGenType secParticleSource::GenTypeNow = EventType;
    }
    else if( cmd == cmd_SourceCentre )
    {
        ptrSrc->SrcCentre = cmd->ConvertTo3Vector(NewVal);
    }
    else if( cmd == cmd_SourceSize )
    {
        ptrSrc->SrcSize = cmd->ConvertTo3Vector(NewVal);
    }
    
}
G4String secSourceMacro::GetCurrentValue(G4UIcommand* cmd)
{
    return "";
}