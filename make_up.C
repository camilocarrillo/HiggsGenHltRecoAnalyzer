#include <iostream>
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>
#include "TLegend"

//#define Maxselection 1
//#define Nhltpaths 442

void make_up(){
  gROOT->Reset();
  //  gStyle->SetOptStat(1111);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("2.2f");
  
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/higgs/yy/hlt/CMSSW_5_3_2_patch4/src/genAnalyzer/GenAnalyzer/genAnalyzer.root");
<<<<<<< HEAD
  TFile * theFile = new TFile("genAnalyzer.root");
  //TFile * theFile = new TFile("HiggsGenHltRecoAnalyzer/test/genAnalyzer.root");
=======
  TFile * theFile = new TFile("HiggsGenHltRecoAnalyzer/test/genAnalyzer.root");
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/all.root");
>>>>>>> 2908c8155495873c1df20881babf7e78b9fd93ad

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
  Draw1D("higgsP","#p (GeV)","gen/",theFile); 

  
  //turnon curve
  //Interesting bits
  
  //  string interesting_bits[]={"418","6","416","8","5","412","233","7","234","10","187","9","22","198","430","404","188","199","21","190","197","208","201","207","195","196","206","193","424","192","204","194","203","205","209","189","200","191"};
  
  //string in_bits[]={"418","6"}
  //for(int k=0;k<2;k++){
  
  //Definition for the interesting bits                                                                                                  
  //0 gen                                                                                                                                
  //1 acc                                                                                                                                
  //2 194*acc                                                                                                                                
  //3 195*acc                                                                                                                                
  //4 205*acc                                                                                                                                
  //5 OR*acc                                                                                                                                 
  //6 OR*acc                                                                                                                                 
  

<<<<<<< HEAD
  TH2F * numerator;
  TH2F * denominator;
  cout<<"going for 2D"<<endl;
=======
  for(int k=0;k<38;k++){//efficiency hlt/acc = hlt/gen / acc/gen
    //cout<<k<<endl;
    TEfficiency * Effbit_num = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]+"_1").c_str()));
    TEfficiency * Effbit_den = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]+"_2").c_str()));
    if(!Effbit) cout<<"Effbit not found"<<endl;
    renormalized_binomialEfficiency2D(Effbit_num,Effbit_den);
    Ca0->SaveAs(("hlt/acc_bit_"+interesting_bits[k]+"_"+Effbit_num->GetTitle()+"_efficiency.png").c_str());
    Ca0->Clear();
  }
