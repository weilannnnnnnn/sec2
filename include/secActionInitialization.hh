#ifndef secActionInitialization_hh
#define secActionInitialization_hh

#include "G4VUserActionInitialization.hh"

class secActionInitialization : public G4VUserActionInitialization
{
public:
    secActionInitialization(void);
    virtual ~secActionInitialization();

public:
    virtual void BuildForMaster(void) const;
    virtual void Build(void) const;
};

#endif // secActionInitialization.hh
