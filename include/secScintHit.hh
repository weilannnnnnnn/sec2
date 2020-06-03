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
        secScintHit &SetGenPhotons(G4int PhotonsGen);
        secScintHit &SetMuonEdep(G4double Eneg);

        G4double GetPhotonEneg(void) const;
        G4int GetGenPhotons(void) const;
        G4double GetMuonEdep(void) const ;

    private:
        G4double PhotonEneg;
        /*
            Currently, variable PhotonEneg saves the Energy of all
            generated photons in a scintillator in a single event.
         */
        G4double MuonEnegDeposit;
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

inline G4double secScintHit::GetPhotonEneg(void) const
{
    return PhotonEneg;
}

inline G4int secScintHit::GetGenPhotons(void) const
{
    return GenPhotons;
}

inline G4double secScintHit::GetMuonEdep(void) const
{
    return MuonEnegDeposit;
}

#endif
