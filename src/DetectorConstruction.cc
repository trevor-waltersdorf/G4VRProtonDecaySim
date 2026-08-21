#include "DetectorConstruction.hh"
#include "G4ios.hh"
G4VPhysicalVolume *DetectorConstruction::Construct() {
	G4bool checkOverlaps = false;
  	
// Define materials & optical properties
	// Instantiate materials
	G4NistManager* man = G4NistManager::Instance();
	G4Material* airMat = man->FindOrBuildMaterial("G4_AIR");
	G4Material* waterMat = man->FindOrBuildMaterial("G4_WATER");
	G4Material* vacuumMat = man->FindOrBuildMaterial("G4_Galactic");
	G4Material* tankMat = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	G4Material* glassMat = man->FindOrBuildMaterial("G4_Pyrex_Glass");
	G4Material* coneMat = man->FindOrBuildMaterial("G4_Al");
	G4OpticalSurface* photoSurf = new G4OpticalSurface("PhotocathodeSurface");

	// Set photon energy across Hamamatsu R3600 spectral response
	std::vector<G4double> photonEnergy = {1.9074 * eV, 2.1377 * eV, 2.4311 * eV, 2.8178 * eV, 3.3509 * eV, 4.1328 * eV};
	// Set index of refraction for all optical materials
	std::vector<G4double> rIndexAir = {1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003};
	std::vector<G4double> rIndexWater = {1.3315, 1.3335, 1.3362, 1.3401, 1.3466, 1.3591};
	std::vector<G4double> rIndexGlass = {1.4807, 1.4829, 1.4861, 1.4910, 1.4988, 1.5128};
	// Set attentuation/absorption length in water
	std::vector<G4double> absLengthWater = {2.8653 * m, 9.2593 * m, 28.0112 * m, 68.9655 * m, 33.3333 * m, 7.0922 * m};
	// Set quantum efficiency + reflectivity of photocathode
	std::vector<G4double> photocathodeReflectivity = {0., 0., 0., 0., 0., 0.};
	std::vector<G4double> photocathodeEfficiency = {.22, .22, .22, .22, .22, .22}; //TODO

	// Create property tables and add them to materials
	auto mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", photonEnergy, rIndexAir);
	airMat->SetMaterialPropertiesTable(mptAir);
	auto mptWater = new G4MaterialPropertiesTable();
	mptWater->AddProperty("RINDEX", photonEnergy, rIndexWater);
	mptWater->AddProperty("ABSLENGTH", photonEnergy, absLengthWater);
	waterMat->SetMaterialPropertiesTable(mptWater);
	auto mptGlass = new G4MaterialPropertiesTable();
	mptGlass->AddProperty("RINDEX", photonEnergy, rIndexGlass);
	glassMat->SetMaterialPropertiesTable(mptGlass);
	auto mptPhotocathode = new G4MaterialPropertiesTable();
	mptPhotocathode->AddProperty("REFLECTIVITY", photonEnergy, photocathodeReflectivity);
	mptPhotocathode->AddProperty("EFFICIENCY", photonEnergy, photocathodeEfficiency);
	photoSurf->SetType(dielectric_metal);
	photoSurf->SetModel(unified);
	photoSurf->SetFinish(polished);
	photoSurf->SetMaterialPropertiesTable(mptPhotocathode);

// Initialize world, tank, and water
	
	// World + tank dimensions
	G4double world_hx = 25. * m;
	G4double world_hy = 25. * m;
	G4double world_hz = 25. * m;
	G4double tank_ir = 16.9 * m;
	G4double tank_w = 2.5 * m;
	G4double tank_or = tank_ir + tank_w;
	G4double tank_hh = 18.1 * m;

	// Place world volume
	G4Box* worldSol = new G4Box("solidWorld", world_hx, world_hy, world_hz);
	G4LogicalVolume* worldLog = new G4LogicalVolume(worldSol, airMat, "logicWorld");
	G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

	// Place tank barrel
	G4Tubs* tankBarSol = new G4Tubs("solidTankBarrel", tank_ir, tank_or, tank_hh, 0., 360. * deg);
	G4LogicalVolume* tankBarLog = new G4LogicalVolume(tankBarSol, tankMat, "logicTankBarrel");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), tankBarLog, "physTankBarrel", worldLog, false, 0, checkOverlaps);

	// Place tank caps
	G4Tubs* tankCapSol = new G4Tubs("solidTankCap", 0, tank_or, (tank_w / 2), 0., 360. * deg);
	G4LogicalVolume* tankCapLog = new G4LogicalVolume(tankCapSol, tankMat, "logicTankCap");
	new G4PVPlacement(0, G4ThreeVector(0., 0., (tank_hh + tank_w / 2)), tankCapLog, "physTankCeiling", worldLog, false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(0., 0., -(tank_hh + tank_w / 2)), tankCapLog, "physTankFloor", worldLog, false, 0, checkOverlaps);

	// Fill tank with water
	G4Tubs* waterSol = new G4Tubs("solidWater", 0., tank_ir, tank_hh, 0., 360. * deg);
	G4LogicalVolume* waterLog = new G4LogicalVolume(waterSol, waterMat, "logicWater");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), waterLog, "physWater", worldLog, false, checkOverlaps);

