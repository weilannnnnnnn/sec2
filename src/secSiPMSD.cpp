#include "secScintSD.hh"
#include "secSiPMSD.hh"
#include "secSourceMacro.hh"
#include "secParticleSource.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Threading.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "Randomize.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TBranch.h"

#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cmath>

//lock for saving data.
std::mutex mtx_SiPM;

secSiPMSD::secSiPMSD(const G4String &SDname, const std::vector<G4String> SDHCnameVect, secScintSD* pSD) : 
    G4VSensitiveDetector(SDname),
    DecayEventID(0),
    NoiseResponseID(0),
    NormalResponseID(0),
    EventWaitTime(0.),
    IsFirstMuonInDoubleBang(true),
    UpDoubleBangHist("UpDoubleBangHist", 8000, 0., 20000.*ns),
    DownDoubleBangHist("DownDoubleBangHist", 8000, 0., 20000.*ns),
    pScintSD(pSD),
    pHCup(nullptr),
    pHCdown(nullptr)
{
/* register the names of the HCs to G4VSensitiveDetector object so
    that the HCids which are assigned by G4 Kenel could be assessed by
    G4SDManager object */
    for(G4String str : SDHCnameVect)
    {
        collectionName.insert(str);
    }
}

secSiPMSD::~secSiPMSD()
{
}

void secSiPMSD::Initialize(G4HCofThisEvent* HC)
{
    //SensitiveDetectorName is a string defined in G4VSensitiveDetector.hh
    //collectionName is a STL vector defined in G4CollectionVector.hh

    //create HCs
    auto SDmgr = G4SDManager::GetSDMpointer();
    pHCup   = new secSiPMHitsCollection(SensitiveDetectorName, collectionName[0]);
    pHCdown = new secSiPMHitsCollection(SensitiveDetectorName, collectionName[1]);

    //Get collection IDs from the G4SDManager object, and register the created HCs to G4HCofThisEvent object.
    G4int HCIDup   = SDmgr->GetCollectionID(collectionName[0]);
    G4int HCIDdown = SDmgr->GetCollectionID(collectionName[1]);

    HC->AddHitsCollection(HCIDup, pHCup);
    HC->AddHitsCollection(HCIDdown, pHCdown);
    EventType = secParticleSource::GetEventType();
}

G4bool secSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory* )
{
    //the return value of this function is currently researved and
    //it may be used in the future update of Geant4 application.

/*============================================================================
    The case that other particles hit the sipm. Function will retrun and do 
    nothing.
==============================================================================*/
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();    
    if( *ParticleNow != *G4OpticalPhoton::Definition() )
    {
        //not an optical photon, return, DO NOT generate hit
        return false;
    }

/*============================================================================    
    The case that an optical photon hits one of the SiPM! the copy number of 
    the UPPER SiPM is 1 and that of the lower one is 2!!
===============================================================================*/
    
    //this is a hard coded version. may be changed in the future.
    //generate the time stamp for each event!

    const G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4double aPhotonEneg = step->GetPreStepPoint()->GetKineticEnergy(); //MeV
    G4double GlobalTime  = step->GetTrack()->GetGlobalTime() ;          //ns
    //the photons is absorbed by the SiPM, so the track must be killed
    step->GetTrack()->SetTrackStatus(fStopAndKill);
    if( VolumeCpyNb == 1 )
    {
        //upper SiPM!!
	    auto newHitUp = new secSiPMHit();
        (*newHitUp).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCup->insert(newHitUp);
    }
    else if( VolumeCpyNb == 2 )
    {
        //lower SiPM!!
        auto newHitDown = new secSiPMHit();
        (*newHitDown).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCdown->insert(newHitDown);
    }
    else
    {
        //Do nothing, never reach here.
    }
    return true;
}

