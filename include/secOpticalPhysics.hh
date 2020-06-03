#ifndef secOpticalPhysics_hh
#define secOpticalPhysics_hh

#include "G4OpticalProcessIndex.hh"
#include "G4OpticalSurface.hh"
#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include <vector>

class secScintillation;

class G4VProcess;
class G4EmSaturation;
class G4Cerenkov;
class G4OpWLS;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;
class G4OpAbsorption;


class secOpticalPhysics : public G4VPhysicsConstructor
{
  public:

    secOpticalPhysics(G4int verbose = 0, const G4String& name = "Optical");
    virtual ~secOpticalPhysics();

  protected:

    // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:

    /// Not implemented
    secOpticalPhysics(const secOpticalPhysics& right);
    /// Not implemented
    secOpticalPhysics& operator=(const secOpticalPhysics& right);

  public:

    // configure G4OpticalPhysics builder
    void Configure(G4OpticalProcessIndex, G4bool );

    void SetTrackSecondariesFirst(G4OpticalProcessIndex, G4bool );

    // Cerenkov
    void SetMaxNumPhotonsPerStep(G4int);
    void SetMaxBetaChangePerStep(G4double);
    void SetCerenkovStackPhotons(G4bool);
    void SetCerenkovTrackSecondariesFirst(G4bool);
    void SetCerenkovVerbosity(G4int);

    // Scintillation
    void SetScintillationYieldFactor(G4double );
    void SetScintillationExcitationRatio(G4double );
    void SetScintillationByParticleType(G4bool );
    void SetScintillationTrackInfo(G4bool );
    void SetScintillationTrackSecondariesFirst(G4bool);
    void SetFiniteRiseTime(G4bool );
    void SetScintillationStackPhotons(G4bool );
    void SetScintillationVerbosity(G4int);
    //void AddScintillationSaturation(G4EmSaturation* );

    // WLS
    void SetWLSTimeProfile(G4String );
    void SetWLSVerbosity(G4int);

    //boundary
    void SetBoundaryVerbosity(G4int);
    void SetInvokeSD(G4bool );

    void SetAbsorptionVerbosity(G4int);
    void SetRayleighVerbosity(G4int);
    void SetMieVerbosity(G4int);

  private:

    // methods
    void PrintStatistics() const;

    // The vector of process configuration
    std::vector<G4bool>         fProcessUse;

    // The vector of track secondaries options;
    // the option to track secondaries before finishing their parent track
    std::vector<G4bool>         fProcessTrackSecondariesFirst;

    // scintillation /////////////////
    static G4ThreadLocal secScintillation* fScintillationProcess;
    /// scintillation yield factor
    G4double                    fYieldFactor;

    /// scintillation excitation ratio
    G4double                    fExcitationRatio;

    /// option to set a finite rise-time; Note: the illation
    /// process expects the user to have set the constant material
    /// property FAST/SLOWSCINTILLATIONRISETIME
    G4bool                      fFiniteRiseTime;

    /// option to  allow for the light yield to be a function of
    /// particle type and deposited energy in case of non-linear
    /// light emission in scintillators
    G4bool                      fScintillationByParticleType;

    /// option to allow for illationTrackInformation
    /// to be attached to a scintillation photon's track
    G4bool                      fScintillationTrackInfo;

    /// option to allow stacking of secondary Scintillation photons
    G4bool                      fScintillationStackPhotons;

    G4int                       fScintillationVerbosity;

    ////////////////// Cerenkov
    static G4ThreadLocal G4Cerenkov* fCerenkovProcess;
    /// max number of Cerenkov photons per step
    G4int                       fMaxNumPhotons;
    /// max change of beta per step
    G4double                    fMaxBetaChange;
    /// option to allow stacking of secondary Cerenkov photons
    G4bool                      fCerenkovStackPhotons;
    G4int                       fCerenkovVerbosity;

    ///////////////// WLS
    static G4ThreadLocal G4OpWLS* fWLSProcess;
    G4String                    fWLSTimeProfileName;
    G4int                       fWLSVerbosity;

    static G4ThreadLocal G4OpAbsorption* fAbsorptionProcess;
    G4int                       fAbsorptionVerbosity;

    static G4ThreadLocal G4OpRayleigh* fRayleighProcess;
    G4int                       fRayleighVerbosity;

    static G4ThreadLocal G4OpMieHG*                  fMieProcess;
    G4int                       fMieVerbosity;

    static G4ThreadLocal G4OpBoundaryProcess* fBoundaryProcess;
    /// G4OpBoundaryProcess to call InvokeSD method
    G4bool                      fInvokeSD;
    G4int                       fBoundaryVerbosity;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // G4OpticalPhysics_h
