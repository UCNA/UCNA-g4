#ifndef UCNbMaterials_h
#define UCNbMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class UCNbMaterials
{
  public:

    ~UCNbMaterials();
 
    static UCNbMaterials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:
 
    UCNbMaterials();

    void CreateMaterials();

  private:

    static UCNbMaterials* instance;

    G4NistManager* nistMan;

    G4Material* air;
    G4Material* aluminum;
    G4Material* polystyrene;
    G4Material* teflon;
    G4Material* vacuum;

};

#endif /*UCNbMaterials_h*/
