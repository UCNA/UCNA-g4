#ifndef UCNbTrajectory_h_seen
#define UCNbTrajectory_h_seen 1

#include <vector>
#include <stdlib.h>

#include "globals.hh"

#include "G4ios.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VTrajectory.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"

typedef std::vector<G4VTrajectoryPoint*> UCNbTrajectoryPointContainer;

class UCNbTrajectory : public G4VTrajectory
{

//--------
   public: // without description
//--------

// Constructor/Destructor

     UCNbTrajectory();
     UCNbTrajectory(const G4Track* aTrack);
     UCNbTrajectory(UCNbTrajectory &);
     virtual ~UCNbTrajectory();

// Operators

     inline void* operator new(size_t);
     inline void  operator delete(void*);
     inline int operator == (const UCNbTrajectory& right) const
     { return (this==&right); }

// Get/Set functions

     inline G4int GetTrackID() const { return fTrackID; }
     inline G4int GetParentID() const { return fParentID; }
     inline G4String GetParticleName() const { return ParticleName; }
     inline G4double GetCharge() const { return PDGCharge; }
     inline G4int GetPDGEncoding() const { return PDGEncoding; }
     inline G4ThreeVector GetInitialMomentum() const { return initialMomentum; }

// Other member functions

     virtual void ShowTrajectory(std::ostream& os=G4cout) const;
     virtual void DrawTrajectory(G4int i_mode=0) const;
     virtual void AppendStep(const G4Step* aStep);
     virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

     G4ParticleDefinition* GetParticleDefinition();

     virtual int GetPointEntries() const
     { return fpPointsContainer->size(); }
     virtual G4VTrajectoryPoint* GetPoint(G4int i) const
     { return (*fpPointsContainer)[i]; }

    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;

    void SetDrawTrajectory(G4bool b){drawit=b;}
    void WLS(){wls=true;}
    void SetForceDrawTrajectory(G4bool b){forceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){forceNoDraw=b;}

//---------
   private:
//---------

// Member data

     UCNbTrajectoryPointContainer* fpPointsContainer;

     G4int fTrackID;
     G4int fParentID;
     G4double PDGCharge;
     G4int    PDGEncoding;
     G4String ParticleName;
     G4ThreeVector initialMomentum;

     G4bool wls;
     G4bool drawit;
     G4bool forceNoDraw;
     G4bool forceDraw;

     G4ParticleDefinition* particleDefinition;

};

#if defined G4TRACKING_ALLOC_EXPORT
  extern G4DLLEXPORT G4Allocator<UCNbTrajectory> UCNbTrajectoryAllocator;
#else
  extern G4DLLIMPORT G4Allocator<UCNbTrajectory> UCNbTrajectoryAllocator;
#endif

inline void* UCNbTrajectory::operator new(size_t) {
    void* aTrajectory = (void*) UCNbTrajectoryAllocator.MallocSingle();
    return aTrajectory;
}

inline void UCNbTrajectory::operator delete(void* aTrajectory) {
    UCNbTrajectoryAllocator.FreeSingle((UCNbTrajectory*)aTrajectory);
}

#endif
