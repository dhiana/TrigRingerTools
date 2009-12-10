#ifndef FILTERLVL2TREE_H
#define FILTERLVL2TREE_H

#ifdef __MAKECINT__
#pragma link C++ class std::vector<float>;
#pragma link C++ class std::vector<unsigned long>;
#pragma link C++ class std::vector<unsigned int>;
#pragma link C++ class std::vector<unsigned short>;
#pragma link C++ class std::vector<long>;
#pragma link C++ class std::vector<unsigned char>;
#pragma link C++ class std::vector<int>;
#pragma link C++ class std::vector<double>;
#pragma link C++ class std::vector<std::vector<double> >;
#endif

//#include "CollectionTree.h"
#include "CollectionTree.C"
#include "Threshold.h"
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include <vector>

class FilterLvl2Tree : public CollectionTree {
  
  float deltaEtaMinimumMatch;
  float deltaPhiMinimumMatch;
  UInt_t numberOfRingsOnRoI;

  float CutL2Et;
  float CutL2Et2;
  float CutL2Rcore;
  float CutL2Eratio;
  float CutL2HadE;

  std::vector<unsigned short> *accepted;

  public:

  void setStatusOfMyBranches();
  
  FilterLvl2Tree(TTree *tree, bool setOnlyMyBranches = true)
    : CollectionTree(tree), accepted(0)
  {
    CutL2Et = 10.e3;
    CutL2Et2 = 90.e3;
    CutL2Rcore = 0.85;
    CutL2Eratio = 0.60;
    CutL2HadE = 2.e3;
    deltaEtaMinimumMatch = 0.2f;
    deltaPhiMinimumMatch = 0.2f;
    numberOfRingsOnRoI = 100;
    
    // Set only interesting pointers?
    if (setOnlyMyBranches) {
      setStatusOfMyBranches();
    }

    fChain->SetCacheSize(0); // ROOT bug correction!
  }

  FilterLvl2Tree *filter(const Threshold &thLvl1, Long64_t &TotalRoI, Long64_t &TotalRoIPassed);
  void Fill();
  
  inline float doDeltaEta(float eta1, float eta2) {
    return fabsf(eta1 - eta2);
  }

  inline float doDeltaPhi(float phi1, float phi2) {
    return fabsf(TMath::Pi() - fabsf(TMath::Pi() - fabsf(phi1 - phi2)));
  }
  
};

void FilterLvl2Tree::Fill() {
  fChain->Fill();
}

