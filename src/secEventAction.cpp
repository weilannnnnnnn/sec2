#include "secEventAction.hh"
#include "secAnalysis.hh"
#include "secScintHit.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

#include "G4UnitsTable.hh"
secEventAction::secEventAction(void) :
    G4UserEventAction(),
     PhotonsRecvUp(0),
     PhotonsRecvDown(0) 
{

}

secEventAction::~secEventAction()
{

}

void secEventAction::BeginOfEventAction(const G4Event* )
{
     
}

void secEventAction::EndOfEventAction(const G4Event* )
{/*   
    //Analysis Manager and Sensitive Detector Manager.
    auto AnalysisMgr = G4AnalysisManager::Instance();

    AnalysisMgr->FillH1(2, PhotonsRecvUp);
    AnalysisMgr->FillH1(3, PhotonsRecvDown);

    Reset();
*/
}
