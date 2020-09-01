#ifndef secSiPMSD_hh
#define secSiPMSD_hh

#include "G4VSensitiveDetector.hh"
#include "secSiPMHit.hh"
#include "globals.hh"
#include <initializer_list>
#include <vector>

class secScintSD;
class G4Step;
class G4HCofThisEvent;


class secSiPMSD : public G4VSensitiveDetector
{

    public:
        
        secSiPMSD(const G4String& SDname, const std::vector<G4String> SDHCnameVect, secScintSD* pSD);
        virtual ~secSiPMSD();

        //from G4VSensitiveDetector
        virtual void Initialize(G4HCofThisEvent *hitCollection);
        virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
        virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
        
        G4bool IsADecayEvent(void);
        void ResetDecayFlag(void);
        
    private:
        
        void PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter, 
                         unsigned int nbins, G4double Xmin, G4double Xmax);
        
        void PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter);
        
        void PrintData(G4String FileName, G4double val);
        
        G4int DecayEventID;
        G4bool IsNoise; // a hard-coded version for noise simulation
        G4double EventWaitTime;
        std::vector<G4double> NoiseWaitTimeVect;

        secScintSD* pScintSD;
        secSiPMHitsCollection *pHCup;
        secSiPMHitsCollection *pHCdown;
};


#endif