void secSiPMSD::EndOfEvent(G4HCofThisEvent*)
{
    //At the end of event, specify the type of the event and save the result
    //generate time stamp.
    if( EventType == secParticleSource::Muons )
        EventWaitTime = GetMuonTS();

    else if( EventType != secParticleSource::Muons )
        EventWaitTime = secParticleSource::GenNoiseWaitTime( G4Threading::G4GetThreadId() );

    // empty HC, the PM haven't been triggered!
    if( !(pHCup->GetSize()) && !(pHCdown->GetSize()) )
    {
        ResetDecayFlag();
        ResetDoubleBangFlag();
	    return;//ignore the event that didn't trigger both of the SiPMs!
    }
    else if( EventType != secParticleSource::Muons ) // the PM is Triggered by Noise particle( mainly electrons )
    {
        ++NoiseResponseID;
        //using mutex lock, because CERN ROOT doesn't support handling multiple TFiles in different threads.
        // What a BAD feature !
    //=====================================================================
                       //Creating Noise Histograms
        //Up Histogram
        if( pHCdown->GetSize() < 800 )
        {//ignore the noise events with less than 100 photons' response.
            return;
        }
		mtx_SiPM.lock();
		tools::histo::h1d UpHist("UpNoiseHist", 160, 0., 400.*ns);
		FillG4Hist(pHCup, &secSiPMHit::GetGlobalTime, &UpHist);
        G4Hist2TTree(&UpHist, secScintSD::UpNoiseTree);

		//Down histogram		
        tools::histo::h1d DownHist("DownNoiseHist", 160, 0., 400.*ns);
        FillG4Hist(pHCdown, &secSiPMHit::GetGlobalTime, &DownHist);
        G4Hist2TTree(&DownHist, secScintSD::DownNoiseTree);
    	mtx_SiPM.unlock();
	//=====================================================================
    	//PrintData("NoiseWaitTime.txt", EventWaitTime);
	}
    else if( IsADecayEvent() ) // a decay event
    {
        if( !(pHCup->GetSize()) || !(pHCdown->GetSize()) )
        {
            ResetDecayFlag();
            return;
        }
        DecayEventID++;
		//never forget this!!
        ResetDecayFlag();

    //====================================================================
                        //Creating Decay Histograms
	    mtx_SiPM.lock();
        tools::histo::h1d UpHist("UpDecayHist", 8000, 0., 20000.*ns);
        FillG4Hist(pHCup, &secSiPMHit::GetGlobalTime, &UpHist);
        G4Hist2TTree(&UpHist, secScintSD::UpDecayTree);

        tools::histo::h1d DownHist("DownDecayHist", 8000, 0., 20000.*ns);
        FillG4Hist(pHCdown, &secSiPMHit::GetGlobalTime, &DownHist);
        G4Hist2TTree(&DownHist, secScintSD::DownDecayTree);
        mtx_SiPM.unlock();
    //====================================================================
    }
    else if( IsADoubleBang() )
    {
        if( !(pHCup->GetSize()) || !(pHCdown->GetSize()) )
        {
            ResetDoubleBangFlag();
            return;
        }
        if( IsFirstMuonInDoubleBang )
        {
            IsFirstMuonInDoubleBang = false;
            FillG4Hist(pHCup,   &secSiPMHit::GetGlobalTime,   &UpDoubleBangHist);
            FillG4Hist(pHCdown, &secSiPMHit::GetGlobalTime, &DownDoubleBangHist);
        }
        else
        {
            IsFirstMuonInDoubleBang = true; // reset for next double-bang event.
            ResetDoubleBangFlag();
            const double DeltaT = pScintSD->GetDoubleBangDeltaT();
            FillG4HistDoubleBang(pHCup,   &secSiPMHit::GetGlobalTime, &UpDoubleBangHist,   DeltaT);
            FillG4HistDoubleBang(pHCdown, &secSiPMHit::GetGlobalTime, &DownDoubleBangHist, DeltaT);
        //=================================================================
                            //Creating double-bang histogram
            mtx_SiPM.lock();
            G4Hist2TTree(&UpDoubleBangHist,   secScintSD::UpNormalTree);
            G4Hist2TTree(&DownDoubleBangHist, secScintSD::DownNormalTree);
            mtx_SiPM.unlock();
        //=================================================================
            UpDoubleBangHist.reset();
            DownDoubleBangHist.reset();
        }
        
    }
    else // normal muon events
    {      
    //==========================================================================
                                //Creating Normal Histograms

        //NOTICE: fill the extra branches first, or you may run into nasty problems.
        unsigned idx = GetNoiseIdx();
        mtx_SiPM.lock();
        tools::histo::h1d UpHist("UpNormalHist", 160, 0., 400.*ns);
        //TBranch* BranchUpIdx = secScintSD::UpNormalTree->GetBranch("Coupled index");
        //BranchUpIdx->SetAddress(&idx);
        secScintSD::UpNormalTree->SetBranchAddress("Coupled index", &idx);
        //BranchUpIdx->Fill();
        FillG4Hist(pHCup, &secSiPMHit::GetGlobalTime, &UpHist);
        G4Hist2TTree(&UpHist, secScintSD::UpNormalTree);
        
        tools::histo::h1d DownHist("DownNormalHist", 160, 0., 400.*ns);
        //TBranch* BranchDownIdx = secScintSD::DownNormalTree->GetBranch("Coupled index");
        //BranchDownIdx->SetAddress(&idx);
        secScintSD::DownNormalTree->SetBranchAddress("Coupled index", &idx);
        //BranchDownIdx->Fill();
        FillG4Hist(pHCdown, &secSiPMHit::GetGlobalTime, &DownHist);
        G4Hist2TTree(&DownHist, secScintSD::DownNormalTree);
        mtx_SiPM.unlock();
    //==========================================================================
    }
}

G4bool secSiPMSD::IsADecayEvent()
{
    return pScintSD->DecayFlagSiPM;    
}

