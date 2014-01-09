#include <iostream>
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>
//#define Maxselection 3
//#define Nhltpaths 442

void make_up(){
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("2.2f");
  
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/higgs/yy/hlt/CMSSW_5_3_2_patch4/src/genAnalyzer/GenAnalyzer/genAnalyzer.root");
  TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/higgs/yy/hlt/CMSSW_5_3_11/src/HiggsGenHltRecoAnalyzer/HiggsGenHltRecoAnalyzer/test/genAnalyzer.root");
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/all.root");

  system("mkdir gen");
  system("mkdir hlt");

  cout<<"creating canvas"<<endl;
  TCanvas * Ca0 = new TCanvas("Ca0","bit0",1200,800);
  Ca0->cd();

  Draw1D("ptLead","p_{T} (GeV)","gen/",theFile);
  Draw1D("ptTrail","p_{T} (GeV)","gen/",theFile);
  Draw1D("massDiphoton","mass_{#gamma #gamma} (GeV)","gen/",theFile);
  Draw1D("massHiggs","mass_{H} (GeV)","gen/",theFile);
  Draw1D("higgsEta","#eta_{H} (GeV)","gen/",theFile);
  Draw1D("higgsPhi","#phi_{H} (GeV)","gen/",theFile);
  Draw1D("higgsPt","#p (GeV)","gen/",theFile);
  Draw1D("numerator","HLT bit","hlt/",theFile);
  Draw1D("denominator","HLT bit","hlt/",theFile);
  // Ca0->SetLogy();
  Draw1D("higgsP","#p (GeV)","gen/",theFile); 
  
  //all hlt bits efficiency plot
  TH1F * numerator = (TH1F*)(theFile->Get("numerator_1"));//We just plot the efficiency w.r.t the last selection
  TH1F * denominator = (TH1F*)(theFile->Get("denominator_1"));
  if(TEfficiency::CheckConsistency(*numerator,*denominator)){
    TEfficiency* pEff = new TEfficiency(*numerator,*denominator);
    binomialEfficiency1D(pEff);
  }
  cout<<"cleaning canvas"<<endl;
  Ca0->SaveAs("hlt/all_hlt_eff.png");
  Ca0->Clear();
  
  //turnon curve
  //Interesting bits
  
  string interesting_bits[]={"418","6","416","8","5","412","233","7","234","10","187","9","22","198","430","404","188","199","21","190","197","208","201","207","195","196","206","193","424","192","204","194","203","205","209","189","200","191"};
  
  //string in_bits[]={"418","6"}
  //for(int k=0;k<2;k++){
  
  for(int k=0;k<38;k++){//Efficiency w.r.t hlt/generator
    //cout<<k<<endl;
    TEfficiency * Effbit = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]+"_1").c_str()));
    if(!Effbit) cout<<"Effbit not found"<<endl;
    binomialEfficiency2D(Effbit);
    Ca0->SaveAs(("hlt/bit_"+interesting_bits[k]+"_"+Effbit->GetTitle()+"_efficiency.png").c_str());
    Ca0->Clear();
  }

  for(int k=0;k<38;k++){//efficiency hlt/acc = hlt/gen / acc/gen
    //cout<<k<<endl;
    TEfficiency * Effbit_num = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]+"_1").c_str()));
    TEfficiency * Effbit_den = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]+"_2").c_str()));
    if(!Effbit) cout<<"Effbit not found"<<endl;
    renormalized_binomialEfficiency2D(Effbit_num,Effbit_den);
    Ca0->SaveAs(("hlt/acc_bit_"+interesting_bits[k]+"_"+Effbit->GetTitle()+"_efficiency.png").c_str());
    Ca0->Clear();
  }

  TEfficiency * EffbitOR = (TEfficiency*) (theFile->Get("Eff_bit_194_195_205_1"));
  if(!EffbitOR) cout<<"EffbitOR not found"<<endl;
  binomialEfficiency2D(EffbitOR);
  Ca0->SaveAs("hlt/bitOR_efficiency.png");
  Ca0->Clear();

  TEfficiency * EffbitOR_den = (TEfficiency*) (theFile->Get("Eff_bit_194_195_205_2"));
  if(!EffbitOR) cout<<"EffbitOR_den not found"<<endl;
  renormalized_binomialEfficiency2D(EffbitOR,EffbitOR_den);
  Ca0->SaveAs("hlt/acc_bitOR_efficiency.png");
  Ca0->Clear();
  
  TH2F * phi1phi2 = (TH2F*)(theFile->Get("phi1phi2"));
  phi1phi2->Draw("colz");
  phi1phi2->SetXTitle("#phi_{Lead}");
  phi1phi2->SetYTitle("#phi_{Trail}");
  Ca0->SaveAs("gen/phi1phi2.png");Ca0->Clear();

  TH2F * eta1eta2 = (TH2F*)(theFile->Get("eta1eta2"));
  eta1eta2->Draw("colz");
  eta1eta2->SetXTitle("#eta_{Lead}");
  eta1eta2->SetYTitle("#eta_{Trail}");
  Ca0->SaveAs("gen/eta1eta2.png");Ca0->Clear();

  TH2F * pt1pt2 = (TH2F*)(theFile->Get("pt1pt2"));
  pt1pt2->Draw("colz");
  pt1pt2->SetXTitle("p_{T} Lead (GeV)");
  pt1pt2->SetYTitle("p_{T} Trail (GeV)");
  Ca0->SaveAs("gen/pt1pt2.png");Ca0->Clear();
  
  exit(0);
}

