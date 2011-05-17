
#ifndef UCNbEventAction_h
#define UCNbEventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"

class UCNbRunAction;

class UCNbEventAction : public G4UserEventAction
{
  public:

    UCNbEventAction(UCNbRunAction*);
    ~UCNbEventAction();

  public:

    void   BeginOfEventAction(const G4Event*);
    void     EndOfEventAction(const G4Event*);

    G4int GetEventNo();
    void SetEventVerbose(G4int);

    void SetDrawFlag(G4String val)  { drawFlag = val; };
    void SetPrintModulo(G4int val)  { printModulo = val; };

    void SetForceDrawPhotons(G4bool b){forcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){forcenophotons=b;}
    
    int GetHitsNumber();
 
  private:

    UCNbRunAction* runaction;

    G4int verboselevel;
    G4int printModulo;
 
    G4int pmtCollID;
    
    G4int n_hit;
 
    G4String drawFlag;

    G4bool forcedrawphotons;
    G4bool forcenophotons;
    

};

#endif
