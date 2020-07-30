#include "secVRandGen.hh"
#include "Randomize.hh"
#include <iostream>
#include <cassert>

secVRandGen::secVRandGen() : 
    pdfXmin(0.),
    pdfXmax(1.),
    pdfYmax(1.)
{

}

secVRandGen::~secVRandGen()
{

}



G4double secVRandGen::Shoot(size_t PDFidx)
{
    //using the slice sampling method to shoot samples!
    //using slice sampling method can guarantee that using 
    //almost all probability density function is usable.
    G4bool IsEmpty = XminVect.empty() || XmaxVect.empty() || YmaxVect.empty();
    G4bool IsOverflow = ( PDFidx >= XminVect.size() ) || ( PDFidx >= XmaxVect.size() ) || 
                        ( PDFidx >= YmaxVect.size() );
    
    if( IsEmpty )
    {
        G4cerr << "===========================================================\n"
               << "                    Error From sec2\n"
               << "In function secVRandGen::Shoot(), Empty pdf boundary vector!\n"
               << G4endl;

        assert( true );
    }

    if( IsOverflow )
    {
        G4cerr << "===========================================================\n"
               << "                    Error From sec2\n"
               << "In function secVRandGen::Shoot(), Index OverFlow!\n"
               << G4endl;

        assert( true );
    }

    G4double samp = 0, AcptProb = 0;
    G4double pdfXmin = XminVect[PDFidx];
    G4double pdfXmax = XmaxVect[PDFidx];
    G4double pdfYmax = YmaxVect[PDFidx];

    do
    {
        samp = (pdfXmax - pdfXmin) * G4UniformRand() + pdfXmin;
        AcptProb = PDF(samp, PDFidx) / pdfYmax;
    } while ( G4UniformRand() > AcptProb );
    
    return samp;
}
