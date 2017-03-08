//////////////////////////////////////////////////////
/// \file Kinematics.cxx                           ///
/// \author D. Connolly                            ///
/// \brief implementats class Kinematics           ///
//////////////////////////////////////////////////////

#include "Kinematics.hxx"
#include "TAtomicMass.h"
#include "Constants.hxx"

void sonik::Kinematics::Kinematics();
{
  TAtomicMassTable *TAMT = new TAtomicMassTable();
  reset();
}

void sonik::Kinematics::Kinematics(Int_t Z_b, Int_t A_b, Int_t Z_t, Int_t A_t, Double_t E_b, Double_t theta)
{
  TAtomicMassTable *TAMT = new TAtomicMassTable();
  TAtomicMassTable::MassExcess_t *Delta_b = TAMT.MassExcess_t;
  TAtomicMassTable::MassExcess_t *Delta_t = TAMT.MassExcess_t;
  Delta_b = Delta_b->GetMassExcess(Z_b, A_b);
  Delta_t = Delta_t->GetMassExcess(Z_t, A_t);
  RxnPair.fDelta_b = Delta_b.fValue;
  RxnPair.fDelta_t = Delta_t.fValue;
}

void sonik::Kinematics::Kinematics(const char Symbol_b, const char Symbol_t, Double_t E_b)
{

}

void sonik::Kinematics::reset()
{
  RxnPair.fSymbol_b   = 0;
  RxnPair.fSymbol_t   = 0;
  RxnPair.fA_b        = 0;
  RxnPair.fA_t        = 0;
  RxnPair.fZ_b        = 0;
  RxnPair.fZ_t        = 0;
  RxnPair.fDelta_b    = 0;
  RxnPair.fDelta_t    = 0;
  RxnPair.fm_b        = 0;
  RxnPair.fm_t        = 0;
  RxnPair.fej_b       = 0;
  RxnPair.frec_t      = 0;
  RxnPair.fE_b        = 0;
  RxnPair.fGammaLab   = 0;
  RxnPair.fBetaLab    = 0;
  RxnPair.fminvariant = 0;
  RxnPair.fRapidity   = 0;
  RxnPair.fTheta_CM   = 0;
  RxnPair.fT_ej       = 0;
  RxnPair.fT_rec      = 0;
}
