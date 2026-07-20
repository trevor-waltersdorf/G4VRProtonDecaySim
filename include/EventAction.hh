#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"
#include "G4SDManager.hh"
#include "PHit.hh"

#include <iostream>
#include <fstream>

class G4Event;
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    RunAction* fRunAction = nullptr;
    G4double fEdep = 0.;
};

#endif
