/*
Class Description:
        This class is used to conctruct the geometry, logical volume, physical volume of the 
        detector.

Member functions:
    
    secDetectorConstruction(void)
    void ~secDetectorConstruction()
    //constructors and destructors, mostly default.

     virtual G4VPhysicalVolume* Construct(void)
    //IMPORTANT: Construct Method, construct the geometry, logical volume, physical volume, 
    region of the detector
    
    virtual void ConstructSDandField();
    //IMPORTANT: Construct the sensitive detector of the detector, currently, these logical 
    //volumes had been specified as sensitive detector: sci_log1, sci_log2, pm_log1, pm_log2.


    void SetLVOpticalProperties(G4LogicalVolume* LV, G4String& FileName)
    //Unfinish feature. We decided to use this function to set the optical properties. 
    //There are two kinds of optical properties. Some are described as mathematical functions 
    //of the photon's energy, the others are "const properties", which are considered as constant 
    //to photons' energy.

    2020/7/19
*/

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
