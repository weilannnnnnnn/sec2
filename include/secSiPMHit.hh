#ifndef secSiPMHit_hh
#define secSiPMHit_hh

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include <vector>
#include "globals.hh"

class secSiPMHit : public G4VHit
{  
    public:
        using DataGetter = G4double (secSiPMHit::*)(void) const;
        secSiPMHit();
        secSiPMHit(const secSiPMHit& rhs);
        virtual ~secSiPMHit();

        //= and ==
        const secSiPMHit& operator=(const secSiPMHit& rhs);
        G4bool operator==(const secSiPMHit&) const ;

        //overload new delete
        inline void* operator new(size_t);
        inline void  operator delete(void* );

        virtual void Draw(void);
        virtual void Print(void);
        
        //set methods
        secSiPMHit& SetPhotonEneg(G4double eneg);
        secSiPMHit& SetGlobalTime(G4double time);

        //get methods
        G4double GetPhotonEneg(void) const;
        G4double GetGlobalTime(void) const;

    private:
        G4double PhotonEneg;
        G4double GlobalTime;

};

using secSiPMHitsCollection = G4THitsCollection<secSiPMHit>;
//using secHitsCollection = std::vector<secSiPMHit>;

extern G4ThreadLocal G4Allocator<secSiPMHit> *secSiPMHitAllocator;

inline void* secSiPMHit::operator new(size_t)
{
    if(!secSiPMHitAllocator)
    {
        secSiPMHitAllocator = new G4Allocator<secSiPMHit>;
    }
    return (void *)secSiPMHitAllocator->MallocSingle();
}

inline void secSiPMHit::operator delete(void* hit)
{
    secSiPMHitAllocator->FreeSingle((secSiPMHit*) hit);
}

inline secSiPMHit& secSiPMHit::SetPhotonEneg(G4double eneg) 
{ 
    PhotonEneg = eneg;
    return *this; 
}

inline secSiPMHit& secSiPMHit::SetGlobalTime(G4double time)
{
    GlobalTime = time;
    return *this;
}
inline G4double secSiPMHit::GetPhotonEneg(void) const 
{
    return PhotonEneg;
}
inline G4double secSiPMHit::GetGlobalTime(void) const
{
    return GlobalTime;
}

#endif
