#ifndef secRandGenFromFx_hh
#define secRandGenFromFx_hh

#include "secVRandGen.hh"
#include "globals.hh"

/*==========================================================================
    class discription:
        this class is used to generate the random sample from analytic PDF 
        or CDF. We usually call these functions f(x). So that's the reason I 
        called this class secRandGenFrom"Fx"
============================================================================
*/

class secRandGenFromFx : public secVRandGen
{
    public:

        static secRandGenFromFx* GetInstance();

        virtual G4double Shoot(size_t idx, secVRandGen::DistFuncType Type = secVRandGen::PDF_TYPE);
        
        //users can set there own probability density function here and 
        //Shoot() method here.
        virtual G4double PDF(G4double X, size_t idx);
        virtual G4double InverseCDF(G4double Y, size_t idx);
        
        secRandGenFromFx(secRandGenFromFx const&) = delete;
        void operator=(secRandGenFromFx const&) = delete;

    private:
        secRandGenFromFx();
        virtual ~secRandGenFromFx();
};

#endif
