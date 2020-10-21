#ifndef secParticleSource_hh
#define secParticleSource_hh

#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"

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
        static G4double GenNoiseWaitTime( G4bool IsUpdate = false );        
    private:
        
        secRandGenFromFile* RandGenFile;
        secRandGenFromFx*   RandGenFx;
        void GenMuons(G4Event* Evt);
        void GenNoiseBeta(G4Event* Evt);

};

#endif
