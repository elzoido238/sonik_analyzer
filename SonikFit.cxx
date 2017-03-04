////////////////////////////////////////////////////
/// Implemantaion of SonikFit.hxx                ///
///                                              ///
/// 02/2017 - Devin Connolly                     ///
////////////////////////////////////////////////////

#include "SonikFit.h"
ClassImp(SonikFit);

//==================== Class SonikFit ====================//

SonikFit::SonikFit(char* title, Int_t Nevents, Double_t binning)
{
  SetVars(title, Nevents, binning);
}

// Public member functions

RooFitResult* SonikFit::ArgBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t cutoff, Double_t* range, Bool_t DrawFlag)
{
  // construct variable
  RooRealVar x("x","x",0,4);
  x.setRange("Rarg",range[0],range[1]);

  // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
  RooDataHist dh("dh","dh",x,Import(*hh));

  // B u i l d   G a u s s i a n   p . d . f s
	RooRealVar sigma("sigma","sigma",0.02,0.001,1);
  RooRealVar mean3("mean3","mean3",peak3,peak3-3*sigma.getVal(),peak3+3*sigma.getVal());
	RooRealVar nhe3("nhe3","# ^{3}He events",0,fNevents);

  RooRealVar mean4("mean4","mean4",peak4,peak4-3*sigma.getVal(),peak4+3*sigma.getVal());;
	RooRealVar nhe4("nhe4","# ^{4}He events",0,fNevents);

  RooGaussian he3("he3","he3",x,mean3,sigma);
  RooGaussian he4("he4","he4",x,mean4,sigma);

  // B u i l d   a r g u s   p d f
  RooConstVar argm0("argm0","argus constant",cutoff);
  RooRealVar argc("argc","argus shape par",-0.1,-10,-0.01);
  RooRealVar farg("farg","fraction of argus",0.1,0.,1.);
  RooArgusBG argus("argus","argus bg",x,argm0,argc);

  RooRealVar nbg("nbg","# bg events",0,fNevents);

  // Sum the composite signal and background
  RooAddPdf model("model","signal+bg",RooArgList(argus,he3,he4),RooArgList(nbg,nhe3,nhe4));

  // P r i n t   r e s u l t   a n d   r e t u r n   r e s u l t   p o i n t e r
  // -----------------------------------------------------------------------------------------------
	RooFitResult* result = model.fitTo(dh, Range("Rarg"), Extended(), Save());

	result->Print();

  if(DrawFlag){
    DrawFrame(dh, x, model, he3, he4, argus, range);
  }

	return(result);
}


RooFitResult* SonikFit::Exp_3HeBG(TH1D* hh, Double_t peak3, Double_t lambda0, Double_t* range, Bool_t DrawFlag)
{
  RooRealVar x("x","x",0,4);
  x.setRange("Rexp",range[0],range[1]);

  // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
  RooDataHist dh("dh","dh",x,Import(*hh));

  // P l o t   a n d   f i t   a   R o o D a t a H i s t
  // ---------------------------------------------------
  // Fit a Gaussian p.d.f to the data
	RooRealVar sigma("sigma","sigma",0.02,0.001,1);
  RooRealVar mean3("mean3","mean3",peak3,peak3-3*sigma.getVal(),peak3+3*sigma.getVal());
	RooRealVar nhe3("nhe3","# ^{3}He events",0,fNevents);

  RooGaussian he3("he3","he3",x,mean3,sigma);

  // Build exponential pdf
  RooRealVar lambda("lambda","exp constant",lambda0,-100.,-0.01);
  RooExponential bg("bg","exponential bg",x,lambda);
  RooRealVar nbg("nbg","# bg events",0,fNevents);

  // Sum the composite signal and background
  RooAddPdf model("model","signal+bg",RooArgList(bg,he3),RooArgList(nbg,nhe3));

  // P r i n t   r e s u l t   a n d   r e t u r n   r e s u l t   p o i n t e r
  // ----------------------------------------------------------------------------------------
	RooFitResult* result = model.fitTo(dh, Range("Rexp"), Extended(), Save());
	result->Print();

  if(DrawFlag){
    DrawFrame(dh, x, model, he3, bg, range);
  }

	return(result);
}


