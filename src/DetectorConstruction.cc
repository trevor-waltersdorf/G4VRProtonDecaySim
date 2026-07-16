#include "DetectorConstruction.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4bool checkOverlaps = true;
  
  G4NistManager* man = G4NistManager::Instance();
  G4Material* water = man->FindOrBuildMaterial("G4_WATER");

  std::vector<G4double> photonEnergy = {2.034 * eV, 2.406 * eV, 2.884 * eV, 3.442 * eV};
  std::vector<G4double> rIndex = {1.33, 1.33, 1.34, 1.35};
  auto mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, rIndex);
  water->SetMaterialPropertiesTable(mpt);

  G4double world_hx = 2.5 * m;
  G4double world_hy = 2.5 * m;
  G4double world_hz = 2.5 * m;

  G4Box* worldBox = new G4Box("solidWorld", world_hx, world_hy, world_hz);

  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, water, "logicWorld");

  G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

  return worldPhys;
}
