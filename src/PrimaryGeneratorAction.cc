#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
	fParticleGun = new G4ParticleGun(1);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	// Define the pion and positron
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *pi0 = particleTable->FindParticle("pi0");
	G4ParticleDefinition *positron = particleTable->FindParticle("e+");

	// Set position to center
	G4ThreeVector pos(0., 0., 0.);

	// Generate a random world angle and
	// convert to Cartesian unit vector $\hat{n}$
	G4double phi = G4UniformRand() * 2. * 3.14159265358979323846;
	G4double cosTheta = (G4UniformRand() - 0.5) * 2.;
	G4double theta = acosf(cosTheta);

	G4double nx = cosTheta * sinf(phi);
	G4double ny = sinf(theta) * sinf(phi);
	G4double nz = cosf(phi);

	// $|\vec{p}|$ and the final momentum
	G4double p = 459.426 * MeV;
	G4ThreeVector mom(p * nx, p * ny, p * nz);

	// Generate Neutral Pion
	fParticleGun->SetParticleDefinition(pi0);
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentum(mom);
	fParticleGun->GeneratePrimaryVertex(event);
	
	// Generate Positron
	fParticleGun->SetParticleDefinition(positron);
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentum(-mom);
	fParticleGun->GeneratePrimaryVertex(event);
}
