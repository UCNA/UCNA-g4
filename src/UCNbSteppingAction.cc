

#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"

#include "UCNbSteppingAction.hh"
#include "UCNbDetectorConstruction.hh"
// #include "UCNbSteppingActionMessenger.hh"
#include "UCNbPhotonDetSD.hh"

#include "G4ParticleTypes.hh"

#include "UCNbUserTrackInformation.hh"

#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include <sstream>

// Purpose: Save relevant information into User Track Information

static const G4ThreeVector ZHat = G4ThreeVector(0.0,0.0,1.0);

G4int UCNbSteppingAction::detectorHits[5];

G4int UCNbSteppingAction::maxRndmSave = 10000;

UCNbSteppingAction::UCNbSteppingAction(UCNbDetectorConstruction* DC)
: detector(DC)
{
	// steppingMessenger = new UCNbSteppingActionMessenger(this);
	
	counterEnd = 0;
	counterMid = 0;
	bounceLimit = 10000;
	
	ResetDetectorCounters();
	ResetCounters();
}

void UCNbSteppingAction::ResetDetectorCounters() {
	detectorHits[0] = 0;
	detectorHits[1] = 0;
	detectorHits[2] = 0;
	detectorHits[3] = 0;
	detectorHits[4] = 0;
	
}

void UCNbSteppingAction::registerHit(G4int detector_id) {	
	detectorHits[detector_id]++;
}

G4int* UCNbSteppingAction::getDetectorHits() {
	return detectorHits;
}

UCNbSteppingAction::~UCNbSteppingAction()
{
	// delete steppingMessenger;
}

void  UCNbSteppingAction::SetBounceLimit(G4int i)   {bounceLimit = i;}
G4int UCNbSteppingAction::GetNumberOfBounces()      {return counterBounce;}
G4int UCNbSteppingAction::GetNumberOfClad1Bounces() {return counterClad1Bounce;}
G4int UCNbSteppingAction::GetNumberOfClad2Bounces() {return counterClad2Bounce;}
G4int UCNbSteppingAction::GetNumberOfWLSBounces()   {return counterWLSBounce;}
G4int UCNbSteppingAction::ResetSuccessCounter()     {
	G4int temp = counterEnd; counterEnd = 0; return temp;
}

// save the random status into a sub-directory
// Pre: subDir must be empty or ended with "/"
inline void UCNbSteppingAction::saveRandomStatus(G4String subDir) {

	// don't save if the maximum amount has been reached
	if (UCNbSteppingAction::maxRndmSave == 0) return;

	G4RunManager* theRunManager = G4RunManager::GetRunManager();
	G4String randomNumberStatusDir = theRunManager->GetRandomNumberStoreDir();

	G4String fileIn  = randomNumberStatusDir + "currentEvent.rndm";

	std::ostringstream os;

	os << "run" << theRunManager->GetCurrentRun()->GetRunID() << "evt"
		<< theRunManager->GetCurrentEvent()->GetEventID() << ".rndm" << '\0';

	G4String fileOut = randomNumberStatusDir + subDir + os.str();

	G4String copCmd = "/control/shell cp "+fileIn+" "+fileOut;
	G4UImanager::GetUIpointer()->ApplyCommand(copCmd);

	UCNbSteppingAction::maxRndmSave--;
}

void UCNbSteppingAction::UpdateHistogramSuccess(G4StepPoint* ,G4Track* ) { }

void UCNbSteppingAction::UpdateHistogramReflect(G4StepPoint* ,G4Track* ) { }

void UCNbSteppingAction::UpdateHistogramEscape(G4StepPoint* , G4Track* ) { }

void UCNbSteppingAction::UpdateHistogramAbsorb(G4StepPoint* , G4Track* ) { }

