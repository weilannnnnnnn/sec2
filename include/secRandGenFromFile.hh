#ifndef secRandGenFromFile_hh
#define secRandGenFromFile_hh

#include "secVRandGen.hh"
#include "G4PhysicsFreeVector.hh"

#include <string>
#include <vector>
#include "globals.hh"

/*
    class discription: 
        this class is used to generate the random samples from the data points
        in the loaded file. Appropriate interpolation method( default: liner )
        has been used.
*/

class G4PhysicsFreeVector;

class secRandGenFromFile : public secVRandGen
{
    public:
        static secRandGenFromFile* GetInstance();
        void LoadFile(const std::string& Name);

        secRandGenFromFile(secRandGenFromFile const&) = delete;
        void operator=(secRandGenFromFile const&)     = delete;

    protected:
        virtual G4double PDF(G4double X, size_t PDFidx);

    private:
        secRandGenFromFile();
        virtual ~secRandGenFromFile();
        std::vector<G4PhysicsFreeVector> XYvectors;
};

#endif
