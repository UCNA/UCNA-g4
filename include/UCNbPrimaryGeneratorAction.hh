// GEANT4 tag $Name: geant4-09-03-patch-01 $
//

#ifndef UCNbPrimaryGeneratorAction_h
#define UCNbPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4AffineTransform.hh"

class G4ParticleGun;
class G4Event;

class UCNbPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
  
		UCNbPrimaryGeneratorAction();
		~UCNbPrimaryGeneratorAction();

	public:
  
		void GeneratePrimaries(G4Event*);

		void BuildEmissionSpectrum();

		void SetOptPhotonPolar(G4double);

		void SetDecayTimeConstant(G4double);

	protected:

		G4PhysicsTable* theIntegralTable;
        
	private:

		void SetOptPhotonPolar();
		void SetOptPhotonTime();

		G4GeneralParticleSource*   particleGun;

		G4AffineTransform global2local;

		G4double timeConstant;
    
};

#endif


