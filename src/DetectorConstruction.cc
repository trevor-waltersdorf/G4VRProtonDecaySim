#include "DetectorConstruction.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
	G4bool checkOverlaps = true;
  	
	// Set World Material to Water and Detector Material to Glass 
	// Future design will have individual PMT's
	G4NistManager* man = G4NistManager::Instance();
	G4Material* air = man->FindOrBuildMaterial("G4_AIR");
	G4Material* water = man->FindOrBuildMaterial("G4_WATER");
	G4Material* detMat = man->FindOrBuildMaterial("G4_Pyrex_Glass");

	std::vector<G4double> photonEnergy = {2.034 * eV, 2.406 * eV, 2.884 * eV, 3.442 * eV};
	std::vector<G4double> rIndexAir = {1.00, 1.00, 1.00, 1.00};
	std::vector<G4double> rIndexWater = {1.33, 1.33, 1.34, 1.35};
	std::vector<G4double> rIndexGlass = {1.515, 1.521, 1.533, 1.558};
	auto mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", photonEnergy, rIndexAir);
	air->SetMaterialPropertiesTable(mptAir);
	auto mptWater = new G4MaterialPropertiesTable();
	mptWater->AddProperty("RINDEX", photonEnergy, rIndexWater);
	water->SetMaterialPropertiesTable(mptWater);
	auto mptGlass = new G4MaterialPropertiesTable();
	mptGlass->AddProperty("RINDEX", photonEnergy, rIndexGlass);
	detMat->SetMaterialPropertiesTable(mptGlass);


	// Initialize World
	G4double world_hx = 25. * m;
	G4double world_hy = 25. * m;
	G4double world_hz = 25. * m;
	G4double tank_r = 10. * m;
	G4double tank_h = 12.5 * m;

	G4Box* worldSol = new G4Box("solidWorld", world_hx, world_hy, world_hz);
	G4LogicalVolume* worldLog = new G4LogicalVolume(worldSol, air, "logicWorld");
	G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

	G4Tubs* waterSol = new G4Tubs("solidWater", 0., tank_r, tank_h, 0., 360. * deg);
	G4LogicalVolume* waterLog = new G4LogicalVolume(waterSol, water, "logicWater");
	new G4PVPlacement(0, G4ThreeVector(0., 0., -12.5 * m), waterLog, "physWater", worldLog, false, checkOverlaps);

	// Initialize Detector
	G4double det_r = 50. * cm; 
	G4double det_h = 25. * m;
	G4Tubs* detSol = new G4Tubs("solidDetector", tank_r, (tank_r + det_r), det_h, 0., 360. * deg);
	G4LogicalVolume* detLog = new G4LogicalVolume(detSol, detMat, "logicDetector");
	logicDetector = detLog;

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), detLog, "physDetector", worldLog, false, 0, checkOverlaps);
	
	G4VisAttributes* waterVisAtt = new G4VisAttributes(G4Color(0.0, 0.8, 0.8, 0.25));
	waterVisAtt->SetForceSolid(true);
	waterLog->SetVisAttributes(waterVisAtt);
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
