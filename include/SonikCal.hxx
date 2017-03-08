////////////////////////////////////////////////////
/// Class to write energy calibrated Sonik tree  ///
///                                              ///
/// 03/2017 - Devin Connolly                     ///
////////////////////////////////////////////////////
#ifndef HAVE_SONIK_CAL_HXX
#define HAVE_SONIK_CAL_HXX
#ifndef __MAKECINT__
#include <iostream>
#endif

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

//#include "Gainmatch.hxx"

namespace sonik {

class Gainmatch;

const Int_t Nbins = pow(2.0,12);

class SonikCal{ //class for sonik data
public: // Constants
	static const Int_t MAX_CHANNELS = 32;

public: // Methods
	SonikCal() { reset(); }
	inline virtual ~SonikCal() { }
	void reset();
	void FillTree(vector<int> &runs, const char* runfile = "${S1025}/data/rootfiles/ecal/run%i.root", const char* gainfile = "${he3}/gainmatch.root");   //#
	/// Files only valid for ogopogo

public:
	Double_t gainmatched[MAX_CHANNELS];  //#
	Double_t ecal[MAX_CHANNELS];         //#

private:
	ClassDef(sonik::SonikCal,1)
};

} // namespace sonik




#endif
