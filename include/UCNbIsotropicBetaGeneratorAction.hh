
#ifndef UCNbIsotropicBetaGeneratorAction_h
#define UCNbIsotropicBetaGeneratorAction_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class UCNbIsotropicBetaGeneratorAction : public G4UCNbIsotropicBetaGeneratorAction
{
public:
  UCNbIsotropicBetaGeneratorAction(G4double energy, G4double x, G4double y, G4double z);
  ~UCNbIsotropicBetaGeneratorAction();
  static G4ThreeVector initMomentum;
  
public:
  void GeneratePrimaries(G4Event* anEvent);
  
private:
    G4ParticleGun* particleGun;
};

#endif


