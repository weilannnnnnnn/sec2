#include "secSourceMacro.hh"
#include "secParticleSource.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

secSourceMacro::secSourceMacro(secParticleSource* secSrc) :
    ptrSrc(secSrc)
{
    cmd_AlphaEneg = new G4UIcmdWithADoubleAndUnit("/sec/Source/AlphaEneg", this);
    cmd_AlphaEneg->SetGuidance("Specify the energy for alpha particle");
    cmd_AlphaEneg->SetParameterName("AlphaEneg", false);
    cmd_AlphaEneg->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_NoiseIntensity = new G4UIcmdWithADouble();
    cmd_NoiseIntensity->SetGuidance("Specify the activity of beta spectrum");
    cmd_NoiseIntensity->SetParameterName("BetaActivity", false);
    cmd_NoiseIntensity->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_BetaAlphaRatio = new G4UIcmdWithADouble("/sec/Source/BetaAlphaRatio", this);
    cmd_BetaAlphaRatio->SetGuidance("Specify the beta/alpha ratio");
    cmd_BetaAlphaRatio->SetParameterName("Beta/Alpha", false);
    cmd_BetaAlphaRatio->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_EventType = new G4UIcmdWithAString("/sec/Source/EventType", this);
    cmd_EventType->SetGuidance("Specify the Event Type");
    cmd_EventType->SetParameterName("EventType", false);
    cmd_EventType->AvailableForStates(G4State_PreInit, G4State_Idle);
    cmd_EventType->SetCandidates("Muon NoiseBeta NoiseAll");

    cmd_SourceCentre = new G4UIcmdWith3VectorAndUnit("/sec/Source/NoiseCentre", this);
    cmd_SourceCentre->SetGuidance("Specify the centre of the noise source");
    cmd_SourceCentre->SetParameterName("X", "Y", "Z", false);
    cmd_SourceCentre->AvailableForStates(G4State_PreInit, G4State_Idle);


    cmd_SourceSize = new G4UIcmdWith3VectorAndUnit("/sec/Source/NoiseSourceSize", this);
    cmd_SourceSize->SetGuidance("Specify the half size of the noise source");
    cmd_SourceSize->SetParameterName("HalfX", "HalfY", "HalfZ", false);
    cmd_SourceSize->AvailableForStates(G4State_PreInit, G4State_Idle);

}

secSourceMacro::~secSourceMacro()
{
    //delete the commands
    delete cmd_AlphaEneg;
    delete cmd_NoiseIntensity;
    delete cmd_BetaAlphaRatio;
    delete cmd_EventType;
    delete cmd_SourceCentre;
    delete cmd_SourceSize;

}

void secSourceMacro::SetNewValue(G4UIcommand* cmd, G4String NewVal)
{
    if (cmd == cmd_AlphaEneg)
    {
        ptrSrc->AlphaEneg = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(NewVal);
    }
    else if ( cmd == cmd_NoiseIntensity )
    {
        secParticleSource::NoiseIntensity = G4UicmdWithAdouble::GetNewDoubleValue(NewVal);
    }
    
    else if (cmd == cmd_BetaAlphaRatio)
    {
        ptrSrc->BetaAlphaRatio = G4UIcmdWithADouble::GetNewDoubleValue(NewVal);
    }
    else if (cmd == cmd_EventType)
    {
        const G4String MuonName = "Muon";
        const G4String BetaName = "NoiseBeta";
        const G4String NoiseAllName = "NoiseAll";
        secParticleSource::secSourceGenType EventType = secParticleSource::secSourceGenType::Muons;
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
        secParticleSource::GenTypeNow = EventType;
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
G4String secSourceMacro::GetCurrentValue(G4UIcommand* )
{
    return "";
}
