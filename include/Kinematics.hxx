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

class TAtomicMassTable;

namespace sonik {

class Kinematics : public TObject {
public:
  Kinematics();
  Kinematics(Int_t Z_t, Int_t A_t, Int_t Z_b, Int_t A_b, Double_t E_b);
  Kinematics(const char Symbol_t, const char Symbol_b, Double_t E_b);

public:
  CalcBeta_t();


private:
  Double_t GetExcess(Int_t Z, Int_t A);
  Double_t CalcMassEnergy(Int_t A, Double_t Delta);
  Double_t ClacS(Double_t M_t, Double_t M_b, Double_t E_b);
  Double_t CalcXi0(Double_t M_t, Double_t M_b);
  Double_t CalcXi1(Double_t M_t, Double_t M_b, Double_t E_b);

private:
  ClassDef(sonik::Kinematics,1)
};

}



#endif
