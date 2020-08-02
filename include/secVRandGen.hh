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
        
        //distribution function type!
        enum DistFuncType
        {
            PDF_TYPE, 
            CDF_TYPE
        };
        //method for generate sample!
        virtual G4double Shoot(size_t PDFidx, DistFuncType Method = PDF_TYPE);

        //Two Dimention probably density function
        virtual G4double PDF(G4double X, size_t PDFidx) = 0;

        //the inverse function of cumulative distribution.
        virtual G4double InverseCDF(G4double Y, size_t CDFidx) = 0;
    
    protected:
        std::vector<G4double> XminVect;
        std::vector<G4double> XmaxVect;
        std::vector<G4double> YmaxVect;

    private:
};


#endif
