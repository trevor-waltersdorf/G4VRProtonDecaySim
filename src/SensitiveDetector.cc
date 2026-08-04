#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
	collectionName.insert("HitCollection");
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
	G4Track* track = step->GetTrack();

	// Check if it's an optical photon
	if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
		return false;
	}

	// Get rid of non-boundary interactions
	G4StepPoint* postStepPoint = step->GetPostStepPoint();
	if (postStepPoint->GetStepStatus() != fGeomBoundary) {
		return false;
	}

	// Cache G4OpBoundaryProcess instance
	static G4ThreadLocal G4OpBoundaryProcess* boundaryProcess = nullptr;
	if (!boundaryProcess) {
		G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
		G4ProcessVector* processList = pm->GetProcessList();
		for (G4int i = 0; i < pm->GetProcessListLength(); ++i) {
			if ((*processList)[i]->GetProcessName() == "OpBoundary") {
				boundaryProcess = static_cast<G4OpBoundaryProcess*>((*processList)[i]);
				break;
			}
		}
	}

	// Only keep "detection" resultant hits
	if (!boundaryProcess || boundaryProcess->GetStatus() != Detection) {
		return false;
	}

	// Record the hit as happening in the glass to keep a unique copy number
	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	G4int copyNo = preStepPoint->GetTouchableHandle()->GetVolume()->GetCopyNo();
	
	PHit* hit = new PHit();
	hit->SetPosition(postStepPoint->GetPosition()); // Give it position on the boundary
	hit->SetEdep(postStepPoint->GetKineticEnergy());
	hit->SetCopyNo(copyNo);
	hitCollection->insert(hit);

	// Kill photon (optional but helps me sleep)
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
