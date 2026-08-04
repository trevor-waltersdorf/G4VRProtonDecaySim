#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction) : fEventAction(eventAction) {
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

	// Determine whether if proton is free (H) or bound (O)
	G4bool isFreeProton = (G4UniformRand() < 0.2);
	fEventAction->SetIsFreeProton(isFreeProton);
	
	// Obtain proton's effective mass
	G4double mProt = (938.272 - (1.) * !isFreeProton) * MeV; // TODO sample binding energy + reduce p mass

	// Set proton position to center of tank
	// TODO set to random
	G4ThreeVector pos(0., 0., 0.);

	// Generate a random solid angle and
	// convert to Cartesian unit vector $\hat{n}$
	G4double phi = G4UniformRand() * 2. * 3.14159265358979323846;
	G4double cosTheta = (G4UniformRand() - 0.5) * 2.;
	G4double theta = acosf(cosTheta);

	G4double nx = cosf(phi) * sinf(theta);
	G4double ny = sinf(phi) * sinf(theta);
	G4double nz = cosTheta;

	// Construct Pion and Positron CM 4-Momentum
	G4double mPion = 134.977 * MeV;
	G4double mPosi = 0.511 * MeV;
	
	G4double p = 459.426 * MeV; // TODO re-derive momentum from m'
	G4double px = p * nx;
	G4double py = p * ny;
	G4double pz = p * nz;
	G4double pionEnergy = sqrt(px * px + py * py + pz * pz + mPion * mPion);
	G4double posiEnergy = sqrt(px * px + py * py + pz * pz + mPosi * mPosi);

	G4LorentzVector pion4Mom(px, py, pz, pionEnergy);
	G4LorentzVector posi4Mom(-px, -py, -pz, posiEnergy);
	
	// If proton is bound, account for effects of Fermi motion
	if (!isFreeProton) {
		// Sample an isotropic boost direction
		G4double FMphi = G4UniformRand() * 2. * 3.14159265358979323846;
		G4double FMcosTheta = (G4UniformRand() - 0.5) * 2.;
		G4double FMtheta = acosf(FMcosTheta);

		G4double nxF = cosf(FMphi) * sinf(FMtheta);
		G4double nyF = sinf(FMphi) * sinf(FMtheta);
		G4double nzF = FMcosTheta;
	
		// Set fermi momentum, and obtain energy from effective proton mass
		G4double pf = 225 * MeV * pow(G4UniformRand(), (1. / 3.)); // Simple hard sphere fermi gas
		G4double Ep = sqrt(pow(pf, 2) + pow(mProt, 2));
	
		// Obtain boost vector
		G4ThreeVector pF(pf * nxF, pf * nyF, pf * nxF);
		G4ThreeVector beta = pF / Ep;

		// Apply the boost to bring CM 4-momenta to lab frame
		pion4Mom.boost(beta);
		posi4Mom.boost(beta);
	}

	// Calculate opening angle and tag it to the event
	G4double dotProd = (pion4Mom.x() * posi4Mom.x()) + (pion4Mom.y() * posi4Mom.y()) + (pion4Mom.z() * posi4Mom.z());
	G4double cosOpenAng = dotProd / (posi4Mom.vect().mag() * pion4Mom.vect().mag());
	cosOpenAng = std::max(-1.0, std::min(1.0, cosOpenAng));
	G4double openAng = acos(cosOpenAng);
	fEventAction->SetOpenAngle(openAng);

	// Generate Neutral Pion
	fParticleSource->SetParticleDefinition(pi0);
	fParticleSource->SetParticlePosition(pos);
	fParticleSource->SetParticleMomentum(pion4Mom.vect());
	fParticleSource->GeneratePrimaryVertex(event);
	
	// Generate Positron
	fParticleSource->SetParticleDefinition(positron);
	fParticleSource->SetParticlePosition(pos);
	fParticleSource->SetParticleMomentum(posi4Mom.vect());
	fParticleSource->GeneratePrimaryVertex(event);
}
