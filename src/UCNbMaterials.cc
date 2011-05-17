#include "UCNbMaterials.hh"

// private constructor prevents instantiation from other classes
UCNbMaterials::UCNbMaterials()
{
	nistMan = G4NistManager::Instance();
	nistMan->SetVerbose(0);
	CreateMaterials();
}

UCNbMaterials::~UCNbMaterials()
{
	delete air;
	delete aluminum;
	delete polystyrene;
	delete teflon;
	delete vacuum;
}

UCNbMaterials* UCNbMaterials::instance = 0;

UCNbMaterials* UCNbMaterials::GetInstance()
{
	if (instance == 0)
	{
		instance = new UCNbMaterials();
	}
	return instance;
}

G4Material* UCNbMaterials::GetMaterial(const G4String material)
{
	G4Material* mat =  nistMan->FindOrBuildMaterial(material);

	if (!mat) mat = G4Material::GetMaterial(material);
	if (!mat) {
		std::ostringstream o;
		o << "Material " << material << " not found!";
		G4Exception("UCNbMaterials::GetMaterial","",
		FatalException,o.str().c_str());
	}

	return mat;
}

void UCNbMaterials::CreateMaterials()
{
	vacuum      = nistMan->FindOrBuildMaterial("G4_Galactic");
	air         = nistMan->FindOrBuildMaterial("G4_AIR");
	teflon      = nistMan->FindOrBuildMaterial("G4_TEFLON");
	aluminum    = nistMan->FindOrBuildMaterial("G4_Al");

	G4double density;
	G4int ncomponents;
	G4double fractionmass;
	std::vector<G4int> natoms;
	std::vector<G4double> fractionMass;
	std::vector<G4String> elements;

	elements.push_back("C");     natoms.push_back(8);
	elements.push_back("H");     natoms.push_back(8);

	density = 1.050*g/cm3;

	polystyrene = nistMan->ConstructNewMaterial("polystyrene", elements, natoms, density);

	elements.clear();
	natoms.clear();


	// generate material properties table

	const G4int nEntries = 50;

	G4double PhotonEnergy[nEntries] =
		{ 2.00*eV, 2.03*eV, 2.06*eV, 2.09*eV, 2.12*eV,
		  2.15*eV, 2.18*eV, 2.21*eV, 2.24*eV, 2.27*eV,
		  2.30*eV, 2.33*eV, 2.36*eV, 2.39*eV, 2.42*eV,
		  2.45*eV, 2.48*eV, 2.51*eV, 2.54*eV, 2.57*eV,
		  2.60*eV, 2.63*eV, 2.66*eV, 2.69*eV, 2.72*eV,
		  2.75*eV, 2.78*eV, 2.81*eV, 2.84*eV, 2.87*eV,
		  2.90*eV, 2.93*eV, 2.96*eV, 2.99*eV, 3.02*eV,
		  3.05*eV, 3.08*eV, 3.11*eV, 3.14*eV, 3.17*eV,
		  3.20*eV, 3.23*eV, 3.26*eV, 3.29*eV, 3.32*eV,
		  3.35*eV, 3.38*eV, 3.41*eV, 3.44*eV, 3.47*eV};
	
	//--------------------------------------------------
	// Vacuum
	//--------------------------------------------------

	G4double RefractiveIndexVacuum[nEntries] =
		{ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };

	G4MaterialPropertiesTable* MPTVacuum = new G4MaterialPropertiesTable();
	MPTVacuum->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexVacuum, nEntries);

	vacuum->SetMaterialPropertiesTable(MPTVacuum);
	
	//--------------------------------------------------
	// Air
	//--------------------------------------------------

	G4double RefractiveIndex[nEntries] =
		{ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };

	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	MPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);

	air->SetMaterialPropertiesTable(MPT);

	//--------------------------------------------------
	// Teflon
	//--------------------------------------------------

	G4double RefractiveIndexTeflon[nEntries] =
		{ 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30,
		  1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30,
		  1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30,
		  1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30,
		  1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30 };

	G4MaterialPropertiesTable* MPTTeflon = new G4MaterialPropertiesTable();
	MPTTeflon->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexTeflon, nEntries);

	teflon->SetMaterialPropertiesTable(MPTTeflon);
	

	//--------------------------------------------------
	//  Polystyrene
	//--------------------------------------------------

	G4double RefractiveIndexPS[nEntries] =
		{ 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
		  1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
		  1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
		  1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
		  1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};

	G4double AbsPS[nEntries] =
		{ 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
		  5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
		  5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m, 1.10*m,
		  1.10*m, 1.10*m, 1.10*m, 1.10*m, 1.10*m, 1.10*m, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm,
		  1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm};

	G4double EmissionPS[nEntries] =
		{ 0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
		  3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
		  12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
		  15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
		  0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};	
	
	G4double ScintilFast[nEntries] =
		{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	// Add entries into properties table
	G4MaterialPropertiesTable* MPTPolystyrene = new G4MaterialPropertiesTable();
	
	MPTPolystyrene->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexPS, nEntries);
	MPTPolystyrene->AddProperty("WLSABSLENGTH", PhotonEnergy, AbsPS, nEntries);
	MPTPolystyrene->AddProperty("WLSCOMPONENT", PhotonEnergy, EmissionPS, nEntries);
	MPTPolystyrene->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintilFast, nEntries);
	
	MPTPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 10./keV);
	MPTPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
	MPTPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
	MPTPolystyrene->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
	
	polystyrene->SetMaterialPropertiesTable(MPTPolystyrene);

	// Set the Birks Constant for the Polystyrene scintillator
	polystyrene->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

}
