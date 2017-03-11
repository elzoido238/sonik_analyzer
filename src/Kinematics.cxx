//////////////////////////////////////////////////////
/// \file Kinematics.cxx                           ///
/// \author D. Connolly                            ///
/// \brief implementats class Kinematics           ///
//////////////////////////////////////////////////////

#include "Kinematics.hxx"

#include "TAtomicMass.h"

#include "TMath.h"

class TAtomicMassTable;

const Double_t sonik::Kinematics::ThetaLab[13] = {22.5, 25., 30., 35., 40., 45., 55., 65., 75., 90., 120., 135.};

sonik::Kinematics::Kinematics()
{
  fScatterTree.SetMarkerStyle(20);
  fScatterTree.Branch("scat", "sonik::Kinematics::Scatter_t", &fScatterBranchAddr);
}


sonik::Kinematics::Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b, const char* fname)
{
  fScatterTree.SetMarkerStyle(20);
  fScatterTree.Branch("scatter", "sonik::Kinematics::Scatter_t", &fScatterBranchAddr);

  fFile = TFile::Open(fname);

  FillTree(Z_b, A_b, Z_t, A_t, T_b);

  fFile->Write();
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


void sonik::Kinematics::FillTree(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{

  sonik::Kinematics* KIN = new sonik::Kinematics();
  Scatter_t* scat = KIN->GetScatterData(Z_b, A_b, Z_t, A_t, T_b);

  fScatterTree.Fill();

}

sonik::Kinematics::Scatter_t* sonik::Kinematics::GetScatterData(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t T_b)
{
  TAtomicMassTable AMT;
  sonik::Kinematics::Scatter_t *scat;

  fScatterBranchAddr = scat;

  scat->fT_b        = T_b;
  scat->fVelSquared = T_b / scat->fm_b;
  scat->fA_b        = A_b;
  scat->fA_t        = A_t;
  scat->fZ_b        = Z_b;
  scat->fZ_t        = Z_t;
  scat->fDelta_b    = AMT.AtomicMassExcess(Z_b, A_b);
  scat->fDelta_t    = AMT.AtomicMassExcess(Z_t, A_t);
  scat->fm_b        = CalcMass(A_b, scat->fDelta_b);
  scat->fm_t        = CalcMass(A_t, scat->fDelta_t);


  if ( A_t == A_b && Z_t == Z_b ){
    std::cout << "Error: Mott scating is not supported yet! \n";
    exit (EXIT_FAILURE);
  }
  else if( scat->fm_t < scat->fm_b ){
    scat->fm_ej  = scat->fm_t;
    scat->fm_rec = scat->fm_b;
  }
  else{
    scat->fm_ej  = scat->fm_b;
    scat->fm_rec = scat->fm_t;
  }

  scat->fGammaLab    = GammaLab(scat->fm_b, T_b);
  scat->fBetaLab     = BetaLab(scat->fGammaLab);
  scat->fInvariantM  = InvariantMass(scat->fm_b, scat->fm_t, T_b);
  scat->fp_CM        = p_CM(scat->fm_b, scat->fm_t, scat->fInvariantM);
  scat->fRapidity    = Rapidity(scat->fp_CM, scat->fm_t);
  scat->fGammaCM     = GammaCM(scat->fRapidity);
  scat->fBetaCM      = BetaLab(scat->fRapidity);

  for ( Int_t i = 0; i < 13; ++i){
    scat->fThetaLab[i]        = ThetaLab[i];
    scat->fp_ej[i]            = p_ej(scat->fp_CM, scat->fRapidity, scat->fThetaLab[i], scat->fm_ej);
    scat->fp_rec[i]           = p_rec(scat->fp_CM, scat->fRapidity, scat->fThetaLab[i], scat->fm_rec);
    scat->fT_ej[i]            = T_ej(scat->fp_ej[i], scat->fm_ej);
    scat->fT_rec[i]           = T_rec(scat->fp_rec[i], scat->fm_rec);
    scat->fThetaCM_ej[i]      = ThetaCM_ej(scat->fp_CM, scat->fp_ej[i], scat->fThetaLab[i]);
    scat->fThetaCM_rec[i]     = ThetaCM_rec(scat->fThetaLab[i]);
    scat->fThetaCM_rec_det[i] = ThetaCM_rec_det(scat->fp_CM, scat->fp_rec[i], scat->fThetaLab[i]);
  }

  return scat;
}