void FilterLvl2Tree::setStatusOfMyBranches() {
  fChain->SetBranchStatus("*",0);

  // If T2CaloAccept is to be used, set it yourself!

  fChain->SetBranchStatus("L1Em_nRoI",1); 
  fChain->SetBranchStatus("L1Em_Core",1);
  fChain->SetBranchStatus("L1Em_EmClus",1);
  fChain->SetBranchStatus("L1Em_EmIsol",1);
  fChain->SetBranchStatus("L1Em_HdIsol",1);
  fChain->SetBranchStatus("L1Em_HdCore",1); 
  fChain->SetBranchStatus("L1Em_eta",1);
  fChain->SetBranchStatus("L1Em_phi",1);

  fChain->SetBranchStatus("Type",1);
  fChain->SetBranchStatus("PtGen",1);
  fChain->SetBranchStatus("EtaGen",1);
  fChain->SetBranchStatus("PhiGen",1);

  fChain->SetBranchStatus("T2CaNclus",1);
  fChain->SetBranchStatus("T2CaEmE",1); 
  fChain->SetBranchStatus("T2CaEta",1);
  fChain->SetBranchStatus("T2CaPhi",1);
  fChain->SetBranchStatus("T2CaRcore",1);
  fChain->SetBranchStatus("T2CaEratio",1); 
  fChain->SetBranchStatus("T2CaHadE",1);
  fChain->SetBranchStatus("T2CaEmES0",1);
  fChain->SetBranchStatus("T2CaEmES1",1);
  fChain->SetBranchStatus("T2CaEmES2",1);
  fChain->SetBranchStatus("T2CaEmES3",1);
  fChain->SetBranchStatus("T2CaWidth",1);
  fChain->SetBranchStatus("T2CaF73",1);
  fChain->SetBranchStatus("T2CaHadES0",1);
  fChain->SetBranchStatus("T2CaHadES1",1);
  fChain->SetBranchStatus("T2CaHadES2",1);
  fChain->SetBranchStatus("T2CaHadES3",1);

  // T2 Inner Detector  
  fChain->SetBranchStatus("T2IdNtracks",1);
  fChain->SetBranchStatus("T2IdEtac",1);
  fChain->SetBranchStatus("T2IdPhic",1); 
  fChain->SetBranchStatus("T2IdPt",1);
  fChain->SetBranchStatus("T2IdAlgo",1);

  fChain->SetBranchStatus("eg_nc_EF",1); 
  fChain->SetBranchStatus("eg_e_EF",1);
  fChain->SetBranchStatus("eg_eta_EF",1);
  fChain->SetBranchStatus("eg_phi_EF",1);
  fChain->SetBranchStatus("eg_IsEM_EF",1); 
  fChain->SetBranchStatus("eg_cl_et_EF",1);
  fChain->SetBranchStatus("eg_trkmatchnt_EF",1);
  
  fChain->SetBranchStatus("Ele_nc",1);
  fChain->SetBranchStatus("Ele_e",1); 
  fChain->SetBranchStatus("Ele_eta",1);
  fChain->SetBranchStatus("Ele_phi",1);
  fChain->SetBranchStatus("Ele_IsEM",1);
  
  fChain->SetBranchStatus("Ringer_LVL1_Id",1);
  fChain->SetBranchStatus("Ringer_NCells",1); 
  fChain->SetBranchStatus("Ringer_NClusters",1);
  fChain->SetBranchStatus("Ringer_Roi_Id",1);
  fChain->SetBranchStatus("Ringer_LVL1_Eta",1);
  fChain->SetBranchStatus("Ringer_LVL1_Phi",1); 
  fChain->SetBranchStatus("Ringer_DetCells",1);
  fChain->SetBranchStatus("Ringer_PhiResCells",1);
  fChain->SetBranchStatus("Ringer_EtaResCells",1);
  fChain->SetBranchStatus("Ringer_EtaCells",1);
  fChain->SetBranchStatus("Ringer_PhiCells",1);
  fChain->SetBranchStatus("Ringer_ECells",1); 
  fChain->SetBranchStatus("Ringer_LVL2_Et",1);
  fChain->SetBranchStatus("Ringer_LVL2_Eta",1);
  fChain->SetBranchStatus("Ringer_LVL2_Phi",1);
  fChain->SetBranchStatus("Ringer_Rings",1); 

}

