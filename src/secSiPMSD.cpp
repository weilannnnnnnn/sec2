#include "secSiPMSD.hh"
#include "secScintSD.hh"
#include "secAnalysis.hh"
#include "secParticleSource.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Threading.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SystemOfUnits.hh"
#include "tools/histo/h1d"

#include "TFile.h"
#include "TH1D.h"

#include <mutex>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cassert>

class G4VPhysicalVolume;
std::mutex mtx;

secSiPMSD::secSiPMSD(const G4String &SDname, const std::vector<G4String> SDHCnameVect, secScintSD* pSD) : 
    G4VSensitiveDetector(SDname),
    DecayEventID(0),
    NoiseResponseID(0),
    NormalResponseID(0),
    IsMuon(false),
    IsNoise(false),
    HasEntered(false),
    EventWaitTime(0.),
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

    std::ifstream ifstrm;
    ifstrm.open("NoiseWaitTime.dat", std::ifstream::in);
    if( ifstrm.is_open() )
    {
        std::string line;
        while( getline(ifstrm, line) )
        {
            NoiseWaitTimeVect.push_back( atof( line.c_str() ) );
        }
    }
    ifstrm.close();
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

}

G4bool secSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory* )
{
//the return value of this function is currently researved and
//it may be used in the future update of Geant4 application.
    auto ParticleNow = step->GetTrack()->GetParticleDefinition();    
    if( *ParticleNow != *G4OpticalPhoton::Definition() )
    {
        //not an optical photon, return, DO NOT generate hit
        return false;
    }
    //the case that an optical photon hits one of the SiPM
    //the copy number of the UPPER SiPM is 1 and that of the lower one is 2!!
    //generate the time stamp for each event!
    IsMuon = pScintSD->IsMuon();
    IsNoise = pScintSD->IsNoise();
    
    if( !HasEntered && IsMuon )
    {
        HasEntered = true;
        EventWaitTime = secParticleSource::MuonWaitTime();
    }
    
    const G4int VolumeCpyNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4double aPhotonEneg = step->GetPreStepPoint()->GetKineticEnergy(); //MeV
    G4double GlobalTime  = step->GetTrack()->GetGlobalTime() ;
    //the photons is absorbed by the SiPM, so the track must be killed
    step->GetTrack()->SetTrackStatus(fStopAndKill);
    if( VolumeCpyNb == 1 )
    {
        //upper SiPM!!
	    //std::cout << "In Up" << std::endl;
	    auto newHitUp = new secSiPMHit();
        (*newHitUp).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCup->insert(newHitUp);
    }
    else if( VolumeCpyNb == 2 )
    {
        //lower SiPM!!
        //std::cout << "In Down" << std::endl;
        auto newHitDown = new secSiPMHit();
        (*newHitDown).SetPhotonEneg(aPhotonEneg).SetGlobalTime(GlobalTime);
        pHCdown->insert(newHitDown);
    }
    else
    {
        //do nothing.
    }
    return true;
}

