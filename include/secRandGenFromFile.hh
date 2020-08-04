#ifndef secRandGenFromFile_hh
#define secRandGenFromFile_hh

#include "secVRandGen.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include <string>
#include <vector>
#include "globals.hh"

/*================================================================================================
    class discription: 
            This class is a singleton class, which is used to generate the random 
        samples from the data points in the loaded file. Appropriate interpo-
        lation method( default: liner ) has been used. For detailed information of the
        interpolation method, please consult G4PhysicsVector.hh and G4PhysicsOrderedFreeVector.hh
==================================================================================================
*/

class secRandMacro;
class G4PhysicsOrderedFreeVector;

class secRandGenFromFile : public secVRandGen
{
    public:
        static secRandGenFromFile* GetInstance();
        void LoadFile(const std::string& Name, secVRandGen::DistFuncType Type = secVRandGen::PDF_TYPE);

        secRandGenFromFile(secRandGenFromFile const&) = delete;
        void operator=(secRandGenFromFile const&)     = delete;

        virtual G4double PDF(G4double X, size_t PDFidx);
        virtual G4double InverseCDF(G4double X, size_t CDFidx);
        
    private:
        secRandGenFromFile();
        virtual ~secRandGenFromFile();
        
        secRandMacro* RandMacro;
        std::vector<G4PhysicsOrderedFreeVector> PDFXYvectors;
        std::vector<G4PhysicsOrderedFreeVector> CDFXYvectors;
};

#endif