RooFitResult* SonikFit::ExpBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t lambda0, Double_t* range, Bool_t DrawFlag)
{
  RooRealVar x("x","x",0,4);
  x.setRange("Rexp",range[0],range[1]);

  // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
  RooDataHist dh("dh","dh",x,Import(*hh));

  // P l o t   a n d   f i t   a   R o o D a t a H i s t
  // ---------------------------------------------------
  // Fit a Gaussian p.d.f to the data
	RooRealVar sigma("sigma","sigma",0.02,0.001,1);
  RooRealVar mean3("mean3","mean3",peak3,peak3-3*sigma.getVal(),peak3+3*sigma.getVal());
	RooRealVar nhe3("nhe3","# ^{3}He events",0,fNevents);

  RooRealVar mean4("mean4","mean4",peak4,peak4-3*sigma.getVal(),peak4+3*sigma.getVal());;
	RooRealVar nhe4("nhe4","# ^{4}He events",0,fNevents);

  RooGaussian he3("he3","he3",x,mean3,sigma);
  RooGaussian he4("he4","he4",x,mean4,sigma);

  // Build exponential pdf
  RooRealVar lambda("lambda","exp constant",lambda0,-100.,-0.01);
  RooExponential exp("exp","exponential bg",x,lambda);
  RooRealVar nexp("nexp","# bg events",0,fNevents);

  // S u m   t h e   c o m p o s i t e   s i g n a l   a n d   b a c k g r o u n d
  RooAddPdf model("model","signal+bg",RooArgList(exp,he3,he4),RooArgList(nexp,nhe3,nhe4));

  // P r i n t   r e s u l t   a n d   r e t u r n   r e s u l t   p o i n t e r
  // ----------------------------------------------------------------------------------------
	RooFitResult* result = model.fitTo(dh, Range("Rexp"), Extended(), Save());
	result->Print();

  if(DrawFlag){
    DrawFrame(dh, x, model, he3, he4, exp, range);
  }

	return(result);
}


RooFitResult* SonikFit::LandauBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t peakL, Double_t* range, Bool_t DrawFlag)
{
  RooRealVar x("x","x",0,4);
  x.setRange("Rlandau",range[0],range[1]);

  // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
  RooDataHist dh("dh","dh",x,Import(*hh));

  // P l o t   a n d   f i t   a   R o o D a t a H i s t
  // ---------------------------------------------------
  // Fit a Gaussian p.d.f to the data
	RooRealVar sigma("sigma","sigma",0.02,0.001,1);
  RooRealVar mean3("mean3","mean3",peak3,peak3-3*sigma.getVal(),peak3+3*sigma.getVal());
	RooRealVar nhe3("nhe3","# ^{3}He events",0,fNevents);

  RooRealVar mean4("mean4","mean4",peak4,peak4-3*sigma.getVal(),peak4+3*sigma.getVal());;
	RooRealVar nhe4("nhe4","# ^{4}He events",0,fNevents);

  RooGaussian he3("he3","he3",x,mean3,sigma);
  RooGaussian he4("he4","he4",x,mean4,sigma);

	// Build landau pdf
	RooRealVar sigmaL("sigmaL","sigma of Landau",0.1,0,5);
	RooRealVar meanL("meanL","mean of Landau",peakL,peakL-3*sigmaL.getVal(),peakL+3*sigmaL.getVal());
	RooLandau landau("landau","landau bg",x,meanL,sigmaL);

  RooRealVar nbg("nbg","# bg events",0,fNevents);

  // Sum the composite signal and background
  RooAddPdf model("model","signal+bg",RooArgList(landau,he4,he3),RooArgList(nbg,nhe3,nhe4));

  // P r i n t   r e s u l t   a n d   r e t u r n   r e s u l t   p o i n t e r
  // ---------------------------------------------------------------------------
	RooFitResult* result = model.fitTo(dh, Range("Rlandau"), Extended(), Save());
	result->Print();

  if(DrawFlag){
    DrawFrame(dh, x, model, he3, he4, landau, range);
  }

	return(result);
}


