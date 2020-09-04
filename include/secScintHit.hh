#ifndef secScintHit_hh
#define secScintHit_hh

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include <vector>
#include "globals.hh"

class secScintHit : public G4VHit
{
    public:
    
        using DataGetter = G4double (secScintHit::*)(void) const;
        secScintHit();
        secScintHit(const secScintHit &rhs);
        virtual ~secScintHit();

        const secScintHit &operator=(const secScintHit& rhs);
        G4bool operator==(const secScintHit&) const;

        inline void *operator new(size_t);
        inline void operator delete(void *);

        virtual void Draw(void);
        virtual void Print(void);

        secScintHit &SetEneg(G4double E);
        secScintHit &SetGlobalTime(G4double T);
        secScintHit &SetEdep(G4double E);
        secScintHit &SetVelocity(G4double V);

        G4double GetEneg(void)       const { return Eneg; }
        G4double GetGlobalTime(void) const { return GlobalTime; }
        G4double GetEdep(void)       const { return EnegDeposit; }
        G4double GetVelocity(void)   const { return Velocity; }  

    private:
        G4double Eneg;
        G4double GlobalTime;
        G4double EnegDeposit;
	G4double Velocity;
};

using secScintHitsCollection = G4THitsCollection<secScintHit>;
//using secScintHitCollection = std::vector<secScintHit>;

extern G4ThreadLocal G4Allocator<secScintHit> *secScintHitAllocator;

inline void* secScintHit::operator new(size_t)
{
    if(!secScintHitAllocator)
    {
        secScintHitAllocator = new G4Allocator<secScintHit>;
    }
    return (void *)secScintHitAllocator->MallocSingle();
}

inline void secScintHit::operator delete(void* hit)
{
    secScintHitAllocator->FreeSingle((secScintHit*) hit);
}

inline secScintHit& secScintHit::SetEneg(G4double E)
{
    Eneg = E;
    return *this;
}

inline secScintHit& secScintHit::SetGlobalTime(G4double T)
{
    GlobalTime = T;
    return *this;
}

inline secScintHit& secScintHit::SetEdep(G4double Edep)
{
    EnegDeposit = Edep;
    return *this;
}

inline secScintHit& secScintHit::SetVelocity(G4double V)
{
    Velocity = V;
    return *this;
}

#endif
