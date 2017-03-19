//////////////////////////////////////////////////////
/// \file Kinematics.hxx                           ///
/// \author D. Connolly                            ///
/// \brief Class to claculate kinematics for SONIK ///
//////////////////////////////////////////////////////
#ifndef SONIK_KINEMATICS_HXX
#define SONIK_KINEMATICS_HXX
#include <map>
#include <memory>
#include <fstream>
#ifndef __MAKECINT__
#include <iostream>
#endif

// #include <TROOT.h>
// #include <TObject.h>

#include "Constants.hxx"

class Constants;

namespace sonik {

/// \full Class for calculating kinematics for scattering reactions.

class Kinematics {//: public TObject {
public:
  /// Laboratory scattering angles for all observation points (in degrees)
  static const Double_t ThetaLab[13];

public:
  /// Atomic number of the beam ion.
  Int_t fA_b;
  /// Atomic number of the target ion.
  Int_t fA_t;
  /// Proton number of the beam ion.
  Int_t fZ_b;
  /// Proton number of the beam ion.
  Int_t fZ_t;
  /// Kinetic energy of the beam ion in MeV.
  Double_t fT_b;
  /// Speed squared of the beam ion in MeV / u.
  Double_t fVelSquared;
  /// Mass deficit of the beam ion in MeV.
  Double_t fDelta_b;
  /// Mass deficit of the target ion in MeV.
  Double_t fDelta_t;
  /// Energy of the two particle system in the center of momentum frame (in MeV).
  Double_t fE_CM;
  /// Mass of the beam ion in MeV \f$ c^{-2} \f$.
  Double_t fM_b;
  /// Mass of the target ion in MeV \f$ c^{-2} \f$.
  Double_t fM_t;
  /// Mass of the ejectile in MeV \f$ c^{-2} \f$.
  Double_t fM_ej;
  /// Mass of the recoil in MeV \f$ c^{-2} \f$.
  Double_t fM_rec;
  /// Lorentz factor of the beam ions in the lab frame.
  Double_t fGammaLab;
  /// Speed of the beam ions (as a fraction of the speed of light \f$\beta\f$) in the lab frame.
  Double_t fBetaLab;
  /// Lorentz factor \f$\gamma\f$ of the center of momentum frame.
  Double_t fGammaCM;
  /// Speed of the center of momentum frame as a fraction of the speed of light \f$\beta\f$.
  Double_t fBetaCM;
  /// Invariant mass of the two-particle system MeV \f$ c^{-2} \f$.
  Double_t fInvariantM;
  /// Momentum of the center of momentum frame
  Double_t fP_CM;
  /// Rapidity of the two-particle system.
  Double_t fRapidity;
  /// Scattering angle of the ejectile in the center of momentum frame.
  Double_t fThetaCM_ej[13];
  /// Scattering angle of the (detected) recoil in the center of momentum frame in degrees.
  Double_t fThetaCM_rec[13];
  /// Maximum scattering angle of the ejectile in the lab frame.
  Double_t fThetaMax_ej;
  /// Scattering angle of the (detected) recoil in the lab frame in degrees.
  Double_t fThetaMax_rec;
  /// Momentum of the ejectile in the lab frame.
  Double_t fP_ej[13];
  /// Momentum of the recoil in the lab frame.
  Double_t fP_rec[13];
  /// Kinetic energy of the ejectile in the lab frame in MeV.
  Double_t fT_ej[13];
  /// Kinetic energy of the recoil in the lab frame in MeV.
  Double_t fT_rec[13];


public:
  Kinematics();
  Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);

public: // public member functions
  /// Calculate the fraction of the speed of light of the center of momentum frame
  Double_t GetBetaCM() const { return tanh(fRapidity); }
  /// Calculate the speed of beam ions (as a fraction of the speed of light) in the lab frame
  Double_t GetBetaLab() const { return sqrt(1 - 1 / pow(fGammaLab,2) ); }
  /// Calculate the center of mass energy of the two-particle system
  Double_t GetE_CM() const { return sqrt(fInvariantM) - (fM_b + fM_t); }
  /// Calculate the Lorentz factor of the center of momentum frame
  Double_t GetGammaCM() const { return cosh(fRapidity); }
  /// Calculate the Lorentz factor of the beam ions in the lab frame
  Double_t GetGammaLab() const { return fT_b / fM_b + 1; }
  /// Calculate the invariant mass of the two-particle system
  Double_t GetInvariantMass() const { return 2*fM_t*fT_b + pow((fM_b + fM_t), 2); }
  /// Calculate the mass of nucleus in MeV / \f$c^2\f$
  Double_t CalcMass(Int_t A, Double_t Delta) { return A*sonik::Constants::AMU() + Delta; }
  /// Calculate the momentum of the center of momentum frame
  Double_t GetP_CM() const;
  /// Calculate the rapidity of the two-particle system
  Double_t GetRapidity() const { return log( (fP_CM + sqrt(fM_t*fM_t + fP_CM*fP_CM) ) / fM_t); }
  /// Calculate the momentum of the ejectile
  Double_t GetP_ej(Double_t theta_lab);
  /// Calculate the momentum of the recoil
  Double_t GetP_rec(Double_t theta_lab);
  /// Calculate the kinetic energy of the ejectile
  Double_t GetT_ej(Double_t& p_ej) { return sqrt(p_ej*p_ej + fM_ej*fM_ej ) - fM_ej; }
  /// Calculate the kinetic energy of the recoil
  Double_t GetT_rec(Double_t& p_rec) { return sqrt(p_rec*p_rec + fM_rec*fM_rec ) - fM_rec; }
  /// Calculate the center of momentum scattering angle of the ejectile (in degrees)
  Double_t GetThetaCM_ej(Double_t& p_ej, Double_t theta_lab);
  /// Calculate the center of momentum scattering angle of the recoil (in degrees)
  Double_t GetThetaCM_rec(Double_t theta_lab) { return 180*(TMath::Pi() - 2*theta_lab) / TMath::Pi(); }
  /// Calculate the maximum scattering angle of the ejectile
  Double_t GetThetaMax_ej() const;
  /// Calculate the maximum scattering angle of the recoil
  Double_t GetThetaMax_rec() const;


private:
  void Init(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);

};

} // namespace sonik



#endif
