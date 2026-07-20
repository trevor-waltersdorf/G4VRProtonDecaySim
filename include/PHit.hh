#ifndef PHIT_H
#define PHIT_H

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class PHit : public G4VHit {
	public:
		PHit() = default;
		~PHit() override = default;
		
		void Print() override;
		
		void SetEdep(G4double e) { fEdep = e; }
		void SetPosition(const G4ThreeVector& p) { fPosition = p; }
		void SetCopyNo(G4int n) { fCopyNo = n; }

		G4double GetEdep() const { return fEdep; }
		G4ThreeVector GetPosition() const { return fPosition; }
		G4int GetCopyNo() const { return fCopyNo; }

		void* operator new(size_t);
		void operator delete(void* hit);
	private:
		G4double fEdep = 0.;
		G4ThreeVector fPosition;
		G4int fCopyNo = 0;
};

using HitCollection = G4THitsCollection<PHit>;
extern G4ThreadLocal G4Allocator<PHit>* HitAllocator;

#endif
