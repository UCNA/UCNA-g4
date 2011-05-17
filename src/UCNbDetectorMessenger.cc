#include "UCNbDetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

UCNbDetectorMessenger::UCNbDetectorMessenger(UCNbDetectorConstruction * Det)
 : Detector(Det)
{
	detDir = new G4UIdirectory("/UCNb/");
	detDir->SetGuidance("Geometry Setup");

	UpdateCmd = new G4UIcmdWithoutParameter("/UCNb/Update",this);
	UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
	UpdateCmd->SetGuidance("if you changed geometrical value(s).");
	UpdateCmd->AvailableForStates(G4State_Idle);
	
	SetNumOfPMTsCmd = new G4UIcmdWithAnInteger("/UCNb/setNumOfPMTs", this);
	SetNumOfPMTsCmd->SetGuidance("Select the number of PMTs");
	SetNumOfPMTsCmd->SetGuidance("Maximum number is 5");
	SetNumOfPMTsCmd->SetParameterName("numberOfPMTs", false);
	SetNumOfPMTsCmd->SetRange("numberOfPMTs>=1 && numberOfPMTs<=5");
	SetNumOfPMTsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	SetTeflonReflectivityCmd = new G4UIcmdWithADouble("/UCNb/setTeflonReflectivity", this);
	SetTeflonReflectivityCmd->SetGuidance("Set the reflectivity of the scintillator-teflon boundary");
	SetTeflonReflectivityCmd->SetParameterName("reflectivity", false);
	SetTeflonReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
	SetTeflonReflectivityCmd->AvailableForStates(G4State_PreInit);
}

UCNbDetectorMessenger::~UCNbDetectorMessenger()
{
  delete detDir;
  delete UpdateCmd;
  delete SetNumOfPMTsCmd;
  delete SetTeflonReflectivityCmd;
}

void UCNbDetectorMessenger::SetNewValue(G4UIcommand* command, G4String val)
{
	if (command == UpdateCmd) {
		Detector->UpdateGeometry();
	} else if (command == SetNumOfPMTsCmd) {
		Detector->SetNumberOfPMTs(G4UIcmdWithAnInteger::GetNewIntValue(val));
		Detector->UpdateGeometry();
	} else if (command == SetTeflonReflectivityCmd) {
		Detector->SetTeflonReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
	}
}
