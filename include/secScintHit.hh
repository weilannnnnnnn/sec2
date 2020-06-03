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

        secScintHit &SetPhotonEneg(G4double Eneg);
        secScintHit &SetPhotonGlobalTime(G4double Time);
        secScintHit &SetGenPhotons(G4int PhotonsGen);
        secScintHit &SetMuonEdep(G4double Eneg);
        secScintHit &SetMuonGlobalTime(G4double Time);
        secScintHit &SetMuonVelocity(G4double V);

        G4double GetPhotonEneg(void)       const { return PhotonEneg; }
        G4double GetPhotonGlobalTime(void) const { return PhotonGlobalTime; }
        G4int    GetGenPhotons(void)       const { return GenPhotons; }
        G4double GetMuonEdep(void)         const { return MuonEnegDeposit; }
        G4double GetMuonGlobalTime(void)   const { return MuonGlobalTime; }
        G4double GetMuonVelocity(void)     const { return MuonVelocity; }

    private:
        G4double PhotonEneg;
        G4double PhotonGlobalTime;
        /*
            Currently, variable PhotonEneg saves the Energy of all
            generated photons in a scintillator in a single event.
         */
        G4double MuonEnegDeposit;
        G4double MuonGlobalTime;
        G4double MuonVelocity;

        G4int GenPhotons;
        /*
            GenPhotons saves the the number of generated photons in a scintillator in a
            single event.
         */
        
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

inline secScintHit& secScintHit::SetPhotonEneg(G4double Eneg)
{
    PhotonEneg = Eneg;
    return *this;
}

inline secScintHit& secScintHit::SetPhotonGlobalTime(G4double Time)
{
    PhotonGlobalTime = Time;
    return *this;
}

inline secScintHit& secScintHit::SetGenPhotons(G4int PhotonsGen)
{
    GenPhotons = PhotonsGen;
    return *this;
}

inline secScintHit& secScintHit::SetMuonEdep(G4double Eneg)
{
    MuonEnegDeposit = Eneg;
    return *this;
}

inline secScintHit& secScintHit::SetMuonGlobalTime(G4double Time)
{
    MuonGlobalTime = Time;
    return *this;
}

inline secScintHit& secScintHit::SetMuonVelocity(G4double V)
{
    MuonVelocity = V;
    return *this;
}

#endif
