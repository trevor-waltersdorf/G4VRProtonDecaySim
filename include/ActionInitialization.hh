#ifndef ACTION_INITIALIZATION_H
#define ACTION_INITIALIZATION_H

#include "G4VUserActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

class ActionInitialization : public G4VUserActionInitialization {
  public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;
};

#endif
