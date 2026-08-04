#include "DetectorConstruction.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
	G4bool checkOverlaps = false;
  	
	// Set materials & optical properties
	G4NistManager* man = G4NistManager::Instance();
	G4Material* air = man->FindOrBuildMaterial("G4_AIR");
	G4Material* water = man->FindOrBuildMaterial("G4_WATER");
	G4Material* vacuum = man->FindOrBuildMaterial("G4_Galactic");
	G4Material* tankMat = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
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

	// Initialize world, tank, and water
	G4double world_hx = 25. * m;
	G4double world_hy = 25. * m;
	G4double world_hz = 25. * m;
	G4double tank_ir = 16.9 * m;
	G4double tank_or = 19.4 * m;
	G4double tank_hh = world_hz;

	G4Box* worldSol = new G4Box("solidWorld", world_hx, world_hy, world_hz);
	G4LogicalVolume* worldLog = new G4LogicalVolume(worldSol, air, "logicWorld");
	G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

	G4Tubs* tankSol = new G4Tubs("solidTank", tank_ir, tank_or, tank_hh, 0., 360. * deg);
	G4LogicalVolume* tankLog = new G4LogicalVolume(tankSol, tankMat, "logicTank");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), tankLog, "physTank", worldLog, false, 0, checkOverlaps);
	
	G4Tubs* waterSol = new G4Tubs("solidWater", 0., tank_ir, tank_hh, 0., 360. * deg);
	G4LogicalVolume* waterLog = new G4LogicalVolume(waterSol, water, "logicWater");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), waterLog, "physWater", worldLog, false, checkOverlaps);

	// Initialize Detectors
	G4double det_r = 25. * cm;
	G4double det_wall = 2. * cm;
	G4double vac_r = det_r - det_wall;

	// Create glass sphere
	G4Sphere* glassSol = new G4Sphere("solidGlassPMT", 0., det_r, 0., 360. * deg, 0., 360. * deg);
	G4LogicalVolume* glassLog = new G4LogicalVolume(glassSol, detMat, "logicGlassPMT");
	logicGlass = glassLog;

	// Create vacuum inside PMT
	G4Sphere* vacSol = new G4Sphere("solidVacuumPMT", 0., vac_r, 0., 360. * deg, 0., 360. * deg);
	G4LogicalVolume* vacLog = new G4LogicalVolume(vacSol, vacuum, "logicVacuumPMT");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vacLog, "physVacuumPMT", glassLog, false, 0, checkOverlaps);

	// Create photocathode on glass-vacuum boundary
	G4OpticalSurface* pmtOpSurf = new G4OpticalSurface("PhotocathodeSurface");
	pmtOpSurf->SetType(dielectric_metal);
	pmtOpSurf->SetModel(unified);
	pmtOpSurf->SetFinish(polished);

	// Set reflecitivy and efficiency TODO: swap in real numbers from Hamamatsu datasheet or WCSim
	std::vector<G4double> pmtReflectivity = {0., 0., 0., 0.};
	std::vector<G4double> pmtEfficiency = {0.02, 0.20, 0.25, 0.15};

	auto mptPMT = new G4MaterialPropertiesTable();
	mptPMT->AddProperty("REFLECTIVITY", photonEnergy, pmtReflectivity);
	mptPMT->AddProperty("EFFICIENCY", photonEnergy, pmtEfficiency);
	pmtOpSurf->SetMaterialPropertiesTable(mptPMT);

	new G4LogicalSkinSurface("PhotocathodeSkin", vacLog, pmtOpSurf);

	//Place Detectors
	G4double dAngle = 2. * deg;
	G4double dHeight = 60. * cm;
	G4int cNr = 0;
	G4int cNh = 0;
	for (G4double z = -tank_hh + (30. * cm); z < tank_hh - (30. * cm); z += dHeight) {
		cNr = 0;
		for (G4double j = 0; j < 360. * deg; j += dAngle) {
			G4double x = (tank_ir - det_r) * std::cos(j);
			G4double y = (tank_ir - det_r) * std::sin(j);
			new G4PVPlacement(0, G4ThreeVector(x, y, z), glassLog, "physDetector", waterLog, false, (cNh * 1000 + cNr), checkOverlaps);
			cNr += 1;
		}
		cNh += 1;
	}

	// Set vis attributes
//	G4VisAttributes* waterVisAtt = new G4VisAttributes(G4Color(0., 0.8, 0.8, 0.25));
//	waterVisAtt->SetForceSolid(true);
//	waterLog->SetVisAttributes(waterVisAtt);
	G4VisAttributes* tankVisAtt = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.5));
	tankVisAtt->SetForceSolid(true);
	tankLog->SetVisAttributes(tankVisAtt);
	G4VisAttributes* detVisAtt = new G4VisAttributes(G4Color(0.8, 0.8, 0., 0.5));
	detVisAtt->SetForceSolid(true);
	glassLog->SetVisAttributes(detVisAtt);

	return worldPhys;
}
void DetectorConstruction::ConstructSDandField() {
	SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");
	logicGlass->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
