////////////////////////////////////////////////////
/// Implementation of SonikCal.hxx               ///
///                                              ///
/// 03/2017 - Devin Connolly                     ///
////////////////////////////////////////////////////
#include "SonikCal.hxx"
ClassImp(sonik::SonikCal);


void sonik::SonikCal::FillTree(vector<int> &runs, const char* runfile, const char* gainfile){

	Int_t adc[MAX_CHANNELS];
	Double_t pedestal[MAX_CHANNELS];

	TFile *fout = TFile::Open("tree_cal.root", "RECREATE");
	// f_gain = TFile::Open("/home/elzoido238/Desktop/SONIK/S1025/scripts/3He_alpha/gainmatch.root");
	cout << gSystem->ExpandPathName(gainfile) << "\n";
	TFile *fgain = TFile::Open(gSystem->ExpandPathName(gainfile));
	fgain->cd();

	TTree *tg = (TTree *)fgain->Get("tg");

	for(Int_t i = 0; i < MAX_CHANNELS; ++i){
		tg->GetEntry(i);
		adc[i]      = tg->GetLeaf("channel")->GetValue();
		pedestal[i] = tg->GetLeaf("threshold")->GetValue();
		cout << adc[i] << "\t" << pedestal[i] << "\n";
	}

	TVectorD *c   = (TVectorD *)fgain->Get("c");
	Double_t c0   = ((*c))[0];

	cout << "Writing gainmatch tree to file..." << "\n";

	fout->cd();
	c->Write("c");
	tg->CloneTree()->Write();

	cout << "...done!" << "\n";

	fgain->cd(), fgain->Close(), fgain->Delete();

	// create sonik tree
	TTree *ts = new TTree("ts","SONIK Energy Spectrum");
	SonikCal *sonikcal = new SonikCal;
	ts->Branch("sonikcal","SonikCal",&sonikcal);

	TChain *chain = new TChain("t3");

	cout << "Chaining runs...\n";
	for(unsigned long i = 0; i < runs.size(); ++i) {
		char fname[1024];
		sprintf(fname, runfile, runs.at(i) );
		cout << gSystem->ExpandPathName(fname) << "\n";
		TFile fchain(fname);
		chain->AddFile(fname);
	}

	cout << "...done!\n";

	Double_t val;

	for(Long_t evt = 0; evt < chain->GetEntries(); ++evt){
		cout << "\r" << "Processing event " << std::setw(2) << evt << std::setw(2) << std::flush;
		chain->GetEntry(evt);
		for(Int_t i=0; i < MAX_CHANNELS; i++){
			val = chain->GetLeaf("dsssd.ecal")->GetValue(adc[i]);
			if(val < pedestal[i]) continue;
			sonikcal->gainmatched[i] = val;
			sonikcal->ecal[i]        = c0*val;
		}
		ts->Fill();
	}

	fout->cd();

	cout << "\n Writing sonik tree to file...\n";
	ts->Write();
	// fout->Write();
	cout << "...done!" << "\n";

	fout->Close();

}


void sonik::SonikCal::reset()
{
	std::fill(gainmatched, gainmatched + MAX_CHANNELS, 0);
	std::fill(ecal, ecal + MAX_CHANNELS, 0);
}
