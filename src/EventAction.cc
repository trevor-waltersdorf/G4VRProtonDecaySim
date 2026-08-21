#include "EventAction.hh"

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

void EventAction::BeginOfEventAction(const G4Event*) {
	fEdep = 0.;
	
	// Get current date and time
	std::time_t now = std::time(nullptr);
    	std::tm* localTime = std::localtime(&now);
    	char buffer[16];
    	std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", localTime);

	// Set name of run to YYMMDDHHMMSS format
	fRunName = std::string(buffer);
}

void EventAction::EndOfEventAction(const G4Event* event) {
	// Add edep to run total
	fRunAction->AddEdep(fEdep);

	// Get hit collection
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("SensitiveDetector/HitCollection");
	auto hc = static_cast<HitCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
	
//Save each hit to csv

	// Set name of data storage to run name
	std::string fileName = fRunName + ".csv";
	std::filesystem::path dirPath("../data/" + fRunName);
	std::filesystem::path filePath = dirPath / fileName;
	std::filesystem::create_directories(dirPath);
	std::ofstream Output(filePath);

	// Append proton state and opening angle to runInfo.csv
	// for analysis of many runs
	std::ofstream RunInfo("../data/runInfo.csv", std::ios::app);
	RunInfo << isFreeProton << "," << openAng << std::endl;
	RunInfo.close();

	// Print hit attributes to the csv file
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