RooFitResult* SonikFit::L_ArgBG(TH1D* hh, Double_t peak3, Double_t peak4, Double_t peakL, Double_t cutoff, Double_t* range, Bool_t DrawFlag)
{
  // construct variable
  RooRealVar x("x","x",0,4);
  x.setRange("Rmodel",range[0],range[1]);

  // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
  RooDataHist dh("dh","dh",x,Import(*hh));

  // B u i l d   G a u s s i a n   p . d . f s
	RooRealVar sigma("sigma","sigma",0.02,0.001,1);
  RooRealVar mean3("mean3","mean3",peak3,peak3-3*sigma.getVal(),peak3+3*sigma.getVal());
	RooRealVar nhe3("nhe3","# ^{3}He events",0,fNevents);

  RooRealVar mean4("mean4","mean4",peak4,peak4-3*sigma.getVal(),peak4+3*sigma.getVal());;
	RooRealVar nhe4("nhe4","# ^{4}He events",0,fNevents);

  RooGaussian he3("he3","he3",x,mean3,sigma);
  RooGaussian he4("he4","he4",x,mean4,sigma);

	// B u i l d   l a n d a u   p d f
	RooRealVar sigmaL("sigmaL","sigma of Landau",0.1,0,5);
	RooRealVar meanL("meanL","mean of Landau",peakL,peakL-3*sigmaL.getVal(),peakL+3*sigmaL.getVal());
  RooRealVar flandau("flandau","fraction of landau",0.,1.);
	RooLandau landau("landau","landau bg",x,meanL,sigmaL);

  // B u i l d   a r g u s   p d f
  RooConstVar argm0("argm0","argus constant",cutoff);
  RooRealVar argc("argc","argus shape par",-0.1,-10,-0.01);
  RooRealVar farg("farg","fraction of argus",0.1,0.,1.);
  RooArgusBG argus("argus","argus bg",x,argm0,argc);

  RooAddPdf bg("bg","background",RooArgList(landau,argus),flandau);
  RooRealVar nbg("nbg","# bg events",0,fNevents);

  // Sum the composite signal and background
  RooAddPdf model("model","signal+bg",RooArgList(bg,he3,he4),RooArgList(nbg,nhe3,nhe4));

  // P r i n t   r e s u l t   a n d   r e t u r n   r e s u l t   p o i n t e r
  // -----------------------------------------------------------------------------------------------
	RooFitResult* result = model.fitTo(dh, Range("Rmodel"), Extended(), Save());

	result->Print();

  if(DrawFlag){
    DrawFrame(dh, x, model, he3, he4, bg, range);
  }

	return(result);
}

// Private member functions

void SonikFit::DrawFrame(RooDataHist dh, RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooArgusBG bg, Double_t* range)
{
  // Make plot of binned dataset showing Poisson error bars (RooFit default)
  RooPlot *frame = x.frame(Title(fTitle));
	frame->GetXaxis()->SetTitle("Energy [MeV]");
	frame->GetYaxis()->SetTitle(Form("Counts / ^{}%.2g [keV]",1e3*fBinning));
  dh.plotOn(frame);

	model.plotOn(frame, LineColor(2014), Precision(1e-4));
	model.plotOn(frame, LineColor(3), LineStyle(7), Components(he3), Precision(1e-4));
	model.plotOn(frame, LineColor(4), LineStyle(7), Components(he4), Precision(1e-4));
	model.plotOn(frame, LineColor(2), LineStyle(7), Components(bg), Precision(1e-4));

  char ytitle[32];
  sprintf(ytitle,"Events / %.4g [keV]",1e3*fBinning);

  TCanvas *c2 = new TCanvas();
  frame->GetXaxis()->CenterTitle(); frame->GetXaxis()->SetRangeUser(range[0], range[1]); frame->GetYaxis()->SetRangeUser(1,2e6); frame->GetYaxis()->CenterTitle(); frame->GetXaxis()->SetTitle("Energy [MeV]"); frame->GetYaxis()->SetTitle(ytitle);
  c2->SetLogy(kTRUE);
  frame->Draw();

  // TCanvas *c3 = new TCanvas();
  // frame2->GetYaxis()->SetRangeUser(1,fNevents+1e2);
  // c3->SetLogy(kTRUE);
  // frame2->Draw();

}


void SonikFit::DrawFrame(RooDataHist dh, RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooExponential bg, Double_t* range)
{
  // Make plot of binned dataset showing Poisson error bars (RooFit default)
  RooPlot *frame = x.frame(Title(fTitle));
	frame->GetXaxis()->SetTitle("Energy [MeV]");
	frame->GetYaxis()->SetTitle(Form("Counts / ^{}%.2g [keV]",1e3*fBinning));
  dh.plotOn(frame);

	model.plotOn(frame, LineColor(2014), Precision(1e-4));
	model.plotOn(frame, LineColor(3), LineStyle(7), Components(he3), Precision(1e-4));
	model.plotOn(frame, LineColor(4), LineStyle(7), Components(he4), Precision(1e-4));
	model.plotOn(frame, LineColor(2), LineStyle(7), Components(bg), Precision(1e-4));

  char ytitle[32];
  sprintf(ytitle,"Events / %.4g [keV]",1e3*fBinning);

  TCanvas *c2 = new TCanvas();
  frame->GetXaxis()->CenterTitle(); frame->GetXaxis()->SetRangeUser(range[0], range[1]); frame->GetYaxis()->SetRangeUser(1,2e6); frame->GetYaxis()->CenterTitle(); frame->GetXaxis()->SetTitle("Energy [MeV]"); frame->GetYaxis()->SetTitle(ytitle);
  c2->SetLogy(kTRUE);
  frame->Draw();

}


