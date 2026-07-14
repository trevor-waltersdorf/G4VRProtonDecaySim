#include "RunAction.hh"

RunAction::RunAction() {
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fEdep);
}

void RunAction::BeginOfRunAction(const G4Run*) {
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run) {
  G4int nOfEvents = run->GetNumberOfEvent();
  if (nOfEvents == 0) return;

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  G4double edep = fEdep.GetValue();
}

void RunAction::AddEdep(G4double edep) {
  fEdep += edep;
}
