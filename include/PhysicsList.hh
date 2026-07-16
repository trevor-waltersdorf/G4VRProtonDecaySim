#ifndef PHYSICS_LIST_H
#define PHYSICS_LIST_H

#include "G4VModularPhysicsList.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

class PhysicsList : public G4VModularPhysicsList {
	public:
		PhysicsList();
		~PhysicsList();
};

#endif
