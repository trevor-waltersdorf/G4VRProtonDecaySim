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
	std::time_t now = std::time(nullptr);
    	std::tm* localTime = std::localtime(&now);
    	char buffer[16];
    	std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", localTime);

	std::string runName(buffer);
	std::string fileName = runName + ".csv";
	std::filesystem::path dirPath("../data/" + runName);
	std::filesystem::path filePath = dirPath / fileName;
	std::filesystem::create_directories(dirPath);
	std::ofstream Output(filePath);

	G4int numHits = hc->entries();
	for (G4int i = 0; i < numHits; i++) {
		PHit* hit = (*hc)[i];
		G4double edep = hit->GetEdep();
		G4ThreeVector pos = hit->GetPosition();
		G4int copyNo = hit->GetCopyNo();
		// Hit #, Copy Number, Energy Deposited, X Position, Y Position, Z Position
		Output << i << "," 
			<< copyNo << "," 
			<< edep << "," 
			<< pos.x() << "," 
			<< pos.y() << "," 
			<< pos.z() << std::endl;
	}

	Output.close();
}