>>>>>>> 2908c8155495873c1df20881babf7e78b9fd93ad

  numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_5"));
  denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,true);
  Ca0->SaveAs(("hlt/or_over_acc_efficiency.png"));
  Ca0->Clear();
  binomialEfficiency2D(numerator,denominator,false);
  Ca0->SaveAs(("hlt/eff_wide_pt1pt2.png"));
  Ca0->Clear();
  
  numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_4"));
  denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  cout<<"calling binomial efficiency 4/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,true);
  Ca0->SaveAs(("hlt/205_over_acc_efficiency.png"));
  Ca0->Clear();
  
  numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_3"));
  denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  cout<<"calling binomial efficiency 3/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,true);
  Ca0->SaveAs(("hlt/195_over_acc_efficiency.png"));
  Ca0->Clear();
  
  numerator  = (TH2F*) (theFile->Get("wide_pt1pt2_2"));
  denominator= (TH2F*) (theFile->Get("wide_pt1pt2_1"));
  cout<<"calling binomial efficiency 2/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,true);
  Ca0->SaveAs(("hlt/194_over_acc_efficiency.png"));
  Ca0->Clear();
  
  numerator  = (TH2F*) (theFile->Get("pt1pt2_5"));
  denominator= (TH2F*) (theFile->Get("pt1pt2_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,false);
  Ca0->SaveAs(("hlt/eff_pt1pt2.png"));
  Ca0->Clear();

  numerator  = (TH2F*) (theFile->Get("eta1eta2_5"));
  denominator= (TH2F*) (theFile->Get("eta1eta2_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,false);
  Ca0->SaveAs(("hlt/eff_eta1eta2.png"));
  Ca0->Clear();

  numerator  = (TH2F*) (theFile->Get("phi1phi2_5"));
  denominator= (TH2F*) (theFile->Get("phi1phi2_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator->GetEntries()/denominator->GetEntries()<<endl;
  binomialEfficiency2D(numerator,denominator,false);
  Ca0->SaveAs(("hlt/eff_phi1phi2.png"));
  Ca0->Clear();

  Draw2D("wide_pt1pt2","p_{T} Lead (GeV)","p_{T} Trail (GeV)","gen/",theFile);
  Draw2D("phi1phi2","#phi_{Lead}","#phi_{Trail}","gen/",theFile);
  Draw2D("eta1eta2","#eta_{Lead}","#eta_{Trail}","gen/",theFile);
  Draw2D("pt1pt2","p_{T} Lead (GeV)","p_{T} Trail (GeV)","gen/",theFile);

  //Binomial Efficiency 1D

  TH1F * numerator1D;
  TH1F * denominator1D; 

  numerator1D = (TH1F*) (theFile->Get("higgsEta_5"));
  denominator1D = (TH1F*) (theFile->Get("higgsEta_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_higgsEta.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("higgsPhi_5"));
  denominator1D = (TH1F*) (theFile->Get("higgsPhi_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_higgsPhi.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("higgsP_5"));
  denominator1D = (TH1F*) (theFile->Get("higgsP_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_higgsP.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("higgsPt_5"));
  denominator1D = (TH1F*) (theFile->Get("higgsPt_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_higgsPt.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("massDiphoton_5"));
  denominator1D = (TH1F*) (theFile->Get("massDiphoton_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_massDiphoton.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("massHiggs_5"));
  denominator1D = (TH1F*) (theFile->Get("massHiggs_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_massHiggs.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("ptLead_5"));
  denominator1D = (TH1F*) (theFile->Get("ptLead_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_ptLead.png"));
  Ca0->Clear();

  numerator1D = (TH1F*) (theFile->Get("ptTrail_5"));
  denominator1D = (TH1F*) (theFile->Get("ptTrail_1"));
  cout<<"calling binomial efficiency 5/1 ="<<numerator1D->GetEntries()/denominator1D->GetEntries()<<endl;
  binomialEfficiency1D(numerator1D,denominator1D);
  Ca0->SaveAs(("gen/eff_ptTrail.png"));
  Ca0->Clear();

  exit(0);
}

void binomialEfficiency1D(TH1F * numerator,TH1F * denominator){
  TH1F * efficiency = numerator->Clone("efficiency");
  efficiency->SetXTitle(numerator->GetXaxis()->GetTitle());
  efficiency->SetYTitle("#epsilon");
  for(int j=0;j<=numerator->GetXaxis()->GetNbins() ;j++){
    if(denominator->GetBinContent(j)!=0){
      float eff = numerator->GetBinContent(j)/denominator->GetBinContent(j);
      float err = sqrt(eff*(1-eff)/denominator->GetBinContent(j));
      efficiency->SetBinContent(j,eff);
      efficiency->SetBinError(j,err);
      cout<<"1Deff "<<j<<" "<<eff<<" +/- "<<err<<endl;
    }
  }
  efficiency->Draw("E");
  efficiency->SetMarkerColor(kRed);
  efficiency->SetMarkerStyle(23);
  efficiency->SetMarkerSize(2);
}
      
void Draw1D(string savedname,string theXtitle,string folder,TFile * theFile){
  
  TLegend *leg = new TLegend(0.65,0.93,0.89,0.7);
  
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
      leg->AddEntry(histoArray[selection],"GEN","f");
    }else if(selection==1){
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kRed);
      leg->AddEntry(histoArray[selection],"ACC","f");
    }else if(selection==5){
      histoArray[selection]->Draw("same");
      histoArray[selection]->SetFillColor(kYellow);
      leg->AddEntry(histoArray[selection],"ACCxHLT","f");
    }
    histoArray[selection]->SetXTitle(theXtitle.c_str());
    histoArray[selection]->SetMinimum(0);
  }
  
  
  leg->SetFillColor(0);
  leg->Draw("same");
  
  Ca0->SaveAs((folder+savedname+".png").c_str());
  Ca0->Clear();
}

void binomialEfficiency2D(TH2F * numerator,TH2F * denominator,bool text2D){
  if(!numerator) cout<<"numerator not found"<<endl;
  if(!denominator) cout<<"denominator not found"<<endl;

  TH1F * efficiency = numerator->Clone("efficiency");
  
  efficiency->SetXTitle(numerator->GetXaxis()->GetTitle());
  efficiency->SetYTitle(numerator->GetYaxis()->GetTitle());
  
  float eff,err;
  for(int i=0;i<=numerator->GetXaxis()->GetNbins();i++)
    for(int j=0;j<=numerator->GetYaxis()->GetNbins();j++){
      if(denominator->GetBinContent(i,j)!=0){
	eff = numerator->GetBinContent(i,j)/denominator->GetBinContent(i,j);
	err = sqrt(eff*(1-eff)/denominator->GetBinContent(i,j));
	efficiency->SetBinContent(i,j,eff);
	efficiency->SetBinError(i,j,err);
	cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
      }
    }
  if(text2D==true){
    efficiency->Draw("colztextE");
    //efficiency->Draw("colz");
  }else{
    efficiency->Draw("colz");
  }
}

void Draw2D(string savedname,string theXtitle,string theYtitle,string folder,TFile * theFile){
  TH2F * histoArray[7  ];
  for(int selection=0;selection<7  ;selection++){ //Loop over the different histograms
    //std::string histo = std::to_string(selection); THIS WORKS IN C++ BUT NOT IN CINT
    char histo[20];
    sprintf(histo,"%d",selection);
    cout<<savedname+"_"+histo<<endl;
    histoArray[selection] = (TH2F*)(theFile->Get((savedname+"_"+histo).c_str()));
    if(selection==0 || selection==1 || selection==5 ){
      histoArray[selection]->SetXTitle(theXtitle.c_str());
      histoArray[selection]->SetYTitle(theYtitle.c_str());
      histoArray[selection]->Draw("colz");
      Ca0->SaveAs(("gen/"+savedname+"_"+histo+".png").c_str());
      Ca0->Clear();
    } 
  }
}


