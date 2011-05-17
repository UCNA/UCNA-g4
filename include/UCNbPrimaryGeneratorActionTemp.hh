
#ifndef UCNbPrimaryGeneratorActionTemp_h
#define UCNbPrimaryGeneratorActionTemp_h 1

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

class UCNbPrimaryGeneratorActionTemp : public G4VUserPrimaryGeneratorAction
{
public:
  UCNbPrimaryGeneratorActionTemp(G4double energy, G4double x, G4double y, G4double z);
  ~UCNbPrimaryGeneratorActionTemp();
  static G4ThreeVector initMomentum;
  /*
  G4double x;
  G4double y;
  G4double z;
  
  G4double px;
  G4double py;
  G4double pz;
  */
  
public:
  void GeneratePrimaries(G4Event* anEvent);
  
private:
    G4ParticleGun* particleGun;
};

#endif


