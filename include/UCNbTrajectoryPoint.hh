#ifndef UCNbTrajectoryPoint_h_seen
#define UCNbTrajectoryPoint_h_seen 1

#include "globals.hh"

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"

#include "G4StepStatus.hh"

class G4Track;
class G4Step;
class G4VProcess;

class UCNbTrajectoryPoint : public G4TrajectoryPoint {

//--------
  public: // without description
//--------

// Constructor/Destructor

    UCNbTrajectoryPoint();
    UCNbTrajectoryPoint(const G4Track* aTrack);
    UCNbTrajectoryPoint(const G4Step* aStep);
    UCNbTrajectoryPoint(const UCNbTrajectoryPoint &right);
    virtual ~UCNbTrajectoryPoint();

// Operators

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const UCNbTrajectoryPoint& right) const
    { return (this==&right); };

// Get/Set functions

    inline G4double GetTime() const { return fTime; };
    inline const G4ThreeVector GetMomentum() const { return fMomentum; };
    inline G4StepStatus GetStepStatus() const { return fStepStatus; };
    inline G4String GetVolumeName() const { return fVolumeName; };

// Get method for HEPRep style attributes

   virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
   virtual std::vector<G4AttValue>* CreateAttValues() const;

//---------
  private:
//---------

// Member data

    G4double fTime;
    G4ThreeVector fMomentum;
    G4StepStatus fStepStatus;
    G4String fVolumeName;

};

#if defined G4TRACKING_ALLOC_EXPORT
  extern G4DLLEXPORT G4Allocator<UCNbTrajectoryPoint> UCNbTrajPointAllocator;
#else
  extern G4DLLIMPORT G4Allocator<UCNbTrajectoryPoint> UCNbTrajPointAllocator;
#endif

inline void* UCNbTrajectoryPoint::operator new(size_t)
{
    void *aTrajectoryPoint = (void *) UCNbTrajPointAllocator.MallocSingle();
    return aTrajectoryPoint;
}

inline void UCNbTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
    UCNbTrajPointAllocator.FreeSingle(
        (UCNbTrajectoryPoint *) aTrajectoryPoint);
}

#endif
