#include <vector>

#include "Riostream.h"
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TF1.h"
#include "TObject.h"
#include "TVectorD.h"

// header file for SONIK gainmatch detectors
// D. Connolly

static const char* calfile = "${he3}/SONIK_Cal.csv";
static const Int_t MAX_CHANNELS = 19; //32
static const Int_t Nalpha       = 3;
static const Int_t Npulser      = 3;

const Double_t alpha_E[3] = {5.15659,5.48556,5.80477}; // alpha energies of triple alpha source in MeV
const Double_t dLayer1    = 8.0e-5; //ORTEC IIS dead layer thickness (Si equivalent) in mm.
const Double_t dLayer2    = 5.0e-5; //PIPS dead layer thickness (Si equivalent) in mm.
// const Double_t dEdx[3]    = {139.006,133.701,129.296}; // stopping powers (MeV/mm) of alphas in Si corresponding to above energies according to SRIM 2008
// const Double_t E_peak1[3] = {alpha_E[0]-dEdx[0]*dLayer1,alpha_E[1]-dEdx[1]*dLayer1,alpha_E[2]-dEdx[2]*dLayer1};
// const Double_t E_peak2[3] = {alpha_E[0]-dEdx[0]*dLayer2,alpha_E[1]-dEdx[1]*dLayer2,alpha_E[2]-dEdx[2]*dLayer2};
const Double_t E_peak1[3] = {alpha_E[0]-0.023685,alpha_E[1]-0.022975,alpha_E[2]-0.021994}; //LISE 200 nm Si
const Double_t E_peak2[3] = {alpha_E[0]-0.0059122,alpha_E[1]-0.0057372,alpha_E[2]-0.0054921}; //LISE 50 nm Si

class Gainmatch{// class for gainmatch data
public:
	Gainmatch();

public:
	Int_t channel;         //#
	Int_t region;          //#
	Double_t angle;        //#
	Int_t threshold;       //#
	Int_t pulser[Npulser]; //#
	Double_t u_p[Npulser]; //#
	Int_t alpha[Nalpha];   //#
	Double_t u_a[Nalpha];  //#
	Double_t offset;       //#
	Double_t gain;         //#
	Double_t INL;          //#
	ClassDef(Gainmatch,1)
};
