#include <vector>
#include <cstdio>
#include <cstring>
#include <sstream>

#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TH1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TChain.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TVectorT.h"
#include "gainmatch.h"


const char* runfile = "${S1025}/data/rootfiles/ecal/run%i.root";
const char* gainfile = "${he3}/gainmatch.root";
const Int_t Nbins = pow(2.0,12);

class Sonikcal{ //class for sonik data
public:
	Double_t gainmatched[MAX_CHANNELS];  //#
	Double_t ecal[MAX_CHANNELS];         //#
	ClassDef(Sonikcal,1)
};
