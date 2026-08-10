#include "PHit.hh"

G4ThreadLocal G4Allocator<PHit>* HitAllocator = nullptr;

void* PHit::operator new(size_t) {
	if (!HitAllocator) HitAllocator = new G4Allocator<PHit>;
	return (void*)HitAllocator->MallocSingle();
}

void PHit::operator delete(void* hit) {
	HitAllocator->FreeSingle((PHit*)hit);
}

void PHit::Print() {
	G4cout << "Photon Hit!" << G4endl
		<< " Pos = " << G4BestUnit(fPosition, "Length") << G4endl
		<< " E = " << G4BestUnit(fEdep, "Energy") << G4endl
		<< " CopyNo = " << fCopyNo << G4endl;
}
