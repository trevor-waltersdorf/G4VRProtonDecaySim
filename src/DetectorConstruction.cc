#include "DetectorConstruction.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4bool checkOverlaps = true;
  
  G4NistManager* man = G4NistManager::Instance();
  G4Material* Air = man->FindOrBuildMaterial("G4_WATER");

  G4double world_hx = 0.5 * m;
  G4double world_hy = 0.5 * m;
  G4double world_hz = 0.5 * m;

  G4Box* worldBox = new G4Box("solidWorld", world_hx, world_hy, world_hz);

  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, Air, "logicWorld");

  G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, checkOverlaps);

  return worldPhys;
}
