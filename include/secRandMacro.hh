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

    private:
        
        secRandGenFromFile* Generator;
        G4UIdirectory*      secRanddir;
        G4UIcmdWithAString* cmd_FuncType;
        G4UIcmdWithAString* cmd_FileName;
        
        secVRandGen::DistFuncType FuncType;
};

#endif
