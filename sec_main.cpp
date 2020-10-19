//file sec_main.cpp
#include "secActionInitialization.hh"
#include "secDetectorConstruction.hh"
#include "secPhysicsList.hh"
#include "secRandGenFromFile.hh"

#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
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
    
    G4Random::setTheEngine(new CLHEP::RanecuEngine());
   
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
#else
    G4RunManager* runManager = new G4RunManager;
#endif
     
    auto RandGen = secRandGenFromFile::GetInstance();
    RandGen->LoadFile("ThetaPDF.txt", secVRandGen::PDF_TYPE);
    RandGen->LoadFile("EnegCDF.txt", secVRandGen::CDF_TYPE);
    RandGen->LoadFile("BetaSpectumPDF.txt", secVRandGen::PDF_TYPE);
    
    runManager -> SetUserInitialization( new secDetectorConstruction() );
//---------------------------------------------------------------------
    runManager -> SetUserInitialization( new secPhysicsList() );
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
