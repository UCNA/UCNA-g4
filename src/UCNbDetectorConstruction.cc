// GEANT4 tag $Name: geant4-09-03-patch-01 $

#include "UCNbDetectorConstruction.hh"

#include "G4ios.hh"
#include "globals.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4GeometryManager.hh"
#include "G4SDManager.hh"

#include "G4SolidStore.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4RunManager.hh"

#include "UCNbDetectorConstruction.hh"
#include "UCNbDetectorMessenger.hh"
#include "UCNbMaterials.hh"
#include "UCNbPhotonDetSD.hh"

UCNbDetectorConstruction::UCNbDetectorConstruction() : physicalWorld(NULL)
{
	
	detectorMessenger = new UCNbDetectorMessenger(this);
	
	materials = NULL;
	
	PMTSDNames[0] = "UCNb/pmtN";
	PMTSDNames[1] = "UCNb/pmtS";
	PMTSDNames[2] = "UCNb/pmtE";
	PMTSDNames[3] = "UCNb/pmtW";
	PMTSDNames[4] = "UCNb/pmtT";
	
	PMTSDPVNames[0] = "pmtN"; 
	PMTSDPVNames[1] = "pmtS"; 
	PMTSDPVNames[2] = "pmtE"; 
	PMTSDPVNames[3] = "pmtW"; 
	PMTSDPVNames[4] = "pmtT"; 
	
	PMTHCNames[0] = "HitCollectionN";
	PMTHCNames[1] = "HitCollectionS";
	PMTHCNames[2] = "HitCollectionE";
	PMTHCNames[3] = "HitCollectionW";
	PMTHCNames[4] = "HitCollectionT";
	
	
	numberOfPMTs = 5;

}

UCNbDetectorConstruction::~UCNbDetectorConstruction()
{
	if (materials) delete materials;
}

G4VPhysicalVolume* UCNbDetectorConstruction::Construct()
{
	materials = UCNbMaterials::GetInstance();
	G4VPhysicalVolume* detector = ConstructDetector();
	return detector;
}

