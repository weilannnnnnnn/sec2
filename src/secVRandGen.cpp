#include "secVRandGen.hh"
#include "Randomize.hh"
#include <iostream>

secVRandGen::secVRandGen()
{

}

secVRandGen::~secVRandGen()
{

}



G4double secVRandGen::Shoot(size_t PDFidx)
{
    //using the slice sampling method to shoot samples!
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