void UCNbSteppingAction::UserSteppingAction(const G4Step* theStep)
{
	G4Track* theTrack = theStep->GetTrack();
	UCNbUserTrackInformation* trackInformation
		= (UCNbUserTrackInformation*)theTrack->GetUserInformation();

	G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

	G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

	G4String thePrePVname  = " ";
	G4String thePostPVname = " ";

	if (thePostPV) {
		thePrePVname  = thePrePV->GetName();
		thePostPVname = thePostPV->GetName();
		//G4cout << "" << thePrePVname << " -> " << thePostPVname << G4endl;
	}

	// Recording data for start
	if (theTrack->GetParentID()==0) {
		// This is a primary track
		if ( theTrack->GetCurrentStepNumber() == 1 ) {
			//G4double x  = theTrack->GetVertexPosition().x();
			//G4double y  = theTrack->GetVertexPosition().y();
			G4double z  = theTrack->GetVertexPosition().z();
			//G4double pz = theTrack->GetVertexMomentumDirection().z();
			initTheta = theTrack->GetVertexMomentumDirection().angle(ZHat);
			initZ = z;
		}
	}

	// Retrieve the status of the photon
	G4OpBoundaryProcessStatus theStatus = Undefined;

	G4ProcessManager* OpManager =
	G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

	if (OpManager) {
		G4int MAXofPostStepLoops =
			OpManager->GetPostStepProcessVector()->entries();
		G4ProcessVector* fPostStepDoItVector =
			OpManager->GetPostStepProcessVector(typeDoIt);

		for ( G4int i=0; i<MAXofPostStepLoops; i++) {
			G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
			opProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
			if (opProcess) { 
				theStatus = opProcess->GetStatus(); break;
			}
		}
	}

	// Assumed photons are originated at the fiber OR
	// the fiber is the first material the photon hits
	switch (theStatus) {

	// Exiting the scintillator
	case FresnelRefraction:
		// G4cout << "FresnelRefraction" << G4endl;
	case SameMaterial:
		// G4cout << "SameMaterial" << G4endl;
		G4bool isScint;
		isScint = thePostPVname == "scintillator";

		if (isScint) {

			if (trackInformation->isStatus(outside))
				trackInformation->AddStatusFlag(inside);

		// Set the Exit flag when the photon refracted out of the scintillator
		} else if (trackInformation->isStatus(inside)) {
			
		}

	return;

	// Internal Reflections
	case TotalInternalReflection:
		// G4cout << "TotalInternalReflection" << G4endl;
		// Kill the track if it's number of bounces exceeded the limit
		if (bounceLimit > 0 && counterBounce >= bounceLimit)
		{
			theTrack->SetTrackStatus(fStopAndKill);
			trackInformation->AddStatusFlag(killed);
			ResetCounters();
			G4cout << "\n Bounce Limit Exceeded" << G4endl;
			return;
		}

	case FresnelReflection:
		// G4cout << "FresnelReflection" << G4endl;
	counterBounce++;

	if ( thePrePVname == "WLSFiber") counterWLSBounce++;

	else if ( thePrePVname == "Clad1") counterClad1Bounce++;

	else if ( thePrePVname == "Clad2") counterClad2Bounce++;

	// Reflection of the 
	case LambertianReflection:
	case LobeReflection:
	case SpikeReflection:

	// Check if it hits the coating
	if ( thePostPVname == "coating" )
		trackInformation->AddStatusFlag(reflected);

	return;

	// Detected by a detector
	case Detection:
		
		// G4cout << "Detection" << G4endl;

		// Check if the photon hits the detector and process the hit if it does
		
		for ( G4int i = 0; i < detector->numberOfPMTs; i++) {
			if (thePostPVname == detector->PMTSDPVNames[i]) {

			// G4cout << i << "";
			
			UCNbSteppingAction::registerHit(i);

			G4SDManager* SDman = G4SDManager::GetSDMpointer();
			
			G4String SDname = detector->PMTSDNames[i];
			
			UCNbPhotonDetSD* PMTSD = (UCNbPhotonDetSD*)SDman->FindSensitiveDetector(SDname);

			if (PMTSD) PMTSD->ProcessHits_constStep(theStep, NULL);

			// Record Photons that escaped at the end
			//          if (trackInformation->isStatus(EscapedFromReadOut))
			//                              UpdateHistogramSuccess(thePostPoint,theTrack);

			// Stop Tracking when it hits the detector's surface
			ResetCounters();
			theTrack->SetTrackStatus(fStopAndKill);
				
			}
		}
	
	break;

	default: break;

}

// Check for absorbed photons
if (theTrack->GetTrackStatus() != fAlive  && trackInformation->isStatus(inside))
{
	//     UpdateHistogramAbsorb(thePostPoint,theTrack);
	ResetCounters();
	return;
}

}