void SonikFit::DrawFrame(RooDataHist dh, RooRealVar x, RooAddPdf model, RooGaussian he3, RooExponential bg, Double_t* range)
{
  // Make plot of binned dataset showing Poisson error bars (RooFit default)
  RooPlot *frame = x.frame(Title(fTitle));
	frame->GetXaxis()->SetTitle("Energy [MeV]");
	frame->GetYaxis()->SetTitle(Form("Counts / ^{}%.2g [keV]",1e3*fBinning));
  dh.plotOn(frame);

	model.plotOn(frame, LineColor(2014), Precision(1e-4));
	model.plotOn(frame, LineColor(3), LineStyle(7), Components(he3), Precision(1e-4));
	model.plotOn(frame, LineColor(2), LineStyle(7), Components(bg), Precision(1e-4));

  char ytitle[32];
  sprintf(ytitle,"Events / %.4g [keV]",1e3*fBinning);

  TCanvas *c2 = new TCanvas();
  frame->GetXaxis()->CenterTitle(); frame->GetXaxis()->SetRangeUser(range[0], range[1]); frame->GetYaxis()->SetRangeUser(1,2e6); frame->GetYaxis()->CenterTitle(); frame->GetXaxis()->SetTitle("Energy [MeV]"); frame->GetYaxis()->SetTitle(ytitle);
  c2->SetLogy(kTRUE);
  frame->Draw();

}


void SonikFit::DrawFrame(RooDataHist dh, RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooLandau bg, Double_t* range)
{
  // Make plot of binned dataset showing Poisson error bars (RooFit default)
  RooPlot *frame = x.frame(Title(fTitle));
	frame->GetXaxis()->SetTitle("Energy [MeV]");
	frame->GetYaxis()->SetTitle(Form("Counts / ^{}%.2g [keV]",1e3*fBinning));
  dh.plotOn(frame);

	model.plotOn(frame, LineColor(2014), Precision(1e-4));
	model.plotOn(frame, LineColor(3), LineStyle(7), Components(he3), Precision(1e-4));
	model.plotOn(frame, LineColor(4), LineStyle(7), Components(he4), Precision(1e-4));
	model.plotOn(frame, LineColor(2), LineStyle(7), Components(bg), Precision(1e-4));

  char ytitle[32];
  sprintf(ytitle,"Events / %.4g [keV]",1e3*fBinning);

  TCanvas *c2 = new TCanvas();
  frame->GetXaxis()->CenterTitle(); frame->GetXaxis()->SetRangeUser(range[0], range[1]); frame->GetYaxis()->SetRangeUser(1,2e6); frame->GetYaxis()->CenterTitle(); frame->GetXaxis()->SetTitle("Energy [MeV]"); frame->GetYaxis()->SetTitle(ytitle);
  c2->SetLogy(kTRUE);
  frame->Draw();

}


void SonikFit::DrawFrame(RooDataHist dh, RooRealVar x, RooAddPdf model, RooGaussian he3, RooGaussian he4, RooAddPdf bg, Double_t* range)
{
  // Make plot of binned dataset showing Poisson error bars (RooFit default)
  RooPlot *frame = x.frame(Title(fTitle));
	frame->GetXaxis()->SetTitle("Energy [MeV]");
	frame->GetYaxis()->SetTitle(Form("Counts / ^{}%.2g [keV]",1e3*fBinning));
  dh.plotOn(frame);

	model.plotOn(frame, LineColor(2014), Precision(1e-4));
	model.plotOn(frame, LineColor(3), LineStyle(7), Components(he3), Precision(1e-4));
	model.plotOn(frame, LineColor(4), LineStyle(7), Components(he4), Precision(1e-4));
	model.plotOn(frame, LineColor(2), LineStyle(7), Components(bg), Precision(1e-4));

  char ytitle[32];
  sprintf(ytitle,"Events / %.4g [keV]",1e3*fBinning);

  TCanvas *c2 = new TCanvas();
  frame->GetXaxis()->CenterTitle(); frame->GetXaxis()->SetRangeUser(range[0], range[1]); frame->GetYaxis()->SetRangeUser(1,2e6); frame->GetYaxis()->CenterTitle(); frame->GetXaxis()->SetTitle("Energy [MeV]"); frame->GetYaxis()->SetTitle(ytitle);
  c2->SetLogy(kTRUE);
  frame->Draw();

}


void SonikFit::SetVars(char* title, Int_t Nevents, Double_t binning)
{
  fTitle   = title;
  fNevents = Nevents;
  fBinning = binning;
}
