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
    virtual void ConstructSDandField();

    void SetLVOpticalProperties(G4LogicalVolume* LV, G4String& FileName);
/*
    this function is used to configure the optical properties (e.g. scintillation)
    of logical volume. The properties are discribed as functions of optical photons'
    energy. 
*/
private:


    void ConstructOpticalScint (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalFoil  (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalSiPM  (G4Material *&, G4OpticalSurface *&);
    void ConstructOpticalPlate (G4Material *&, G4OpticalSurface *&);
};

#endif
