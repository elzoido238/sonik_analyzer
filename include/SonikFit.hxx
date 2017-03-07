////////////////////////////////////////////////////
/// Class to fit Sonik data                      ///
///                                              ///
/// 02/2017 - Devin Connolly                     ///
////////////////////////////////////////////////////
#ifndef HAVE_SONIK_FIT_HXX
#define HAVE_SONIK_FIT_HXX
#ifndef __MAKECINT__
#include <iostream>
#endif

#include "TH1.h"
#include "TCanvas.h"
#include "TObject.h"


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooArgusBG.h"
#include "RooConstVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooFitResult.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooLognormal.h"
#include "RooPlot.h"
#include "RooRealVar.h"
using namespace RooFit;

class RooFitResult;
class TObject;

//namespace sonik {

class SonikFit : public TObject {
public:
  SonikFit(char* title, Int_t Nevents, Double_t binning);
  SonikFit() { fTitle = 0; fNevents = 0; fBinning = 0; }

public:
  RooFitResult* ArgBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t cutoff, Double_t* range, Bool_t DrawFlag = kTRUE);
  RooFitResult* Exp_3HeBG(TH1D* hh, Double_t peak3, Double_t lambda0, Double_t* range, Bool_t DrawFlag = kTRUE);
  RooFitResult* ExpBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t lambda0, Double_t* range, Bool_t DrawFlag = kTRUE);
  RooFitResult* LandauBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t peakL, Double_t* range, Bool_t DrawFlag = kTRUE);
  RooFitResult* L_ArgBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t peakL, Double_t cutoff, Bool_t DrawFlag = kTRUE);

private:
  void DrawFrame(RooDataHist dh,RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooArgusBG bg, Double_t* range);
  void DrawFrame(RooDataHist dh,RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooExponential bg, Double_t* range);
  void DrawFrame(RooDataHist dh,RooRealVar x, RooAddPdf model, RooGaussian he3, RooExponential bg, Double_t* range);
  void DrawFrame(RooDataHist dh,RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooAddPdf bg);
  void DrawFrame(RooDataHist dh,RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooLandau bg, Double_t* range);
  void SetVars(char* title, Int_t Nevents, Double_t binning);

private:
  char* fTitle;      //!
  Int_t fNevents;
  Double_t fBinning;

  ClassDef(SonikFit,1)
};


//} // namespace sonik

#endif
