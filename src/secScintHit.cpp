#include "secScintHit.hh"

G4ThreadLocal G4Allocator<secScintHit> *secScintHitAllocator = 0;

secScintHit::secScintHit() :
G4VHit(),
Eneg(0.),
GlobalTime(0.),
EnegDeposit(0.),
Velocity(0.)
{

}
secScintHit::~secScintHit()
{

}
secScintHit::secScintHit(const secScintHit& rhs)
{
    Eneg = rhs.Eneg;
    GlobalTime = rhs.GlobalTime;
    EnegDeposit = rhs.EnegDeposit;
}
const secScintHit& secScintHit::operator=(const secScintHit& rhs)
{
    Eneg = rhs.Eneg;
    GlobalTime = rhs.GlobalTime;
    EnegDeposit = rhs.EnegDeposit;
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
