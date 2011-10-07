
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "UCNbUserTrackInformation.hh"

UCNbUserTrackInformation::UCNbUserTrackInformation ()
{
   status = undefined;
   exitPosition = G4ThreeVector(0.,0.,0.);
}

UCNbUserTrackInformation::~UCNbUserTrackInformation () { }

// Try adding a status flag and return if it is successful or not
// Cannot Add Undefine or a flag that conflicts with another flag
// Return true if the addition of flag is successful, false otherwise

G4bool UCNbUserTrackInformation::AddStatusFlag(TrackStatus s)
{
	
	/*
   switch (s) {

      case left:
      case right:

        // Allow the user to set left or right
        // only if the track is undefined
        if (status == undefined) return status |= s;

        return false;
 
      case EscapedFromSide:
      case EscapedFromReadOut:

        // Allow the user to set escaped flag
        // only if the photon hasn't exited the fiber yet

        if ((status == undefined) || (status & OutsideOfFiber)) return false;

        return status |= s;
 
      case ReflectedAtMirror:
      case ReflectedAtReadOut:
      case murderee:

        return status |= s;

      case InsideOfFiber:
 
        return ( status =
         (status & ~(EscapedFromSide + EscapedFromReadOut + OutsideOfFiber)) | s);

      case OutsideOfFiber:

        return ( status = (status & ~InsideOfFiber) | s );
 
      default:
 
        return false;
   }
      */
      return false;
}
