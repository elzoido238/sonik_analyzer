//////////////////////////////////////////////////////
/// \file Kinematics.cxx                           ///
/// \author D. Connolly                            ///
/// \brief implementats class Kinematics           ///
//////////////////////////////////////////////////////

#include "Kinematics.hxx"

#include "TAtomicMass.h"
#include "Constants.hxx"
using namespace dragon;

#include "TMath.h"


void sonik::Kinematics::Kinematics();
{
  TAtomicMassTable *fAMT = new TAtomicMassTable();
  reset();
}


void sonik::Kinematics::Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b, Double_t theta_Lab)
{
  TAtomicMassTable *fAMT = new TAtomicMassTable();
  TAtomicMassTable::MassExcess_t *fME;
  SetVars(Z_b, A_b, Z_t, A_t, T_b);
}


void sonik::Kinematics::Kinematics(const char Symbol_b, const char Symbol_t, Double_t T_b)
{
  TAtomicMassTable *fAMT = new TAtomicMassTable();
  TAtomicMassTable::MassExcess_t *fME;
  TAtomicMassTable::Nucleus_t *fNuc;
  SetVars(Symbol_b, Symbol_t, T_b);
}

void sonik::Kinematics::reset()
{
  Rxn_t.fSymbol_b   = 0;
  Rxn_t.fSymbol_t   = 0;
  Rxn_t.fA_b        = 0;
  Rxn_t.fA_t        = 0;
  Rxn_t.fZ_b        = 0;
  Rxn_t.fZ_t        = 0;
  Rxn_t.fDelta_b    = 0;
  Rxn_t.fDelta_t    = 0;
  Rxn_t.fm_b        = 0;
  Rxn_t.fm_t        = 0;
  Rxn_t.fej_b       = 0;
  Rxn_t.frec_t      = 0;
  Rxn_t.fE_b        = 0;
  Rxn_t.fGammaLab   = 0;
  Rxn_t.fBetaLab    = 0;
  Rxn_t.fminvariant = 0;
  Rxn_t.fRapidity   = 0;
  Rxn_t.fTheta_CM   = 0;
  Rxn_t.fT_ej       = 0;
  Rxn_t.fT_rec      = 0;
}


Double_t p_CM(Double_t m_b, Double_t m_t, Double_t s)
{
  return sqrt((pow(s - m_b*m_b - m_t*m_t,2) - 4*m_t*m_t*m_b*m_b) / (4*s) );
}


Double_t p_ej(Double_t p_CM, Double_t rapidity, Double_t theta_rad, Double_t m_ej)
{
  return ( sqrt(m_ej*m_ej + p_CM*p_CM)*cos(theta_rad)*sinh(rapidity)+cosh(rapidity)*
           sqrt(p_CM*p_CM - pow(m_ej*sin(thet_rad)*sinh(rapidity),2) ) ) /
    ( 1 + pow(sin(theta_rad)*sinh(rapidity),2) );
}


Double_t p_rec(Double_t p_CM, Double_t rapidity, Double_t theta_CM, Double_t m_rec)
{
  return ( sqrt(m_rec*m_rec + p_CM*p_CM)*cos(theta_rad)*sinh(rapidity)+cosh(rapidity)*
           sqrt(p_CM*p_CM - pow(m_rec*sin(thet_rad)*sinh(rapidity),2) ) ) /
    ( 1 + pow(sin(theta_rad)*sinh(rapidity),2) );
}


Double_t GetExcess(Int_t Z, Int_t A)
{
  TAtomicMassTable::MassExcess_t *Delta;
  Delta = fAMT->GetMassExcess(Z, A);
  return Delta.fValue / 1.0e3;
}


Rxn_t* ScatteredVars(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b, Double_t theta_Lab)
{
  Rxn_t.fGammaLab    = GammaLab(Rxn_t.fm_b, T_b);
  Rxn_t.fBetaLab     = BetaLab(Rxn_t.fGamaLab);
  Rxn_t.fInvariantM  = InvariantMass(Rxn_t.fm_b, Rxn_t.fm_t, T_b);
  Rxn_t.fp_CM        = p_CM(Rxn_t.fm_b, Rxn_t.fm_t, Rxn_t.InvariantM);
  Rxn_t.fRapidity    = Rapidity(Rxn_t.fp_CM, Rxn_t.fm_t);
  Rxn_t.fGammaCM     = GammaLab(Rxn_t.fRapidity);
  Rxn_t.fBetaLab     = BetaLab(Rxn_t.fRapidity);
  Rxn_t.fp_ej        = p_ej(p_CM, rapidity, theta_CM, m_ej);
  Rxn_t.fp_rec       = p_rec(p_CM, rapidity, theta_CM, m_rec);
  Rxn_t.fThetaCM_ej  = ThetaCM_ej(p_CM, p_ej, theta_Lab);
  Rxn_t.fThetaCM_rec = ThetaCM_rec(theta_Lab);
}


void SetVars(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  Rxn_t.fT_b        = T_b;
  Rxn_t.fVelSquared = T_b / Rxn_t.fm_b;
  Rxn_t.fTheta_Lab  = theta_Lab;
  Rxn_t.fA_b        = A_b;
  Rxn_t.fA_t        = A_t;
  Rxn_t.fZ_b        = Z_b;
  Rxn_t.fZ_t        = Z_t;
  Rxn_t.fDelta_b    = GetExcess(Z_b, A_b);
  Rxn_t.fDelta_t    = GetExcess(Z_t, A_t);
  Rxn_t.fm_b        = MassEnergy(A_b, Rxn_t.fDelta_b);
  Rxn_t.fm_t        = MassEnergy(A_t, Rxn_t.fDelta_t);


  if ( A_t == A_b && Z_t == Z_b ){
    cout << "Error: Mott scattering is not supported yet! \n";
    exit (EXIT_FAILURE);
  }
  else if( Rxn_t.fm_t < Rxn_t.fm_b ){
    Rxn_t.fm_ej  = Rxn_t.fm_t;
    Rxn_t.fm_rec = Rxn_t.fm_b;
  }
  else{
    Rxn_t.fm_ej  = Rxn_t.fm_b;
    Rxn_t.fm_rec = Rxn_t.fm_t;
  }

}


void SetVars(const char Symbol_b, const char Symbol_t, Double_t T_b)
{
  Rxn_t.fT_b        = T_b;
  Rxn_t.fVelSquared = T_b / Rxn_t.fm_b;
  Rxn_t.fTheta_Lab  = theta_Lab;
  Rxn_t.fA_b        = A_b;
  Rxn_t.fA_t        = A_t;
  Rxn_t.fZ_b        = Z_b;
  Rxn_t.fZ_t        = Z_t;
  Rxn_t.fDelta_b    = ME->GetExcess(Symbol_b);
  Rxn_t.fDelta_t    = ME->GetExcess(Symbol_t);
  Rxn_t.fm_b        = MassEnergy(A_b, Rxn_t.fDelta_b);
  Rxn_t.fm_t        = MassEnergy(A_t, Rxn_t.fDelta_t);


  if ( A_t == A_b && Z_t == Z_b ){
    cout << "Error: Mott scattering is not supported yet! \n";
    exit (EXIT_FAILURE);
  }
  else if( Rxn_t.fm_t < Rxn_t.fm_b ){
    Rxn_t.fm_ej  = Rxn_t.fm_t;
    Rxn_t.fm_rec = Rxn_t.fm_b;
  }
  else{
    Rxn_t.fm_ej  = Rxn_t.fm_b;
    Rxn_t.fm_rec = Rxn_t.fm_t;
  }

}
