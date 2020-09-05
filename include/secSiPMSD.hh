#ifndef secSiPMSD_hh
#define secSiPMSD_hh

#include "secSiPMHit.hh"
#include "secAnalysis.hh"

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include <vector>
#include <memory>

class secScintSD;
class G4Step;
class G4HCofThisEvent;
class TH1D;
class TFile;

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
    
    //output methods!

        //print data in CERN ROOT format
        TFile* CreateFile(G4String FileName);
        void FillHist(TH1D* HistPtr, secSiPMHitsCollection* pHC, secSiPMHit::DataGetter Getter);
        //remember to close the file.

        //print data in ASCII / Binary format
        //fill the values in the Hits into a histogram and print the histogram,
        //the empty bins will be ignored!
        void PrintData(G4String FileName, G4String HistName, 
		       secSiPMHitsCollection* pHC, 
		       secSiPMHit::DataGetter Getter, 
		       unsigned int nbins, G4double Xmin, G4double Xmax);
        
        //directly print the values in the Hits
        void PrintData(G4String FileName, G4String HCname,
                       secSiPMHitsCollection* pHC, 
		       secSiPMHit::DataGetter Getter);
        
        //print the value generated in a single event.
        void PrintData(G4String FileName, G4double val);
        
        G4int DecayEventID;
        G4int NoiseResponseID;
        G4int NormalResponseID;
        G4bool IsMuon;
        G4bool IsNoise;
        G4bool HasEntered;
        G4double EventWaitTime;
        std::vector<G4double> NoiseWaitTimeVect;

        TFile* pFile;
        secScintSD* pScintSD;
        secSiPMHitsCollection *pHCup;
        secSiPMHitsCollection *pHCdown;
};


#endif
