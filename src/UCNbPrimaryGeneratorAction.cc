// GEANT4 tag $Name: geant4-09-03-patch-01 $
//


#include "G4ios.hh"
#include "globals.hh"

#include "G4Event.hh"

#include "G4GeneralParticleSource.hh"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4PhysicsTable.hh"

#include "Randomize.hh"

#include "UCNbPrimaryGeneratorAction.hh"

UCNbPrimaryGeneratorAction::UCNbPrimaryGeneratorAction()
{
	theIntegralTable = NULL;
	particleGun = new G4GeneralParticleSource();
	timeConstant = 0.0;
	G4String particleName;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="opticalphoton"));
	particleGun->SetParticlePosition(G4ThreeVector(0.0 , 0.0, 10.0*cm));
}

UCNbPrimaryGeneratorAction::~UCNbPrimaryGeneratorAction()
{
	delete particleGun;
	if (theIntegralTable) {
		theIntegralTable->clearAndDestroy();
		delete theIntegralTable;
	}
}

void UCNbPrimaryGeneratorAction::SetDecayTimeConstant(G4double time)
{
	timeConstant = time;
}

void UCNbPrimaryGeneratorAction::BuildEmissionSpectrum()
{
   if (theIntegralTable) return;

   const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

   G4int numOfMaterials = G4Material::GetNumberOfMaterials();

   if(!theIntegralTable)theIntegralTable = new G4PhysicsTable(numOfMaterials);

   for (G4int i = 0; i < numOfMaterials; i++) {
       G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector = new G4PhysicsOrderedFreeVector();
       G4Material* aMaterial = (*theMaterialTable)[i];
       G4MaterialPropertiesTable* aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
       if (aMaterialPropertiesTable) {
          G4MaterialPropertyVector* theWLSVector = aMaterialPropertiesTable->GetProperty("WLSCOMPONENT");
          if (theWLSVector) {
             theWLSVector->ResetIterator();
             ++(*theWLSVector);
             G4double currentIN = theWLSVector->GetProperty();
             if (currentIN >= 0.0) {
                G4double currentPM = theWLSVector->GetPhotonEnergy();
                G4double currentCII = 0.0;
                aPhysicsOrderedFreeVector->InsertValues(currentPM , currentCII);
                G4double prevPM  = currentPM;
                G4double prevCII = currentCII;
                G4double prevIN  = currentIN;

                while(++(*theWLSVector)) {
                  currentPM = theWLSVector->GetPhotonEnergy();
                  currentIN=theWLSVector->GetProperty();
                  currentCII = 0.5 * (prevIN + currentIN);
                  currentCII = prevCII + (currentPM - prevPM) * currentCII;
                  aPhysicsOrderedFreeVector->
                                     InsertValues(currentPM, currentCII);
                  prevPM  = currentPM;
                  prevCII = currentCII;
                  prevIN  = currentIN;
                }
             }
          }
       }
       theIntegralTable->insertAt(i, aPhysicsOrderedFreeVector);
   }
}

void UCNbPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	BuildEmissionSpectrum();
	const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
	G4double sampledEnergy = 3.0*eV;
	for (size_t j = 0; j < theMaterialTable->size(); j++) {
		G4Material* fMaterial = (*theMaterialTable)[j];
		if (fMaterial->GetName() == "polystyrene" ) {
			G4MaterialPropertiesTable* aMaterialPropertiesTable = fMaterial->GetMaterialPropertiesTable();
			const G4MaterialPropertyVector* WLSIntensity = aMaterialPropertiesTable->GetProperty("WLSCOMPONENT");
			if (WLSIntensity) {
				G4int MaterialIndex = fMaterial->GetIndex();
				G4PhysicsOrderedFreeVector* WLSIntegral = (G4PhysicsOrderedFreeVector*)((*theIntegralTable)(MaterialIndex));
				G4double CIImax = WLSIntegral->GetMaxValue();
				G4double CIIvalue = G4UniformRand() * CIImax;
				sampledEnergy = WLSIntegral->GetEnergy(CIIvalue);
			}
		}
	}

	if(particleGun->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
		SetOptPhotonPolar();
		SetOptPhotonTime();
	}
	
	G4cout << "Generating Primaries" << G4endl;

	particleGun->GeneratePrimaryVertex(anEvent);
}

void UCNbPrimaryGeneratorAction::SetOptPhotonPolar()
{
  G4double angle = G4UniformRand() * 360.0*deg;
  SetOptPhotonPolar(angle);
}

void UCNbPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  if (particleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
  {
     G4cout << "-> warning from UCNbPrimaryGeneratorAction::SetOptPhotonPolar()"
            << ":  the particleGun is not an opticalphoton" << G4endl;
     return;
  }

  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = particleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product*product;

  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);

  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  particleGun->SetParticlePolarization(polar);

}

void UCNbPrimaryGeneratorAction::SetOptPhotonTime()
{
   G4double time = -std::log(G4UniformRand())*timeConstant;
   particleGun->SetParticleTime(time);
}