// Initialize Detectors

	// Photomultiplier tube dimensions
	G4double det_r = 25. * cm;
	G4double det_wall = 2. * cm;

	// Initialize PMT Logical Volume
	PMTConstruction* pmt = new PMTConstruction(glassMat, vacuumMat, coneMat, photoSurf, det_r, det_wall);
	G4LogicalVolume* PMTLogicalVolume = pmt->getLogicalVolume();
	pmtLog = PMTLogicalVolume;

	// Define PMT barrel placement parameters
	G4double placementRadius = tank_ir - det_r;
	G4double dAngle = 3. * deg;
	G4double dSpace = 60. * cm;
	G4double zStart = -tank_hh + (30. * cm);
	G4int nPerRing = static_cast<G4int>(std::round(360 * deg / dAngle));
	G4int nRings = static_cast<G4int>(std::floor((2. * (tank_hh - 30. * cm)) / dSpace)) + 1;
	G4int nBarrelPMTs = nRings * nPerRing;

	// Define PMT cap placement parameters
	G4int nPerY[] = {11, 19, 23, 27, 29, 31, 35, 39, 39, 41, 41, 43, 45, 47, 47, 49, 49, 51, 51, 51, 51, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 49, 49, 47, 47, 45, 43, 41, 41, 39, 39, 35, 31, 29, 27, 23, 19, 11};
	G4int nCapPMTs = 0;
	for (int i = 0; i < 53; i++) nCapPMTs += nPerY[i];
	G4int cumOffsetTop[53];
	G4int cumOffsetBot[53];
	cumOffsetTop[0] = nBarrelPMTs;
	for (int i = 1; i < 53; i++) {
		cumOffsetTop[i] = cumOffsetTop[i-1] + nPerY[i-1];
	}
	for (int i = 0; i < 53; i++) {
		cumOffsetBot[i] = cumOffsetTop[i] + nCapPMTs;
	}
	G4double xStart = -15.6 * m;
	G4double zCap = tank_hh - det_r; 

	// Calculate total number of PMTs used
	G4int totalPMTs = nBarrelPMTs + 2 * nCapPMTs;

	// Place detectors with G4PVParameterised
	auto* tankParam = new PMTParameterisation(placementRadius, 
						  dAngle, 
						  dSpace, 
						  zStart, 
						  nPerRing, 
						  nBarrelPMTs, 
						  nCapPMTs, 
						  xStart,
						  zCap,
						  nPerY,
						  cumOffsetTop,
						  cumOffsetBot);
	new G4PVParameterised("physDetector", PMTLogicalVolume, waterLog, kUndefined, totalPMTs, tankParam, checkOverlaps);

// Set vis attributes
	// Water: Cyan (25% Opacity)
	G4VisAttributes* waterVisAtt = new G4VisAttributes(G4Color(0., 0.8, 0.8, 0.25));
	waterVisAtt->SetForceSolid(true);
	waterLog->SetVisAttributes(waterVisAtt);
	// Tank: Grey (25% Opacity)
	G4VisAttributes* tankVisAtt = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.25));
	tankVisAtt->SetForceSolid(true);
	tankBarLog->SetVisAttributes(tankVisAtt);
	tankCapLog->SetVisAttributes(tankVisAtt);

	return worldPhys;
}
void DetectorConstruction::ConstructSDandField() {
	// Instantiate sensitive detector and adding to the PMT volume
	SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");
	pmtLog->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
