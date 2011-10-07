

#ifndef UCNbRunAction_h
#define UCNbRunAction_h 1

#include "globals.hh"

#include "G4UserRunAction.hh"
#include "UCNbDetectorConstruction.hh"

class G4Run;

// class UCNbRunActionMessenger;

class UCNbRunAction : public G4UserRunAction
{
  public:

    UCNbRunAction(UCNbDetectorConstruction* DC);
    ~UCNbRunAction();

  public:

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

    void  SetRndmFreq(G4int val) { saveRndm = val; }
    G4int GetRndmFreq()          { return saveRndm; }
    
	UCNbDetectorConstruction* detector;

    inline void SetAutoSeed (const G4bool val) { autoSeed = val; }

  private:
 
    // UCNbRunActionMessenger* runMessenger;
    G4int saveRndm;
    G4bool autoSeed;

};

#endif
