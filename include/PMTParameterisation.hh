#ifndef PMT_PARAMETERISATION_HH
#define PMT_PARAMETERISATION_HH

#include "G4VPVParameterisation.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

#include <cmath>
#include <algorithm>

class G4VPhysicalVolume;

class PMTParameterisation : public G4VPVParameterisation {
	public:
		PMTParameterisation(G4double placementRadius, 
				    G4double dAngle, 
				    G4double dSpace, 
				    G4double zStart, 
				    G4int nPerRing,
				    G4int nPerBarrel,
				    G4int nPerCap,
				    G4double xStart,
				    G4double zCap,
				    G4int (&nPerY)[53],
				    G4int (&cumOffsetTop)[53],
				    G4int (&cumOffsetBot)[53]);
		~PMTParameterisation() override = default;
		
		void ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const;
	
	private:
		G4double fPlacementRadius;
		G4double fDAngle;
		G4double fDSpace;
		G4double fZStart;
		G4int fNPerRing;
		G4int fNPerBarrel;
		G4int fNPerCap;
		G4double fXStart;
		G4double fZCap;
		G4int fNPerY[53];
		G4int fCumOffsetTop[53];
		G4int fCumOffsetBot[53];
};

#endif
