#include "PMTConstruction.hh"

PMTConstruction::PMTConstruction(G4Material* gM,
				 G4Material* vM, 
				 G4Material* cM,
				 G4OpticalSurface* pS,
				 G4double r, 
				 G4double gT) : 
		fGlassMaterial(gM),
		fPhotocathodeSurface(pS),
		fVacuumMaterial(vM),
		fConeMaterial(cM),
		fRadius(r),
		fGlassThickness(gT) {}

G4LogicalVolume* PMTConstruction::getLogicalVolume() {
	// Obtain additional measurements
	G4double vacuumRadius = fRadius - fGlassThickness;
	G4double quarterRadius = fRadius / 4.;

	// Initialize glass cover
	G4Sphere* solidGlass = new G4Sphere("SolidGlass", vacuumRadius, fRadius, 0., 360. * deg, 0., 360. * deg);
	G4LogicalVolume* glassLog = new G4LogicalVolume(solidGlass, fGlassMaterial, "logicalGlass");
	G4VisAttributes* detVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0., 0.5));
	detVisAtt->SetForceSolid(true);
	glassLog->SetVisAttributes(detVisAtt);

	return glassLog;
}