G4bool secSiPMSD::IsADoubleBang()
{
    return pScintSD->IsDoubleBang;
}

void secSiPMSD::ResetDecayFlag()
{
    pScintSD->DecayFlagSiPM = false;
}

void secSiPMSD::ResetDoubleBangFlag()
{
    pScintSD->IsDoubleBang = false;
}

G4double secSiPMSD::GetMuonTS()  
{ 
	return pScintSD->MuonTimeStamp; 
}
G4double secSiPMSD::GetNoiseIdx() 
{ 
	return pScintSD->NoiseIdx; 
}
void secSiPMSD::FillRootHist(TH1D* pHist, secSiPMHitsCollection* pHC, secSiPMHit::DataGetter Getter)
{
    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        pHist->Fill(val);
    }
}
//print histogram version
void secSiPMSD::PrintData(G4String FileName, G4String HistName, 
		                  secSiPMHitsCollection* pHC, 
			              secSiPMHit::DataGetter Getter, 
			              unsigned int nbins, G4double Xmin, G4double Xmax)
{
    //fill histogram
    tools::histo::h1d hist("aHist", nbins, Xmin, Xmax);
    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {   
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        hist.fill(val);
    }

    //create files
    std::ostringstream sstrm;
    sstrm << FileName << "_t" << G4Threading::G4GetThreadId();
    std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary);
    
    if( !fstrm.is_open() )
    {
        std::cerr << "***Unable to open file: " << FileName << " ***" << '\n';
        std::cerr << "***The generated data will not be recorded!***" << '\n';
    }

    //assert( fstrm.is_open() );
    fstrm << HistName + "_t" << G4Threading::G4GetThreadId() << '\n';
    
    //write file
    const std::vector<unsigned int>& entries = hist.bins_entries();
    std::vector<G4double> edges;
    edges.push_back(-1.);
    const G4double binw = (Xmax - Xmin) / nbins;
    for( size_t i = 0; i != nbins + 1; ++i )
    {
	    const G4double edge = Xmin + i * binw;
        edges.push_back( edge );
    }
    //std::cout << "entries size = " << entries.size() << " edges size = " << edges.size() << '\n';
    size_t sz = entries.size() <= edges.size() ? entries.size() : edges.size();
    
    for( size_t i = 0; i != sz; ++i )
    {
        if( entries[i] == 0 )
	    {
            continue;
	    }
        fstrm << edges[i] << '\t' << entries[i] << '\n';
    }

    fstrm.flush();
    //close files
    fstrm.close();
}

//direct print HC version
void secSiPMSD::PrintData(G4String FileName, G4String HCname,
                          secSiPMHitsCollection* pHC, 
			              secSiPMHit::DataGetter Getter)
{
    //create files
    std::ostringstream sstrm;
    sstrm << FileName << "_t" << G4Threading::G4GetThreadId();
    std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary);
    
    if( !fstrm.is_open() )
    {
        std::cerr << "***Unable to open file: " << FileName << " ***" << '\n';
        std::cerr << "***The generated data will not be recorded!***" << '\n';
    }

    //assert( fstrm.is_open() );
    fstrm << HCname + "_t" << G4Threading::G4GetThreadId() << '\n';

    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        fstrm << val << '\n';
    }    

    fstrm.flush();
    //close files
    fstrm.close();

}

//print single double value version
void secSiPMSD::PrintData(G4String FileName, G4double val)
{
    	std::ostringstream sstrm;
        sstrm << FileName << "_t" << G4Threading::G4GetThreadId();

        std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary );
        fstrm << val << '\n';
}

void secSiPMSD::FillG4Hist(secSiPMHitsCollection* pHC,
                           secSiPMHit::DataGetter Getter,
                           tools::histo::h1d* histptr)
{
    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {   
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )();
        histptr->fill(val);
    }
}

void secSiPMSD::FillG4HistDoubleBang(secSiPMHitsCollection* pHC, 
                                     secSiPMHit::DataGetter Getter,
                                     tools::histo::h1d* histptr,
                                    const G4double DeltaT)
{
    for( size_t i = 0; i != pHC->GetSize(); ++i )
    {
        G4double val = ( ( (*pHC)[i] ) ->* (Getter) )() + DeltaT;
        histptr->fill(val);
    }
}

void secSiPMSD::G4Hist2TTree(tools::histo::h1d* histptr,
                             TTree* DataTree)
{
    const std::vector<unsigned int>& EntriesVect = histptr->bins_entries();    
	unsigned sz = EntriesVect.size() - 2; // remove the underflow and overflow entries.
	
	DataTree->SetBranchAddress("ArraySize", &sz);
    DataTree->SetBranchAddress("Entries", (unsigned*) &(EntriesVect[1]));
    DataTree->SetBranchAddress("TimeStamp", &EventWaitTime);
        
	//fill the entire tree here, you had better fill the extra branches at first.
	DataTree->Fill();
}
