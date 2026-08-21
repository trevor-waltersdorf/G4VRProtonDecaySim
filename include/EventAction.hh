#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "G4UserEventAction.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "RunAction.hh"
#include "G4SDManager.hh"
#include "PHit.hh"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <string>

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
    void SetIsFreeProton(G4bool ifp) { isFreeProton = ifp; }
    void SetOpenAngle(G4double oa) { openAng = oa; }

  private:
    RunAction* fRunAction = nullptr;
    std::string fRunName;
    G4double fEdep = 0.;
    G4bool isFreeProton = true;
    G4double openAng = 0.;
};

#endif
