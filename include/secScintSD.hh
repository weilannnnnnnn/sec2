#ifndef secScintSD_hh
#define secScintSD_hh
#include "G4VSensitiveDetector.hh"
#include "secScintHit.hh"
#include "secSiPMSD.hh"
#include "globals.hh"

class G4Step;
class G4HCofThisEvent;

class secScintSD : public G4VSensitiveDetector
{
    friend G4bool secSiPMSD::IsADecayEvent();
    friend void secSiPMSD::ResetDecayFlag();

    public:
        secScintSD(const G4String &SDname, const std::vector<G4String> SDHCnameVect);
        virtual ~secScintSD();

        virtual void Initialize(G4HCofThisEvent *hitCollection);
        virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
        virtual void EndOfEvent(G4HCofThisEvent *hitCollection);

    private:
        void     Reset(void);//user should not invoke this method!!

        void     PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter, 
                         unsigned int nbins, G4double Xmin, G4double Mmax);
                         
        void     PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter);

        void     PrintData(G4String FileName, G4double val);

        G4bool   DecayFlagSiPM;
        G4bool   DecayFlagScint;
        G4int    DecayEventID;
	    G4double DecayTime;
        G4int    PhotonsGenUp;
        G4int    PhotonsGenDown;
        G4double PhotonEnegUp;
        G4double PhotonEnegDown;
        G4double MuonEdepUp;
        G4double MuonEdepDown;
        G4double MuonArriveTime;
	    G4int    FormerID;

        secScintHitsCollection *pPhotonHCup;   //photon's hitscollection in the upper scintillator
        secScintHitsCollection *pPhotonHCdown; //photons's hitscollection in the lower scintillator
        secScintHitsCollection *pMuonHCup;     //Muon's HC up
        secScintHitsCollection *pMuonHCdown;   //Muon's HC down
};

inline void secScintSD::Reset(void)
{
    PhotonsGenUp = 0;
    PhotonsGenDown = 0;
    PhotonEnegUp = 0.;
    PhotonEnegDown = 0.;
    MuonEdepUp = 0.;
    MuonEdepDown = 0.;
    FormerID = -1;
}

#endif
