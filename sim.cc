#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    G4UIExecutive *ui = nullptr;

    if (argc == 1) {
      ui = new G4UIExecutive(argc, argv);
    }

    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Physics List
    runManager->SetUserInitialization(new PhysicsList());

    // Detector Construction
    runManager->SetUserInitialization(new DetectorConstruction());

    // Action Initialization
    runManager->SetUserInitialization(new ActionInitialization());

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    
    if (ui) {
    	UImanager->ApplyCommand("/control/execute vis.mac");
    	ui->SessionStart();
	delete ui;
    } else {
	G4String command = "/control/execute ";
	G4String fileName = argv[1];
	UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
