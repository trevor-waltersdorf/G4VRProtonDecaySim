#ifndef SENSITIVE_DETECTOR_H
#define SENSITIVE_DETECTOR_H

#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4OpticalPhoton.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"

#include "PHit.hh"

class SensitiveDetector : public G4VSensitiveDetector {
	public:
		SensitiveDetector(G4String name);
		~SensitiveDetector() override = default;;

		G4bool ProcessHits(G4Step* step, G4TouchableHistory* hist) override;
		void Initialize(G4HCofThisEvent* HCE) override;
		void EndOfEvent(G4HCofThisEvent* HCE) override;
	private:
		HitCollection* hitCollection = nullptr;
};

#endif