FilterLvl2Tree *FilterLvl2Tree::filter(const Threshold &thLvl1, Long64_t &TotalRoI, Long64_t &TotalRoIPassed) {
  fChain->SetBranchStatus("*", 1);
  TTree *output = fChain->CloneTree(0);  // Clone STRUCTURE and don't copy entries!
  FilterLvl2Tree *fout = new FilterLvl2Tree(output);

  fout->accepted = 0;
  fout->fChain->Branch("T2CaloAccept", &fout->accepted);
  
  setStatusOfMyBranches();

  fout->fChain->SetBranchStatus("T2CaloAccept", 1); // Just to be sure

  TotalRoI = 0;
  TotalRoIPassed = 0;

  Long64_t nPassed = 0;                      // Count the number of RoIs passed for statistics
  Long64_t nEntries = fChain->GetEntries();  // Number of entries on the input

  for (Long64_t i = 0; i < nEntries; i++) {
    GetEntry(i);

    // Now process it
    nPassed = 0;                           // Number of RoIs that passed the cuts
    Long64_t roiAfterAppliedLvl1Index = 0; // Index for items after re-application of Lvl1 filter

    fout->T2IdNtracks = 0;
    fout->Ele_nc = 0;
    fout->eg_nc_EF = 0;

    for (Int_t j = 0; j < L1Em_nRoI; j++) {

      bool passedAppliedLvl1 = false;
      bool passedT2Calo = false;
      
      // Make LVL1 already applied cuts again to identify which elements passed
      passedAppliedLvl1 = thLvl1.cut(L1Em_EmClus->at(j), L1Em_EmIsol->at(j), L1Em_HdCore->at(j), L1Em_HdIsol->at(j));

      if (passedAppliedLvl1) {
      
        TotalRoI++;
	
        // T2Calo
        if (T2CaEta->at(roiAfterAppliedLvl1Index) <= 98.0) { // If clustering succeeds
	  bool crack = TMath::Abs(T2CaEta->at(roiAfterAppliedLvl1Index))>2.37;
	  crack = crack || (TMath::Abs(T2CaEta->at(roiAfterAppliedLvl1Index))>1.37 && TMath::Abs(T2CaEta->at(roiAfterAppliedLvl1Index))<1.52);
	  float et =  T2CaEmE->at(roiAfterAppliedLvl1Index)/(TMath::CosH(T2CaEta->at(roiAfterAppliedLvl1Index)) );
	  if ( (et > CutL2Et) && (T2CaRcore->at(roiAfterAppliedLvl1Index) > CutL2Rcore) &&
	       ((T2CaEratio->at(roiAfterAppliedLvl1Index) > CutL2Eratio) || crack) &&
	       ( (T2CaHadES0->at(roiAfterAppliedLvl1Index)/TMath::CosH(T2CaEta->at(roiAfterAppliedLvl1Index)) < CutL2HadE) ||
	         ( (et > CutL2Et2) && (T2CaHadES0->at(roiAfterAppliedLvl1Index)/TMath::CosH(T2CaEta->at(roiAfterAppliedLvl1Index)) < 999.0 ) ) ) ) {
            // Passed T2Calo cuts
	    passedT2Calo = true;
	    nPassed++;
	    TotalRoIPassed++;
          }
	}

        fout->L1Em_Core->push_back(L1Em_Core->at(j));
        fout->L1Em_EmClus->push_back(L1Em_EmClus->at(j));
        fout->L1Em_EmIsol->push_back(L1Em_EmIsol->at(j));
        fout->L1Em_HdCore->push_back(L1Em_HdCore->at(j));
        fout->L1Em_HdIsol->push_back(L1Em_HdIsol->at(j));
        fout->L1Em_eta->push_back(L1Em_eta->at(j));
        fout->L1Em_phi->push_back(L1Em_phi->at(j));

        fout->Type->push_back(Type->at(j));
        fout->PtGen->push_back(PtGen->at(j));
        fout->EtaGen->push_back(EtaGen->at(j));
        fout->PhiGen->push_back(PhiGen->at(j));

        fout->T2CaEmE->push_back(T2CaEmE->at(roiAfterAppliedLvl1Index));
        fout->T2CaEta->push_back(T2CaEta->at(roiAfterAppliedLvl1Index));
        fout->T2CaPhi->push_back(T2CaPhi->at(roiAfterAppliedLvl1Index));
        fout->T2CaRcore->push_back(T2CaRcore->at(roiAfterAppliedLvl1Index));
        fout->T2CaEratio->push_back(T2CaEratio->at(roiAfterAppliedLvl1Index));
        fout->T2CaHadE->push_back(T2CaHadE->at(roiAfterAppliedLvl1Index));
 
        fout->T2CaEmES0->push_back(T2CaEmES0->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaEmES1->push_back(T2CaEmES1->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaEmES2->push_back(T2CaEmES2->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaEmES3->push_back(T2CaEmES3->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaHadES0->push_back(T2CaHadES0->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaHadES1->push_back(T2CaHadES1->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaHadES2->push_back(T2CaHadES2->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaHadES3->push_back(T2CaHadES3->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaWidth->push_back(T2CaWidth->at(roiAfterAppliedLvl1Index)); 
        fout->T2CaF73->push_back(T2CaF73->at(roiAfterAppliedLvl1Index)); 

        if (passedT2Calo)
	  fout->accepted->push_back(1);
	else
	  fout->accepted->push_back(0);

        fout->Ringer_LVL1_Id->push_back(Ringer_LVL1_Id->at(roiAfterAppliedLvl1Index));
        fout->Ringer_NCells->push_back(Ringer_NCells->at(roiAfterAppliedLvl1Index));
        fout->Ringer_Roi_Id->push_back(Ringer_Roi_Id->at(roiAfterAppliedLvl1Index));
        fout->Ringer_LVL1_Eta->push_back(Ringer_LVL1_Eta->at(roiAfterAppliedLvl1Index));
        fout->Ringer_LVL1_Phi->push_back(Ringer_LVL1_Phi->at(roiAfterAppliedLvl1Index));
        fout->Ringer_LVL2_Et->push_back(Ringer_LVL2_Et->at(roiAfterAppliedLvl1Index));
        fout->Ringer_LVL2_Eta->push_back(Ringer_LVL2_Eta->at(roiAfterAppliedLvl1Index));
        fout->Ringer_LVL2_Phi->push_back(Ringer_LVL2_Phi->at(roiAfterAppliedLvl1Index));

        Long64_t NCellsBase = 0;
        if (roiAfterAppliedLvl1Index != 0) {
	  for (Long64_t k = 0; k < roiAfterAppliedLvl1Index; k++) {
	    NCellsBase += Ringer_NCells->at(k);
	  }
	}

        for (Long64_t k = NCellsBase; k < (NCellsBase + Ringer_NCells->at(roiAfterAppliedLvl1Index)); k++) {
          fout->Ringer_EtaResCells->push_back(Ringer_EtaResCells->at(k));
          fout->Ringer_PhiResCells->push_back(Ringer_PhiResCells->at(k));
          fout->Ringer_EtaCells->push_back(Ringer_EtaCells->at(k));
	  fout->Ringer_DetCells->push_back(Ringer_DetCells->at(k));
	  fout->Ringer_PhiCells->push_back(Ringer_PhiCells->at(k));
	  fout->Ringer_ECells->push_back(Ringer_ECells->at(k));
        }

	UInt_t RingsBase = numberOfRingsOnRoI*roiAfterAppliedLvl1Index;
        for (Long64_t k = RingsBase; k < (RingsBase+numberOfRingsOnRoI); k++) {
	  fout->Ringer_Rings->push_back(Ringer_Rings->at(k));
	}

        // Those below this line must be matched by eta/phi
	// Offline vectors 
	// Match eta and phi

	Int_t kBestMatch;
	float deltaEta, deltaPhi, deltaEtaBestMatch, deltaPhiBestMatch;

	kBestMatch = -1;
	deltaEtaBestMatch = deltaEtaMinimumMatch;
	deltaPhiBestMatch = deltaPhiMinimumMatch;
        for (Long64_t k = 0; k < Ele_nc; k++) {
	  deltaEta = doDeltaEta(Ele_eta->at(k), L1Em_eta->at(j));
	  deltaPhi = doDeltaPhi(Ele_phi->at(k), L1Em_phi->at(j));
	  if ( (deltaEta < deltaEtaBestMatch) &&
	       (deltaPhi < deltaPhiBestMatch) ) {
	    kBestMatch = k;
	    deltaEtaBestMatch = deltaEta;
	    deltaPhiBestMatch = deltaPhi;
	  }
        }
	  
        if (kBestMatch > -1) {  // Was there a match??
          // Yes!
          fout->Ele_e->push_back(Ele_e->at(kBestMatch));
          fout->Ele_eta->push_back(Ele_eta->at(kBestMatch));
          fout->Ele_phi->push_back(Ele_phi->at(kBestMatch));
          fout->Ele_IsEM->push_back(Ele_IsEM->at(kBestMatch));
          fout->Ele_nc++;
        }
	
        // EF vectors  
        // Match eta and phi
        kBestMatch = -1;
        deltaEtaBestMatch = deltaEtaMinimumMatch;
        deltaPhiBestMatch = deltaPhiMinimumMatch;
        for (Long64_t k = 0; k < eg_nc_EF; k++) {
          deltaEta = doDeltaEta(eg_eta_EF->at(k), L1Em_eta->at(j));
          deltaPhi = doDeltaPhi(eg_phi_EF->at(k), L1Em_phi->at(j));
          if ( (deltaEta < deltaEtaBestMatch) &&
               (deltaPhi < deltaPhiBestMatch) ) {
            kBestMatch = k;
            deltaEtaBestMatch = deltaEta;
            deltaPhiBestMatch = deltaPhi;
          }
        }

        if (kBestMatch > -1) {  // Was there a match??
          // Yes!
          fout->eg_e_EF->push_back(eg_e_EF->at(kBestMatch));
          fout->eg_eta_EF->push_back(eg_eta_EF->at(kBestMatch));
          fout->eg_phi_EF->push_back(eg_phi_EF->at(kBestMatch));
          fout->eg_IsEM_EF->push_back(eg_IsEM_EF->at(kBestMatch));
          fout->eg_cl_et_EF->push_back(eg_cl_et_EF->at(kBestMatch));
          fout->eg_trkmatchnt_EF->push_back(eg_trkmatchnt_EF->at(kBestMatch));
	  fout->eg_nc_EF++;
        }

        // T2 Inner Detector  
        // Match eta and phi
        kBestMatch = -1;
        deltaEtaBestMatch = deltaEtaMinimumMatch;
        deltaPhiBestMatch = deltaPhiMinimumMatch;
        for (Long64_t k = 0; k < T2IdNtracks; k++) {
          deltaEta = doDeltaEta(T2IdEtac->at(k), L1Em_eta->at(j));
          deltaPhi = doDeltaPhi(T2IdPhic->at(k), L1Em_phi->at(j));
          if ( (deltaEta < deltaEtaBestMatch) &&
               (deltaPhi < deltaPhiBestMatch) ) {
            kBestMatch = k;
            deltaEtaBestMatch = deltaEta;
            deltaPhiBestMatch = deltaPhi;
          }
        }
  
        if (kBestMatch > -1) {  // Was there a match??
          // Yes!
          fout->T2IdEtac->push_back(T2IdEtac->at(kBestMatch));
          fout->T2IdPhic->push_back(T2IdPhic->at(kBestMatch));
          fout->T2IdPt->push_back(T2IdPt->at(kBestMatch));
          fout->T2IdAlgo->push_back(T2IdAlgo->at(kBestMatch));
          fout->T2IdNtracks++;
        }
        
        roiAfterAppliedLvl1Index++;
      }
    }

    fout->L1Em_nRoI = L1Em_nRoI;
    fout->T2CaNclus = T2CaNclus;
    fout->Ringer_NClusters = Ringer_NClusters;

    fout->Fill();

    // GetEntry was not called for the output, so we must clear all std::vectors!
    fout->L1Em_Core->clear();
    fout->L1Em_EmClus->clear();
    fout->L1Em_EmIsol->clear();
    fout->L1Em_HdCore->clear();
    fout->L1Em_HdIsol->clear();
    fout->L1Em_eta->clear();
    fout->L1Em_phi->clear();

    fout->Type->clear();
    fout->PtGen->clear();
    fout->EtaGen->clear();
    fout->PhiGen->clear();

    fout->T2CaEmE->clear();
    fout->T2CaEta->clear();
    fout->T2CaPhi->clear();
    fout->T2CaRcore->clear();
    fout->T2CaEratio->clear();
    fout->T2CaHadE->clear();

    fout->T2CaEmES0->clear();
    fout->T2CaEmES1->clear();
    fout->T2CaEmES2->clear();
    fout->T2CaEmES3->clear();
    fout->T2CaHadES0->clear();
    fout->T2CaHadES1->clear();
    fout->T2CaHadES2->clear();
    fout->T2CaHadES3->clear();
    fout->T2CaWidth->clear();
    fout->T2CaF73->clear();

    fout->T2IdEtac->clear();
    fout->T2IdPhic->clear();
    fout->T2IdPt->clear();
    fout->T2IdAlgo->clear();

    fout->eg_e_EF->clear();
    fout->eg_eta_EF->clear();
    fout->eg_phi_EF->clear();
    fout->eg_IsEM_EF->clear();
    fout->eg_cl_et_EF->clear();
    fout->eg_trkmatchnt_EF->clear();
    
    fout->Ele_e->clear();
    fout->Ele_eta->clear();
    fout->Ele_phi->clear();
    fout->Ele_IsEM->clear();
    
    fout->Ringer_LVL1_Id->clear();
    fout->Ringer_NCells->clear();
    fout->Ringer_Roi_Id->clear();
    fout->Ringer_LVL1_Eta->clear();
    fout->Ringer_LVL1_Phi->clear();
    fout->Ringer_LVL2_Et->clear();
    fout->Ringer_LVL2_Eta->clear();
    fout->Ringer_LVL2_Phi->clear();

    fout->Ringer_DetCells->clear();
    fout->Ringer_EtaCells->clear();
    fout->Ringer_EtaResCells->clear();
    fout->Ringer_PhiResCells->clear();
    fout->Ringer_PhiCells->clear();
    fout->Ringer_ECells->clear();
    fout->Ringer_Rings->clear();

    fout->accepted->clear();
  }

  return fout;
}


#endif

