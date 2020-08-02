#ifndef secRandGenFromFile_hh
#define secRandGenFromFile_hh

#include "secVRandGen.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include <string>
#include <vector>
#include "globals.hh"

/*
    class discription: 
        this class is used to generate the random samples from the data points
        in the loaded file. Appropriate interpolation method( default: liner )
        has been used.
*/

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
        std::vector<G4PhysicsOrderedFreeVector> PDFXYvectors;
        std::vector<G4PhysicsOrderedFreeVector> CDFXYvectors;
};

#endif
