#include "ActionInitialization.hh"

void ActionInitialization::BuildForMaster() const {
  auto runAction = new RunAction;
  SetUserAction(runAction);
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  auto runAction = new RunAction;
  SetUserAction(runAction);

  auto eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
}
