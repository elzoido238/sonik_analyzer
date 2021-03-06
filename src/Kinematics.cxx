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
  return sqrt((pow(fInvariantM - fM_b*fM_b - fM_t*fM_t,2) - 4*fM_t*fM_t*fM_b*fM_b) / (4*fInvariantM) );
}


Double_t sonik::Kinematics::GetP_ej(Double_t theta_lab)
{
  return ( cos(theta_lab)*sinh(fRapidity)*sqrt(pow(fM_ej,2) + pow(fP_CM,2)) + cosh(fRapidity)*
           sqrt(pow(fP_CM,2) - pow(fM_ej*sin(theta_lab)*sinh(fRapidity),2) ) ) /
    ( 1 + pow(sin(theta_lab)*sinh(fRapidity),2) );
}


Double_t sonik::Kinematics::GetP_rec(Double_t theta_lab)
{
  return ( cos(theta_lab)*sinh(fRapidity)*sqrt(fM_rec*fM_rec + fP_CM*fP_CM) + cosh(fRapidity)*
           sqrt(fP_CM*fP_CM - pow(fM_rec*sin(theta_lab)*sinh(fRapidity),2) ) ) /
    ( 1 + pow(sin(theta_lab)*sinh(fRapidity),2) );
}


Double_t sonik::Kinematics::GetThetaCM_ej(Double_t& p_ej, Double_t theta_lab)
{
  return 180.0*acos( ( p_ej*cos(theta_lab) - sinh(fRapidity)*sqrt(pow(fP_CM,2) + pow(fM_ej,2)) ) /
                     ( fP_CM*cosh(fRapidity) ) ) / TMath::Pi();
}


Double_t sonik::Kinematics::GetThetaMax_ej() const
{
  Double_t temp = 180.0*asin(fP_CM / ( fM_ej*sinh(fRapidity) ) ) / TMath::Pi();
  if (isnan(temp) && fM_b > fM_t ) return 90.0;
  else if (isnan(temp) && fM_b < fM_t ) return 180.0;
  else return temp;
}


Double_t sonik::Kinematics::GetThetaMax_rec() const
{
  Double_t temp = 180.0*asin( fP_CM / ( fM_rec*sinh(fRapidity) ) ) / TMath::Pi();
  if (isnan(temp)) return 90.0;
  else return temp;
}


void sonik::Kinematics::Init(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  const char* amefile = "${SONIKSYS}/include/mass.mas12";

  TAtomicMassTable *mt = new TAtomicMassTable(gSystem->ExpandPathName(amefile));

  fT_b        = T_b;
  fA_b        = A_b;
  fA_t        = A_t;
  fZ_b        = Z_b;
  fZ_t        = Z_t;
  fDelta_b    = mt->AtomicMassExcess(Z_b, A_b) / 1.0e3;
  fDelta_t    = mt->AtomicMassExcess(Z_t, A_t) / 1.0e3;
  fM_b        = CalcMass(A_b, fDelta_b);
  fM_t        = CalcMass(A_t, fDelta_t);
  fVelSquared = fT_b / (fM_b / sonik::Constants::AMU() );

  fInvariantM  = GetInvariantMass();
  fE_CM        = GetE_CM();
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
  else if( fM_t < fM_b ){
    fM_ej  = fM_t;
    fM_rec = fM_b;
  }
  else{
    fM_ej  = fM_b;
    fM_rec = fM_t;
  }

  Double_t theta_rad;

  for ( Int_t i = 0; i < 13; ++i){
    theta_rad           = TMath::Pi()*ThetaLab[i] / 180.0;
    fP_ej[i]            = GetP_ej(theta_rad);
    fP_rec[i]           = GetP_rec(theta_rad);
    fT_ej[i]            = GetT_ej(fP_ej[i]);
    fT_rec[i]           = GetT_rec(fP_rec[i]);
    fThetaMax_ej        = GetThetaMax_ej();
    fThetaMax_rec       = GetThetaMax_rec();

    if (ThetaLab[i] <= fThetaMax_ej){
      fThetaCM_ej[i] = GetThetaCM_ej(fP_ej[i], theta_rad);
    }
    else fThetaCM_ej[i] = 0;

    if (ThetaLab[i] <= fThetaMax_rec){
      fThetaCM_rec[i] = GetThetaCM_rec(theta_rad);
    }
    else fThetaCM_rec[i] = 0;

  }

}
