#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
	fParticleSource = new G4ParticleGun(1);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	// Define the pion and positron
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *pi0 = particleTable->FindParticle("pi0");
	G4ParticleDefinition *positron = particleTable->FindParticle("e+");

	// Set position to center
	G4ThreeVector pos(2.0 * m, 0., 0.);
/*
	// Generate a random world angle and
	// convert to Cartesian unit vector $\hat{n}$
	G4double phi = G4UniformRand() * 2. * 3.14159265358979323846;
	G4double cosTheta = (G4UniformRand() - 0.5) * 2.;
	G4double theta = acosf(cosTheta);

	G4double nx = cosTheta * sinf(phi);
	G4double ny = sinf(theta) * sinf(phi);
	G4double nz = cosf(phi);
*/
	// $|\vec{p}|$ and the final momentum
	G4double p = 459.426 * MeV;
	G4ThreeVector mom(p * -1, p * 0, p * 0);

	// Generate Neutral Pion
	fParticleSource->SetParticleDefinition(pi0);
	fParticleSource->SetParticlePosition(pos);
	fParticleSource->SetParticleMomentum(mom);
	fParticleSource->GeneratePrimaryVertex(event);
	
	// Generate Positron
	fParticleSource->SetParticleDefinition(positron);
	fParticleSource->SetParticlePosition(pos);
	fParticleSource->SetParticleMomentum(-mom);
	fParticleSource->GeneratePrimaryVertex(event);
}

