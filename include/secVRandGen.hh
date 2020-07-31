#ifndef secVRandGen_hh
#define secVRandGen_hh

#include "globals.hh"
#include <string>
#include <vector>

/*
    class discription: 
        Base class for random generator.
*/

class secVRandGen
{
    public:

        secVRandGen();
        virtual ~secVRandGen();    
        
        //method for generate sample!
        virtual G4double Shoot(size_t PDFidx);

        //Two Dimention probably density function
        virtual G4double PDF(G4double X, size_t PDFidx) = 0;
    
    protected:
        std::vector<G4double> XminVect;
        std::vector<G4double> XmaxVect;
        std::vector<G4double> YmaxVect;

    private:
};


#endif
