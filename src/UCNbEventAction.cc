#include "UCNbRunAction.hh"
#include "UCNbPhotonDetHit.hh"
#include "UCNbTrajectory.hh"
#include "UCNbDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"

#include "UCNbEventAction.hh"
#include "UCNbSteppingAction.hh"

#include "UCNbIsotropicBetaGeneratorAction.hh"

#include "Randomize.hh"

// Purpose: Invoke visualization at the end
//          Also can accumulate statistics regarding hits
//          in the PhotonDet detector

UCNbEventAction::UCNbEventAction(UCNbRunAction* RA)
	 : runaction(RA), verboselevel(0),
	   printModulo(100), drawFlag("all")
{
  // eventMessenger = new UCNbEventActionMessenger(this);

  forcedrawphotons = false;
  forcenophotons   = false;

}

UCNbEventAction::~UCNbEventAction()
{
  //  delete eventMessenger;
}

void UCNbEventAction::BeginOfEventAction(const G4Event* event)
{
	G4int eventNb = event->GetEventID();
	if (eventNb % printModulo == 0)
		G4cout << "\n---> Begin of Event: " << eventNb << G4endl;

	if(verboselevel > 0)
		G4cout << "<<< Event  " << eventNb << " started." << G4endl;
		
	// G4cout << "g4event ";
}

void UCNbEventAction::EndOfEventAction(const G4Event* event)
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

	// Visualization of Trajectory
	if(pVVisManager) {
		G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();

		G4int n_trajectories = 0;
		if (trajectoryContainer) {
			n_trajectories = trajectoryContainer->entries();
		}
		
		// G4cout << "n_trajectories: " << n_trajectories << G4endl;
		
		if (drawFlag == "all") {
			// G4cout << "draw all trajectories" << G4endl;
		}

		for(G4int i=0; i<n_trajectories; i++) { 
			UCNbTrajectory* trj 
				= (UCNbTrajectory *)((*(event->GetTrajectoryContainer()))[i]);
			if (drawFlag == "all") {
				//G4cout << "Now calling DrawTrajectory" << G4endl;
				//G4cout << "Particle Name: " << trj->GetParticleName() << G4endl;
				trj->DrawTrajectory(50);
			} else if ((drawFlag == "charged")&&(trj->GetCharge() != 0.)) {
				trj->DrawTrajectory(50);
			} else if (trj->GetParticleName()=="opticalphoton") {
				G4cout << "We should be drawing an opticalphoton" << G4endl;
				trj->SetForceDrawTrajectory(forcedrawphotons);
				trj->SetForceNoDrawTrajectory(forcenophotons);
				trj->DrawTrajectory(50);
			}
		}
	}

	// G4cout << "Verbosity Level: " << verboselevel << G4endl;

	if (verboselevel>0) {
		// G4cout << "<<< Event  " << event->GetEventID() << " ended." << G4endl;
	}

	// Save the Random Engine Status at the of event
	if (runaction->GetRndmFreq() == 2) {
		CLHEP::HepRandom::saveEngineStatus("endOfEvent.rndm");
		G4int eventNb = event->GetEventID();
		if (eventNb%printModulo == 0) {
			// G4cout << "\n---> End of Event: " << eventNb << G4endl;
			CLHEP::HepRandom::showEngineStatus();
		}
	}

	// Get Hits from the detector if any
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
	for (G4int i = 0; i < runaction->detector->numberOfPMTs; i++) {
		
		G4String colName = runaction->detector->PMTHCNames[i];

		pmtCollID = SDman->GetCollectionID(colName);

		G4HCofThisEvent* HCE = event->GetHCofThisEvent();
		UCNbPhotonDetHitsCollection* pmtHC = 0;

		// Get the hit collections
		if (HCE) {
			if (pmtCollID>=0) {
				pmtHC = (UCNbPhotonDetHitsCollection*)(HCE->GetHC(pmtCollID));
			}
		}
		// Get hit information about photons that reached the detector in this event
		if (pmtHC) {
			n_hit = pmtHC->entries();
			//G4cout << "" << n_hit << G4endl;		
			//G4cout << "" << G4endl;
		}
	}
	G4ThreeVector p = UCNbIsotropicBetaGeneratorAction::initMomentum;
	G4int* hits = UCNbSteppingAction::getDetectorHits();
	G4cout << "UCNb_event " << hits[0] << " " << hits[1] << " " << hits[2] << " " << hits[3] << " " << hits[4] << " "
		<< p.x() << " " << p.y() << " " << p.z() << G4endl;
	UCNbSteppingAction::ResetDetectorCounters();
}

int UCNbEventAction::GetHitsNumber() {
	return n_hit;
}

G4int UCNbEventAction::GetEventNo()
{
	G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
	return evno;
}

void UCNbEventAction::SetEventVerbose(G4int level)
{
	verboselevel = level;
}
