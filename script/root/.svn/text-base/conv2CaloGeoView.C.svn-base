/**
This script will convert a NTuple dumped from athena for the format used by
the CaloGeoView software, so we can visualize our RoI there,
*/

#include <iostream>
#include <vector>
#include <cmath>

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace std;

#ifdef __MAKECINT__
#pragma link C++ class vector<UInt_t>;
#pragma link C++ class vector<Float_t>;
#pragma link C++ class vector<UChar_t>;
#endif


void conv2CaloGeoView(const Char_t *input, const Char_t *output)
{
  TChain *rootChain = new TChain("CollectionTree");
  rootChain->Add(input);
  
  const Int_t nevent = rootChain->GetEntries(); 
  cout << "Formating " << nevent << " events..." << endl;

  //Input vectors.
  UInt_t nClusters;
  vector<UInt_t> *rootNCells = new vector<UInt_t>;
  vector<UChar_t> *rootLayers = new vector<UChar_t>;
  vector<Float_t> *rootEnergy = new vector<Float_t>;
  vector<Float_t> *rootEta = new vector<Float_t>;
  vector<Float_t> *rootPhi = new vector<Float_t>;
  vector<UInt_t> *rootLVL1Id = new vector<UInt_t>;
  vector<UInt_t> *rootRoIId = new vector<UInt_t>;
  vector<Float_t> *rootLVL1Eta = new vector<Float_t>;
  vector<Float_t> *rootLVL1Phi = new vector<Float_t>;
  vector<Float_t> *rootLVL2Eta = new vector<Float_t>;
  vector<Float_t> *rootLVL2Phi = new vector<Float_t>;
  vector<Float_t> *rootLVL2Et = new vector<Float_t>;

  // Output vectors.
  UInt_t nCellsEM;
  vector<UChar_t> *layersEM = new vector<UChar_t>;
  vector<Float_t> *energyEM = new vector<Float_t>;
  vector<Float_t> *etaEM = new vector<Float_t>;
  vector<Float_t> *phiEM = new vector<Float_t>;
  UInt_t nCellsHAD;
  vector<UChar_t> *layersHAD = new vector<UChar_t>;
  vector<Float_t> *energyHAD = new vector<Float_t>;
  vector<Float_t> *etaHAD = new vector<Float_t>;
  vector<Float_t> *phiHAD = new vector<Float_t>;
  UInt_t lvl1Id;
  UInt_t roiId;
  Float_t lvl1Eta;
  Float_t lvl1Phi;
  Float_t lvl2Eta;
  Float_t lvl2Phi;
  Float_t lvl2Et;

  //Creating the output tree.
  TTree *outTree = new TTree("CollectionTree", "RoI for CaloGeoView.");
  outTree->Branch("NCellsLRoI", &nCellsEM, "NCellsLRoI/i");
  outTree->Branch("DetCellsLRoI", &layersEM);
  outTree->Branch("ECellsLRoI", &energyEM);
  outTree->Branch("EtaCellsLRoI", &etaEM);
  outTree->Branch("PhiCellsLRoI", &phiEM);
  outTree->Branch("NCellsTRoI", &nCellsHAD, "NCellsTRoI/i");
  outTree->Branch("DetCellsTRoI", &layersHAD);
  outTree->Branch("ECellsTRoI", &energyHAD);
  outTree->Branch("EtaCellsTRoI", &etaHAD);
  outTree->Branch("PhiCellsTRoI", &phiHAD);
  outTree->Branch("LVL1_Id", &lvl1Id, "LVL1_Id/i");
  outTree->Branch("RoI_Id", &roiId, "RoI_Id/i");
  outTree->Branch("LVL1_Eta", &lvl1Eta, "LVL1_Eta/f");
  outTree->Branch("LVL1_Phi", &lvl1Phi, "LVL1_Phi/f");
  outTree->Branch("LVL2_Eta", &lvl2Eta, "LVL2_Eta/f");
  outTree->Branch("LVL2_Phi", &lvl2Phi, "LVL2_Phi/f");
  outTree->Branch("LVL2_Et", &lvl2Et, "LVL2_Et/f");

  //Selecting the branches we want to read.
  rootChain->SetBranchStatus("*",0);  // disable all branches
  rootChain->SetBranchStatus("Ringer_NClusters",1);
  rootChain->SetBranchStatus("Ringer_NCells",1);
  rootChain->SetBranchStatus("Ringer_DetCells",1);
  rootChain->SetBranchStatus("Ringer_ECells",1);
  rootChain->SetBranchStatus("Ringer_EtaCells",1);
  rootChain->SetBranchStatus("Ringer_PhiCells",1);
  rootChain->SetBranchStatus("Ringer_LVL1_Id",1);
  rootChain->SetBranchStatus("Ringer_Roi_Id",1);
  rootChain->SetBranchStatus("Ringer_LVL1_Eta",1);
  rootChain->SetBranchStatus("Ringer_LVL1_Phi",1);
  rootChain->SetBranchStatus("Ringer_LVL2_Eta",1);
  rootChain->SetBranchStatus("Ringer_LVL2_Phi",1);
  rootChain->SetBranchStatus("Ringer_LVL2_Et",1);
  
  // Associating branches with the input vectors.
  rootChain->SetBranchAddress("Ringer_NClusters", &nClusters);
  rootChain->SetBranchAddress("Ringer_NCells", &rootNCells);
  rootChain->SetBranchAddress("Ringer_DetCells", &rootLayers);
  rootChain->SetBranchAddress("Ringer_ECells", &rootEnergy);
  rootChain->SetBranchAddress("Ringer_EtaCells", &rootEta);
  rootChain->SetBranchAddress("Ringer_PhiCells", &rootPhi);
  rootChain->SetBranchAddress("Ringer_LVL1_Id", &rootLVL1Id);
  rootChain->SetBranchAddress("Ringer_Roi_Id", &rootRoIId);
  rootChain->SetBranchAddress("Ringer_LVL1_Eta", &rootLVL1Eta);
  rootChain->SetBranchAddress("Ringer_LVL1_Phi", &rootLVL1Phi);
  rootChain->SetBranchAddress("Ringer_LVL2_Eta", &rootLVL2Eta);
  rootChain->SetBranchAddress("Ringer_LVL2_Phi", &rootLVL2Phi);
  rootChain->SetBranchAddress("Ringer_LVL2_Et", &rootLVL2Et);

  for (Int_t ev=0; ev<nevent; ev++)
  {
    rootChain->GetEntry(ev);
    if (!nClusters) continue;
    
    UInt_t roiCellBegin = 0;
    nCellsEM = nCellsHAD = 0;
    for (UInt_t roi=0; roi<nClusters; roi++)
    {
      //Copying RoI header values.
      lvl1Id = rootLVL1Id->at(roi);
      roiId = rootRoIId->at(roi);
      lvl1Eta = rootLVL1Eta->at(roi);
      lvl1Phi = rootLVL1Phi->at(roi);
      lvl2Eta = rootLVL2Eta->at(roi);
      lvl2Phi = rootLVL2Phi->at(roi);
      lvl2Et = rootLVL2Et->at(roi);

      //Copying the RoI elements.
      for (UInt_t c = roiCellBegin; c < (roiCellBegin+rootNCells->at(roi)); c++)
      {
        if (rootLayers->at(c) < 8) // If it is EM...
        {
          layersEM->push_back(rootLayers->at(c));
          energyEM->push_back(rootEnergy->at(c));
          etaEM->push_back(rootEta->at(c));
          phiEM->push_back(rootPhi->at(c));
          nCellsEM++;
        }
        else // If it is Hadronic...
        {
          layersHAD->push_back(rootLayers->at(c));
          energyHAD->push_back(rootEnergy->at(c));
          etaHAD->push_back(rootEta->at(c));
          phiHAD->push_back(rootPhi->at(c));
          nCellsHAD++;
        }
      }
        
      //Pointing to the next RoI.
      roiCellBegin += rootNCells->at(roi);
     
      //Saving to the output ntuple.
      outTree->Fill();
      
      //Clearing the vectors for the next cycle.
      layersEM->clear();
      energyEM->clear();
      etaEM->clear();
      phiEM->clear();
      layersHAD->clear();
      energyHAD->clear();
      etaHAD->clear();
      phiHAD->clear();
    }
  }
  
  //Saving the NTuple file.
  TFile outFile(output, "RECREATE");
  outTree->Write();
  outFile.Close();

  delete rootChain;
  delete outTree;
  delete rootNCells;
  delete rootLayers;
  delete rootEnergy;
  delete rootEta;
  delete rootPhi;
  delete layersEM;
  delete energyEM;
  delete etaEM;
  delete phiEM;
  delete layersHAD;
  delete energyHAD;
  delete etaHAD;
  delete phiHAD;
}
