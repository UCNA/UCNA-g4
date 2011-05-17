

#ifndef UCNbSteppingAction_h
#define UCNbSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class UCNbDetectorConstruction;
// class UCNbSteppingActionMessenger;

class G4Track;
class G4StepPoint;

class G4OpBoundaryProcess;

class UCNbSteppingAction : public G4UserSteppingAction
{
  public:

    UCNbSteppingAction(UCNbDetectorConstruction*);
    ~UCNbSteppingAction();

    void UserSteppingAction(const G4Step*);
 
    // Set the bounce limit, 0 for no limit
    void  SetBounceLimit(G4int);
 
    G4int GetNumberOfBounces();
    G4int GetNumberOfClad1Bounces();
    G4int GetNumberOfClad2Bounces();
    G4int GetNumberOfWLSBounces();
    // return number of successful events and reset the counter
    G4int ResetSuccessCounter();
    static void ResetDetectorCounters();
    static void registerHit(G4int detector_id);
    static G4int* getDetectorHits();
 
  private:

    static G4int detectorHits[5];
    
    // Artificially kill the photon after it has bounced more than this number
    G4int bounceLimit;
    // number of photons that reach the end
    G4int counterEnd;
    // number of photons that didn't make it to the end
    G4int counterMid;
    // total number of bounces that a photon been through
    G4int counterBounce;
    // number of bounces that a photon been through within the fibre
    G4int counterUCNbBounce;
    // number of bounces that a photon been through from Cladding 1 to 2
    G4int counterClad1Bounce;
    // number of bounces that a photon been through from Cladding 2 to World
    G4int counterClad2Bounce;
    
    G4int counterWLSBounce;

    // The starting position of the photon
    G4double initZ;
    // initial gamma of the photon
    G4double initGamma;
    // initial theta of the photon
    G4double initTheta;

    G4OpBoundaryProcess* opProcess;

    // maximum number of save states
    static G4int maxRndmSave;
    
    UCNbDetectorConstruction* detector;

    // UCNbSteppingActionMessenger* steppingMessenger;

    inline void ResetCounters()
    { 
      counterBounce = counterUCNbBounce =
      counterClad1Bounce = counterClad2Bounce = 0;
      initGamma = initTheta = -1;
    }

    void UpdateHistogramSuccess(G4StepPoint*, G4Track*);
    void UpdateHistogramReflect(G4StepPoint*, G4Track*);
    void UpdateHistogramEscape(G4StepPoint*, G4Track*);
    void UpdateHistogramAbsorb(G4StepPoint*, G4Track*);

    // save the random status into a sub-directory
    // Pre: subDir must be empty or ended with "/"
    inline void saveRandomStatus(G4String subDir);

};

#endif
