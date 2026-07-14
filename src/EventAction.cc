#include "EventAction.hh"

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

void EventAction::BeginOfEventAction(const G4Event*) {
  fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event*) {
  fRunAction->AddEdep(fEdep);
}