void secSiPMSD::EndOfEvent(G4HCofThisEvent*)
{  
/*
    At the end of event, specify the type of the event and save the result
*/
    const G4double BackTimeWindow = 20000*ns;
    const G4double FrontTimeWindow = 100*ns;
    
    if( !(pHCup->GetSize()) && !(pHCdown->GetSize()) ) // empty HC, the PM haven't been triggered!
    {
        ResetDecayFlag();
        //ignore the event that didn't trigger both of the SiPMs!
	    return;
    }
    else if( IsNoise ) // the PM is Triggered by Noise particle( mainly electrons )
    {
        ++NoiseResponseID;
	    G4String UpName = "UpNoiseResponse ", DownName = "DownNoiseResponse ";
        char Buf[50] = {};
        sprintf(Buf, "%d_t%d", NoiseResponseID, G4Threading::G4GetThreadId());
        UpName += Buf;
        DownName += Buf;
        
	    mtx.lock();
        //creating Up histograms
        pFile->cd("UpNoise");
	    TH1D UpHist(UpName.c_str(), UpName.c_str(), 160, 0., 400.*ns);
	    FillRootHist(&UpHist, pHCup, &secSiPMHit::GetGlobalTime);
        
        pFile->cd("DownNoise");
	    TH1D DownHist(DownName.c_str(), DownName.c_str(), 160, 0., 400.*ns);
        FillRootHist(&DownHist, pHCdown, &secSiPMHit::GetGlobalTime);
	
    	UpHist.Write();
        DownHist.Write();	
        //creating Down Histograms
        mtx.unlock();
    }
    else if( IsADecayEvent() ) // a decay event
    {
        if( !(pHCup->GetSize()) || !(pHCdown->GetSize()) )
        {
            ResetDecayFlag();
            return;
        }
        DecayEventID++;
        ResetDecayFlag();
        G4String UpName = "UpDecayID ", DownName = "DownDecayID ";
        char Buf[50] = {};
        sprintf(Buf, "%d_t%d", DecayEventID, G4Threading::G4GetThreadId());
        UpName += Buf;
        DownName += Buf;
        
	    mtx.lock();

        pFile->cd("UpDecay");
        TH1D UpHist(UpName.c_str(), UpName.c_str(), 8000, 0., 20000.*ns);
        FillRootHist(&UpHist, pHCup, &secSiPMHit::GetGlobalTime);

        pFile->cd("DownDecay");
        TH1D DownHist(DownName.c_str(), DownName.c_str(), 8000, 0., 20000.*ns);
        FillRootHist(&DownHist, pHCdown, &secSiPMHit::GetGlobalTime);

        UpHist.Write();
        DownHist.Write();
        mtx.unlock();
        
        char Buf[10] = {};        
        sprintf(Buf, "%d", DecayEventID);
        G4String Description = "DecayEvtID ";
        Description += Buf;

	    PrintData("DecayMuonWaitTime.dat", Description, EventWaitTime);
    }
    else // normal muon events
    {
        G4bool IsPrint = false;
        const size_t sz = NoiseWaitTimeVect.size();

        static thread_local size_t idx = 0;

        //locate the closest noise wait time
        while( EventWaitTime - NoiseWaitTimeVect[idx] > 0 )
        {
            ++idx;
            if( idx == sz )
            {
                break;
            }
        }
        
        if( idx == 0 )
        {
            if( NoiseWaitTimeVect[idx] - EventWaitTime <= BackTimeWindow )
                IsPrint = true;
        }
        else if( idx == sz )
        {
            if( EventWaitTime - NoiseWaitTimeVect[sz-1] <= FrontTimeWindow )
            {
                IsPrint = true;
            }
        }
        else if ( EventWaitTime - NoiseWaitTimeVect[idx-1] <= FrontTimeWindow ||
                  NoiseWaitTimeVect[idx] - EventWaitTime <= BackTimeWindow )
        {
                IsPrint = true;
        }
        else
        {
            //do nothing
        }
        
        if( IsPrint )
        {
            ++NormalResponseID;
            G4String UpName = "UpNormalID ", DownName = "DownNormalID ";
            char Buf[50] = {};
            sprintf(Buf, "%d", NormalResponseID);
            UpName += Buf;
            DownName += Buf;

            mtx.lock();
            pFile->cd("UpNorm");
            TH1D UpHist(UpName.c_str(); UpName.c_str(), 160, 0., 400.*ns);
            FillRootHist(&UpHist, pHCup, &secSiPMHit::GetGlobalTime);

            pFile->cd("UpNorm");
            TH1D DownHist(DownName.c_str(), DownName.c_str(), 160, 0., 400.*ns);
            FillRootHist(&DownHist, pHCdown, &secSiPMHit::GetGlobalTime);

            UpHist->Write();
            DownHist->Write();
            mtx.unlock();

            char Buf[10] = {};
            sprintf(Buf, "%d", NormalResponseID);
            G4String Description = "NormID ";
    	    PrintData("NormalMuonWaitTime.dat", Description, EventWaitTime);
        }
    }
    //reset the flag for generating the time stamp at the end of the event!
    HasEntered = false;
}

G4bool secSiPMSD::IsADecayEvent()
{
    return pScintSD->DecayFlagSiPM;    
}
void secSiPMSD::ResetDecayFlag()
{
    pScintSD->DecayFlagSiPM = false;
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
void secSiPMSD::PrintData(G4String FileName, G4String ValDescription, G4double val)
{
    	std::ostringstream sstrm;
        sstrm << FileName << "_t" << G4Threading::G4GetThreadId();

        std::ofstream fstrm(sstrm.str(), std::ofstream::app | std::ofstream::binary );
        fstrm << ValDescription << "_t" << G4Threading::G4GetThreadId() << val << '\n';
}
