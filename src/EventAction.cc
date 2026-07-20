#include "EventAction.hh"

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

void EventAction::BeginOfEventAction(const G4Event*) {
	fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event* event) {
	fRunAction->AddEdep(fEdep);
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("SensitiveDetector/TestHitCollection");
	auto hc = static_cast<HitCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
	
	//Save each hit to csv
	std::ofstream Output("output.csv");
	Output << "Photon #,Edep,X Pos,Y Pos,Z Pos" << std::endl;

	G4int numHits = hc->entries();
	for (G4int i = 0; i < numHits; i++) {
		PHit* hit = (*hc)[i];

		G4double edep = hit->GetEdep();
		G4ThreeVector pos = hit->GetPosition();
		
		Output << i << "," << edep << "," << pos.x() << "," << pos.y() << "," << pos.z() << std::endl;
	}

	Output.close();
}
