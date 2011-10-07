#include "UCNbPrimaryGeneratorActionTemp.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <cmath>

UCNbIsotropicBetaGeneratorAction::UCNbIsotropicBetaGeneratorAction(G4double energy, G4double x, G4double y, G4double z)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = "e-";
  particleGun->SetParticleDefinition(particleTable->
				     FindParticle(particleName));
  // Default energy, position, momentum
  particleGun->SetParticleEnergy(energy);
  particleGun->SetParticlePosition(G4ThreeVector(x,y,z));
}

UCNbIsotropicBetaGeneratorAction::~UCNbIsotropicBetaGeneratorAction()
{
    delete particleGun;
}

G4ThreeVector UCNbIsotropicBetaGeneratorAction::initMomentum = G4ThreeVector(0,0,0);

void UCNbIsotropicBetaGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // uniformly sample points from a sphere    
  G4double theta = 2.0 * 3.14159265359 * G4UniformRand();
  G4double phi = asin(G4UniformRand() * 2.0 - 1.0);
  G4double px = cos(theta) * cos(phi);
  G4double py = sin(theta) * cos(phi);
  G4double pz = sin(phi);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
  initMomentum = G4ThreeVector(px, py, pz);
  particleGun->GeneratePrimaryVertex(anEvent);
}


