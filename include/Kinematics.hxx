//////////////////////////////////////////////////////
/// \file Kinematics.hxx                           ///
/// \author D. Connolly                            ///
/// \brief Class to claculate kinematics for SONIK ///
//////////////////////////////////////////////////////
#ifndef HAVE_KINEMATICS_HXX
#define HAVE_KINEMATICS_HXX
#ifndef __MAKECINT__
#include <iostream>
#endif

#include "TH1.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TObject.h"

#include "Constants.hxx"

class Constants;

namespace sonik {

class Kinematics : public TObject {
public:
    /// Contains relativistic kinematics data for a scattering reaction of a pair of non-identical particles.
  struct Scatter_t {
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
    Double_t fp_CM;
    /// Rapidity of the two-particle system.
    Double_t fRapidity;
    /// Scattering angle of the ejectile in the center of momentum frame.
    Double_t fThetaCM_ej[13];
    /// Scattering angle of the (undetected) recoil in the center of momentum frame in degrees.
    Double_t fThetaCM_rec[13];
    /// Scattering angle of the detected recoil in the center of momentum frame in degrees.
    Double_t fThetaCM_rec_det[13];
    /// Momentum of the ejectile in the lab frame.
    Double_t fp_ej[13];
    /// Momentum of the recoil in the lab frame.
    Double_t fp_rec[13];
    /// Kinetic energy of the ejectile in the lab frame in MeV.
    Double_t fT_ej[13];
    /// Kinetic energy of the recoil in the lab frame in MeV.
    Double_t fT_rec[13];
    /// Laboratory scattering angles for all observation points (in degrees)
    Double_t fThetaLab[13];
  };

  static const Double_t ThetaLab[13];

public:
  Kinematics();
  Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b, const char* fname);


public: // public member functions
  /// Calculate the fraction of the speed of light of the center of momentum frame
  Double_t BetaCM(Double_t rapidity) { return tanh(rapidity); }
  /// Calculate the speed of beam ions (as a fraction of the speed of light) in the lab frame
  Double_t BetaLab(Double_t gamma_lab) { return sqrt(1 - 1 / pow(gamma_lab,2) ); }
  /// Calculate the center of mass energy of the two-particle system
  Double_t E_CM(Double_t S, Double_t m_b, Double_t m_t) { return sqrt(S) - (m_b + m_t); }
  /// Calculate the Lorentz factor of the center of momentum frame
  Double_t GammaCM(Double_t rapidity) { return cosh(rapidity); }
  /// Calculate the Lorentz factor of the beam ions in the lab frame
  Double_t GammaLab(Double_t m_b, Double_t T_b) { return T_b / m_b + 1; }
  /// Calculate the invariant mass of the two-particle system
  Double_t InvariantMass(Double_t m_b, Double_t m_t, Double_t T_b) { return 2*m_t*T_b + pow(m_b + m_t,2); }
  /// Calculate the mass of nucleus in MeV / \f$c^2\f$
  Double_t CalcMass(Int_t A, Double_t Delta) { return A*dragon::Constants::AMU() + Delta; }
  /// Calculate the maximum scattering angle of the ejectile
  Double_t MaxTheta_ej(Double_t p_CM, Double_t rapidity, Double_t m_ej) { return asin(p_CM / (m_ej*rapidity)); }
  /// Calculate the maximum scattering angle of the recoil
  Double_t MaxTheta_rec(Double_t p_CM, Double_t rapidity, Double_t m_rec) { return asin(p_CM / (m_rec*rapidity)); }
  /// Calculate the momentum of the center of momentum frame
  Double_t p_CM(Double_t m_b, Double_t m_t, Double_t S);
  /// Calculate the rapidity of the two-particle system
  Double_t Rapidity(Double_t p_CM, Double_t m_b) { return log( (p_CM + sqrt(m_b*m_b + p_CM*p_CM)) / m_b); }
  /// Calculate the momentum of the ejectile
  Double_t p_ej(Double_t p_CM, Double_t rapidity, Double_t theta_Lab, Double_t m_ej);
  /// Calculate the momentum of the recoil
  Double_t p_rec(Double_t p_CM, Double_t rapidity, Double_t theta_Lab, Double_t m_rec);
  /// Calculate the kinetic energy of the ejectile
  Double_t T_ej(Double_t p_ej, Double_t m_ej) { return sqrt(p_ej*p_ej + m_ej*m_ej ) - m_ej; }
  /// Calculate the kinetic energy of the recoil
  Double_t T_rec(Double_t p_rec, Double_t m_rec) { return sqrt(p_rec*p_rec + m_rec*m_rec ) - m_rec; }
  /// Calculate the center of momentum scattering angle of the ejectile (in degrees)
  Double_t ThetaCM_ej(Double_t p_CM, Double_t p_ej, Double_t theta_Lab) { return 180*asin( sin(theta_Lab)*p_ej / p_CM) / ( 2*TMath::Pi() ); }
  /// Calculate the center of momentum scattering angle of the (undetected) recoil (in degrees)
  Double_t ThetaCM_rec(Double_t theta_Lab) { return 180*(TMath::Pi() - theta_Lab) / ( 2*TMath::Pi() ); }
  /// Calculate the center of momentum scattering angle of the detected recoil (if applicable)
  Double_t ThetaCM_rec_det(Double_t p_CM, Double_t p_rec, Double_t theta_Lab) { return 180*asin( sin(theta_Lab)*p_rec / p_CM) / ( 2*TMath::Pi() ); }
  Scatter_t* GetScatterData(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);
  void FillTree(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b);

public:
  TTree fScatterTree;
  TFile* fFile;

private:
  Scatter_t *fScatterBranchAddr;

private:
  ClassDef(sonik::Kinematics,1)
};


}



#endif
