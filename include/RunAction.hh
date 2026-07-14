#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "G4UserRunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Accumulable.hh"
#include "globals.hh"
#include "G4AccumulableManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

class G4Run;

class RunAction : public G4UserRunAction {
  public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void AddEdep(G4double edep);

  private:  
    G4Accumulable<G4double> fEdep = 0.;
};

#endif
