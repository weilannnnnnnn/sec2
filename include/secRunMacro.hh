#ifndef secRunMacro_hh
#define secRunMacro_hh

#include "secParticleSource.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

//this is a singleton class
//used to configure the runtime parameters
class secRunMacro : public G4UImessenger
{
    public :
        secRunMacro* GetInstance();
        secRunMacro(secRunMacro const&) = delete;
        void operator=(secRunMacro const&) = delete;

        virtual void SetNewValue(G4UIcommand* cmd, G4String NewVal) override;
        virtual G4String GetCurrentValue(G4UIcommand* cmd) override;
        
        G4double GetAlphaEneg() { return AlphaEneg; }
        G4double GetBetaAlphaRatio() { return BetaAlphaRatio; }
        secParticleSource::secSourceGenType GetEventType() { return EventType; }

    private :
        secRunMacro();
        virtual void ~secRunMacro();
                
        G4UIcmdWithADoubleAndUnit* cmd_AlphaEneg; 
        G4double AlphaEneg;

        G4UIcmdWithADouble* cmd_BetaAlphaRatio;
        G4double BetaAlphaRatio;

        G4UIcmdWithAString* cmd_EventType; 
        secParticleSource::secSourceGenType EventType;
};
#endif
