

#include "UCNbRunAction.hh"
// #include "UCNbRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "Randomize.hh"

#include "UCNbDetectorConstruction.hh"
#include "UCNbSteppingAction.hh"

UCNbRunAction::UCNbRunAction(UCNbDetectorConstruction* DC)
	: detector(DC), saveRndm(0), autoSeed(false)
{
  // runMessenger = new UCNbRunActionMessenger(this);
}

UCNbRunAction::~UCNbRunAction()
{
  // delete runMessenger;
}

void UCNbRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

  if (autoSeed) {
     // automatic (time-based) random seeds for each run
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     time_t systime = time(NULL);
     seeds[0] = (long) systime;
     seeds[1] = (long) (systime*G4UniformRand());
     CLHEP::HepRandom::setTheSeeds(seeds);
     CLHEP::HepRandom::showEngineStatus();
  } else {
     CLHEP::HepRandom::showEngineStatus();
  }

  if (saveRndm > 0) CLHEP::HepRandom::saveEngineStatus("BeginOfRun.rndm");

}

void UCNbRunAction::EndOfRunAction(const G4Run* )
{
  if (saveRndm == 1)
  {
     CLHEP::HepRandom::showEngineStatus();
     CLHEP::HepRandom::saveEngineStatus("endOfRun.rndm");
  }
}
