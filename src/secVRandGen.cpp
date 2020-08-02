#include "secVRandGen.hh"
#include "Randomize.hh"
#include <iostream>
#include <cassert>

secVRandGen::secVRandGen()
{

}

secVRandGen::~secVRandGen()
{

}



G4double secVRandGen::Shoot(size_t idx, DistFuncType Method)
{
    //using the slice sampling method to shoot samples!
    //using slice sampling method can guarantee that using 
    //almost all probability density function is usable.
    G4bool IsEmpty = XminVect.empty() || XmaxVect.empty() || YmaxVect.empty();
    G4bool IsOverflow = ( idx >= XminVect.size() ) || ( idx >= XmaxVect.size() ) || 
                        ( idx >= YmaxVect.size() );
    
    if( IsEmpty )
    {
        std::cerr << "===========================================================\n"
                  << "                    Error From sec2\n"
                  << "In function secVRandGen::Shoot(), Empty pdf boundary vector!\n"
                  << std::endl;

        assert( true );
    }

    if( IsOverflow )
    {
        std::cerr << "===========================================================\n"
                  << "                    Error From sec2\n"
                  << "In function secVRandGen::Shoot(), Index OverFlow!\n"
                  << std::endl;

        assert( true );
    }

    if( Method == PDF_TYPE )
    {
        G4double samp = 0, AcptProb = 0;
        G4double pdfXmin = XminVect[idx];
        G4double pdfXmax = XmaxVect[idx];
        G4double pdfYmax = YmaxVect[idx];

        do
        {
            samp = (pdfXmax - pdfXmin) * G4UniformRand() + pdfXmin;
            AcptProb = PDF(samp, idx) / pdfYmax;
        } while ( G4UniformRand() > AcptProb );
        
        return samp;
    }
    
    if( Method == CDF_TYPE )
    {
        return InverseCDF(G4UniformRand(), idx);
    }
    std::cerr << "===========================================================\n"
              << "                    Error From sec2\n"
              << "In function secVRandGen::Shoot(), Illegal distribution fun-\n"
              << "ction Type\n"
              << std::endl;

    assert( true );
    return 1.;
    
}
