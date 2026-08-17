#ifndef PMT_CONSTRUCTION_HH
#define PMT_CONSTRUCTION_HH

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"

#include "G4Sphere.hh"
#include "G4Polycone.hh"

class PMTConstruction {
	public:
		PMTConstruction(G4Material* gM, G4Material* vM, G4Material* cM, G4OpticalSurface* pS, G4double r, G4double gT);
		G4LogicalVolume* getLogicalVolume();
	private:
		G4Material* fGlassMaterial;
		G4Material* fVacuumMaterial;
		G4Material* fConeMaterial;
		G4OpticalSurface* fPhotocathodeSurface;

		G4double fRadius;
		G4double fGlassThickness;
};

#endif
