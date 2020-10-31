#ifndef secRandMacro_hh
#define secRandMacro_hh
#include "G4UImessenger.hh" 
#include "secRandGenFromFile.hh"
#include "secParticleSource.hh"
/*==================================================================
    class discription:
        This class manages the macro commands which are used to spe-
        cify the file name and probability distribution function type
        of secRandGenFromFile, and also accounts for initializating 
        the particle source
====================================================================
*/

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

class secRandMacro : public G4UImessenger
{
    public:
        //ctor and dtor

        secRandMacro(secRandGenFromFile* gen);
        virtual ~secRandMacro();

        virtual void SetNewValue(G4UIcommand* cmd, G4String NewVal) override;
        virtual G4String GetCurrentValue(G4UIcommand* cmd) override;

        void SetSourceAlphaEneg(secParticleSource* pSrc) {pSrc->AlphaEneg      = this->AlphaEneg}
        void SetBetaAlphaRatio(secParticleSource* pSrc)  {pSrc->BetaAlphaRatio = this->BetaAlphaRatio }
        void SetSourceEventType(secParticleSource* pSrc) {pSrc->EventType      = this->EventType}

    private:
        
        secRandGenFromFile* Generator;
        G4UIdirectory*      secRanddir;
        G4UIcmdWithAString* cmd_FuncType;
        G4UIcmdWithAString* cmd_FileName;

        G4UIcmdWithADoubleAndUnit* cmd_AlphaEneg; 
        G4double AlphaEneg;

        G4UIcmdWithADouble* cmd_BetaAlphaRatio;
        G4double BetaAlphaRatio;

        G4UIcmdWithAString* cmd_EventType; 
        secParticleSource::secSourceGenType EventType;

        secVRandGen::DistFuncType FuncType;
};

#endif
