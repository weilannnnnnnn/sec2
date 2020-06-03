#ifndef secSiPMSD_hh
#define secSiPMSD_hh

#include "G4VSensitiveDetector.hh"
#include "secSiPMHit.hh"
#include "globals.hh"

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
        
        G4int DecayEventID;
        secScintSD* pScintSD;
        secSiPMHitsCollection *pHCup;
        secSiPMHitsCollection *pHCdown;
};


#endif
