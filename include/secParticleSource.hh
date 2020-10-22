#ifndef secParticleSource_hh
#define secParticleSource_hh

#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include <atomic>
/*========================================================
    class description:
        This class is a temporary particle source of sec 
    detector. 
==========================================================
*/
class secRandGenFromFile;
class secRandGenFromFx;
class G4ParticleDefinition;
class G4Event;

class secParticleSource : public G4VPrimaryGenerator
{
    public:
        //ctor and dtor
        secParticleSource();
        virtual ~secParticleSource();

        //event generator.
        virtual void GeneratePrimaryVertex(G4Event* Evt) override;
        static G4double MuonWaitTime();

        static G4double GenNoiseWaitTime( G4Int ThreadID, G4bool IsInit = true, 
                                          G4bool IsUpdate = false, G4double NoiseInten = 0. );  
        /*
            1. if IsInit = false, will create and initialize the static noise wait time array and return -1.
            2. if IsInit = true, IsUpdate = true, will Update the Local WaitTime Pointer.
            3. if IsInit = false, IsUpdate = false, will just return the NoiseWaitTime that ThreadID own.
        */     

    private:
        
        secRandGenFromFile* RandGenFile;
        secRandGenFromFx*   RandGenFx;
        void GenMuons(G4Event* Evt);
        void GenNoiseBeta(G4Event* Evt);

};

#endif
