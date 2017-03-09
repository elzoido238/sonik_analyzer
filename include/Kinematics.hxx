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
#include "TObject.h"

#include "TAtomicMass.h"
#include "Constants.hxx"

class TAtomicmassTable;

namespace sonik {

class Kinematics : public TObject {
public:
  Kinematics();
  Kinematics(Int_t Z_t, Int_t A_t, Int_t Z_b, Int_t A_b, Double_t T_b, Double_t theta);
  Kinematics(const char Symbol_t, const char Symbol_b, Double_t T_b);


public: // Data members
  // contains particle data for a scattering reaction
  struct Rxn_t {         // data structure for elastic scattering reaction
    Double_t fT_b;           // of non-identical particles.
    Double_t fVelSquare;
    Double_t fTheta_Lab;
    char fSymbol_b[3];
    char fSymbol_t[3];
    Int_t fA_b;
    Int_t fA_t;
    Int_t fZ_b;
    Int_t fZ_t;
    Double_t fDelta_b;
    Double_t fDelta_t;
    Double_t fm_b;
    Double_t fm_t;
    Double_t fm_ej;
    Double_t fm_rec;
    Double_t fGammaLab;
    Double_t fBetaLab;
    Double_t fGammaCM;
    Double_t fBetaCM;
    Double_t fInvariantM;
    Double_t fRapidity;
    Double_t fThetaCM_ej;
    Double_t fThetaCM_rec;
    Double_t fp_ej;
    Double_t fp_rec;
    Double_t fT_ej;
    Double_t fT_rec;
  };


public: // public member functions
  Double_t BetaCM(Double_t rapidity) { return tanh(rapidity); }
  Double_t BetaLab(Double_t gamma_lab) { return sqrt(1 - 1 / pow(gamma_lab,2) ); }
  Double_t E_CM(Double_t S, Double_t m_b, Double_t m_t) { return sqrt(S) - (m_b + m_t); }
  Double_t GammaCM(Double_t rapidity) { return cosh(rapidity); }
  Double_t GammaLab(Double_t m_b, Double_t T_b) { return E_b / M_b + 1; }
  Double_t InvariantMass(Double_t m_b, Double_t m_t, Double_t T_b) { return 2*m_t*T_b + pow(m_b + m_t,2); }
  Double_t MaxTheta_ej(Double_t p_CM, Double_t rapidity, Double_t m_ej) { return asin(p_CM / (m_ej*rapidity)); }
  Double_t MaxTheta_rec(Double_t p_CM, Double_t rapidity, Double_t m_rec) { return asin(p_CM / (m_rec*rapidity)); }
  Double_t p_CM(Double_t m_b, Double_t m_t, Double_t S);
  Double_t Rapidity(Double_t p_CM, Double_t m_t) { return log( (p_CM + sqrt(m_b*m_b + p_CM*p_CM)) / m_b); }
  Double_t p_ej(Double_t p_CM, Double_t rapidity, Double_t theta_CM, Double_t m_ej);
  Double_t p_rec(Double_t p_CM, Double_t rapidity, Double_t theta_CM, Double_t m_rec);
  Double_t T_ej(Double_t p_ej, Double_t m_ej) { return sqrt(p_ej*p_ej + m_ej*m_ej ) - m_ej; }
  Double_t T_rec(Double_t p_rec, Double_t m_rec) { return sqrt(p_rec*p_rec + m_rec*m_rec ) - m_rec; }
  Double_t ThetaCM_ej(Double_t p_CM, Double_t p_ej, Double_t theta_Lab) { return TMath::2*Pi()*asin( sin(theta_rad)*p_ej / p_CM); }
  Double_t ThetaCM_rec(Double_t theta_Lab) { return TMath::Pi() - theta_ej; }


private: // private data members
  TAtomicMassTable *fAMT;              //!
  TAtomicMassTable::MassExcess_t *fME; //!
  TAtomicMassTable::Nucleus_t *fNuc;   //!

private: // private member functions
  Double_t CalcMassEnergy(Int_t A, Double_t Delta){ return A*Constants.AMU() / 1.0e3 + Delta; }
  Double_t GetExcess(Int_t Z, Int_t A);
  void CalcVars();
  void reset();
  void SetVars();


private:
  ClassDef(sonik::Kinematics,1)
};


}



#endif
