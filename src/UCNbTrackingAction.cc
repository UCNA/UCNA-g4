
#include "globals.hh"
#include "G4RunManager.hh"

#include "UCNbTrajectory.hh"

#include "UCNbUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"

#include "UCNbTrackingAction.hh"

void UCNbTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new UCNbTrajectory(aTrack));

  UCNbUserTrackInformation* trackInformation = new UCNbUserTrackInformation();
	
	
	// G4cout << aTrack->GetMomentumDirection().z() << G4endl;

/*
  if (aTrack->GetMomentumDirection().z()>0.0) {
     trackInformation->AddStatusFlag(right);
  } else {
     trackInformation->AddStatusFlag(left);
  }
*/

  G4String PVName = aTrack->GetVolume()->GetName();
  
  /* AWESOMENESS HERE */
  if (PVName == "scintillator")
     trackInformation->AddStatusFlag(inside);

  fpTrackingManager->SetUserTrackInformation(trackInformation);
}

void UCNbTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  UCNbTrajectory* trajectory =
                      (UCNbTrajectory*)fpTrackingManager->GimmeTrajectory();

  if (aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  {
     if (aTrack->GetParentID()==0) trajectory->SetDrawTrajectory(true);
     else {
        const G4VProcess* creator = aTrack ->GetCreatorProcess();
        if (creator && creator->GetProcessName()=="OpWLS")
        {
           trajectory->WLS();
           trajectory->SetDrawTrajectory(true);
        }
     }
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);
}