void binomialEfficiency2D(TEfficiency * teff){
  TH2D numerator = (TH2D)teff->GetPassedHistogram();
  TH2D denominator = (TH2D)teff->GetTotalHistogram();
  
  const Double_t bins[17]={20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
  TH2F * efficiency = new TH2F ("efficiency",teff->GetTitle(),16,bins,16,bins);
  efficiency->SetXTitle("E_{T} #gamma Lead");
  efficiency->SetYTitle("E_{T} #gamma Trail");
  float eff,err;
  for(int i=0;i<17;i++)
    for(int j=0;j<=i;j++){
      if(denominator.GetBinContent(i,j)!=0){
	eff = numerator.GetBinContent(i,j)/denominator.GetBinContent(i,j);
	err = sqrt(eff*(1-eff)/denominator.GetBinContent(i,j));
	efficiency->SetBinContent(i,j,eff);
	efficiency->SetBinError(i,j,err);
	//cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
      }
    }
  efficiency->Draw("colztextE");
}

void renormalized_binomialEfficiency2D(TEfficiency * teff_num, TEfficiency * teff_den){
  TH2D numerator   = (TH2D)teff_num->GetPassedHistogram();
  TH2D denominator = (TH2D)teff_den->GetPassedHistogram();
  
  const Double_t bins[17]={20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
  TH2F * efficiency = new TH2F ("efficiency",teff_num->GetTitle(),16,bins,16,bins);
  efficiency->SetXTitle("E_{T} #gamma Lead");
  efficiency->SetYTitle("E_{T} #gamma Trail");
  float eff,err;
  for(int i=0;i<17;i++)
    for(int j=0;j<=i;j++){
      if(denominator.GetBinContent(i,j)!=0){
	eff = numerator.GetBinContent(i,j)/denominator.GetBinContent(i,j);
	err = sqrt(eff*(1-eff)/denominator.GetBinContent(i,j));
	efficiency->SetBinContent(i,j,eff);
	efficiency->SetBinError(i,j,err);
	//cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
      }
    }
  efficiency->Draw("colztextE");
}


void binomialEfficiency1D(TEfficiency * teff){
  TH1D numerator = (TH1D)teff->GetPassedHistogram();
  TH1D denominator = (TH1D)teff->GetTotalHistogram();
  TH1F * efficiency = new TH1F ("efficiency","hlt bit efficiency %",442  ,-0.5,442  -0.5);
  efficiency->SetXTitle("HLT bit");
  efficiency->SetYTitle("#epsilon");
  float eff,err;
  for(int j=0;j<=442  ;j++){
    if(denominator.GetBinContent(j)!=0){
      eff = numerator.GetBinContent(j)/denominator.GetBinContent(j);
      err = sqrt(eff*(1-eff)/denominator.GetBinContent(j));
      efficiency->SetBinContent(j,eff);
      efficiency->SetBinError(j,err);
      cout<<"hlteff "<<j<<" "<<eff<<" +/- "<<err<<endl;
    }
  }
  efficiency->Draw("E");
  efficiency->SetMarkerColor(kRed);
  efficiency->SetMarkerStyle(23);
  efficiency->SetMarkerSize(0.5);
}
      
void Draw1D(string savedname,string theXtitle,string folder,TFile * theFile){
  TH1F * histoArray[3  ];
  for(int selection=0;selection<3  ;selection++){ //Loop over the different histograms
    //std::string histo = std::to_string(selection);
    char histo[20];
    sprintf(histo,"%d",selection);
    cout<<savedname+"_"+histo<<endl;
    histoArray[selection] = (TH1F*)(theFile->Get((savedname+"_"+histo).c_str()));
    if(selection==0){
      histoArray[selection]->Draw();
      histoArray[selection]->SetFillColor(kBlue);
    }else if(selection==1){
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kRed);
    }else{
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kYellow);
    }
    histoArray[selection]->SetXTitle(theXtitle.c_str());
  }
  Ca0->SaveAs((folder+savedname+".png").c_str());
  Ca0->Clear();
}
