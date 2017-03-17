//////////////////////////////////////////////////////
/// \file Kinematics.hxx                           ///
/// \author D. Connolly                            ///
/// \brief Class to claculate kinematics for SONIK ///
//////////////////////////////////////////////////////
#ifndef HAVE_KINEMATICS_HXX
#define HAVE_KINEMATICS_HXX
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

class Kinematics {//: public TObject {
public:
  static const Double_t ThetaLab[13];


public:
  Kinematics();
  Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);

public: // public member functions
  /// Calculate the fraction of the speed of light of the center of momentum frame
  Double_t GetBetaCM() const { return tanh(fRapidity); }
  /// Calculate the speed of beam ions (as a fraction of the speed of light) in the lab frame
  Double_t GetBetaLab() const { return sqrt(1 - 1 / pow(fGammaLab,2) ); }
  /// Calculate the center of mass energy of the two-particle system
  Double_t GetE_CM() const { return sqrt(fInvariantM) - (fm_b + fm_t); }
  /// Calculate the Lorentz factor of the center of momentum frame
  Double_t GetGammaCM() const { return cosh(fRapidity); }
  /// Calculate the Lorentz factor of the beam ions in the lab frame
  Double_t GetGammaLab() const { return fT_b / fm_b + 1; }
  /// Calculate the invariant mass of the two-particle system
  Double_t GetInvariantMass() const { return 2*fm_t*fT_b + pow(fm_b + fm_t,2); }
  /// Calculate the mass of nucleus in MeV / \f$c^2\f$
  Double_t CalcMass(Int_t A, Double_t Delta) { return A*sonik::Constants::AMU() + Delta; }
  /// Calculate the maximum scattering angle of the ejectile
  Double_t GetMaxTheta_ej() const { return asin(fP_CM / (fm_ej*fRapidity)); }
  /// Calculate the maximum scattering angle of the recoil
  Double_t GetMaxTheta_rec() const { return asin(fP_CM / (fm_rec*fRapidity)); }
  /// Calculate the momentum of the center of momentum frame
  Double_t GetP_CM() const;
  /// Calculate the rapidity of the two-particle system
  Double_t GetRapidity() const { return log( (fP_CM + sqrt(fm_b*fm_b + fP_CM*fP_CM)) / fm_b); }
  /// Calculate the momentum of the ejectile
  Double_t GetP_ej(Double_t theta_lab);
  /// Calculate the momentum of the recoil
  Double_t GetP_rec(Double_t theta_lab);
  /// Calculate the kinetic energy of the ejectile
  Double_t GetT_ej(Double_t& p_ej) { return sqrt(p_ej*p_ej + fm_ej*fm_ej ) - fm_ej; }
  /// Calculate the kinetic energy of the recoil
  Double_t GetT_rec(Double_t& p_rec) { return sqrt(p_rec*p_rec + fm_rec*fm_rec ) - fm_rec; }
  /// Calculate the center of momentum scattering angle of the ejectile (in degrees)
  Double_t GetThetaCM_ej(Double_t& p_ej, Double_t theta_Lab) { return 180*asin( sin(theta_Lab*TMath::Pi() / 180)*p_ej / fP_CM ) / TMath::Pi(); }
  /// Calculate the center of momentum scattering angle of the (undetected) recoil (in degrees)
  Double_t GetThetaCM_rec(Double_t theta_Lab) { return 180*(TMath::Pi() - TMath::Pi()*theta_Lab / 180 ) / TMath::Pi(); }
  /// Calculate the center of momentum scattering angle of the detected recoil (if applicable)
  Double_t GetThetaCM_rec_det(Double_t& p_rec, Double_t theta_Lab) { return 180*asin( sin(TMath::Pi()*theta_Lab / 180)*p_rec / fP_CM ) / TMath::Pi(); }

private:
  void Init(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);

private:
  /// Atomic number of the beam ion.
  Int_t fA_b;
  /// Atomic number of the target ion.
  Int_t fA_t;
  /// Proton number of the beam ion.
  Int_t fZ_b;
  /// Proton number of the beam ion.
  Int_t fZ_t;
  /// Kinetic energy of the beam ion MeV.
  Double_t fT_b;
  /// Speed squared of the beam ion MeV / u.
  Double_t fVelSquared;
  /// Mass deficit of the beam ion MeV.
  Double_t fDelta_b;
  /// Mass deficit of the target ion MeV.
  Double_t fDelta_t;
  /// Mass of the beam ion in MeV / \f$ c^{2} \f$.
  Double_t fm_b;
  /// Mass of the target ion in MeV / \f$ c^{2} \f$.
  Double_t fm_t;
  /// Mass of the ejectile in MeV / \f$ c^{2} \f$.
  Double_t fm_ej;
  /// Mass of the recoil in MeV / \f$ c^{2} \f$.
  Double_t fm_rec;
  /// Lorentz factor of the beam ions in the lab frame.
  Double_t fGammaLab;
  /// Speed of the beam ions (as a fraction of the speed of light \f$\beta\f$) in the lab frame.
  Double_t fBetaLab;
  /// Lorentz factor \f$\gamma\f$ of the center of momentum frame.
  Double_t fGammaCM;
  /// Speed of the center of momentum frame as a fraction of the speed of light \f$\beta\f$.
  Double_t fBetaCM;
  /// Invariant mass of the two-particle system MeV / \f$ c^{2} \f$.
  Double_t fInvariantM;
  /// Momentum of the center of momentum frame
  Double_t fP_CM;
  /// Rapidity of the two-particle system.
  Double_t fRapidity;
  /// Laboratory scattering angles for all observation points (in degrees)
  Double_t fThetaLab[13];
  /// Scattering angle of the ejectile in the center of momentum frame.
  Double_t fThetaCM_ej[13];
  /// Scattering angle of the (undetected) recoil in the center of momentum frame in degrees.
  Double_t fThetaCM_rec[13];
  /// Scattering angle of the detected recoil in the center of momentum frame in degrees.
  Double_t fThetaCM_rec_det[13];
  /// Momentum of the ejectile in the lab frame.
  Double_t fP_ej[13];
  /// Momentum of the recoil in the lab frame.
  Double_t fP_rec[13];
  /// Kinetic energy of the ejectile in the lab frame in MeV.
  Double_t fT_ej[13];
  /// Kinetic energy of the recoil in the lab frame in MeV.
  Double_t fT_rec[13];

};

}



#endif
