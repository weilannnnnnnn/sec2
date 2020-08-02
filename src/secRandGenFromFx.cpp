#include "secRandGenFromFx.hh"

secRandGenFromFx* secRandGenFromFx::GetInstance()
{
    static secRandGenFromFx theInstance;
    return &theInstance;

}

G4double secRandGenFromFx::PDF(G4double, size_t)
{
    //incomplete method.
    return 0.;
}

G4double secRandGenFromFx::InverseCDF(G4double, size_t)
{  
    //incomplete method.
    return 0;
}

G4double secRandGenFromFx::Shoot(size_t, secVRandGen::DistFuncType)
{
    //incomplete method.
    return 0.;
}

secRandGenFromFx::secRandGenFromFx()
{

}

secRandGenFromFx::~secRandGenFromFx()
{

}
