#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
	collectionName.insert("TestHitCollection");
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
	G4Track* track = step->GetTrack();

	// Check if it's an optical photon
	if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
		return false;
	}

	G4StepPoint* preStepPoint = step->GetPreStepPoint();

	PHit* hit = new PHit();
	hit->SetPosition(preStepPoint->GetPosition());
	hit->SetEdep(preStepPoint->GetKineticEnergy());
	hit->SetCopyNo(preStepPoint->GetTouchableHandle()->GetVolume()->GetCopyNo());
	hitCollection->insert(hit);

	// Kill photon
	track->SetTrackStatus(fStopAndKill);

	return true;
}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
	hitCollection = new HitCollection(GetName(), collectionName[0]);

	static G4int HCID = -1;
	if (HCID < 0) HCID = GetCollectionID(0);
	HCE->AddHitsCollection(HCID, hitCollection);
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
	G4cout << "SD: " << hitCollection->entries() << " photon hit(s) recorded." << G4endl;
}
