#ifndef secScintSD_hh
#define secScintSD_hh
#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "secSourceMacro.hh"
#include "secScintHit.hh"
#include "secSiPMSD.hh"
#include "secTrackingAction.hh"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"

class secSiPMSD;
class G4Step;
class G4HCofThisEvent;
class TFile;
class TTree;
class TBranch;

class secScintSD : public G4VSensitiveDetector
{
    friend G4bool   secSiPMSD::IsADecayEvent();
    friend G4bool   secSiPMSD::IsDoubleBangFirst();
    friend G4bool   secSiPMSD::IsDoubleBangSecond();
    friend G4bool   secSiPMSD::IsDoubleBangAbort();
    friend void     secSiPMSD::ResetDecayFlag();
    friend void     secSiPMSD::ResetDoubleBangFirstFlag();
    friend void     secSiPMSD::ResetDoubleBangSecondFlag();
    friend void     secSiPMSD::ResetDoubleBangAbortFlag();
    friend G4double secSiPMSD::GetMuonTS();
    friend G4int    secSiPMSD::GetNoiseIdx();
    friend void     secTrackingAction::PostUserTrackingAction(const G4Track* );
    
    public:
        secScintSD(const G4String &SDname, const std::vector<G4String> SDHCnameVect);
        virtual ~secScintSD();

        virtual void Initialize(G4HCofThisEvent *hitCollection);
        virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
        virtual void EndOfEvent(G4HCofThisEvent *hitCollection);
        G4bool IsKeptEvent() { return EventIsKept; }
        G4double GetDoubleBangDeltaT() { return DoubleBangDeltaT; }
    
        static TFile* pFile;
        static TTree* UpNoiseTree;
        static TTree* DownNoiseTree;
        static TTree* UpDecayTree;
        static TTree* DownDecayTree; 
        static TTree* UpNormalTree;
        static TTree* DownNormalTree;

    private:
        void     Reset(void);//user should not invoke this method!!
        static void InitDataFile();
        static void InitTrees();
        static void ReadTrees();
        void     PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter, 
                         unsigned int nbins, G4double Xmin, G4double Mmax);
                         
        void     PrintData(G4String FileName, secScintHitsCollection* pHC, secScintHit::DataGetter Getter);

        void     PrintData(G4String FileName, G4double val);

        //for event saving
        G4int    NoiseIdx;
        G4double MuonTimeStamp;
        G4double MuonTimeStampNext;
        G4double DoubleBangDeltaT;
        G4int    GetCoupledIdx(G4double MuonTS);

        G4bool   HitUp;
        G4bool   HitDown;
        G4bool   DoubleBangFirstFlag;
        G4bool   DoubleBangSecondFlag;
        G4bool   DoubleBangAbortFlag;
        G4bool   IsMuonTimeStampGened;
        G4bool   DecayFlagSiPM;
        G4bool   EventIsKept;
        std::vector<G4double> NoiseWaitTimeVect;

        G4int    DecayEventID;
        G4int    PhotonsGenUp;
        G4int    PhotonsGenDown;
        G4double PhotonEnegUp;
        G4double PhotonEnegDown;
        G4double MuonEdepUp;
        G4double MuonEdepDown;
	    G4int    FormerID;

	    secScintHitsCollection *pPhotonHCup;   //photon's hitscollection in the upper scintillator
        secScintHitsCollection *pPhotonHCdown; //photons's hitscollection in the lower scintillator
        secScintHitsCollection *pMuonHCup;     //Muon's HC up
        secScintHitsCollection *pMuonHCdown;   //Muon's HC down
};

//reset the SD at the end of each event
inline void secScintSD::Reset(void)
{
    HitUp = false;
    HitDown = false;
    EventIsKept = false;
    IsMuonTimeStampGened = false;
    PhotonsGenUp = 0;
    PhotonsGenDown = 0;
    PhotonEnegUp = 0.;
    PhotonEnegDown = 0.;
    MuonEdepUp = 0.;
    MuonEdepDown = 0.;
    FormerID = -1;
}
#endif

