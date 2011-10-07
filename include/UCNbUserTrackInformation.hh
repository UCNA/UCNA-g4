
#ifndef UCNbUserTrackInformation_h
#define UCNbUserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "G4ThreeVector.hh"

enum TrackStatus { 
	undefined = 0,
	defined = 1,
	inside = 2,
	outside = 3,
	killed = 4,
	reflected = 5};

/*TrackStatus:
  undefined:
  left:                   track is going -z
  right:                  track is going +z
  defined:                left or right flag is on (Can't be Set)
  EscapedFromSide:        photon escaped through the side of the fiber
  EscapedFromReadOut:     photon escaped through the read-out end of the fiber
  ReflectedAtMirror:      photon has been reflected by the mirror at far end
  ReflectedAtReadOut:     photon has been reflected at the read-out end
  murderee                photon is artificially killed
  InsideOfFiber           Flag is on if the photon is inside of fiber
  OutsideOfFiber          Flag is on if the photon is outside of fiber
*/

class UCNbUserTrackInformation : public G4VUserTrackInformation
{

  public:

    UCNbUserTrackInformation();
    ~UCNbUserTrackInformation();
 
    const G4ThreeVector& GetExitPosition() const { return exitPosition; }
    void SetExitPosition (const G4ThreeVector& pos) { exitPosition = pos; }

    // Try adding a status flag and return if it is successful or not
    // Cannot Add Undefine or a flag that conflicts with another flag
    // Return true if the addition of flag is successful, false otherwise
    G4bool AddStatusFlag(TrackStatus s);

    // Check if a certain flag is on
    G4bool isStatus(TrackStatus s)
       { return s == undefined ? !(status &= defined) : status & s; }

    void Print() const { }
  
  private:

    G4int status;
    G4ThreeVector exitPosition;

};

#endif
