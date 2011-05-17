
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "UCNbDetectorConstruction.hh"
#include "UCNbPhysicsList.hh"
#include "UCNbPrimaryGeneratorAction.hh"
#include "UCNbPrimaryGeneratorActionTemp.hh"

#include "UCNbRunAction.hh"
#include "UCNbEventAction.hh"
#include "UCNbTrackingAction.hh"
#include "UCNbSteppingAction.hh"
#include "UCNbStackingAction.hh"
// #include "UCNbSteppingVerbose.hh"

#include <iostream>
#include <fstream>
#include <ctime>  

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char** argv) 
{
	G4String physicsList = "QGSP_BERT";
	
	// set the random engine and seed 
	G4int seed = time(0); 
	// if (argc > 1) seed = atoi(argv[argc - 1]);
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
	CLHEP::HepRandom::setTheSeed(seed);	
	
	G4double energy = atof(argv[1]) * keV;
	G4double x = atof(argv[2]) * cm;
	G4double y = atof(argv[3]) * cm;
	G4double z = atof(argv[4]) * cm;
	
	// construct the default run manager
	G4RunManager* runManager = new G4RunManager;

	// construct the mandatory initialization classes
	UCNbDetectorConstruction*   detector  = new UCNbDetectorConstruction;
	UCNbPhysicsList*            physics   = new UCNbPhysicsList(physicsList);
	runManager->SetUserInitialization(detector);
	runManager->SetUserInitialization(physics);

	// initialize the G4 kernel
	runManager->Initialize();
	
	
	G4cout << "UCNbPrimaryGeneratorActionTemp" << G4endl;
	runManager->SetUserAction(new UCNbPrimaryGeneratorActionTemp(energy, x, y, z));
	
	G4cout << "UCNbRunAction" << G4endl;
	UCNbRunAction* runAction = new UCNbRunAction(detector);
	G4cout << "UCNbEventAction" << G4endl;
	UCNbEventAction* eventAction = new UCNbEventAction(runAction);

	G4cout << "runAction" << G4endl;
	runManager->SetUserAction(runAction);
	G4cout << "eventAction" << G4endl;
	runManager->SetUserAction(eventAction);
	G4cout << "UCNbTrackingAction" << G4endl;
	runManager->SetUserAction(new UCNbTrackingAction());
	G4cout << "UCNbSteppingAction" << G4endl;
	runManager->SetUserAction(new UCNbSteppingAction(detector));
	G4cout << "UCNbStackingAction" << G4endl;
	runManager->SetUserAction(new UCNbStackingAction());
	
	/*
	#ifdef G4VIS_USE
	  // construct the visualization manager
	  G4VisManager* visManager = new G4VisExecutive;
	  visManager->Initialize();
	#endif		
	*/

	// execute an argument macro file if it exists
	// otherwise, start an interactive session
	G4UImanager* UImanager = G4UImanager::GetUIpointer();	
	/*
	if(argc > 1) {
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	} else {
		#ifdef G4UI_USE
			G4UIExecutive* UI = new G4UIExecutive(argc, argv);
			UImanager->ApplyCommand("/control/execute vis.mac");
			UI->SessionStart();
			delete UI;
		#endif
	}
	*/
	
	#ifdef G4UI_USE
		G4UIExecutive* UI = new G4UIExecutive(argc, argv);
		UImanager->ApplyCommand("/control/execute vis.mac");
		// UI->SessionStart();
		delete UI;
		// G4cout << "EventAction get hist" << G4endl;
		// G4cout << eventAction->GetEventNo() << G4endl;
		
	#endif
	/*
	#ifdef G4VIS_USE
		delete visManager;
	#endif
	*/
	// terminate the job
	delete runManager;
	
	return 0;
}
