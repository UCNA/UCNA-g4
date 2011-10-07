	
#ifndef UCNbTrackingAction_h
#define UCNbTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class UCNbTrackingAction : public G4UserTrackingAction {

  public:

    UCNbTrackingAction() { };
    ~UCNbTrackingAction() { };

    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);

};

#endif
