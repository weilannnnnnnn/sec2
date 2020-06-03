//file sec_main.cpp
#include "secActionInitialization.hh"
#include "secDetectorConstruction.hh"
#include "secPhysicsList.hh"

#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(4);

    runManager -> SetUserInitialization( new secDetectorConstruction() );
//---------------------------------------------------------------------

    runManager -> SetUserInitialization(new secPhysicsList());

//---------------------------------------------------------------------
    runManager -> SetUserInitialization( new secActionInitialization() );
//---------------------------------------------------------------------

    auto visManager = new G4VisExecutive;
    visManager -> Initialize();

    auto UImanager = G4UImanager::GetUIpointer();

    if( !ui )
    {
	G4String command = "/control/execute ";
	G4String fileName = argv[1];
	UImanager->ApplyCommand(command + fileName);
    }
    else
    {
	UImanager->ApplyCommand("/control/execute init_vis.mac");
	ui -> SessionStart();
        delete ui;
    }

//----------------------------------------------------------------------

    delete visManager;
    delete runManager;

}
