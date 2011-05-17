//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "UCNbPrimaryGeneratorActionTemp.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

UCNbPrimaryGeneratorActionTemp::UCNbPrimaryGeneratorActionTemp(G4double energy, G4double x, G4double y, G4double z){
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  G4String particleName;
  particleGun->SetParticleDefinition(particleTable->
				     FindParticle(particleName="e-"));
  //Default energy,position,momentum
  particleGun->SetParticleEnergy(energy);
  particleGun->SetParticlePosition(G4ThreeVector(x,y,z));
  // G4cout << "particle_position " << x << " " << y << " " << z << G4endl;

}

UCNbPrimaryGeneratorActionTemp::~UCNbPrimaryGeneratorActionTemp(){
    delete particleGun;
}

G4ThreeVector UCNbPrimaryGeneratorActionTemp::initMomentum = G4ThreeVector(0,0,0);

void UCNbPrimaryGeneratorActionTemp::GeneratePrimaries(G4Event* anEvent){
  
  G4double px = G4UniformRand() * 0.2 - 0.1;
  G4double py = G4UniformRand() * 0.2 - 0.1;
  G4double pz = G4UniformRand() * 0.2 - 0.1;
  
  // G4cout << "init_momentum " << px << " " << py << " " << pz << G4endl;
  
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
  initMomentum = G4ThreeVector(px, py, pz);
  particleGun->GeneratePrimaryVertex(anEvent);
}


