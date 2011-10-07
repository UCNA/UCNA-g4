
#ifndef UCNbExtraPhysics_h
#define UCNbExtraPhysics_h 1

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

class UCNbExtraPhysics : public G4VPhysicsConstructor
{
  public:

    UCNbExtraPhysics();
    virtual ~UCNbExtraPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

};

#endif
