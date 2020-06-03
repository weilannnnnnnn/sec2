#ifndef secDetectorConstruction_hh
#define secDetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;

class secDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    //ctor
    secDetectorConstruction(void);
    //dtor
    virtual ~secDetectorConstruction();

    //construct method
    virtual G4VPhysicalVolume* Construct(void);

private:

    void ConstructOpticalScint (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalFoil  (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalSiPM  (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalPlate (G4Material *&, G4OpticalSurface *&);
};

#endif
