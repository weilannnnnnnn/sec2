#include "secEventAction.hh"
#include "secAnalysis.hh"

#include "G4Event.hh"

#include "G4UnitsTable.hh"
#include <iostream>

secEventAction::secEventAction(void) :
    G4UserEventAction(),
     PhotonsGenUp(0), 
     PhotonsRecvUp(0),
     PhotonsGenDown(0),
     PhotonsRecvDown(0), 
     EfficiencyUp(0.),
     EfficiencyDown(0.)
{

}

secEventAction::~secEventAction()
{

}

void secEventAction::BeginOfEventAction(const G4Event* )
{
        
}

void secEventAction::EndOfEventAction(const G4Event* )
{
    auto AnalysisMgr = G4AnalysisManager::Instance();
    
    EfficiencyUp   = (G4double)PhotonsRecvUp / PhotonsGenUp;
    EfficiencyDown = (G4double)PhotonsRecvDown / PhotonsGenDown;
    //fill the histograms
    AnalysisMgr->FillH1(0, PhotonsGenUp);
    AnalysisMgr->FillH1(1, PhotonsGenDown);
    AnalysisMgr->FillH1(2, PhotonsRecvUp);
    AnalysisMgr->FillH1(3, PhotonsRecvDown);
    AnalysisMgr->FillH1(4, EfficiencyUp);
    AnalysisMgr->FillH1(5, EfficiencyDown);
    
    static int i = 0;

    i++;
    std::cout << "Event Number: " << i << '\n';
    std::cout << "PhotonsGenUp = " << PhotonsGenUp << '\n';
    std::cout << "PhotonsGenDown = " << PhotonsGenDown << '\n';
    std::cout << "PhotonsRecvUp = " << PhotonsRecvUp << '\n';
    std::cout << "PhotonsRecvDown = " << PhotonsRecvDown << '\n';

    Reset();
}
