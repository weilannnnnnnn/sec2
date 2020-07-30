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

        secRandGenFromFx* GetInstance();

        virtual G4double Shoot(size_t CDFidx);
        
        //users can set there own probability density function here and 
        //Shoot() method here.
        virtual G4double PDF(G4double X, size_t CDFidx);
        
        secRandGenFromFx(secRandGenFromFx const&) = delete;
        void operator=(secRandGenFromFx const&) = delete;

    private:
        secRandGenFromFx();
        virtual ~secRandGenFromFx();
};

#endif
