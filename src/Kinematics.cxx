//////////////////////////////////////////////////////
/// \file Kinematics.cxx                           ///
/// \author D. Connolly                            ///
/// \brief implementats class Kinematics           ///
//////////////////////////////////////////////////////

#include "Kinematics.hxx"

#include "TAtomicMass.h"

#include "TMath.h"

class TAtomicMassTable;

const Double_t sonik::Kinematics::ThetaLab[12] = {22.5, 25., 30., 35., 40., 45., 55., 65., 75., 90., 120., 135.};

sonik::Kinematics::Kinematics()
{
  Init(0,0,0,0,0.0);
}


sonik::Kinematics::Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  Init(Z_b, A_b, Z_t, A_t, T_b);
}


Double_t sonik::Kinematics::p_CM(Double_t m_b, Double_t m_t, Double_t S)
{
  return sqrt((pow(S - m_b*m_b - m_t*m_t,2) - 4*m_t*m_t*m_b*m_b) / (4*S) );
}


Double_t sonik::Kinematics::p_ej(Double_t p_CM, Double_t rapidity, Double_t theta_Lab, Double_t m_ej)
{
  return ( sqrt(m_ej*m_ej + p_CM*p_CM)*cos(theta_Lab)*sinh(rapidity)+cosh(rapidity)*
           sqrt(p_CM*p_CM - pow(m_ej*sin(theta_Lab)*sinh(rapidity),2) ) ) /
    ( 1 + pow(sin(theta_Lab)*sinh(rapidity),2) );
}


Double_t sonik::Kinematics::p_rec(Double_t p_CM, Double_t rapidity, Double_t theta_Lab, Double_t m_rec)
{
  return ( sqrt(m_rec*m_rec + p_CM*p_CM)*cos(theta_Lab)*sinh(rapidity)+cosh(rapidity)*
           sqrt(p_CM*p_CM - pow(m_rec*sin(theta_Lab)*sinh(rapidity),2) ) ) /
    ( 1 + pow(sin(theta_Lab)*sinh(rapidity),2) );
}


void sonik::Kinematics::Init(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{

  TAtomicMassTable AMT;

  fT_b        = T_b;
  fVelSquared = T_b / fm_b;
  fA_b        = A_b;
  fA_t        = A_t;
  fZ_b        = Z_b;
  fZ_t        = Z_t;
  fDelta_b    = AMT.AtomicMassExcess(Z_b, A_b) / 1.0e3;
  fDelta_t    = AMT.AtomicMassExcess(Z_t, A_t) / 1.0e3;
  fm_b        = CalcMass(A_b, fDelta_b);
  fm_t        = CalcMass(A_t, fDelta_t);


  if ( A_t == A_b && Z_t == Z_b ){
    std::cout << "Error: Mott scating is not supported yet! \n";
    exit (EXIT_FAILURE);
  }
  else if( fm_t < fm_b ){
    fm_ej  = fm_t;
    fm_rec = fm_b;
  }
  else{
    fm_ej  = fm_b;
    fm_rec = fm_t;
  }

  fGammaLab    = GammaLab(fm_b, T_b);
  fBetaLab     = BetaLab(fGammaLab);
  fInvariantM  = InvariantMass(fm_b, fm_t, T_b);
  fp_CM        = p_CM(fm_b, fm_t, fInvariantM);
  fRapidity    = Rapidity(fp_CM, fm_t);
  fGammaCM     = GammaCM(fRapidity);
  fBetaCM      = BetaLab(fRapidity);

  for ( Int_t i = 0; i < 12; ++i){
    fp_ej[i]            = p_ej(fp_CM, fRapidity, ThetaLab[i], fm_ej);
    fp_rec[i]           = p_rec(fp_CM, fRapidity, ThetaLab[i], fm_rec);
    fT_ej[i]            = T_ej(fp_ej[i], fm_ej);
    fT_rec[i]           = T_rec(fp_rec[i], fm_rec);
    fThetaCM_ej[i]      = ThetaCM_ej(fp_CM, fp_ej[i], ThetaLab[i]);
    fThetaCM_rec[i]     = ThetaCM_rec(ThetaLab[i]);
    fThetaCM_rec_det[i] = ThetaCM_rec_det(fp_CM, fp_rec[i], ThetaLab[i]);
  }

}
