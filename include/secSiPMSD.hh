#ifndef secSiPMSD_hh
#define secSiPMSD_hh

#include "secSiPMHit.hh"
<<<<<<< HEAD
=======
#include "secParticleSource.hh"
>>>>>>> secSourceMacro
#include "G4VSensitiveDetector.hh"
#include "tools/histo/h1d"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"
#include "secRunAction.hh"
#include <vector>
#include <memory>

class G4Step;
class secScintSD;
class G4HCofThisEvent;
class TH1D;
class h1d;
class TTree;
class TBranch;

class secSiPMSD : public G4VSensitiveDetector
{
    friend class secRunAction;
    public:
        
        secSiPMSD(const G4String& SDname, const std::vector<G4String> SDHCnameVect, secScintSD* pSD);
        virtual ~secSiPMSD();

        //from G4VSensitiveDetector
        virtual void Initialize(G4HCofThisEvent *hitCollection);
        virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
        virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
        
        G4bool IsADecayEvent(void);
        void ResetDecayFlag(void);
		G4double GetMuonTS();
		G4double GetNoiseIdx();
        
		static TFile* pFile;

    private:

        //print data in ASCII / Binary format
        //fill the values in the Hits into a histogram and print the histogram,
        //the empty bins will be ignored!
        void FillRootHist(TH1D* pHist, secSiPMHitsCollection* pHC, secSiPMHit::DataGetter Getter);

        //this function fills the histogram, using the data from hits collection.
        //class tools::histo::h1d comes with Geant4 kernel. 
        void FillG4Hist(secSiPMHitsCollection* pHC, 
                        secSiPMHit::DataGetter Getter,
                        tools::histo::h1d* histptr);

        //this retrives the data from h1d, and fill them into a tree's branch.
        void G4Hist2TTree(tools::histo::h1d* histptr,
                          TTree* DataTree);
        
        //out of date method.
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

        secParticleSource::secSourceGenType EventType;
        G4double EventWaitTime;
        std::vector<G4double> NoiseWaitTimeVect;
        
        secScintSD* pScintSD;
        secSiPMHitsCollection *pHCup;
        secSiPMHitsCollection *pHCdown;

        static TTree* UpNoiseTree;
        static TTree* DownNoiseTree;
        static TTree* UpDecayTree;
        static TTree* DownDecayTree;
        static TTree* UpNormalTree;
        static TTree* DownNormalTree;
};

#endif
