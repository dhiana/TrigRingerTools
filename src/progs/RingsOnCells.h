#ifndef RINGSONCELLS_H
#define RINGSONCELLS_H

#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include <vector>
#include <map>

#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/rbuild/RingSet.h"


class RingsOnCells {

  static const float deltaEtaMinimumMatch;
  static const float deltaPhiMinimumMatch;

  protected:
  std::vector<float> *Ringer_ECells;
  std::vector<float> *Ringer_EtaCells;
  std::vector<float> *Ringer_PhiCells;
  std::vector<unsigned int> *Ringer_NCells;
  unsigned int Ringer_NClusters;
  std::vector<float> *Ringer_PhiResCells;
  std::vector<float> *Ringer_EtaResCells;
  std::vector<unsigned char> *Ringer_DetCells;
  std::vector<unsigned int> *Ringer_Roi_Id;
  std::vector<unsigned int> *Ringer_LVL1_Id;
  std::vector<float> *Ringer_LVL1_Eta;
  std::vector<float> *Ringer_LVL1_Phi;
  std::vector<float> *Ringer_LVL2_Eta;
  std::vector<float> *Ringer_LVL2_Phi;
  std::vector<float> *Ringer_LVL2_Et;
  std::vector<float> *Ringer_Rings;

  sys::Reporter *m_reporter;

  bool m_global_center;
  double m_eta_window;
  double m_phi_window;
  
  void init();

  public:
  
  TTree *fChain;

  RingsOnCells(TTree *tree, sys::Reporter *reporter, bool global_center = false, double eta_window = 0.1, double phi_window = 0.1);

  void createBranches();
  void setBranches();
  void setStatusOfBranches();

  RingsOnCells *run(std::string &s);
  void Fill();
  
  inline float doDeltaEta(float eta1, float eta2) {
    return std::fabs(eta1 - eta2);
  }

  inline float doDeltaPhi(float phi1, float phi2) {
    return std::fabs(TMath::Pi() - std::fabs(TMath::Pi() - std::fabs(phi1 - phi2)));
  }
  
};

#endif

