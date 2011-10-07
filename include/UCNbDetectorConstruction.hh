// GEANT4 tag $Name: geant4-09-03-patch-01 $

#ifndef UCNbDetectorConstruction_H
#define UCNbDetectorConstruction_H 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4RotationMatrix.hh"

class G4Box;
class G4Tubs;

class G4LogicalVolume;
class G4VPhysicalVolume;

class UCNbMaterials;
class G4Material;

class UCNbDetectorMessenger;

#include "G4VUserDetectorConstruction.hh"

class UCNbDetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		UCNbDetectorConstruction();
		~UCNbDetectorConstruction();

		G4VPhysicalVolume* Construct();
		G4VPhysicalVolume* ConstructDetector();
		
		void UpdateGeometry();
		
		void SetNumberOfPMTs        (G4int number);    // maximum 4 PMTs
		void SetTeflonReflectivity  (G4double reflectivity); 

		
		G4Material* findMaterial(G4String);
		G4String PMTSDNames[5];
		G4String PMTSDPVNames[5];
		G4String PMTHCNames[5];
		G4int numberOfPMTs;
		
	private:
		
		UCNbMaterials*     materials;
		
		G4double           worldSizeX;
		G4double           worldSizeY;
		G4double           worldSizeZ;
		
		G4LogicalVolume*   logicalWorld;
		G4VPhysicalVolume* physicalWorld;
		
		G4LogicalVolume*   logicalScintillator;
		G4VPhysicalVolume* physicalScintillator;
		
		G4LogicalVolume*   logicalCoating;
		G4VPhysicalVolume* physicalCoating;
		
		G4LogicalVolume*   logicalPMT;
		G4VPhysicalVolume* physicalPMT;
		
		G4double           shellThickness;
		G4double           outerSize;
		G4double           innerSize;
		
		G4double           coatingThickness;
		G4double           outerCoatingSize;
		G4double           innerCoatingSize;
		
		G4double           coatingPolish;
		G4double           coatingReflectivity;
		
		G4double           scintillatorPolish;
		G4double           scintillatorReflectivity;
	
		G4double           PMTRadius;
		G4double           PMTLength;
		
		G4double           PMTPolish;
		G4double           PMTReflectivity;
		G4double           PMTEfficiency;
		
	private:
	
		void ConstructScintillator();
		
		UCNbDetectorMessenger* detectorMessenger;
		
};

#endif

