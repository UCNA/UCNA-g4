
#ifndef UCNbPhysicsList_h
#define UCNbPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
// class WLSPhysicsListMessenger;

class UCNbStepMax;
class UCNbOpticalPhysics;

class UCNbPhysicsList: public G4VModularPhysicsList
{
  public:

    UCNbPhysicsList(G4String);
    virtual ~UCNbPhysicsList();

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

    void SetStepMax(G4double);
    UCNbStepMax* GetStepMaxProcess();
    void AddStepMax();

    /// Remove specific physics from physics list.
    void RemoveFromPhysicsList(const G4String&);

    /// Make sure that the physics list is empty.
    void ClearPhysics();

    void ConstructParticle();
    void ConstructProcess();

    // Turn on or off the absorption process
    void SetAbsorption(G4bool);

    void SetNbOfPhotonsCerenkov(G4int);

    void SetVerbose(G4int);

private:

    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;

    G4double MaxChargedStep;
    UCNbStepMax* stepMaxProcess;

    UCNbOpticalPhysics* opticalPhysics;

    // WLSPhysicsListMessenger* fMessenger;

    G4bool AbsorptionOn;

};

#endif
