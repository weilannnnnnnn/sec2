#ifndef secSourceMacro_hh
#define secSourceMacro_hh

#include "secParticleSource.hh"
#include "G4UImessenger.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4ThreeVector;

//this is a singleton class
//used to configure the runtime parameters
class secSourceMacro : public G4UImessenger
{
    public :
        secSourceMacro* GetInstance();
        secSourceMacro(secSourceMacro const&) = delete;
        void operator=(secSourceMacro const&) = delete;

        virtual void SetNewValue(G4UIcommand* cmd, G4String NewVal) override;
        virtual G4String GetCurrentValue(G4UIcommand* cmd) override;
        
        G4double GetAlphaEneg() { return AlphaEneg; }
        G4double GetBetaAlphaRatio() { return BetaAlphaRatio; }
        secParticleSource::secSourceGenType GetEventType() { return EventType; }
        G4ThreeVector GetSourceCentre() { return SrcCentre; }
        G4ThreeVector GetSourceSize()  {return SrcSz; }

    private :
        secSourceMacro();
        virtual void ~secSourceMacro();
                
        G4UIcmdWithADoubleAndUnit* cmd_AlphaEneg; 
        G4double AlphaEneg;

        G4UIcmdWithADouble* cmd_BetaAlphaRatio;
        G4double BetaAlphaRatio;

        G4UIcmdWithAString* cmd_EventType; 
        secParticleSource::secSourceGenType EventType;

        G4UIcmdWith3VectorAndUnit* cmd_SourceCentre;
        G4ThreeVector SrcCentre;

        G4UIcmdWith3VectorAndUnit* cmd_SourceSize;
        G4ThreeVector SrcSz;

};

#endif
