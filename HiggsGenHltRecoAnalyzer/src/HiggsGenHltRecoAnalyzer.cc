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
//#include <memory>

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
#define Nhltpaths 442
#define Maxselection 7

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
      TH1F * higgsEta[Maxselection];
      TH1F * higgsPhi[Maxselection];
      TH1F * higgsP[Maxselection];
      TH1F * higgsPt[Maxselection];
      TH2F * phi1phi2[Maxselection];
      TH2F * eta1eta2[Maxselection];
      TH2F * pt1pt2[Maxselection];
      TH2F * wide_pt1pt2[Maxselection];
      const Double_t bins[17]={20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
      bool fillhisto[Maxselection];
      string arrayHLTpathsNames[Nhltpaths];
      bool filled_hlt_names; 
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
HiggsGenHltRecoAnalyzer::HiggsGenHltRecoAnalyzer(const edm::ParameterSet& iConfig){
   //now do what ever initialization is needed
  theGenParticleLabel = iConfig.getParameter<edm::InputTag>("GenParticleLabel");
  theHltInputLabel    = iConfig.getParameter<edm::InputTag>("hltInputTag");
}


HiggsGenHltRecoAnalyzer::~HiggsGenHltRecoAnalyzer(){
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
   bool fillhisto[Maxselection] = { false };
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
       
       float ptLead = photon1->pt(); float ptTrail = photon0->pt();
       float phiLead = photon1->phi(); float phiTrail = photon0->phi();
       float etaLead = photon1->eta(); float etaTrail = photon0->eta();
       
       if(ptTrail>ptLead){//invert if lead is not lead
	 std::cout<<"lead was not lead"<<std::endl;
	 ptLead = photon0->pt(); ptTrail = photon1->pt();  
	 phiLead = photon0->phi(); phiTrail = photon1->phi();
	 etaLead = photon0->eta(); etaTrail = photon1->eta();
       
       }

       std::cout<<"fillhisto1 ";
       for(selection=0;selection<Maxselection;selection++)
	 std::cout<<fillhisto[selection]<<" ";
       std::cout<<std::endl;

       //Definition for the interesting bits
       //0 gen
       //1 acc
       //2 194*acc
       //3 195*acc
       //4 205*acc
       //5 OR*acc
       //6 OR*acc ???????
       fillhisto[0]=true; //all gen level
       fillhisto[1]=!(fabs(etaLead) > 2.5 || fabs(etaTrail) > 2.5 || (fabs(etaLead) > 1.4442 && fabs(etaLead) < 1.566) || (fabs(etaTrail) > 1.4442 && fabs(etaTrail) < 1.566)); //acceptnace cuts
       fillhisto[2]=allHLTResults[194]&&fillhisto[1];
       fillhisto[3]=allHLTResults[195]&&fillhisto[1];
       fillhisto[4]=allHLTResults[205]&&fillhisto[1];
       fillhisto[5]=(allHLTResults[194]||allHLTResults[195]||allHLTResults[205])&&fillhisto[1];
       fillhisto[6]=(fillhisto[2]||fillhisto[3]||fillhisto[4])&&fillhisto[1];

       std::cout<<"fillhisto2 ";
       for(selection=0;selection<Maxselection;selection++)
	 std::cout<<fillhisto[selection]<<" ";
       std::cout<<std::endl;
	   
       
       for(selection=0;selection<Maxselection;selection++){ //Loop over the different histograms
	 if(fillhisto[selection]){ //all hitograms below will be filled up if the boolean is true.
	   phi1phi2[selection]->Fill(phiLead,phiTrail);
	   eta1eta2[selection]->Fill(etaLead,etaTrail);
	   pt1pt2[selection]->Fill(ptLead,ptTrail);
	   wide_pt1pt2[selection]->Fill(ptLead,ptTrail);
	   higgsEta[selection]->Fill(p.p4().eta());
	   higgsPhi[selection]->Fill(p.p4().phi());
	   higgsP[selection]->Fill(sqrt(p.p4().Vect().Dot(p.p4().Vect()))); //TO CHECK
	   higgsPt[selection]->Fill(p.pt());
	   ptLeadhisto[selection]->Fill(ptLead);
	   ptTrailhisto[selection]->Fill(ptTrail);
	   massHiggshisto[selection]->Fill(p.mass());
	   Particle::LorentzVector diphoton = photon0->p4()+photon1->p4();
	   massDiphotonhisto[selection]->Fill(sqrt(diphoton.Dot(diphoton)));
	 }
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
    higgsEta[selection] = new TH1F (("higgsEta_"+histo).c_str(),("Higgs #eta_histo"+histo).c_str(),100,-5,5);
    higgsPhi[selection] = new TH1F (("higgsPhi_"+histo).c_str(),("Higgs #phi_histo"+histo).c_str(),180,-3.15,3.15);
    higgsP[selection] = new TH1F (("higgsP_"+histo).c_str(),("Higgs P_histo"+histo).c_str(),500,0,1200);
    higgsPt[selection] = new TH1F (("higgsPt_"+histo).c_str(),("Higgs P_{T}_histo"+histo).c_str(),500,0,1200);
    phi1phi2[selection] = new TH2F (("phi1phi2_"+histo).c_str(),"#phi correlation #gamma #gamma",180,-3.15,3.15,180,-3.15,3.15);
    eta1eta2[selection] = new TH2F (("eta1eta2_"+histo).c_str(),"#eta correlation #gamma #gamma",100,-5,5,100,-5,5);
    pt1pt2[selection] = new TH2F (("pt1pt2_"+histo).c_str(),"p_{T} correlation #gamma #gamma",100,20.,120.,100,20.,120.);
    wide_pt1pt2[selection]= new TH2F (("wide_pt1pt2_"+histo).c_str(),";p_T #gamma Lead;p_T #gamma Trail;#epsilon_",17-1,bins,17-1,bins);
  }
}
// ------------ method called once each job just after ending the event loop  ------------
void 
HiggsGenHltRecoAnalyzer::endJob(){
  std::cout<<"beginning endJob"<<std::endl;
  theFileOut->cd();
  for(selection=0;selection<Maxselection;selection++){ //Loop over the different histograms
    ptLeadhisto[selection]->Write();
    ptTrailhisto[selection]->Write();
    massDiphotonhisto[selection]->Write();
    massHiggshisto[selection]->Write();
    higgsEta[selection]->Write();
    higgsPhi[selection]->Write(); 
    higgsP[selection]->Write(); 
    higgsPt[selection]->Write();
    phi1phi2[selection]->Write();
    eta1eta2[selection]->Write();
    pt1pt2[selection]->Write();
    std::cout<<"Writing wide_pt1pt2"<<std::endl;
    wide_pt1pt2[selection]->Write();
  }
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
