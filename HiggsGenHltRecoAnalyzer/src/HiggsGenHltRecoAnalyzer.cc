// -*- C++ -*-
//
// Package:    HiggsGenHltRecoAnalyzer
// Class:      HiggsGenHltRecoAnalyzer
// 
/**\class HiggsGenHltRecoAnalyzer HiggsGenHltRecoAnalyzer.cc genAnalyzer/HiggsGenHltRecoAnalyzer/src/HiggsGenHltRecoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Camilo Andres Carrillo Montoya,42 R-021,+41227671624,
//         Created:  Wed Dec  4 18:13:51 CET 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TEfficiency.h"
#define Nhltpaths 442
#define Maxselection 3

using namespace std;
using namespace edm;
using namespace reco;

//
// class declaration
//

class HiggsGenHltRecoAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HiggsGenHltRecoAnalyzer(const edm::ParameterSet&);
      ~HiggsGenHltRecoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      TFile * theFileOut;
      TH1F * ptLeadhisto[Maxselection];
      TH1F * ptTrailhisto[Maxselection];
      TH1F * massDiphotonhisto[Maxselection];
      TH1F * massHiggshisto[Maxselection];
      TH1F * numerator[Maxselection];
      TH1F * denominator[Maxselection];
      TH1F * higgsEtaHist[Maxselection];
      TH1F * higgsPhiHist[Maxselection];
      TH1F * higgsPHist[Maxselection];
      TH1F * higgsPtHist[Maxselection];
      TH2F * phi1phi2;
      TH2F * eta1eta2;
      TH2F * pt1pt2;
      TEfficiency * Effbit[Nhltpaths][Maxselection];
      TEfficiency * Eff_bit_194_195_205[Maxselection];
      string arrayHLTpathsNames[Nhltpaths];
      bool filled_hlt_names; 
      bool fillhisto[Maxselection];
      int selection;   
   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      edm::InputTag theGenParticleLabel;
      edm::InputTag theHltInputLabel;


  
      
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HiggsGenHltRecoAnalyzer::HiggsGenHltRecoAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  theGenParticleLabel = iConfig.getParameter<edm::InputTag>("GenParticleLabel");
  theHltInputLabel    = iConfig.getParameter<edm::InputTag>("hltInputTag");

}


HiggsGenHltRecoAnalyzer::~HiggsGenHltRecoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
HiggsGenHltRecoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   // Access Gen Particles
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel(theGenParticleLabel, genParticles );
   std::cout<<"before the if genParticles.isValid()"<<std::endl;
   if(genParticles.isValid()) std::cout<<"valid"<<std::endl;
   else return;

   // Access Trigger Results
   edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
   iEvent.getByLabel(theHltInputLabel,hltTriggerResultHandle);

   if(filled_hlt_names==false){
     const edm::TriggerNames & triggerNames = iEvent.triggerNames(*hltTriggerResultHandle);
     std::cout<<triggerNames.triggerName(32)<<std::endl; 
     //std::cout<<arrayHLTpathsNames.size()<<std::endl;
     for(int i=0;i<Nhltpaths;i++){
       arrayHLTpathsNames[i]=triggerNames.triggerName(i);
     }
     filled_hlt_names=true;
     //std::cout<<arrayHLTpathsNames.size()<<std::endl;
   }

   
   int hltCount= hltTriggerResultHandle->size();
   bool allHLTResults[Nhltpaths] = { false };
   int bin;
   
   if(!hltTriggerResultHandle.isValid()) {
     std::cout << "invalid handle for HLT TriggerResults" << std::endl;
   }else{
     for(bin = 0 ; bin < hltCount ; bin++){
       allHLTResults[bin] = hltTriggerResultHandle->accept(bin);
       //std::cout<<"bit:"<<i<<" "<<allHLTResults[i]<<std::endl;
     }
   }
   
   for (size_t i = 0; i < genParticles->size(); i++){
     const Candidate & p = (*genParticles)[i];
     //Higgs in status 3
     if (p.pdgId() == 25 && p.status() == 3){
       std::cout<<"This should happen once per event Higgs mass:"<<p.mass()<<" status:"<<p.status()<<std::endl;
       //size_t nd = p.numberOfDaughters(); 
       //for(size_t j = 0; j < nd; ++ j ) {
       //const Candidate * d = p.daughter(j);
       const Candidate * photon0 = p.daughter(0);
       const Candidate * photon1 = p.daughter(1);
       if (photon0->pdgId()!=22) break; //must be photons
       if (photon1->pdgId()!=22) break; //must be photons
       
       fillhisto[0]=true;
       fillhisto[2]=allHLTResults[194]+allHLTResults[195]+allHLTResults[205];
       
       float ptLead = photon1->pt(); float ptTrail = photon0->pt();
       float phiLead = photon1->phi(); float phiTrail = photon0->phi();
       float etaLead = photon1->eta(); float etaTrail = photon0->eta();
       
       if(ptTrail>ptLead){//invert if lead is not lead
	 std::cout<<"lead was not lead"<<std::endl;
	 ptLead = photon0->pt(); ptTrail = photon1->pt();  
	 phiLead = photon0->phi(); phiTrail = photon1->phi();
	 etaLead = photon0->eta(); etaTrail = photon1->eta();
       }

       if(etaLead > 2.5 || etaTrail > 2.5 
	  || ( etaLead > 1.4442 && etaTrail < 1.566 ) 
	  || ( etaTrail > 1.4442 && etaTrail < 1.566)) fillhisto[1]=false;
       
       phi1phi2->Fill(phiLead,phiTrail);
       eta1eta2->Fill(etaLead,etaTrail);
       pt1pt2->Fill(ptLead,ptTrail);
       
       for(selection=0;selection<Maxselection;selection++){ //Loop over the different histograms
	 if(!fillhisto[selection]) break; //all hitograms below will be filled up if the boolean is true.
	 for(bin = 0 ; bin < hltCount ; bin++){
	   if(allHLTResults[bin])numerator[selection]->Fill(bin);
	   denominator[selection]->Fill(bin);
	 }
	 higgsEtaHist[selection]->Fill(p.p4().eta());
	 higgsPhiHist[selection]->Fill(p.p4().phi());
	 higgsPHist[selection]->Fill(sqrt(p.p4().Vect().Dot(p.p4().Vect()))); //TO CHECK
	 higgsPtHist[selection]->Fill(p.pt());
	 ptLeadhisto[selection]->Fill(ptLead);
	 ptTrailhisto[selection]->Fill(ptTrail);
	 //Efficiency per bit
	 for(int i=0;i<Nhltpaths;i++){
	   Effbit[i][selection]->Fill(allHLTResults[i],ptLead,ptTrail);
	 }
	 Eff_bit_194_195_205[selection]->Fill(allHLTResults[194]+allHLTResults[195]+allHLTResults[205],ptLead,ptTrail);
	 massHiggshisto[selection]->Fill(p.mass());
	 Particle::LorentzVector diphoton = photon0->p4()+photon1->p4();
	 massDiphotonhisto[selection]->Fill(sqrt(diphoton.Dot(diphoton)));
       }
     }
   }
}

void HiggsGenHltRecoAnalyzer::beginJob(){
  filled_hlt_names=false;

  theFileOut = new TFile("genAnalyzer.root", "RECREATE");

  for(selection=0;selection<Maxselection;selection++){ //Loop over the different histograms
    std::string histo = std::to_string(selection);
    ptLeadhisto[selection] = new TH1F (("ptLead_"+histo).c_str(),("ptLeadhisto_"+histo).c_str(),100,0.,150.);
    ptTrailhisto[selection] = new TH1F (("ptTrail_"+histo).c_str(),("ptTrailhisto_"+histo).c_str(),100,0.,150.);
    massDiphotonhisto[selection] = new TH1F (("massDiphoton_"+histo).c_str(),("massDiphotonhisto_"+histo).c_str(),100,80.,150.);
    massHiggshisto[selection]= new TH1F (("massHiggs_"+histo).c_str(),("massHiggshisto_"+histo).c_str(),100,80.,170.);
    higgsEtaHist[selection] = new TH1F (("higgsEta_"+histo).c_str(),("GEN Higgs #eta_histo"+histo).c_str(),100,-5,5);
    higgsPhiHist[selection] = new TH1F (("higgsPhi_"+histo).c_str(),("GEN Higgs #phi_histo"+histo).c_str(),180,-3.15,3.15);
    higgsPHist[selection] = new TH1F (("higgsP_"+histo).c_str(),("GEN Higgs P_histo"+histo).c_str(),500,0,1200);
    higgsPtHist[selection] = new TH1F (("higgsPt_"+histo).c_str(),("GEN Higgs P_{T}_histo"+histo).c_str(),500,0,1200);  
    numerator[selection] = new TH1F (("numerator_"+histo).c_str(),("numerator_histo"+histo).c_str(),Nhltpaths,-0.5,Nhltpaths-0.5);
    denominator[selection] = new TH1F (("denominator_"+histo).c_str(),("denominator_histo"+histo).c_str(),Nhltpaths,-0.5,Nhltpaths-0.5);
    const Double_t bins[15]={30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
    char histo_name[128];
    for(int i=0;i<Nhltpaths;i++){
      std::string bin = std::to_string(i);
      sprintf(histo_name,"bin%s",(bin+"_"+histo).c_str());
      Effbit[i][selection]= new TEfficiency (histo_name,";p_T #gamma Lead;p_T #gamma Trail;#epsilon_",14,bins,14,bins);
    }
    
    sprintf(histo_name,"Eff_bit_194_195_205_%d",selection);
    Eff_bit_194_195_205[selection] = new TEfficiency (histo_name,(";p_T #gamma Lead;p_T #gamma Trail;#epsilon_"+histo).c_str(),14,bins,14,bins);
  }
  phi1phi2 = new TH2F ("phi1phi2","#phi correlation #gamma #gamma",180,-3.15,3.15,180,-3.15,3.15);
  eta1eta2 = new TH2F ("eta1eta2","#eta correlation #gamma #gamma",100,-5,5,100,-5,5);
  pt1pt2 = new TH2F ("pt1pt2","p_{T} correlation #gamma #gamma",100,20.,120.,100,20.,120.);
}
// ------------ method called once each job just after ending the event loop  ------------
void 
HiggsGenHltRecoAnalyzer::endJob(){

  theFileOut->cd();

  for(selection=0;selection<Maxselection;selection++){ //Loop over the different histograms
    ptLeadhisto[selection]->Write();
    ptTrailhisto[selection]->Write();
    massDiphotonhisto[selection]->Write();
    massHiggshisto[selection]->Write();
    numerator[selection]->Write();
    denominator[selection]->Write();
  
    higgsEtaHist[selection]->Write();
    higgsPhiHist[selection]->Write(); 
    higgsPHist[selection]->Write(); 
    higgsPtHist[selection]->Write();
    for(int i=0;i<Nhltpaths;i++){
      Effbit[i][selection]->SetTitle(arrayHLTpathsNames[i].c_str());
      Effbit[i][selection]->Write();
    }
    Eff_bit_194_195_205[selection]->Write();
  }
  
  phi1phi2->Write();
  eta1eta2->Write();
  pt1pt2->Write();
  
  theFileOut->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
HiggsGenHltRecoAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HiggsGenHltRecoAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HiggsGenHltRecoAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HiggsGenHltRecoAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HiggsGenHltRecoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiggsGenHltRecoAnalyzer);
