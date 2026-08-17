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
	G4double halfRadius = fRadius / 2.;
	G4double quarterRadius = fRadius / 4.;
	G4RotationMatrix* coneRot = new G4RotationMatrix();
	coneRot->rotateY(90. * deg);

	// Initialize glass cover
	G4Sphere* glassSol = new G4Sphere("solidGlass", vacuumRadius, fRadius, 90. * deg, 180. * deg, 0., 360. * deg);
	G4LogicalVolume* glassLog = new G4LogicalVolume(glassSol, fGlassMaterial, "logicalGlass");

	// Place vacuum inside
	G4Sphere* vacuumSol = new G4Sphere("solidVacuum", 0., vacuumRadius, 90. * deg, 180. * deg, 0., 360. * deg);
	G4LogicalVolume* vacuumLog = new G4LogicalVolume(vacuumSol, fVacuumMaterial, "logicalVacuum");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vacuumLog, "physVacuum", glassLog, false, 0, false);

	// Place photocathode on their boundary
	new G4LogicalSkinSurface("PhotocathodeSkin", vacuumLog, fPhotocathodeSurface);

	// Outline cone parameters
	G4double planePos[] = { 0., -halfRadius, -fRadius };
	G4double innerRadii[] = { 0., 0., 0. };
	G4double outerRadii[] = { fRadius, quarterRadius, quarterRadius };

	// Place cone
	G4VSolid* coneSol = new G4Polycone("solidCone", 0., 360. * deg, 3, planePos, innerRadii, outerRadii);
	G4LogicalVolume* coneLog = new G4LogicalVolume(coneSol, fConeMaterial, "logicalCone");
	new G4PVPlacement(coneRot, G4ThreeVector(0., 0., 0.), coneLog, "physCone", glassLog, false, 0, false);

	// Set visualization attributes
	// PMTs: Yellow (50% Opacity)
	G4VisAttributes* glassVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0., 0.5));
	glassVisAtt->SetForceSolid(true);
	glassLog->SetVisAttributes(glassVisAtt);
	// Cone: Light Grey (100% Opacity)
	G4VisAttributes* coneVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0.8, 1.));
	coneVisAtt->SetForceSolid(true);
	coneLog->SetVisAttributes(coneVisAtt);
	// Vacuum: Invisible
	vacuumLog->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	return glassLog;
}
