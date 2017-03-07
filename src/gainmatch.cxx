#include "gainmatch.h"

// macro to gainmatch SONIK detectors
// D. Connolly

void gainmatch(){

	cout << E_peak1[0] << "\t" << E_peak1[1] << "\t" << E_peak1[2] << "\n";
	cout << E_peak2[0] << "\t" << E_peak2[1] << "\t" << E_peak2[2] << "\n";
	

	TFile *f = new TFile("gainmatch.root","RECREATE");
	f->cd();

	// vars for writing rootfile
	Gainmatch *gm = new Gainmatch;
	
	TTree *tg = new TTree("tg","Measured and calculated gainmatch parameters");
	tg->Branch("gm","Gainmatch",&gm);

	TGraphErrors *grp = new TGraphErrors(Npulser);
	TGraphErrors *gra = new TGraphErrors(Nalpha);
	char pname[32];
	char aname[32];
	
	TVectorD channel(MAX_CHANNELS);
	TVectorD region(MAX_CHANNELS);
	TVectorD angle(MAX_CHANNELS);
	TVectorD threshold(MAX_CHANNELS);
	TVectorD offset(MAX_CHANNELS);
	TVectorD gain(MAX_CHANNELS);
	TVectorD INL(MAX_CHANNELS);
	TVectorD c(1);
	
	Int_t i = 0;
	Double_t gain_min = pow(2.0,16);
	Int_t min_chan;
	Double_t V0, b1, m1, b2, m2, V_line;

	Int_t ch[MAX_CHANNELS], reg[MAX_CHANNELS], thresh[MAX_CHANNELS], pulse[MAX_CHANNELS][Npulser], alpha[MAX_CHANNELS][Nalpha];
	Double_t ang[MAX_CHANNELS], g[MAX_CHANNELS], os[MAX_CHANNELS], nl[MAX_CHANNELS], up[MAX_CHANNELS][Npulser], ua[MAX_CHANNELS][Nalpha];
	Double_t V[Npulser] = {0.1,0.5,1};

	// for(Int_t i = 0; i < Npulser; ++i){
	// 	if(i == 0) V[i] = 0;
	// 	else
	// 		V[i] = V[i-1] + 1 / (double (Npulser-1));
	// }

	// open file containing pulser walk and source data
	ifstream in;
	// in.open("/Volumes/Volume2/data2/dragon/S1025/scripts/3He_alpha/SONIK_Cal.csv");
	in.open(gSystem->ExpandPathName(calfile));
	// in.open("/home/elzoido238/Desktop/SONIK/S1025/scripts/3He_alpha/SONIK_Cal.csv");

	// fill buffers & write graphs
	for(Int_t i = 0; i < MAX_CHANNELS; ++i){
		// read data values
		in >> ch[i] >> ang[i] >> reg[i] >> thresh[i] >> pulse[i][0] >> pulse[i][1] >> pulse[i][2] >> up[i][0] >> up[i][1] >> up[i][2] >> alpha[i][0] >> alpha[i][1] >> alpha[i][2] >> ua[i][0] >> ua[i][1] >> ua[i][2];
		// if(!in.good()) break;

		// cout << ch[i] << "\t" << ang << "\t" << reg << "\t" << thresh << "\t" << pulse[0] << "\t" << pulse[1] << "\t" << pulse[2] << "\t" << up[0] << "\t" << up[1] << "\t" << up[2] << "\t" << alpha[0] << "\t" << alpha[1] << "\t" << alpha[2] << "\t" << ua[0] << "\t" << ua[1] << "\t" << ua[2] << "\n";

		// ===== pulser data ===== \\
		// find offset via linear fit of pulser data and set tree values
		for(Int_t j=0;j<3;j++){
			grp->SetPoint(j,V[j],pulse[i][j]);
			grp->SetPointError(j,0,up[i][j]);
		}

		TFitResultPtr fit1 = grp->Fit("pol1","QS");
		b1 = fit1->Parameter(0);
		m1 = fit1->Parameter(1);
		os[i] = b1;

		// find maximum INL
		nl[i] = 0;
		for(Int_t j = 0; j < 3; ++j){
			V_line = (pulse[i][j] - b1) / m1;
			Double_t temp = 100.0*TMath::Abs(V[j] - V_line) / V_line;
			if(temp > nl[i]) nl[i] = temp;
		}

		cout << ch[i] << "\t\t" << os[i] << "\t\t" << nl[i] << "\n";

		// ===== source data =====\\
		// find gain via linear fit of source data and set tree values
		if(ch[i] >= 16 && ch[i] <= 25){// PIPS detectors
			for(Int_t j=0; j < 3; j++){
				gra->SetPoint(j,E_peak2[j],alpha[i][j]-os[i]);
				gra->SetPointError(j,0,ua[i][j]);
			}
		}
		else{
			for(Int_t j=0; j < 3; j++){// all other detectors
				gra->SetPoint(j,E_peak1[j],alpha[i][j]-os[i]);
				gra->SetPointError(j,0,ua[i][j]);
			}
		}
		
		TFitResultPtr fit2 = gra->Fit("pol1","QS");
		b2       = fit2->Parameter(0);
		m2       = fit2->Parameter(1);
		g[i]     = m2;

		// find min gain channel
		if(g[i] < gain_min){
			gain_min = g[i];
			min_chan = ch[i];
			c[0]     = 1/gain_min;
		}

		// scale all gains to min gain channel
		if(i == MAX_CHANNELS-1){
			cout << "Min gain channel = " << min_chan << "\n";
	
			// scale gains to min gain channel
			cout << "Channel" << "\t\t" << "Offset" << "\t\t" << "Gain" << "\n";
			cout << "=======" << "\t\t" << "======" << "\t\t" << "====" << "\n";

			for(Int_t j=0; j < MAX_CHANNELS; ++j){
				if( ch[j] == min_chan){
					g[j]     = 1.0;
				}
				else{
					g[j]     = gain_min/g[j];
				}
				gain[j] = g[j];
				cout << ch[j] << "\t\t" << os[j] << "\t\t" << g[j] << "\n"; 
			}
		}

		// write graphs
		sprintf(pname,"ch_%i_pulser",ch[i]);
		sprintf(aname,"ch_%i_alpha",ch[i]);
		grp->Write(pname);
		gra->Write(aname);

		// fill TVectors
		channel[i] = ch[i];
		region[i]  = reg[i];
		angle[i]   = ang[i];
		offset[i]  = os[i];
		INL[i]     = nl[i];

	}

	// fill tree
	for(Int_t i = 0; i < MAX_CHANNELS; ++i){

		for(Int_t j=0;j<3;j++){
			gm->pulser[j] = pulse[i][j];
			gm->u_p[j]    = up[i][j];
			gm->alpha[j]  = alpha[i][j];
			gm->u_a[j]    = ua[i][j];
		}

		gm->channel   = ch[i];
		gm->region    = reg[i];
		gm->angle     = ang[i];
		gm->threshold = thresh[i];
		gm->offset = os[i];
		gm->gain = g[i];
		gm->INL    = nl[i];
		// fill tree
		tg->Fill();
	}

	grp->Delete();
	gra->Delete();
	in.close();

// // Write slopes and offsets to odb
// 	for(Int_t i=0; i< adc.size(); ++i) {
// 		gSystem->Exec(Form("odbedit -c \"set /sonik/variables/adc/slope[%d] %.6g\"\n", ch[i], g[i] ) );
// 		gSystem->Exec(Form("odbedit -c \"set /sonik/variables/adc/offset[%d] %.6g\"\n", ch[i], -os[i]) );
// 		gSystem->Exec(Form("odbedit -c \"set /dragon/dsssd/variables/adc/slope[%d] %.6g\"\n", ch[i], g[i] ) );
// 		gSystem->Exec(Form("odbedit -c \"set /dragon/dsssd/variables/adc/offset[%d] %.6g\"\n", ch[i], -os[i] ) );
// 	}
// 	cout << "ATTENTION: Gains and offsets written to odb!\n";

// // Save current odb state to xml file
// 	gSystem->Exec("odbedit -c 'save -x SONIK_Ecal.xml'"); // save calibration as xml file in pwd
	
	// write TVectors
	c.Write("c");
	channel.Write("channel");
	offset.Write("offset");
	gain.Write("gain");
	INL.Write("INL");
	
	// Write root file
	
	f->Write();

}
