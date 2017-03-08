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
  Kinematics(Int_t Z_t, Int_t A_t, Int_t Z_b, Int_t A_b, Double_t E_b, Double_t theta);
  Kinematics(const char Symbol_t, const char Symbol_b, Double_t E_b);
  void reset();

public: // Data members
  struct RxnPair {
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
    Double_t fE_b;
    Double_t fGammaLab;
    Double_t fBetaLab;
    Double_t fminvariant;
    Double_t fRapidity;
    Double_t fTheta_CM;
    Double_t fT_ej;
    Double_t fT_rec;
  };

public: // methods
  Double_t GammaLab(Double_t m_b, Double_t E_b);
  Double_t BetaLab(Double_t gamma_lab);
  Double_t E_CM(Double_t S, Double_t m_b, Double_t m_t);
  Double_t P_CM(Double_t m_b, Double_t m_t, Double_t S);
  Double_t Rapidity(Double_t p_CM, Double_t m_t);
  Double_t T_ej(Double_t p_CM, Double_t rapidity, Double_t theta_CM, Double_t m_ej);
  Double_t T_rec(Double_t p_CM, Double_t rapidity, Double_t theta_CM, Double_t m_rec);
  Double_t Theta_ej();
  Double_t Theta_rec();
  Double_t MaxTheta_ej(Double_t p_CM, Double_t rapidity, Double_t m_ej);
  Double_t MaxTheta_rec(Double_t p_CM, Double_t rapidity, Double_t m_rec);

private:
  Double_t GetExcess(Int_t Z, Int_t A);
  Double_t CalcMassEnergy(Int_t A, Double_t Delta);
  Double_t ClacS(Double_t m_b, Double_t m_t, Double_t E_b);
  Double_t CalcXi0(Double_t m_b, Double_t m_t);
  Double_t CalcXi1(Double_t m_b, Double_t m_t, Double_t E_b);

private:
  ClassDef(sonik::Kinematics,1)
};


}



#endif
