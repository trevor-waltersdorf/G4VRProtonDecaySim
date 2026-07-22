#include "DetectorConstruction.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
	G4bool checkOverlaps = true;
  	
	// Set World Material to Water and Detector Material to Glass 
	// Future design will have individual PMT's
	G4NistManager* man = G4NistManager::Instance();
	G4Material* water = man->FindOrBuildMaterial("G4_WATER");
	G4Material* detMat = man->FindOrBuildMaterial("G4_Pyrex_Glass");

	std::vector<G4double> photonEnergy = {2.034 * eV, 2.406 * eV, 2.884 * eV, 3.442 * eV};
	std::vector<G4double> rIndexWater = {1.33, 1.33, 1.34, 1.35};
	std::vector<G4double> rIndexGlass = {1.515, 1.521, 1.533, 1.558};
	auto mptWater = new G4MaterialPropertiesTable();
	mptWater->AddProperty("RINDEX", photonEnergy, rIndexWater);
	water->SetMaterialPropertiesTable(mptWater);
	auto mptGlass = new G4MaterialPropertiesTable();
	mptGlass->AddProperty("RINDEX", photonEnergy, rIndexGlass);
	detMat->SetMaterialPropertiesTable(mptGlass);


	// Initialize World
	G4double world_hx = 2.5 * m;
	G4double world_hy = 2.5 * m;
	G4double world_hz = 2.5 * m;

	G4Box* worldBox = new G4Box("solidWorld", world_hx, world_hy, world_hz);

	G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, water, "logicWorld");

	G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

	// Initialize Detectors
	G4double det_hx = 0.5 * cm;
	G4double det_hy = 2.4 * m;
	G4double det_hz = 2.4 * m;
	G4double margin = 5.0 * cm;
	G4double detPos = world_hx - det_hx - margin;
	G4RotationMatrix* yRotM = new G4RotationMatrix();
	yRotM->rotateZ(90. * deg);
	G4RotationMatrix* zRotM = new G4RotationMatrix();
	zRotM->rotateY(90. * deg);

	G4Box* detBox = new G4Box("solidDetector", det_hx, det_hy, det_hz);
	G4LogicalVolume* detLog = new G4LogicalVolume(detBox, detMat, "logicDetector");
	logicDetector = detLog;
	new G4PVPlacement(0, G4ThreeVector(detPos, 0, 0), detLog, "physDetectorXPos", worldLog, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(-detPos, 0, 0), detLog, "physDetectorXNeg", worldLog, false, 1, checkOverlaps);
	new G4PVPlacement(yRotM, G4ThreeVector(0, detPos, 0), detLog, "physDetectorYPos", worldLog, false, 2, checkOverlaps);
	new G4PVPlacement(yRotM, G4ThreeVector(0, -detPos, 0), detLog, "physDetectorYPos", worldLog, false, 3, checkOverlaps);
	new G4PVPlacement(zRotM, G4ThreeVector(0, 0, detPos), detLog, "physDetectorZPos", worldLog, false, 4, checkOverlaps);
	new G4PVPlacement(zRotM, G4ThreeVector(0, 0, -detPos), detLog, "physDetectorZPos", worldLog, false, 5, checkOverlaps);
	
	G4VisAttributes* detVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0.0, 0.5));
	detVisAtt->SetForceSolid(true);
	detLog->SetVisAttributes(detVisAtt);

	return worldPhys;
}

void DetectorConstruction::ConstructSDandField() {
	SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
