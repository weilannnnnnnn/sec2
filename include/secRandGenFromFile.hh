#ifndef secRandGenFromFile_hh
#define secRandGenFromFile_hh

#include "secVRandGen.hh"
#include "G4PhysicsFreeVector.hh"

#include <string>
#include <vector>
#include "globals.hh"

class G4PhysicsFreeVector;

class secRandGenFromFile : public secVRandGen
{
    public:
        secRandGenFromFile();
        virtual ~secRandGenFromFile();
        void LoadFile(const std::string& Name);
    
    protected:
        virtual G4double PDF(G4double X, size_t PDFidx);

    private:
        std::vector<G4PhysicsFreeVector> XYvectors;
};

#endif