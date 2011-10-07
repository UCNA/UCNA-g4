
#include "UCNbPhotonDetSD.hh"
#include "UCNbPhotonDetHit.hh"
#include "UCNbUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

UCNbPhotonDetSD::UCNbPhotonDetSD(G4String name, G4String HCName)
  : G4VSensitiveDetector(name),PhotonDetHitCollection(0)
{
	collectionName.insert(HCName);
}

UCNbPhotonDetSD::~UCNbPhotonDetSD() { }

void UCNbPhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
	PhotonDetHitCollection =
		new UCNbPhotonDetHitsCollection(SensitiveDetectorName,collectionName[0]);
	// Store collection with event and keep ID
	static G4int HCID = -1;
	if (HCID<0) HCID = GetCollectionID(0);
	HCE->AddHitsCollection( HCID, PhotonDetHitCollection );
}

G4bool UCNbPhotonDetSD::ProcessHits(G4Step* , G4TouchableHistory* )
{
	return false;
}

//Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//is generated manually when the photon hits the detector
G4bool UCNbPhotonDetSD::ProcessHits_constStep(const G4Step* aStep,
                                             G4TouchableHistory* )
{
	if (aStep == NULL) return false;
	G4Track* theTrack = aStep->GetTrack();

	// Need to know if this is an optical photon
	if(theTrack->GetDefinition()
	 != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

	// Find out information regarding the hit
	G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

	UCNbUserTrackInformation* trackInformation
	  = (UCNbUserTrackInformation*)theTrack->GetUserInformation();

	G4TouchableHistory* theTouchable
	  = (G4TouchableHistory*)(thePostPoint->GetTouchable());

	G4ThreeVector photonExit   = trackInformation -> GetExitPosition();
	G4ThreeVector photonArrive = thePostPoint -> GetPosition();
	G4double      arrivalTime  = theTrack -> GetGlobalTime();

	// Convert the global coordinate for arriving photons into
	// the local coordinate of the detector
	photonArrive = theTouchable->GetHistory()->
						GetTopTransform().TransformPoint(photonArrive);

	// Creating the hit and add it to the collection
	PhotonDetHitCollection->
		insert(new UCNbPhotonDetHit(photonExit, photonArrive, arrivalTime));

	return true;
}

void UCNbPhotonDetSD::EndOfEvent(G4HCofThisEvent* ){ }

void UCNbPhotonDetSD::clear(){ }

void UCNbPhotonDetSD::DrawAll(){ }

void UCNbPhotonDetSD::PrintAll(){ }
