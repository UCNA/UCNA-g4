#include "G4Track.hh"
#include "G4VParticleChange.hh"

#include "UCNbStepMax.hh"

UCNbStepMax::UCNbStepMax(const G4String& aName)
  : G4VDiscreteProcess(aName), MaxChargedStep(DBL_MAX)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}

UCNbStepMax::~UCNbStepMax() { }

UCNbStepMax::UCNbStepMax(UCNbStepMax& right) : G4VDiscreteProcess(right) { }

G4bool UCNbStepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0.);
}

void UCNbStepMax::SetStepMax(G4double step) { MaxChargedStep = step ; }

G4double UCNbStepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;

  if ( MaxChargedStep > 0.) ProposedStep = MaxChargedStep;

   return ProposedStep;
}

G4VParticleChange* UCNbStepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

G4double UCNbStepMax::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
  return 0.;
}
