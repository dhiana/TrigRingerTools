#include <TVectorT.h>
#include <TList.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TLegend.h>
#include <vector>
#include <iostream>

#ifdef __MAKECINT__
#pragma link C++ class vector<UInt_t>;
#pragma link C++ class vector<Float_t>;
#pragma link C++ class vector<UChar_t>;
#endif

void Validate(char *input, char *output, bool fast = false)
{
  TChain *T = new TChain("CollectionTree");
  T->SetCacheSize(0);
  T->Add(input);

  UInt_t nevent = T->GetEntries();

  std::vector<float> *EmClus = 0;
  std::vector<float> *EmIsolation = 0;
  std::vector<float> *HdCore = 0;
  std::vector<float> *HdIsolation = 0;
  Int_t L1Em_nRoI = 0;

  std::vector<float> *ringList = new std::vector<float>;
  std::vector<unsigned char> *detList = new std::vector<unsigned char>;
  std::vector<float> *etaList = new std::vector<float>;
  std::vector<float> *phiList = new std::vector<float>;
  std::vector<float> *eList = new std::vector<float>;
  vector<UInt_t> *nCellsList = new vector<UInt_t>;

  //If this option is enabled, the whole NTuple's branches are not checked. just
  //the ones used to plot the histogramms.
  if (fast)
  {
    cout << "Doing fast analysis..." << endl;
    T->SetBranchStatus("*",0);  // disable all branches
    T->SetBranchStatus("Ringer_Rings",1);
    T->SetBranchStatus("Ringer_DetCells",1);
    T->SetBranchStatus("Ringer_EtaCells",1);
    T->SetBranchStatus("Ringer_PhiCells",1);
    T->SetBranchStatus("Ringer_ECells",1);
    T->SetBranchStatus("Ringer_NCells",1);
    T->SetBranchStatus("L1Em_EmClus",1);
    T->SetBranchStatus("L1Em_EmIsol",1);
    T->SetBranchStatus("L1Em_HdIsol",1);
    T->SetBranchStatus("L1Em_HdCore",1); 
    T->SetBranchStatus("L1Em_nRoI",1); 
  }

  T->SetBranchAddress("Ringer_Rings", &ringList);
  T->SetBranchAddress("Ringer_DetCells", &detList);
  T->SetBranchAddress("Ringer_EtaCells", &etaList);
  T->SetBranchAddress("Ringer_PhiCells", &phiList);
  T->SetBranchAddress("Ringer_ECells", &eList);
  T->SetBranchAddress("Ringer_NCells", &nCellsList);

  T->SetBranchAddress("L1Em_EmClus", &EmClus);
  T->SetBranchAddress("L1Em_EmIsol", &EmIsolation);
  T->SetBranchAddress("L1Em_HdCore", &HdCore);
  T->SetBranchAddress("L1Em_HdIsol", &HdIsolation);
  T->SetBranchAddress("L1Em_nRoI", &L1Em_nRoI);


  //Creating the histograms.
  TFile *f = new TFile(output,"recreate");

  TH1F *detHist = new TH1F("Det", "Detectors", 25, 0,25);
  TH1F *etaHist = new TH1F("Eta", "#eta", 200, -3,3);
  TH1F *phiHist = new TH1F("Phi", "#phi", 200, -3.2,3.2);
  TH1F *eHist = new TH1F("Energy", "Cell Energy", 200, -100,20000);
  TH1F *etHist = new TH1F("RoIEnergy", "RoI E_{T}", 200, -1000,20000);
  TH1F *ringsHist = new TH1F("Rings", "Rings", 200, -500,500);
  TH2F *enVeta = new TH2F("enVeta", "E_{T} x #eta", 200,-3,3,200,-1000,20000);
  TH2F *enVphi = new TH2F("enVphi", "E_{T} x #phi", 200,-3.2,3.2,200,-1000,20000);
  TH1F *enVdet[4];
  enVdet[0] = new TH1F("enVl0", "E_{T} in Pre-Sampler", 200, -5000,5000);
  enVdet[1] = new TH1F("enVl1", "E_{T} in EM Layer 1", 200, -5000,5000);
  enVdet[2] = new TH1F("enVl2", "E_{T} in EM Layer 2", 200, -5000,5000);
  enVdet[3] = new TH1F("enVl3", "E_{T} in EM Layer 3", 200, -5000,5000);
  TH1F *enVdetHad[3];
  enVdetHad[0] = new TH1F("enVl0Had", "E_{T} in HAD Layer 1", 200, -5000,5000);
  enVdetHad[1] = new TH1F("enVl1Had", "E_{T} in HAD Layer 2", 200, -5000,5000);
  enVdetHad[2] = new TH1F("enVl2Had", "E_{T} in HAD Layer 3", 200, -5000,5000);
  
  TH1F *EmClusHist = new TH1F("EmClusHist", "Level 1 Calo EmClus", 200, -5.e3, 80.e3);
  TH1F *EmIsolHist = new TH1F("EmIsolHist", "Level 1 Calo EmIsol", 200, -5.e3, 80.e3);
  TH1F *HdCoreHist = new TH1F("HdCoreHist", "Level 1 Calo HdCore", 200, -5.e3, 80.e3);
  TH1F *HdIsolHist = new TH1F("HdIsolHist", "Level 1 Calo HdIsol", 200, -5.e3, 80.e3);
  TH1F *L1nRoiHist = new TH1F("L1nRoiHist", "Level 1 Calo number of RoIs", 20, 0.f, 20.f); 

  //Setting some to have the range automatically calculated.
  eHist->SetBit(TH1::kCanRebin);
  etHist->SetBit(TH1::kCanRebin);
  ringsHist->SetBit(TH1::kCanRebin);
  etaHist->SetBit(TH1::kCanRebin);
  enVeta->SetBit(TH1::kCanRebin);
  enVphi->SetBit(TH1::kCanRebin);
  enVdet[0]->SetBit(TH1::kCanRebin);
  enVdet[1]->SetBit(TH1::kCanRebin);
  enVdet[2]->SetBit(TH1::kCanRebin);
  enVdet[3]->SetBit(TH1::kCanRebin);
  enVdetHad[0]->SetBit(TH1::kCanRebin);
  enVdetHad[1]->SetBit(TH1::kCanRebin);
  enVdetHad[2]->SetBit(TH1::kCanRebin);

  EmClusHist->SetBit(TH1::kCanRebin);
  EmIsolHist->SetBit(TH1::kCanRebin);
  HdCoreHist->SetBit(TH1::kCanRebin);
  HdIsolHist->SetBit(TH1::kCanRebin);
  L1nRoiHist->SetBit(TH1::kCanRebin);

  cout << "Numero de Eventos = " << nevent << endl;
  UInt_t numTotCells = 0;
  UInt_t missedEvents = 0;
  UInt_t realEvents = 0;
  unsigned detVals[25];
  for (UInt_t i=0; i<25; i++) detVals[i] = 0;
  for (UInt_t i=0; i<nevent; i++)
  {
    T->GetEntry(i);
    
    // Filling the LVL1 related histos.
    L1nRoiHist->Fill(L1Em_nRoI);
    for (Int_t j = 0; j < L1Em_nRoI; j++)
    {
      EmClusHist->Fill(EmClus->at(j));
      EmIsolHist->Fill(EmIsolation->at(j));
      HdCoreHist->Fill(HdCore->at(j));
      HdIsolHist->Fill(HdIsolation->at(j));
    }

    // If there were no RoI from LVL1, we skip the rest of the loop.
    if (!nCellsList->size())
    {
      missedEvents++;
      continue;
    }
    
    // Getting teh total number of cells in the dataset and the number of RoI aproved.
    numTotCells += eList->size();
    realEvents += nCellsList->size();
    
    //Filling the RoI energy histogram.
    UInt_t initPos = 0;
    for (std::vector<unsigned>::const_iterator numCells=nCellsList->begin(); numCells != nCellsList->end(); numCells++)
    {
      Float_t roiEnergy = 0.;
      for (UInt_t k = initPos; k < (initPos + (*numCells)); k++) roiEnergy += eList->at(k) / cosh(etaList->at(k));
      if (roiEnergy != 0.) etHist->Fill(roiEnergy);
      initPos += (*numCells);
    }
    
    // Filling the rings histogram.
    for (std::vector<float>::const_iterator rings=ringList->begin(); rings != ringList->end(); rings++) ringsHist->Fill(*rings);
  
    // Filling eta, phi, det and energy.
    std::vector<unsigned char>::const_iterator det = detList->begin();
    std::vector<float>::const_iterator eta = etaList->begin();
    std::vector<float>::const_iterator phi = phiList->begin();
    std::vector<float>::const_iterator energy = eList->begin();
    while (energy != eList->end())
    {
      const int detVal = static_cast<int>((*det));
      const Float_t eTrans = (*energy) / cosh(*eta);
      detVals[detVal]++;
      etaHist->Fill(*eta);
      phiHist->Fill(*phi);
      eHist->Fill(*energy);
      enVeta->Fill(*eta, eTrans);
      enVphi->Fill(*phi, eTrans);
      if (detVal < 4) enVdet[detVal]->Fill(eTrans);
      else if ( (detVal >= 12) && (detVal <= 14) ) enVdetHad[detVal-12]->Fill(eTrans);
      det++;
      eta++;
      phi++;
      energy++;
    }    
  }
   
  for (int i=0; i<25; i++) detHist->SetBinContent(i+1, detVals[i]);
  detHist->SetEntries(numTotCells);
  
  TCanvas *canvasPhi = new TCanvas("Phi");
  phiHist->Draw();
  
  TCanvas *canvasDet = new TCanvas("Detectors");
  detHist->Draw();
  
  TCanvas *canvasEta = new TCanvas("Eta");
  etaHist->Draw();

  TCanvas *canvasEmClus = new TCanvas("L1 EmClus");
  EmClusHist->Draw();

  TCanvas *canvasEmIsol = new TCanvas("L1 EmIsol");
  EmIsolHist->Draw();

  TCanvas *canvasHdCore = new TCanvas("L1 HdCore");
  HdCoreHist->Draw();

  TCanvas *canvasHdIsol = new TCanvas("L1 HdIsol");
  HdIsolHist->Draw();

  TCanvas *canvasEnergy = new TCanvas("Energy");
  gPad->SetLogy(1);
  eHist->Draw();

  TCanvas *canvasRoIEnergy = new TCanvas("RoIEnergy");
  etHist->Draw();
//  etHist->Fit("gaus", "M");
//  gStyle->SetOptFit();
  
  TCanvas *canvasRings = new TCanvas("Rings");
  ringsHist->Draw();
  
  TCanvas *canvasEnVeta = new TCanvas("enVeta");
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  enVeta->GetYaxis()->SetTitle("E_{T}");
  enVeta->GetXaxis()->SetTitle("#eta");
  enVeta->Draw("COLZ");
  
  TCanvas *canvasEnVphi = new TCanvas("enVphi");
  gPad->SetLogz(1);
  gStyle->SetPalette(1);
  enVphi->GetYaxis()->SetTitle("E_{T}");
  enVphi->GetXaxis()->SetTitle("#phi");
  enVphi->Draw("COLZ");
  
  TCanvas *canvasEnVdet = new TCanvas("enVdet");
  canvasEnVdet->Divide(2,2);
  for (UInt_t i=0; i<4; i++)
  {
    canvasEnVdet->cd(i+1);
    gPad->SetLogy(1);
    enVdet[i]->Draw();
  }
  
  TCanvas *canvasEnVdetHad = new TCanvas("enVdetHad");
  canvasEnVdetHad->Divide(2,2);
  for (UInt_t i=0; i<3; i++)
  {
    canvasEnVdetHad->cd(i+1);
    gPad->SetLogy(1);
    enVdetHad[i]->Draw();
  }
  
  const Double_t l2Count = enVdet[2]->GetEntries();
  TH1 *detRatio = new TH1F("detRatio","Ratio Between EM Barrel Layers and EM Layer 2 Counts",4,0,3);
  TH1 *detRatioTruth = new TH1F("detRatiotruth","",4,0,3);
  Float_t truthVal[] = {0.25, 2, 1, 0.5}; 
  for (Int_t i=1; i<=4; i++)
  {
    detRatio->SetBinContent(i, enVdet[i-1]->GetEntries() / l2Count);
    detRatioTruth->SetBinContent(i, truthVal[i-1]);
  }
  TCanvas *canvasDetRatio = new TCanvas("detRatio");
  gPad->SetGridy();
  detRatio->GetYaxis()->SetTitle("Ratio");
  detRatio->GetXaxis()->SetTitle("EM Barrel Layer Id");
  detRatio->Draw();
  canvasDetRatio->Update();
  detRatioTruth->SetLineColor(kRed);
  detRatioTruth->Draw("same");
  TLegend *leg = new TLegend(0.,0.,0.25,0.25);
  leg->AddEntry(detRatio, "Dataset");
  leg->AddEntry(detRatioTruth, "Expected values.");
  leg->Draw("same");
  
  f->Write();
  
  cout << "Total de celulas = " << numTotCells << endl;
  cout << "Num RoIs rejeitadas pelo LVL1 = " << missedEvents << endl;
  cout << "Num RoIs aprovadas pelo LVL1 = " << realEvents << endl;

  delete T;
  delete ringList;
  delete detList;
  delete etaList;
  delete phiList;
  delete eList;
  delete nCellsList;
}
