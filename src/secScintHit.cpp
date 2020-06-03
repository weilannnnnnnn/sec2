#include "secScintHit.hh"

G4ThreadLocal G4Allocator<secScintHit> *secScintHitAllocator = 0;

secScintHit::secScintHit() :
G4VHit(),
PhotonEneg(0.),
PhotonGlobalTime(0.),
MuonEnegDeposit(0.),
MuonGlobalTime(0.),
MuonVelocity(0.),
GenPhotons(-1)
{

}
secScintHit::~secScintHit()
{

}
secScintHit::secScintHit(const secScintHit& rhs)
{
    PhotonEneg = rhs.PhotonEneg;
    GenPhotons = rhs.GenPhotons;
}
const secScintHit& secScintHit::operator=(const secScintHit& rhs)
{
    PhotonEneg = rhs.PhotonEneg;
    GenPhotons = rhs.GenPhotons;
    return *this;
}
G4bool secScintHit::operator==(const secScintHit& rhs) const
{
    return (this == &rhs) ? true : false;
}
void secScintHit::Draw(void)
{

}
void secScintHit::Print(void)
{

}