G4VPhysicalVolume* UCNbDetectorConstruction::ConstructDetector()
{
	
	G4cout << "Constructing physical world with " << numberOfPMTs << " PMTs" << G4endl;
	
	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////// Material Names /////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////	
	
	G4String mWorld     = "G4_Galactic";
	G4String mScint     = "polystyrene";
	G4String mRefle     = "G4_TEFLON";
	G4String mPmt       = "G4_Al";
	G4String mInterior  = "G4_Galactic";
 	
 	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Geometry Settings ////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
 	
	G4double worldSizeX        = 1*m;
	G4double worldSizeY        = 1*m;
	G4double worldSizeZ        = 1*m;
	
	G4double scintThickness    = 2.0*mm;
	G4double scintOuterX       = 6.0*cm;
	G4double scintOuterY       = 6.0*cm;
	G4double scintOuterZ       = 6.0*cm;
	G4double scintInnerX       = scintOuterX - scintThickness;	
	G4double scintInnerY       = scintOuterY - scintThickness;	
	G4double scintInnerZ       = scintOuterZ - scintThickness;	
	
	G4double refleThickness    = 5*mm;
	G4double refleOuterX       = scintOuterX + refleThickness;
	G4double refleOuterY       = scintOuterY + refleThickness;
	G4double refleOuterZ       = scintOuterZ + refleThickness;
	G4double refleInnerX       = scintOuterX;
	G4double refleInnerY       = scintOuterY;
	G4double refleInnerZ       = scintOuterZ;
	
	G4double reflePolish       = 0.1;
	G4double reflReflectivity  = 0.99;
	// G4double reflEfficiency    = 0.0;
	
	G4double pmtRadius         = 2.54*cm;
	G4double pmtLength         = 1.0*cm;
	G4double pmtBevelSize      = 0.25 * 2.54 * cm;	
	G4double pmtReflectivity   = 0.1;
	G4double pmtEfficiency     = 0.9;
	
	G4double bottomHoleRadius = 0.5 * 0.505 * 2.54 * cm;
	
	G4double scintReflectivity = 0.2;
	// G4double scintEfficiency   = 0.0;
	
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////// Rotation & Translation //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	G4RotationMatrix* rWorld     = new G4RotationMatrix();
	G4RotationMatrix* rScint     = new G4RotationMatrix();
	G4RotationMatrix* rRefle     = new G4RotationMatrix();
	// G4RotationMatrix* rRefleHole = new G4RotationMatrix();
	G4RotationMatrix* rInterior  = new G4RotationMatrix();

	
	G4RotationMatrix* rNorth = new G4RotationMatrix(CLHEP::HepRotationY(0.0));
	G4RotationMatrix* rEast = new G4RotationMatrix(CLHEP::HepRotationY(0.25 * twopi*rad));
	G4RotationMatrix* rSouth  = new G4RotationMatrix(CLHEP::HepRotationY(0.50 * twopi*rad));
	G4RotationMatrix* rWest  = new G4RotationMatrix(CLHEP::HepRotationY(0.75 * twopi*rad));
	G4RotationMatrix* rTop  = new G4RotationMatrix(CLHEP::HepRotationX(0.25 * twopi*rad));
	G4RotationMatrix* rBottom  = new G4RotationMatrix(CLHEP::HepRotationX(0.75 * twopi*rad));
		
	G4RotationMatrix* rPmts[5];
		rPmts[0] = rNorth;
		rPmts[1] = rSouth;
		rPmts[2] = rEast;
		rPmts[3] = rWest;
		rPmts[4] = rTop;
		
		
	G4double holeDist = (refleOuterZ - refleThickness / 2.0);
	G4double pmtDist = (scintOuterZ + pmtLength);
	
	G4ThreeVector tPmts[5];
		tPmts[0] = G4ThreeVector(0, 0, -pmtDist);
		tPmts[1] = G4ThreeVector(0, 0, pmtDist);		
		tPmts[2] = G4ThreeVector(pmtDist, 0, 0);	
		tPmts[3] = G4ThreeVector(-pmtDist, 0, 0);
		tPmts[4] = G4ThreeVector(0, pmtDist, 0);
		
	G4ThreeVector tHoles[5];
		tHoles[0] = G4ThreeVector(0, 0, -holeDist);
		tHoles[1] = G4ThreeVector(0, 0, holeDist);		
		tHoles[2] = G4ThreeVector(holeDist, 0, 0);	
		tHoles[3] = G4ThreeVector(-holeDist, 0, 0);
		tHoles[4] = G4ThreeVector(0, holeDist, 0);
		
	G4ThreeVector tWorld        = G4ThreeVector(0, 0, 0);
	G4ThreeVector tScint        = G4ThreeVector(0, 0, 0);
	G4ThreeVector tRefle        = G4ThreeVector(0, 0, 0);
	G4ThreeVector tInterior     = G4ThreeVector(0, 0, 0);
	
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Visual Attributes ////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	#ifdef G4VIS_USE	
	
		G4Color coWorld    = G4Color(1.0, 1.0, 1.0);
		G4Color coScint    = G4Color(0.7, 0.6, 1.0);
		G4Color coRefle    = G4Color(0.9, 0.9, 0.9);
		G4Color coPmt      = G4Color(0.9, 0.6, 0.0);
		G4Color coInterior = G4Color(1.0, 1.0, 1.0);
		
		G4VisAttributes* vaScint    = new G4VisAttributes(coScint);
		G4VisAttributes* vaRefle    = new G4VisAttributes(coRefle);
		G4VisAttributes* vaPmt      = new G4VisAttributes(coPmt);
		G4VisAttributes* vaWorld    = new G4VisAttributes(coWorld);
		G4VisAttributes* vaInterior = new G4VisAttributes(coInterior);
		
		vaWorld->SetVisibility(false);
		vaScint->SetForceWireframe(true);
		vaRefle->SetForceWireframe(false);
		vaPmt->SetForceWireframe(true);
		vaInterior->SetVisibility(false);
	
	#endif	
	
	//////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Solid Volumes /////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	G4VSolid* sWorld      = new G4Box("world", worldSizeX, worldSizeY, worldSizeZ);
	G4VSolid* sScintOuter = new G4Box("scintillator-outer", scintOuterX, scintOuterY, scintOuterZ);
	G4VSolid* sScintInner = new G4Box("scintillator-inner", scintInnerX, scintInnerY, scintInnerZ);
	G4VSolid* sScint      = new G4SubtractionSolid("scintillator", sScintOuter, sScintInner);
	G4VSolid* sRefleOuter = new G4Box("reflector-outer", refleOuterX, refleOuterY, refleOuterZ);
	G4VSolid* sRefleInner = new G4Box("reflector-inner", refleInnerX, refleInnerY, refleInnerZ);
	G4VSolid* sRefleShell = new G4SubtractionSolid("reflector shell", sRefleOuter, sRefleInner);
	
	G4VSolid* sInterior = new G4Box("interior", refleOuterX, refleOuterY, refleOuterZ);
	G4VSolid* sRefle = sRefleShell;
	G4VSolid* sPmts[5];
	for (G4int i = 0; i < numberOfPMTs; i++) {
		// punch beveled holes in reflector
		G4VSolid* sRefleHole  = new G4Tubs("pmt-hole", 0, pmtRadius, refleThickness * 1.1, 0, twopi*rad);
		G4VSolid* sBevel = new G4Cons("pmt-bevel", 0, pmtRadius - pmtBevelSize, 0, pmtRadius + pmtBevelSize, pmtBevelSize, 0, twopi * rad);
		sRefle = new G4SubtractionSolid("reflector", sRefle, sRefleHole, rPmts[i], tHoles[i]);
		sRefle = new G4SubtractionSolid("reflector", sRefle, sBevel, rPmts[i], tHoles[i]);
		// construct solids for pmts
		sPmts[i] = new G4Tubs(PMTSDPVNames[i], 0., pmtRadius, pmtLength, 0, twopi*rad);
		sInterior = new G4SubtractionSolid("interior", sInterior, sPmts[i], rPmts[i], tPmts[i]);
	}
	G4VSolid* bottomHole = new G4Tubs("bottomHole", 0, bottomHoleRadius, refleThickness * 2,0, twopi*rad);
	sRefle = new G4SubtractionSolid("reflector", sRefle, bottomHole, rBottom, G4ThreeVector(0, -holeDist, 0));
	sScint = new G4SubtractionSolid("scintillator", sScint, bottomHole, rBottom, G4ThreeVector(0, -holeDist, 0));
	
	sInterior = new G4SubtractionSolid("interior", sInterior, sRefle, rRefle, tRefle);
	sInterior = new G4SubtractionSolid("interior", sInterior, sScint, rScint, tScint);
	// NOTE: remember to subtract all the things inside interior from this solid
	// since it originally spans all of +- Reflector
			
	
	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////// Logical Volumes ////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	G4LogicalVolume* lWorld      = new G4LogicalVolume(sWorld, findMaterial(mWorld), "world");
	G4LogicalVolume* lScint      = new G4LogicalVolume(sScint, findMaterial(mScint), "scintillator");
	G4LogicalVolume* lRefle      = new G4LogicalVolume(sRefle, findMaterial(mRefle), "reflector");
	G4LogicalVolume* lInterior   = new G4LogicalVolume(sInterior, findMaterial(mInterior), "interior");
	G4LogicalVolume* lPmts[5];
	for (G4int i = 0; i < numberOfPMTs; i++) {
		lPmts[i]  =  new G4LogicalVolume(sPmts[i], findMaterial(mPmt), PMTSDPVNames[i]);
	}

	#ifdef G4VIS_USE	
	
		lWorld->SetVisAttributes(vaWorld);
		lRefle->SetVisAttributes(vaRefle);
		lScint->SetVisAttributes(vaScint);
		lInterior->SetVisAttributes(vaInterior);
		for (G4int i = 0; i < numberOfPMTs; i++) {
			lPmts[i]->SetVisAttributes(vaPmt);
		}
	
	#endif
	
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Physical Volumes /////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	G4PVPlacement* pWorld    = new G4PVPlacement(rWorld, tWorld, lWorld, "world", 0, false, 0);
	G4PVPlacement* pScint    = new G4PVPlacement(rScint, tScint, lScint, "scintillator", lWorld, false, 0);
	G4PVPlacement* pRefle    = new G4PVPlacement(rRefle, tRefle, lRefle, "reflector", lWorld, false, 0);
	G4PVPlacement* pInterior = new G4PVPlacement(rInterior, tInterior, lInterior, "interior", lWorld, false, 0);
	G4PVPlacement* pPmts[numberOfPMTs];
	for (G4int i = 0; i < numberOfPMTs; i++) {	
		 pPmts[i] = new G4PVPlacement(rPmts[i], tPmts[i], lPmts[i], PMTSDPVNames[i], lWorld, false, 0);
	}
	
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Surface Properties ///////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	// pmt skin surface material properties
	G4MaterialPropertiesTable* suPmtMPT =  new G4MaterialPropertiesTable();
	G4double MPTenergy[2] = {2.00*eV, 3.47*eV};
	G4double refl_PMT[2] = {pmtReflectivity, pmtReflectivity};
	G4double effi_PMT[2] = {pmtEfficiency, pmtEfficiency};
	suPmtMPT -> AddProperty("REFLECTIVITY", MPTenergy, refl_PMT, 2);
	suPmtMPT -> AddProperty("EFFICIENCY", MPTenergy, effi_PMT, 2);
	
	// scintillator-reflector boundary surface material properties
	G4MaterialPropertiesTable* suScintRefleMPT =  new G4MaterialPropertiesTable();
	G4double rScintRefle[2] = {reflReflectivity, reflReflectivity};
	// G4double eScintRefle[2] = {reflEfficiency, reflEfficiency};
	suScintRefleMPT -> AddProperty("REFLECTIVITY", MPTenergy, rScintRefle, 2);
	// suScintRefleMPT -> AddProperty("EFFICIENCY", MPTenergy, eScintRefle, 2);
	
	// scintillator-interior boundary surface material properties
	G4MaterialPropertiesTable* suScintInteriorMPT =  new G4MaterialPropertiesTable();
	G4double rScintInterior[2] = {scintReflectivity, scintReflectivity};
	// G4double eScintInterior[2] = {scintEfficiency, scintEfficiency};
	suScintInteriorMPT -> AddProperty("REFLECTIVITY", MPTenergy, rScintInterior, 2);
	// suScintInteriorMPT -> AddProperty("EFFICIENCY", MPTenergy, eScintInterior, 2);
	
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////// Surface Property Assignments ////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////	
	
	// scintillator-reflector boundary surface
	G4OpticalSurface* suScintRefle = new G4OpticalSurface("scintillator-reflector", glisur, ground, dielectric_metal, reflePolish);
	suScintRefle -> SetMaterialPropertiesTable(suScintRefleMPT);
	new G4LogicalBorderSurface("scintillator-reflector", pScint, pRefle, suScintRefle);
	
	// pmt skin surface
	G4OpticalSurface* suPmt = new G4OpticalSurface("PMTSurface", glisur, ground, dielectric_metal);	
	suPmt -> SetMaterialPropertiesTable(suPmtMPT);
	for (G4int i = 0; i < numberOfPMTs; i++) {	
		new G4LogicalSkinSurface("pmt skin surface", lPmts[i], suPmt); 
	}

	// scintillator-interior boundary surface
	G4OpticalSurface* suScintInterior = new G4OpticalSurface("scintillator-interior", glisur, ground, dielectric_dielectric);	
	//suScintInterior -> SetMaterialPropertiesTable(suScintInteriorMPT);
	new G4LogicalBorderSurface("scintillator-interior", pScint, pInterior, suScintInterior); 	

	//////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Sensitive Detectors ///////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////	

	// setting the pmts to be sensitive
	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	for (G4int i = 0; i < numberOfPMTs; i++) {		
		UCNbPhotonDetSD* detPmt = new UCNbPhotonDetSD(PMTSDNames[i],PMTHCNames[i]);
		SDman->AddNewDetector(detPmt);
		lPmts[i]->SetSensitiveDetector(detPmt);
	}

	physicalWorld = pWorld;

	return pWorld;
}

void UCNbDetectorConstruction::UpdateGeometry() {
	if (!physicalWorld) return;

	// clean-up previous geometry
	G4GeometryManager::GetInstance()->OpenGeometry();

	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalSkinSurface::CleanSurfaceTable();
	G4LogicalBorderSurface::CleanSurfaceTable();

	//define new one
	// UpdateGeometryParameters();

	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());

	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();

	G4RegionStore::GetInstance()->UpdateMaterialList(physicalWorld);
	
}

void UCNbDetectorConstruction::SetNumberOfPMTs(G4int number) {
	G4cout << "Setting number of PMTs to " << number << "." << G4endl;
	numberOfPMTs = number;
}
void UCNbDetectorConstruction::SetTeflonReflectivity(G4double reflectivity) {
	G4cout << "Setting teflon relfectivity to " << reflectivity << "." << G4endl;
}

G4Material* UCNbDetectorConstruction::findMaterial(G4String name) {
	G4cout << "Looking for material " << name << " ." << G4endl;
    G4Material* material = G4Material::GetMaterial(name, true);
    return material;
}
