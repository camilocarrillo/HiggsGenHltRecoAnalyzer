#include <iostream>
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>
//#define Maxselection 1
//#define Nhltpaths 442

void make_up(){
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("2.2f");
  
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/higgs/yy/hlt/CMSSW_5_3_2_patch4/src/genAnalyzer/GenAnalyzer/genAnalyzer.root");
  //TFile * theFile = new TFile("HiggsGenHltRecoAnalyzer/test/genAnalyzer.root");
  TFile * theFile = new TFile("HiggsGenHltRecoAnalyzer/genAnalyzer.root");
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
  // Ca0->SetLogy();
  Draw1D("higgsP","#p (GeV)","gen/",theFile); 
  
  //turnon curve
  //Interesting bits
  
  //  string interesting_bits[]={"418","6","416","8","5","412","233","7","234","10","187","9","22","198","430","404","188","199","21","190","197","208","201","207","195","196","206","193","424","192","204","194","203","205","209","189","200","191"};
  
  //string in_bits[]={"418","6"}
  //for(int k=0;k<2;k++){
  
  //Definition for the interesting bits                                                                                                  
  //0 gen                                                                                                                                
  //1 acc                                                                                                                                
  //2 194                                                                                                                                
  //3 195                                                                                                                                
  //4 205                                                                                                                                
  //5 OR                                                                                                                                 
  //6 OR*acc       
  
  cout<<"going for 2D"<<endl;
  TH2F * numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_6"));
  TH2F * denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  cout<<"calling binomial efficiency"<<endl;
  binomialEfficiency2D(numerator,denominator);
  cout<<"saving image"<<endl;
  Ca0->SaveAs(("hlt/or_over_acc_efficiency.png"));
  Ca0->Clear();
  
  TH2F * numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_4"));
  TH2F * denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  binomialEfficiency2D(numerator,denominator);
  Ca0->SaveAs(("hlt/205_over_acc_efficiency.png"));
  Ca0->Clear();

  TH2F * numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_3"));
  TH2F * denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  binomialEfficiency2D(numerator,denominator);
  Ca0->SaveAs(("hlt/195_over_acc_efficiency.png"));
  Ca0->Clear();

  TH2F * numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_2"));
  TH2F * denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  binomialEfficiency2D(numerator,denominator);
  Ca0->SaveAs(("hlt/194_over_acc_efficiency.png"));
  Ca0->Clear();

  Draw2D("wide_pt1pt2","#phi_{Lead}","#phi_{Trail}","gen/",theFile);

  Draw2D("phi1phi2","#phi_{Lead}","#phi_{Trail}","gen/",theFile);
  Draw2D("eta1eta2","#eta_{Lead}","#eta_{Trail}","gen/",theFile);
  Draw2D("pt1pt2","p_{T} Lead (GeV)","p_{T} Trail (GeV)","gen/",theFile);
  
  exit(0);
}

void binomialEfficiency2D(TH2F * numerator,TH2F * denominator){
  if(!numerator) cout<<"numerator not found"<<endl;
  if(!denominator) cout<<"denominator not found"<<endl;
  
  const Double_t bins[15]={30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
  TH2F * efficiency = new TH2F ("efficiency",numerator->GetXaxis()->GetTitle(),14,bins,14,bins);
  
  efficiency->SetXTitle("E_{T} #gamma Lead");
  efficiency->SetYTitle("E_{T} #gamma Trail");
  
  float eff,err;
  for(int i=0;i<15;i++)
    for(int j=0;j<=i;j++){//cambiar . por ->
      if(denominator->GetBinContent(i,j)!=0){
	eff = numerator->GetBinContent(i,j)/denominator->GetBinContent(i,j);
	err = sqrt(eff*(1-eff)/denominator->GetBinContent(i,j));
	efficiency->SetBinContent(i,j,eff);
	efficiency->SetBinError(i,j,err);
	cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
      }
    }
  efficiency->Draw("colztextE");
}

void binomialEfficiency1D(TH1F * numerator,TH1F * denominator){
  TH1F numerator = (TH1F)teff->GetPassedHistogram();
  TH1F denominator = (TH1F)teff->GetTotalHistogram();
  TH1F * efficiency = numerator->Clone("efficiency");
  efficiency->SetXTitle(numerator->GetXTitle());
  efficiency->SetYTitle("#epsilon");
  float eff,err;
  for(int j=0;j<=numerator->GetXaxis()->GetNbins() ;j++){
    if(denominator.GetBinContent(j)!=0){
      eff = numerator.GetBinContent(j)/denominator.GetBinContent(j);
      err = sqrt(eff*(1-eff)/denominator.GetBinContent(j));
      efficiency->SetBinContent(j,eff);
      efficiency->SetBinError(j,err);
      cout<<"1Deff "<<j<<" "<<eff<<" +/- "<<err<<endl;
    }
  }
  efficiency->Draw("E");
  efficiency->SetMarkerColor(kRed);
  efficiency->SetMarkerStyle(23);
  efficiency->SetMarkerSize(0.5);
}
      
void Draw1D(string savedname,string theXtitle,string folder,TFile * theFile){
  TH1F * histoArray[7  ];
  for(int selection=0;selection<7  ;selection++){ //Loop over the different histograms
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
    }else if(selection==5){
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kBlack);
    }else if(selection==6){
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kYellow);
    } 
    histoArray[selection]->SetXTitle(theXtitle.c_str());
  }
  Ca0->SaveAs((folder+savedname+".png").c_str());
  Ca0->Clear();
}

void Draw2D(string savedname,string theXtitle,string theYtitle,string folder,TFile * theFile){
  TH2F * histoArray[7  ];
  for(int selection=0;selection<7  ;selection++){ //Loop over the different histograms
    //std::string histo = std::to_string(selection); THIS WORKS IN C++ BUT NOT IN CINT
    char histo[20];
    sprintf(histo,"%d",selection);
    cout<<savedname+"_"+histo<<endl;
    histoArray[selection] = (TH2F*)(theFile->Get((savedname+"_"+histo).c_str()));
    if(selection==0 || selection==1 || selection==5 || selection==6){
      histoArray[selection]->SetXTitle(theXtitle.c_str());
      histoArray[selection]->SetYTitle(theYtitle.c_str());
      histoArray[selection]->Draw("colz");
      Ca0->SaveAs(("gen/"+savedname+"_"+histo+".png").c_str());
      Ca0->Clear();
    } 
  }
}

//0 gen                                                                                                                                                                                                                                      
//1 acc                                                                                                                                                                                                                                      
//2 194                                                                                                                                                                                                                                      
//3 195                                                                                                                                                                                                                                      
//4 205                                                                                                                                                                                                                                      
//5 OR                                                                                                                                                                                                                                       
//6 acc*OR

