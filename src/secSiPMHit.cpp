#include "secSiPMHit.hh"

G4ThreadLocal G4Allocator<secSiPMHit> *secSiPMHitAllocator = 0;

secSiPMHit::secSiPMHit() : 
G4VHit(), 
PhotonEneg(0.), 
GlobalTime(0.)
{

}
secSiPMHit::~secSiPMHit()
{

}
secSiPMHit::secSiPMHit(const secSiPMHit& rhs)
{
    PhotonEneg = rhs.PhotonEneg;
}

const secSiPMHit& secSiPMHit::operator=(const secSiPMHit& rhs)
{
    PhotonEneg = rhs.PhotonEneg;
    return *this;
}

G4bool secSiPMHit::operator==(const secSiPMHit& rhs) const
{
    //compare the ptr of the two secHit object!
    return (this == &rhs) ? true : false;
}
void secSiPMHit::Draw()
{

}
void secSiPMHit::Print()
{
    
}
