#include "PMTParameterisation.hh"

PMTParameterisation::PMTParameterisation(
		G4double placementRadius,
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
		G4int (&cumOffsetBot)[53]) : 
	fPlacementRadius(placementRadius),
	fDAngle(dAngle),
	fDSpace(dSpace),
	fZStart(zStart),
	fNPerRing(nPerRing),
	fNPerBarrel(nPerBarrel),
	fNPerCap(nPerCap),
	fXStart(xStart),
	fZCap(zCap) {
	std::copy(nPerY, nPerY + 53, fNPerY);
	std::copy(cumOffsetTop, cumOffsetTop + 53, fCumOffsetTop);
	std::copy(cumOffsetBot, cumOffsetBot + 53, fCumOffsetBot);
	}

void PMTParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const {
	G4double x, y, z;
	G4RotationMatrix* rotMatrix = new G4RotationMatrix();

	if (copyNo < fNPerBarrel) {
		G4int ringIndex = copyNo / fNPerRing;
		G4int angIndex = copyNo % fNPerRing;

		G4double angle = angIndex * fDAngle;
		z = fZStart + ringIndex * fDSpace;
		x = fPlacementRadius * std::cos(angle);
		y = fPlacementRadius * std::sin(angle);
		rotMatrix->rotateZ(-angle);
	} else if (copyNo < fNPerBarrel + fNPerCap) {
		auto it = std::upper_bound(fCumOffsetTop, fCumOffsetTop + 53, copyNo);
		G4int xIndex = (it - fCumOffsetTop) - 1;
		G4int yIndex = copyNo - fCumOffsetTop[xIndex];
		
		G4double yStart = -1. * (std::floor(fNPerY[xIndex] / 2.)) * fDSpace;
		
		x = fXStart + fDSpace * xIndex;
		y = yStart + fDSpace * yIndex;
		z = fZCap;
		rotMatrix->rotateY(90. * deg);
	} else {
		auto it = std::upper_bound(fCumOffsetBot, fCumOffsetBot + 53, copyNo);
		G4int xIndex = (it - fCumOffsetBot) - 1;
		G4int yIndex = copyNo - fCumOffsetBot[xIndex];
		
		G4double yStart = -1. * (std::floor(fNPerY[xIndex] / 2.)) * fDSpace;
		
		x = fXStart + fDSpace * xIndex;
		y = yStart + fDSpace * yIndex;
		z = -fZCap;
		rotMatrix->rotateY(-90. * deg);
	}

	physVol->SetTranslation(G4ThreeVector(x, y, z));
	physVol->SetRotation(rotMatrix);
}
