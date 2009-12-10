#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <cmath>
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;

#ifdef __MAKECINT__
#pragma link C++ class std::vector<float>;
#endif

void energyResolution(const TString &file = "ntuple.root") {
  const float PI = 3.14159;
  
  TFile f(file);

  TTree *CollectionTree = (TTree *) f.Get("CollectionTree");

  vector<float> *T2CaEta = 0;
  vector<float> *T2CaPhi = 0;
  vector<float> *T2CaEmE = 0;
  vector<float> *T2CaRawEmE = 0;
  vector<float> *RingEta = 0;
  vector<float> *RingPhi = 0;
  vector<float> *RingEmEt = 0;
  vector<float> *Ele_eta = 0;
  vector<float> *Ele_phi = 0;
  vector<float> *Ele_e = 0;
  unsigned int T2CaNclus;
  unsigned int RingNclus;
  int Ele_nc;

  CollectionTree->SetBranchAddress("Ele_nc", &Ele_nc);
  CollectionTree->SetBranchAddress("Ele_eta", &Ele_eta);
  CollectionTree->SetBranchAddress("Ele_phi", &Ele_phi);
  CollectionTree->SetBranchAddress("Ele_e", &Ele_e);
  
  CollectionTree->SetBranchAddress("T2CaNclus",       &T2CaNclus);
  CollectionTree->SetBranchAddress("Ringer_NClusters",&RingNclus);
  CollectionTree->SetBranchAddress("T2CaEta",         &T2CaEta);
  CollectionTree->SetBranchAddress("T2CaPhi",         &T2CaPhi);
  CollectionTree->SetBranchAddress("T2CaEmE",         &T2CaEmE);
  CollectionTree->SetBranchAddress("T2CaRawEmE",      &T2CaRawEmE);
  CollectionTree->SetBranchAddress("Ringer_LVL2_Et",  &RingEmEt);
  CollectionTree->SetBranchAddress("Ringer_LVL2_Eta", &RingEta);
  CollectionTree->SetBranchAddress("Ringer_LVL2_Phi", &RingPhi);

  TFile fW("ringer_hists.root", "RECREATE");
  TH1F *Eres1 = new TH1F("Eres1", "Energy Resolution between Ringer and T2Calo ; Energy Resolution ; Count ", 400, -2., 2.);
  TH2F *Eres2 = new TH2F("Eres2", "Energy Resolution between Ringer and T2Calo ; Energy of T2Calo [GeV] ; Energy Resolution", 100, 0., 100., 400, -2., 2);
  TH2F *Eres3 = new TH2F("Eres3", "Energy Resolution between Ringer and T2Calo ; Energy of Ringer [GeV] ; Energy Resolution", 100, 0., 100., 400, -2., 2);
  TH1F *ClusMismatch = new TH1F("ClusMismatch", "Cluster mismatch between T2Calo and Ringer ; Difference between cluster counts ; Count", 20, -10., 10.);
  TH1F *EtaMismatch = new TH1F("EtaMismatch", "Difference between T2Calo \\eta and Ringer \\eta ; Difference ; Count", 20, -1.0, 1.0);
  TH1F *PhiMismatch = new TH1F("PhiMismatch", "Difference between T2Calo \\phi and Ringer \\phi ; Difference ; Count", 20, -1.0, 1.0);
  TH2F *RingerEtaPhiMismatch = new TH2F("RingerEtaPhiMismatch", "(\\eta,\\phi) positions from Ringer where no match to T2Calo was found ; \\eta ; \\phi", 128, -3.2, 3.2, 128, -3.2, 3.2);
  
  TH1F *Eres1_off = new TH1F("Eres1_off", "Energy Resolution between Ringer and Offline ; Energy Resolution ; Count ", 400, -2., 2.);
  TH2F *Eres2_off = new TH2F("Eres2_off", "Energy Resolution between Ringer and Offline ; Energy of T2Calo [GeV] ; Energy Resolution", 100, 0., 100., 400, -2., 2);
  TH2F *Eres3_off = new TH2F("Eres3_off", "Energy Resolution between Ringer and Offline ; Energy of Ringer [GeV] ; Energy Resolution", 100, 0., 100., 400, -2., 2);
  TH1F *ClusMismatch_off = new TH1F("ClusMismatch_off", "Cluster mismatch between T2Calo and Offline; Difference between cluster counts ; Count", 20, -10., 10.);
  TH1F *EtaMismatch_off = new TH1F("EtaMismatch_off", "Difference between T2Calo \\eta and Offline \\eta ; Difference ; Count", 20, -1.0, 1.0);
  TH1F *PhiMismatch_off = new TH1F("PhiMismatch_off", "Difference between T2Calo \\phi and Offline \\phi ; Difference ; Count", 20, -1.0, 1.0);
  TH2F *RingerEtaPhiMismatch_off = new TH2F("RingerEtaPhiMismatch_off", "(\\eta,\\phi) positions from Ringer where no match to Offline was found ; \\eta ; \\phi", 128, -3.2, 3.2, 128, -3.2, 3.2);
  
  for (unsigned long k = 0; k < CollectionTree->GetEntries(); ++k) {
    CollectionTree->GetEntry(k);
    if (T2CaNclus != RingNclus) {
      cout << "Cluster number from T2Calo is different from Ringer's on event " << k << "!" << endl;
    }
    ClusMismatch->Fill(T2CaNclus - RingNclus);
    ClusMismatch_off->Fill(Ele_nc - RingNclus);
    // For each cluster
    for (unsigned int clus = 0; clus < RingNclus; ++clus) {
      // Match in eta x phi
      bool matched;
      float lastDeltaEta, lastDeltaPhi;
      unsigned int bestT2clus, bestOffclus;
      
      matched = false; // Minimum match in (0.2, 0.2) window
      lastDeltaEta = 0.2;
      lastDeltaPhi = 0.2;
      bestT2clus = T2CaNclus;
      for (unsigned int t2clus = 0; t2clus < T2CaNclus; ++t2clus) {
        float deltaEta = fabs(T2CaEta->at(t2clus) - RingEta->at(clus));
	float deltaPhi = fabs(PI - fabs(PI - fabs(T2CaPhi->at(t2clus) - RingPhi->at(clus))));
        if ( (deltaEta < lastDeltaEta) &&
	     (deltaPhi < lastDeltaPhi) ) {
	  lastDeltaEta = deltaEta;
	  lastDeltaPhi = deltaPhi;
	  matched = true;
	  bestT2clus = t2clus;
	}
      }
      if (matched) {
        EtaMismatch->Fill(lastDeltaEta);
        PhiMismatch->Fill(lastDeltaPhi);
	float resolution = ((RingEmEt->at(clus)*cosh(RingEta->at(clus))) - T2CaEmE->at(bestT2clus))/(T2CaEmE->at(bestT2clus));
	Eres1->Fill(resolution);
	Eres2->Fill(T2CaEmE->at(bestT2clus)/1.e3, resolution);
	Eres3->Fill(RingEmEt->at(clus)*cosh(RingEta->at(clus))/1.e3, resolution);
      } else {
        RingerEtaPhiMismatch->Fill(RingEta->at(clus), RingPhi->at(clus));
      }
      
      matched = false; // Minimum match in (0.2, 0.2) window
      lastDeltaEta = 0.2;
      lastDeltaPhi = 0.2;
      bestOffclus = Ele_nc;
      for (unsigned int offclus = 0; offclus < Ele_nc; ++offclus) {
        float deltaEta = fabs(Ele_eta->at(offclus) - RingEta->at(clus));
	float deltaPhi = fabs(PI - fabs(PI - fabs(Ele_phi->at(offclus) - RingPhi->at(clus))));
        if ( (deltaEta < lastDeltaEta) &&
	     (deltaPhi < lastDeltaPhi) ) {
	  lastDeltaEta = deltaEta;
	  lastDeltaPhi = deltaPhi;
	  matched = true;
	  bestOffclus = offclus;
	}
      }
      if (matched) {
        EtaMismatch_off->Fill(lastDeltaEta);
        PhiMismatch_off->Fill(lastDeltaPhi);
	float resolution = ((RingEmEt->at(clus)*cosh(RingEta->at(clus))) - Ele_e->at(bestOffclus))/(Ele_e->at(bestOffclus));
	Eres1_off->Fill(resolution);
	Eres2_off->Fill(Ele_e->at(bestOffclus)/1.e3, resolution);
	Eres3_off->Fill(RingEmEt->at(clus)*cosh(RingEta->at(clus))/1.e3, resolution);
      } else {
        RingerEtaPhiMismatch_off->Fill(RingEta->at(clus), RingPhi->at(clus));
      }
    }

  }

  f.Close();
  
  Eres1->Write();
  Eres2->Write();
  Eres3->Write();
  ClusMismatch->Write();
  EtaMismatch->Write();
  PhiMismatch->Write();
  RingerEtaPhiMismatch->Write();
  Eres1_off->Write();
  Eres2_off->Write();
  Eres3_off->Write();
  ClusMismatch_off->Write();
  EtaMismatch_off->Write();
  PhiMismatch_off->Write();
  RingerEtaPhiMismatch_off->Write();

  fW.Close();
}

