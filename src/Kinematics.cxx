//////////////////////////////////////////////////////
/// \file Kinematics.cxx                           ///
/// \author D. Connolly                            ///
/// \brief implementats class Kinematics           ///
//////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <sstream>

#include <TMath.h>
#include <TSystem.h>

#include "TAtomicMass.hxx"
#include "Kinematics.hxx"

//ClassImp(sonik::Kinematics);

class TAtomicMassTable;


const Double_t sonik::Kinematics::ThetaLab[13] = {22.5, 25., 30., 35., 40., 45., 55., 60., 65., 75., 90., 120., 135.};


sonik::Kinematics::Kinematics()
{
  Init(0,0,0,0,0.0);
}


sonik::Kinematics::Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  Init(Z_b, A_b, Z_t, A_t, T_b);
}


Double_t sonik::Kinematics::GetP_CM() const
{
  return sqrt((pow(fInvariantM - fm_b*fm_b - fm_t*fm_t,2) - 4*fm_t*fm_t*fm_b*fm_b) / (4*fInvariantM) );
}


Double_t sonik::Kinematics::GetP_ej(Double_t theta_lab)
{
  return ( sqrt(fm_ej*fm_ej + fP_CM*fP_CM)*cos(theta_lab)*sinh(fRapidity)+cosh(fRapidity)*
           sqrt(fP_CM*fP_CM - pow(fm_ej*sin(theta_lab)*sinh(fRapidity),2) ) ) /
    ( 1 + pow(sin(theta_lab)*sinh(fRapidity),2) );
}


Double_t sonik::Kinematics::GetP_rec(Double_t theta_lab)
{
  return ( sqrt(fm_rec*fm_rec + fP_CM*fP_CM)*cos(theta_lab)*sinh(fRapidity)+cosh(fRapidity)*
           sqrt(fP_CM*fP_CM - pow(fm_rec*sin(theta_lab)*sinh(fRapidity),2) ) ) /
    ( 1 + pow(sin(theta_lab)*sinh(fRapidity),2) );
}


void sonik::Kinematics::Init(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  TAtomicMassTable mt;

  fT_b        = T_b;
  fA_b        = A_b;
  fA_t        = A_t;
  fZ_b        = Z_b;
  fZ_t        = Z_t;
  fDelta_b    = mt.AtomicMassExcess(Z_b, A_b) / 1.0e3;
  fDelta_t    = mt.AtomicMassExcess(Z_t, A_t) / 1.0e3;
  fm_b        = CalcMass(A_b, fDelta_b);
  fm_t        = CalcMass(A_t, fDelta_t);
  fVelSquared = fT_b / fm_b;

  fInvariantM  = GetInvariantMass();
  fGammaLab    = GetGammaLab();
  fBetaLab     = GetBetaLab();
  fP_CM        = GetP_CM();
  fRapidity    = GetRapidity();
  fGammaCM     = GetGammaCM();
  fBetaCM      = GetBetaLab();


  if ( A_t == A_b && Z_t == Z_b && A_t != 0){
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

  Double_t temp;

  for ( Int_t i = 0; i < 13; ++i){
    temp                = ThetaLab[i];
    fP_ej[i]            = GetP_ej(temp);
    fP_rec[i]           = GetP_rec(temp);
    fT_ej[i]            = GetT_ej(fP_ej[i]);
    fT_rec[i]           = GetT_rec(fP_rec[i]);
    fThetaCM_ej[i]      = GetThetaCM_ej(fP_ej[i], temp);
    fThetaCM_rec[i]     = GetThetaCM_rec(temp);
    fThetaCM_rec_det[i] = GetThetaCM_rec_det(fP_rec[i], temp);
  }

}
