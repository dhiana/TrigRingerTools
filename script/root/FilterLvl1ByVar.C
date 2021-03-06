// FilterLvl1ByVar.C

// This script receives one or more ROOT input files (in case there are wildcards on their names)
// that have been processed by Athena's LVL1 simulation and it filters the input with a new LVL1 threshold
// One must specify the input file(s), the output file, the tree where the information resides,
// the new LVL1 cut to be performed and the old, Athena applied LVL1 cut.

// This script should read and save exactly these branches:
// L1Em_nRoI, L1Em_Core, L1Em_EmClus, L1Em_EmIsol, L1Em_HdIsol, L1Em_HdCore, L1Em_eta, L1Em_phi,
// Type, PtGen, EtaGen, PhiGen,
// T2CaNclus, T2CaEmE, T2CaEta, T2CaPhi, T2CaRcore, T2CaEratio, T2CaHadE,
// T2CaEmES0, T2CaEmES1, T2CaEmES2, T2CaEmES3, T2CaWidth, T2CaF73,
// T2CaHadES0, T2CaHadES1, T2CaHadES2, T2CaHadES3,
// Ele_nc, Ele_e, Ele_eta, Ele_phi, Ele_IsEM,
// Ringer_LVL1_Id, Ringer_NCells, Ringer_NClusters, Ringer_Roi_Id, Ringer_LVL1_Eta, Ringer_LVL1_Phi, 
// Ringer_DetCells, Ringer_EtaCells, Ringer_PhiCells, Ringer_ECells, Ringer_LVL2_Et, Ringer_LVL2_Eta,
// Ringer_LVL2_Phi, Ringer_Rings

// In order to use it, compile it as a shared library in ROOT.
// Example:
// $ root
// ...
// [0] .L FilterLvl1ByVar.C+
// [1] FilterLvl1ByVar("myInputFile.aan.root", "myOutputFile.aan.root", "CollectionTree", "EM18I", "EM01");
//    [This applies a EM18I cut on the data at tree CollectionTree from myInputFile.aan.root,
//     and writes the output at myOutputFile.aan.root.
//     The input file in the example has been generated by Athena with a LVL1 cut at EM01]
//

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>
#include <map>
#include <iostream>

#ifdef __MAKECINT__
#pragma link C++ class std::vector<float>;
#pragma link C++ class std::vector<unsigned long>;
#pragma link C++ class std::vector<unsigned int>;
#pragma link C++ class std::vector<long>;
#pragma link C++ class std::vector<unsigned char>;
#pragma link C++ class std::vector<int>;
#pragma link C++ class std::vector<double>;
#pragma link C++ class std::vector<std::vector<double> >;
#endif

// Represents a LVL1 THRESHOLD
// Energy values in MeV
class Threshold {
public:
  float m_EmCore;
  float m_EmIsolation;
  float m_HdCore;
  float m_HdIsolation;

  Threshold(const float EmCore = 0.f,
	    const float EmIsolation = 999999.f,
	    const float HdCore = 999999.f,
	    const float HdIsolation = 999999.f)
    : m_EmCore(EmCore),
      m_EmIsolation(EmIsolation),
      m_HdCore(HdCore),
      m_HdIsolation(HdIsolation)
  {
  }
  
  Threshold(const Threshold &m) {
    m_EmCore = m.m_EmCore;
    m_EmIsolation = m.m_EmIsolation;
    m_HdCore = m.m_HdCore;
    m_HdIsolation = m.m_HdIsolation;
  }

  bool operator ==(const Threshold &t) const {
    return ( (m_EmCore == t.m_EmCore) &&
	     (m_EmIsolation == t.m_EmIsolation) &&
	     (m_HdCore == t.m_HdCore) &&
	     (m_HdIsolation == t.m_HdIsolation) );
  }

  bool cut(const float EmCore, const float EmIsolation, const float HdCore, const float HdIsolation) const {
    bool passed = false;
    if (EmCore > m_EmCore) {
      if (EmIsolation <= m_EmIsolation) {
	if (HdCore <= m_HdCore) {
	  if (HdIsolation <= m_HdIsolation) {
	    passed = true;
	  }
	}
      }
    }
    return passed;
  }

  friend ostream &operator <<(ostream &o, const Threshold &m) {
    o << "EmCore:       " << m.m_EmCore << " MeV" << endl
      << "EmIsolation:  " << m.m_EmIsolation << " MeV" << endl
      << "HdCore:       " << m.m_HdCore << " MeV" << endl
      << "HdIsolation:  " << m.m_HdIsolation << " MeV";
    return o;
  }

};

class ThresholdMapLess {
 public:
  bool operator ()(const TString &A, const TString &B) {
    if (A.CompareTo(B) < 0)
      return true;
    return false;
  }
};


typedef std::map<TString, Threshold, ThresholdMapLess> ThresholdMap;

#define IN     0
#define OUT    1

inline float doDeltaEta(float eta1, float eta2) {
  return fabsf(eta1 - eta2);
}

inline float doDeltaPhi(float phi1, float phi2) {
  return fabsf(TMath::Pi() - fabsf(TMath::Pi() - fabsf(phi1 - phi2)));
}

// Function to filter input file with a menu and put the filtered data in output
// Input TTree must be in the format generated by CBNTAA_TrigRinger
void FilterLvl1ByVar(const TString &inputName, const TString &outputName,
		     const TString &tree, const TString &myThreshold,
		     const TString &appliedLvl1Threshold = "EM7",
                     const bool runTruth = true) {

  // Description of input parameters:
  // inputName is/are the input file(s) (it may include wildcards).
  // outputName is the output file, where filtered data will be saved.
  // tree is the name of the TTree where information was saved (usually "CollectionTree")
  // myThreshold is the new, user-defined, threshold.
  // appliedLvl1Thresold is the threshold applied by Athena whence the saving of data
  // runTruth indicates weather or not we should save the truth information (it might not have been generated)

  // TODO: Add other menus

  const float deltaEtaMinimumMatch = 0.2f;
  const float deltaPhiMinimumMatch = 0.2f;

  const UInt_t numberOfRingsOnRoI = 100;

  const float GeV = 1000.f;
  
  ThresholdMap thresholdMap;
  thresholdMap["EM7"]  = Threshold(7*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM11"]  = Threshold(11*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM11I"] = Threshold(11*GeV, 3*GeV, 2*GeV, 1*GeV);
  thresholdMap["EM18"]  = Threshold(18*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM18I"] = Threshold(18*GeV, 3*GeV, 2*GeV, 1*GeV);
  thresholdMap["EM50"]  = Threshold(50*GeV, 999*GeV, 999*GeV, 999*GeV);

  ThresholdMap::const_iterator selectedThreshold = thresholdMap.find(myThreshold);

  if (selectedThreshold == thresholdMap.end()) {
    // This threshold isn't available
    std::cout << "Threshold " << myThreshold << " not found." << std::endl
	      << "Follows a list of available thresholds." << std::endl;
    for (ThresholdMap::const_iterator j = thresholdMap.begin(); j != thresholdMap.end(); j++) {
      std::cout << "Item " << j->first
		<< " with thresholds:" << std::endl
		<< j->second << std::endl;
    }
    return;
  } else {
    std::cout << "Found " << myThreshold << " with thresholds:" << std::endl
	      << selectedThreshold->second << std::endl;
  }

  ThresholdMap::const_iterator theAppliedLvl1Threshold = thresholdMap.find(appliedLvl1Threshold);

  if (theAppliedLvl1Threshold == thresholdMap.end()) {
    // Lvl1 threshold not available!
    std::cout << "Applied Level 1 Threshold " << appliedLvl1Threshold << " not found." << std::endl
	      << "Follows a list of available thresholds." << std::endl;
    for (ThresholdMap::const_iterator j = thresholdMap.begin(); j != thresholdMap.end(); j++) {
      std::cout << "Item " << j->first
		<< " with thresholds:" << std::endl
		<< j->second << std::endl;
    }
    return;
  } else {
    std::cout << "Found " << appliedLvl1Threshold << " (for Applied Level 1) with thresholds:" << std::endl
	      << theAppliedLvl1Threshold->second << std::endl;
  }

  std::cout << "Loading files." << std::endl;

  TChain *inputTree = new TChain(tree);
  inputTree->Add(inputName);
  TFile *outputFile = new TFile(outputName, "RECREATE");

  // Temporary variables used to get data from files
  std::vector<float> *L1Core[] = {0, 0};
  std::vector<float> *EmCore[] = {0, 0};
  std::vector<float> *EmIsolation[] = {0, 0};
  std::vector<float> *HdCore[] = {0, 0};
  std::vector<float> *HdIsolation[] = {0, 0};
  Int_t L1Em_nRoI[] = {0, 0};
  std::vector<float> *bL1Em_eta[] = {0, 0};
  std::vector<float> *bL1Em_phi[] = {0, 0};

  std::vector<long> *bType[] = {0, 0};
  std::vector<float> *bPtGen[] = {0, 0};
  std::vector<float> *bEtaGen[] = {0, 0};
  std::vector<float> *bPhiGen[] = {0, 0};

  UInt_t bT2CaNclus[] = {0, 0};
  std::vector<float> *bT2CaEmE[] = {0, 0};
  std::vector<float> *bT2CaEmES0[] = {0, 0};
  std::vector<float> *bT2CaEmES1[] = {0, 0};
  std::vector<float> *bT2CaEmES2[] = {0, 0};
  std::vector<float> *bT2CaEmES3[] = {0, 0};
  std::vector<float> *bT2CaEta[] = {0, 0};
  std::vector<float> *bT2CaPhi[] = {0, 0};
  std::vector<float> *bT2CaRcore[] = {0, 0};
  std::vector<float> *bT2CaEratio[] = {0, 0};
  std::vector<float> *bT2CaWidth[] = {0, 0};
  std::vector<float> *bT2CaF73[] = {0, 0};
  std::vector<float> *bT2CaHadE[] = {0, 0};
  std::vector<float> *bT2CaHadES0[] = {0, 0};
  std::vector<float> *bT2CaHadES1[] = {0, 0};
  std::vector<float> *bT2CaHadES2[] = {0, 0};
  std::vector<float> *bT2CaHadES3[] = {0, 0};

  UInt_t bT2IdNtracks[] = {0, 0};
  std::vector<float> *bT2IdEtac[] = {0, 0};
  std::vector<float> *bT2IdPhic[] = {0, 0};
  std::vector<float> *bT2IdPt[] = {0, 0};
  std::vector<long> *bT2IdAlgo[] = {0, 0};

  Int_t beg_nc_EF[] = {0, 0};
  std::vector<float> *beg_e_EF[] = {0, 0};
  std::vector<float> *beg_eta_EF[] = {0, 0};
  std::vector<float> *beg_phi_EF[] = {0, 0};
  std::vector<long> *beg_IsEM_EF[] = {0, 0};
  std::vector<float> *beg_cl_et_EF[] = {0, 0};
  std::vector<long> *beg_trkmatchnt_EF[] = {0, 0};

  Int_t bEle_nc[] = {0, 0};
  std::vector<float> *bEle_e[] = {0, 0};
  std::vector<float> *bEle_eta[] = {0, 0};
  std::vector<float> *bEle_phi[] = {0, 0};
  std::vector<long> *bEle_IsEM[] = {0, 0};

  std::vector<unsigned int> *bRinger_LVL1_Id[] = {0, 0};
  std::vector<unsigned int> *bRinger_NCells[] = {0, 0};
  UInt_t bRinger_NClusters[] = {0, 0};
  std::vector<unsigned int> *bRinger_Roi_Id[] = {0, 0};
  std::vector<float> *bRinger_LVL1_Eta[] = {0, 0};
  std::vector<float> *bRinger_LVL1_Phi[] = {0, 0};
  std::vector<unsigned char> *bRinger_DetCells[] = {0, 0};
  std::vector<float> *bRinger_EtaResCells[] = {0, 0};
  std::vector<float> *bRinger_PhiResCells[] = {0, 0};
  std::vector<float> *bRinger_EtaCells[] = {0, 0};
  std::vector<float> *bRinger_PhiCells[] = {0, 0};
  std::vector<float> *bRinger_ECells[] = {0, 0};
  std::vector<float> *bRinger_Rings[] = {0, 0};
  std::vector<float> *bRinger_LVL2_Et[] = {0, 0};
  std::vector<float> *bRinger_LVL2_Eta[] = {0, 0};
  std::vector<float> *bRinger_LVL2_Phi[] = {0, 0};

  // Set inputTree pointers
  inputTree->SetBranchStatus("*",0);

  inputTree->SetBranchStatus("L1Em_nRoI",1); 
  inputTree->SetBranchStatus("L1Em_Core",1);
  inputTree->SetBranchStatus("L1Em_EmClus",1);
  inputTree->SetBranchStatus("L1Em_EmIsol",1);
  inputTree->SetBranchStatus("L1Em_HdIsol",1);
  inputTree->SetBranchStatus("L1Em_HdCore",1); 
  inputTree->SetBranchStatus("L1Em_eta",1);
  inputTree->SetBranchStatus("L1Em_phi",1);

  if (runTruth) {
    inputTree->SetBranchStatus("Type",1);
    inputTree->SetBranchStatus("PtGen",1);
    inputTree->SetBranchStatus("EtaGen",1);
    inputTree->SetBranchStatus("PhiGen",1);
  }

  inputTree->SetBranchStatus("T2CaNclus",1);
  inputTree->SetBranchStatus("T2CaEmE",1); 
  inputTree->SetBranchStatus("T2CaEta",1);
  inputTree->SetBranchStatus("T2CaPhi",1);
  inputTree->SetBranchStatus("T2CaRcore",1);
  inputTree->SetBranchStatus("T2CaEratio",1); 
  inputTree->SetBranchStatus("T2CaHadE",1);
  inputTree->SetBranchStatus("T2CaEmES0",1);
  inputTree->SetBranchStatus("T2CaEmES1",1);
  inputTree->SetBranchStatus("T2CaEmES2",1);
  inputTree->SetBranchStatus("T2CaEmES3",1);
  inputTree->SetBranchStatus("T2CaWidth",1);
  inputTree->SetBranchStatus("T2CaF73",1);
  inputTree->SetBranchStatus("T2CaHadES0",1);
  inputTree->SetBranchStatus("T2CaHadES1",1);
  inputTree->SetBranchStatus("T2CaHadES2",1);
  inputTree->SetBranchStatus("T2CaHadES3",1);

  // T2 Inner Detector  
  inputTree->SetBranchStatus("T2IdNtracks",1);
  inputTree->SetBranchStatus("T2IdEtac",1);
  inputTree->SetBranchStatus("T2IdPhic",1); 
  inputTree->SetBranchStatus("T2IdPt",1);
  inputTree->SetBranchStatus("T2IdAlgo",1);

  inputTree->SetBranchStatus("eg_nc_EF",1); 
  inputTree->SetBranchStatus("eg_e_EF",1);
  inputTree->SetBranchStatus("eg_eta_EF",1);
  inputTree->SetBranchStatus("eg_phi_EF",1);
  inputTree->SetBranchStatus("eg_IsEM_EF",1); 
  inputTree->SetBranchStatus("eg_cl_et_EF",1);
  inputTree->SetBranchStatus("eg_trkmatchnt_EF",1);

  inputTree->SetBranchStatus("Ele_nc",1);
  inputTree->SetBranchStatus("Ele_e",1); 
  inputTree->SetBranchStatus("Ele_eta",1);
  inputTree->SetBranchStatus("Ele_phi",1);
  inputTree->SetBranchStatus("Ele_IsEM",1);

  inputTree->SetBranchStatus("Ringer_LVL1_Id",1);
  inputTree->SetBranchStatus("Ringer_NCells",1); 
  inputTree->SetBranchStatus("Ringer_NClusters",1);
  inputTree->SetBranchStatus("Ringer_Roi_Id",1);
  inputTree->SetBranchStatus("Ringer_LVL1_Eta",1);
  inputTree->SetBranchStatus("Ringer_LVL1_Phi",1); 
  inputTree->SetBranchStatus("Ringer_DetCells",1);
  inputTree->SetBranchStatus("Ringer_EtaResCells",1);
  inputTree->SetBranchStatus("Ringer_EtaCells",1);
  inputTree->SetBranchStatus("Ringer_PhiResCells",1);
  inputTree->SetBranchStatus("Ringer_PhiCells",1);
  inputTree->SetBranchStatus("Ringer_ECells",1); 
  inputTree->SetBranchStatus("Ringer_LVL2_Et",1);
  inputTree->SetBranchStatus("Ringer_LVL2_Eta",1);
  inputTree->SetBranchStatus("Ringer_LVL2_Phi",1);
  inputTree->SetBranchStatus("Ringer_Rings",1); 

  // Now we must set the branch address

  // These are the cut variables!
  inputTree->SetBranchAddress("L1Em_Core", &L1Core[IN]);
  inputTree->SetBranchAddress("L1Em_EmClus", &EmCore[IN]);
  inputTree->SetBranchAddress("L1Em_EmIsol", &EmIsolation[IN]);
  inputTree->SetBranchAddress("L1Em_HdCore", &HdCore[IN]);
  inputTree->SetBranchAddress("L1Em_HdIsol", &HdIsolation[IN]);
  inputTree->SetBranchAddress("L1Em_nRoI", &L1Em_nRoI[IN]);
  inputTree->SetBranchAddress("L1Em_eta", &bL1Em_eta[IN]);
  inputTree->SetBranchAddress("L1Em_phi", &bL1Em_phi[IN]);

  // This is everything else ...
  inputTree->SetBranchAddress("Type", &bType[IN]);
  inputTree->SetBranchAddress("PtGen", &bPtGen[IN]);
  inputTree->SetBranchAddress("EtaGen",&bEtaGen[IN]);
  inputTree->SetBranchAddress("PhiGen", &bPhiGen[IN]);

  inputTree->SetBranchAddress("T2CaNclus", &bT2CaNclus[IN]);
  inputTree->SetBranchAddress("T2CaEmE", &bT2CaEmE[IN]); 
  inputTree->SetBranchAddress("T2CaEta", &bT2CaEta[IN]);
  inputTree->SetBranchAddress("T2CaPhi", &bT2CaPhi[IN]);
  inputTree->SetBranchAddress("T2CaRcore", &bT2CaRcore[IN]);
  inputTree->SetBranchAddress("T2CaEratio", &bT2CaEratio[IN]); 
  inputTree->SetBranchAddress("T2CaHadE", &bT2CaHadE[IN]);

  inputTree->SetBranchAddress("T2CaEmES0",&bT2CaEmES0[IN]);
  inputTree->SetBranchAddress("T2CaEmES1",&bT2CaEmES1[IN]);
  inputTree->SetBranchAddress("T2CaEmES2",&bT2CaEmES2[IN]);
  inputTree->SetBranchAddress("T2CaEmES3",&bT2CaEmES3[IN]);
  inputTree->SetBranchAddress("T2CaWidth",&bT2CaWidth[IN]);
  inputTree->SetBranchAddress("T2CaF73",&bT2CaF73[IN]);
  inputTree->SetBranchAddress("T2CaHadES0",&bT2CaHadES0[IN]);
  inputTree->SetBranchAddress("T2CaHadES1",&bT2CaHadES1[IN]);
  inputTree->SetBranchAddress("T2CaHadES2",&bT2CaHadES2[IN]);
  inputTree->SetBranchAddress("T2CaHadES3",&bT2CaHadES3[IN]);

  // T2 Inner Detector  
  inputTree->SetBranchAddress("T2IdNtracks", &bT2IdNtracks[IN]);
  inputTree->SetBranchAddress("T2IdEtac", &bT2IdEtac[IN]);
  inputTree->SetBranchAddress("T2IdPhic", &bT2IdPhic[IN]); 
  inputTree->SetBranchAddress("T2IdPt", &bT2IdPt[IN]);
  inputTree->SetBranchAddress("T2IdAlgo", &bT2IdAlgo[IN]);

  // Event filter (same variables as Offline, but with the suffix _EF)
  inputTree->SetBranchAddress("eg_nc_EF", &beg_nc_EF[IN]); 
  inputTree->SetBranchAddress("eg_e_EF", &beg_e_EF[IN]);
  inputTree->SetBranchAddress("eg_eta_EF", &beg_eta_EF[IN]);
  inputTree->SetBranchAddress("eg_phi_EF", &beg_phi_EF[IN]);
  inputTree->SetBranchAddress("eg_IsEM_EF", &beg_IsEM_EF[IN]); 
  inputTree->SetBranchAddress("eg_cl_et_EF", &beg_cl_et_EF[IN]);           // Cluster Et
  inputTree->SetBranchAddress("eg_trkmatchnt_EF", &beg_trkmatchnt_EF[IN]); // ??

  // Electron Offline reconstruction
  inputTree->SetBranchAddress("Ele_nc", &bEle_nc[IN]);        // Number of reconstructed seeds
  inputTree->SetBranchAddress("Ele_e", &bEle_e[IN]);          // Energy of the electron
  inputTree->SetBranchAddress("Ele_eta", &bEle_eta[IN]);      // Eta
  inputTree->SetBranchAddress("Ele_phi", &bEle_phi[IN]);      // Phi
  inputTree->SetBranchAddress("Ele_IsEM", &bEle_IsEM[IN]);    // Output (0 = electron, 1 = jets)

  // Ringer data
  inputTree->SetBranchAddress("Ringer_LVL1_Id", &bRinger_LVL1_Id[IN]);
  inputTree->SetBranchAddress("Ringer_NCells", &bRinger_NCells[IN]); 
  inputTree->SetBranchAddress("Ringer_NClusters", &bRinger_NClusters[IN]);
  inputTree->SetBranchAddress("Ringer_Roi_Id", &bRinger_Roi_Id[IN]);
  inputTree->SetBranchAddress("Ringer_LVL1_Eta", &bRinger_LVL1_Eta[IN]);
  inputTree->SetBranchAddress("Ringer_LVL1_Phi", &bRinger_LVL1_Phi[IN]); 
  inputTree->SetBranchAddress("Ringer_DetCells", &bRinger_DetCells[IN]);
  inputTree->SetBranchAddress("Ringer_EtaResCells", &bRinger_EtaResCells[IN]);
  inputTree->SetBranchAddress("Ringer_PhiResCells", &bRinger_PhiResCells[IN]);
  inputTree->SetBranchAddress("Ringer_EtaCells", &bRinger_EtaCells[IN]);
  inputTree->SetBranchAddress("Ringer_PhiCells", &bRinger_PhiCells[IN]);
  inputTree->SetBranchAddress("Ringer_ECells", &bRinger_ECells[IN]); 
  inputTree->SetBranchAddress("Ringer_LVL2_Et", &bRinger_LVL2_Et[IN]);
  inputTree->SetBranchAddress("Ringer_LVL2_Eta", &bRinger_LVL2_Eta[IN]);
  inputTree->SetBranchAddress("Ringer_LVL2_Phi", &bRinger_LVL2_Phi[IN]);
  inputTree->SetBranchAddress("Ringer_Rings", &bRinger_Rings[IN]); 

  TTree *outputTree = inputTree->CloneTree(0);

  // These are the cut variables!
  outputTree->SetBranchAddress("L1Em_Core", &L1Core[OUT]);
  outputTree->SetBranchAddress("L1Em_EmClus", &EmCore[OUT]);
  outputTree->SetBranchAddress("L1Em_EmIsol", &EmIsolation[OUT]);
  outputTree->SetBranchAddress("L1Em_HdCore", &HdCore[OUT]);
  outputTree->SetBranchAddress("L1Em_HdIsol", &HdIsolation[OUT]);
  outputTree->SetBranchAddress("L1Em_nRoI", &L1Em_nRoI[OUT]);
  outputTree->SetBranchAddress("L1Em_eta", &bL1Em_eta[OUT]);
  outputTree->SetBranchAddress("L1Em_phi", &bL1Em_phi[OUT]);

  // This is everything else ...
  outputTree->SetBranchAddress("Type", &bType[OUT]);
  outputTree->SetBranchAddress("PtGen", &bPtGen[OUT]);
  outputTree->SetBranchAddress("EtaGen",&bEtaGen[OUT]);
  outputTree->SetBranchAddress("PhiGen", &bPhiGen[OUT]);

  outputTree->SetBranchAddress("T2CaNclus", &bT2CaNclus[OUT]);
  outputTree->SetBranchAddress("T2CaEmE", &bT2CaEmE[OUT]); 
  outputTree->SetBranchAddress("T2CaEta", &bT2CaEta[OUT]);
  outputTree->SetBranchAddress("T2CaPhi", &bT2CaPhi[OUT]);
  outputTree->SetBranchAddress("T2CaRcore", &bT2CaRcore[OUT]);
  outputTree->SetBranchAddress("T2CaEratio", &bT2CaEratio[OUT]); 
  outputTree->SetBranchAddress("T2CaHadE", &bT2CaHadE[OUT]);

  outputTree->SetBranchAddress("T2CaEmES0",&bT2CaEmES0[OUT]);
  outputTree->SetBranchAddress("T2CaEmES1",&bT2CaEmES1[OUT]);
  outputTree->SetBranchAddress("T2CaEmES2",&bT2CaEmES2[OUT]);
  outputTree->SetBranchAddress("T2CaEmES3",&bT2CaEmES3[OUT]);
  outputTree->SetBranchAddress("T2CaWidth",&bT2CaWidth[OUT]);
  outputTree->SetBranchAddress("T2CaF73",&bT2CaF73[OUT]);
  outputTree->SetBranchAddress("T2CaHadES0",&bT2CaHadES0[OUT]);
  outputTree->SetBranchAddress("T2CaHadES1",&bT2CaHadES1[OUT]);
  outputTree->SetBranchAddress("T2CaHadES2",&bT2CaHadES2[OUT]);
  outputTree->SetBranchAddress("T2CaHadES3",&bT2CaHadES3[OUT]);

  // T2 Inner Detector  
  outputTree->SetBranchAddress("T2IdNtracks", &bT2IdNtracks[OUT]);
  outputTree->SetBranchAddress("T2IdEtac", &bT2IdEtac[OUT]);
  outputTree->SetBranchAddress("T2IdPhic", &bT2IdPhic[OUT]); 
  outputTree->SetBranchAddress("T2IdPt", &bT2IdPt[OUT]);
  outputTree->SetBranchAddress("T2IdAlgo", &bT2IdAlgo[OUT]);

  // Event filter (same variables as Offline, but with the suffix _EF)
  outputTree->SetBranchAddress("eg_nc_EF", &beg_nc_EF[OUT]); 
  outputTree->SetBranchAddress("eg_e_EF", &beg_e_EF[OUT]);
  outputTree->SetBranchAddress("eg_eta_EF", &beg_eta_EF[OUT]);
  outputTree->SetBranchAddress("eg_phi_EF", &beg_phi_EF[OUT]);
  outputTree->SetBranchAddress("eg_IsEM_EF", &beg_IsEM_EF[OUT]); 
  outputTree->SetBranchAddress("eg_cl_et_EF", &beg_cl_et_EF[OUT]);           // Cluster Et
  outputTree->SetBranchAddress("eg_trkmatchnt_EF", &beg_trkmatchnt_EF[OUT]); // ??

  // Electron Offline reconstruction
  outputTree->SetBranchAddress("Ele_nc", &bEle_nc[OUT]);        // Number of reconstructed seeds
  outputTree->SetBranchAddress("Ele_e", &bEle_e[OUT]);          // Energy of the electron
  outputTree->SetBranchAddress("Ele_eta", &bEle_eta[OUT]);      // Eta
  outputTree->SetBranchAddress("Ele_phi", &bEle_phi[OUT]);      // Phi
  outputTree->SetBranchAddress("Ele_IsEM", &bEle_IsEM[OUT]);    // Output (0 = electron, 1 = jets)

  // Ringer data
  outputTree->SetBranchAddress("Ringer_LVL1_Id", &bRinger_LVL1_Id[OUT]);
  outputTree->SetBranchAddress("Ringer_NCells", &bRinger_NCells[OUT]); 
  outputTree->SetBranchAddress("Ringer_NClusters", &bRinger_NClusters[OUT]);
  outputTree->SetBranchAddress("Ringer_Roi_Id", &bRinger_Roi_Id[OUT]);
  outputTree->SetBranchAddress("Ringer_LVL1_Eta", &bRinger_LVL1_Eta[OUT]);
  outputTree->SetBranchAddress("Ringer_LVL1_Phi", &bRinger_LVL1_Phi[OUT]); 
  outputTree->SetBranchAddress("Ringer_DetCells", &bRinger_DetCells[OUT]);
  outputTree->SetBranchAddress("Ringer_EtaResCells", &bRinger_EtaResCells[OUT]);
  outputTree->SetBranchAddress("Ringer_PhiResCells", &bRinger_PhiResCells[OUT]);
  outputTree->SetBranchAddress("Ringer_EtaCells", &bRinger_EtaCells[OUT]);
  outputTree->SetBranchAddress("Ringer_PhiCells", &bRinger_PhiCells[OUT]);
  outputTree->SetBranchAddress("Ringer_ECells", &bRinger_ECells[OUT]); 
  outputTree->SetBranchAddress("Ringer_LVL2_Et", &bRinger_LVL2_Et[OUT]);
  outputTree->SetBranchAddress("Ringer_LVL2_Eta", &bRinger_LVL2_Eta[OUT]);
  outputTree->SetBranchAddress("Ringer_LVL2_Phi", &bRinger_LVL2_Phi[OUT]);
  outputTree->SetBranchAddress("Ringer_Rings", &bRinger_Rings[OUT]); 

  // Address set.
  inputTree->SetCacheSize(0);
  outputTree->SetCacheSize(0);

  UInt_t TotalRoIPassed = 0;                 // Count the total number of (valid) RoIs that passed all the cuts
  UInt_t TotalRoI = 0;                       // Count the total number of (valid) RoIs analysed

  UInt_t nPassed = 0;                        // Count the number of RoIs passed for statistics
  UInt_t nEntries = inputTree->GetEntries(); // Number of entries on the input

  for (UInt_t i = 0; i < nEntries; i++) {

    inputTree->GetEntry(i);                // I've got entries from input file

    // Now process it
    nPassed = 0;                           // Number of RoIs that passed the cuts
    UInt_t roiAfterAppliedLvl1Index = 0;   // Index for items after re-application of Lvl1 filter

    bT2IdNtracks[OUT] = 0;
    bEle_nc[OUT] = 0;
    beg_nc_EF[OUT] = 0;

    for (Int_t j = 0; j < L1Em_nRoI[IN]; j++) {

      bool passedAppliedLvl1 = false;

      // Make LVL1 already applied cuts again to identify which elements passed
      passedAppliedLvl1 = theAppliedLvl1Threshold->second.cut(EmCore[IN]->at(j), EmIsolation[IN]->at(j), HdCore[IN]->at(j), HdIsolation[IN]->at(j));

      if (passedAppliedLvl1) {
	TotalRoI++;
      } // re-application of LVL1 (already applied!) cuts to identify the elements

      // Make the NEW cuts
      if (selectedThreshold->second.cut(EmCore[IN]->at(j), EmIsolation[IN]->at(j), HdCore[IN]->at(j), HdIsolation[IN]->at(j))) {
	if (passedAppliedLvl1) {
	  nPassed++;
	  TotalRoIPassed++;

	  L1Core[OUT]->push_back(L1Core[IN]->at(j));
	  EmCore[OUT]->push_back(EmCore[IN]->at(j));
	  EmIsolation[OUT]->push_back(EmIsolation[IN]->at(j));
	  HdCore[OUT]->push_back(HdCore[IN]->at(j));
	  HdIsolation[OUT]->push_back(HdIsolation[IN]->at(j));
	  bL1Em_eta[OUT]->push_back(bL1Em_eta[IN]->at(j));
	  bL1Em_phi[OUT]->push_back(bL1Em_phi[IN]->at(j));

          if (runTruth) {
	    bType[OUT]->push_back(bType[IN]->at(j));
	    bPtGen[OUT]->push_back(bPtGen[IN]->at(j));
	    bEtaGen[OUT]->push_back(bEtaGen[IN]->at(j));
	    bPhiGen[OUT]->push_back(bPhiGen[IN]->at(j));
          }

	  // T2Calo
	  bT2CaEmE[OUT]->push_back(bT2CaEmE[IN]->at(roiAfterAppliedLvl1Index));
	  bT2CaEta[OUT]->push_back(bT2CaEta[IN]->at(roiAfterAppliedLvl1Index));
	  bT2CaPhi[OUT]->push_back(bT2CaPhi[IN]->at(roiAfterAppliedLvl1Index));
	  bT2CaRcore[OUT]->push_back(bT2CaRcore[IN]->at(roiAfterAppliedLvl1Index));
	  bT2CaEratio[OUT]->push_back(bT2CaEratio[IN]->at(roiAfterAppliedLvl1Index));
	  bT2CaHadE[OUT]->push_back(bT2CaHadE[IN]->at(roiAfterAppliedLvl1Index));

	  bT2CaEmES0[OUT]->push_back(bT2CaEmES0[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaEmES1[OUT]->push_back(bT2CaEmES1[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaEmES2[OUT]->push_back(bT2CaEmES2[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaEmES3[OUT]->push_back(bT2CaEmES3[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaHadES0[OUT]->push_back(bT2CaHadES0[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaHadES1[OUT]->push_back(bT2CaHadES1[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaHadES2[OUT]->push_back(bT2CaHadES2[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaHadES3[OUT]->push_back(bT2CaHadES3[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaWidth[OUT]->push_back(bT2CaWidth[IN]->at(roiAfterAppliedLvl1Index)); 
	  bT2CaF73[OUT]->push_back(bT2CaF73[IN]->at(roiAfterAppliedLvl1Index)); 

	  bRinger_LVL1_Id[OUT]->push_back(bRinger_LVL1_Id[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_NCells[OUT]->push_back(bRinger_NCells[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_Roi_Id[OUT]->push_back(bRinger_Roi_Id[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_LVL1_Eta[OUT]->push_back(bRinger_LVL1_Eta[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_LVL1_Phi[OUT]->push_back(bRinger_LVL1_Phi[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_LVL2_Et[OUT]->push_back(bRinger_LVL2_Et[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_LVL2_Eta[OUT]->push_back(bRinger_LVL2_Eta[IN]->at(roiAfterAppliedLvl1Index));
	  bRinger_LVL2_Phi[OUT]->push_back(bRinger_LVL2_Phi[IN]->at(roiAfterAppliedLvl1Index));

          UInt_t NCellsBase = 0;
          if (roiAfterAppliedLvl1Index != 0) {
	    for (UInt_t k = 0; k < roiAfterAppliedLvl1Index; k++) {
	      NCellsBase += bRinger_NCells[IN]->at(k);
	    }
	  }

          for (UInt_t k = NCellsBase; k < (NCellsBase + bRinger_NCells[IN]->at(roiAfterAppliedLvl1Index)); k++) {
            bRinger_PhiResCells[OUT]->push_back(bRinger_PhiResCells[IN]->at(k));
            bRinger_EtaResCells[OUT]->push_back(bRinger_EtaResCells[IN]->at(k));
            bRinger_EtaCells[OUT]->push_back(bRinger_EtaCells[IN]->at(k));
	    bRinger_DetCells[OUT]->push_back(bRinger_DetCells[IN]->at(k));
	    bRinger_PhiCells[OUT]->push_back(bRinger_PhiCells[IN]->at(k));
	    bRinger_ECells[OUT]->push_back(bRinger_ECells[IN]->at(k));
	  }

	  UInt_t RingsBase = numberOfRingsOnRoI*roiAfterAppliedLvl1Index;
          for (UInt_t k = RingsBase; k < (RingsBase+numberOfRingsOnRoI); k++) {
	    bRinger_Rings[OUT]->push_back(bRinger_Rings[IN]->at(k));
	  }

	  // Those below this line must be matched by eta/phi
	  // Offline vectors 

	  // Match eta and phi
	  Int_t kBestMatch;
	  float deltaEta, deltaPhi, deltaEtaBestMatch, deltaPhiBestMatch;

	  kBestMatch = -1;
	  deltaEtaBestMatch = deltaEtaMinimumMatch;
	  deltaPhiBestMatch = deltaPhiMinimumMatch;

	  for (Int_t k = 0; k < bEle_nc[IN]; k++) {
	    deltaEta = doDeltaEta(bEle_eta[IN]->at(k), bL1Em_eta[IN]->at(j));
	    deltaPhi = doDeltaPhi(bEle_phi[IN]->at(k), bL1Em_phi[IN]->at(j));
	    if ( (deltaEta < deltaEtaBestMatch) &&
		 (deltaPhi < deltaPhiBestMatch) ) {
	      kBestMatch = k;
	      deltaEtaBestMatch = deltaEta;
	      deltaPhiBestMatch = deltaPhi;
	    }
	  }
	  
	  if (kBestMatch > -1) {  // Was there a match??
	    // Yes!
	    bEle_e[OUT]->push_back(bEle_e[IN]->at(kBestMatch));
	    bEle_eta[OUT]->push_back(bEle_eta[IN]->at(kBestMatch));
	    bEle_phi[OUT]->push_back(bEle_phi[IN]->at(kBestMatch));
	    bEle_IsEM[OUT]->push_back(bEle_IsEM[IN]->at(kBestMatch));
	    bEle_nc[OUT]++;
	  }

	  // EF vectors  
	  // Match eta and phi
	  kBestMatch = -1;
	  deltaEtaBestMatch = deltaEtaMinimumMatch;
	  deltaPhiBestMatch = deltaPhiMinimumMatch;
	  for (Int_t k = 0; k < beg_nc_EF[IN]; k++) {
	    deltaEta = doDeltaEta(beg_eta_EF[IN]->at(k), bL1Em_eta[IN]->at(j));
	    deltaPhi = doDeltaPhi(beg_phi_EF[IN]->at(k), bL1Em_phi[IN]->at(j));
	    if ( (deltaEta < deltaEtaBestMatch) &&
		 (deltaPhi < deltaPhiBestMatch) ) {
	      kBestMatch = k;
	      deltaEtaBestMatch = deltaEta;
	      deltaPhiBestMatch = deltaPhi;
	    }
	  }
	  
	  if (kBestMatch > -1) {  // Was there a match??
	    // Yes!
	    beg_e_EF[OUT]->push_back(beg_e_EF[IN]->at(kBestMatch));
	    beg_eta_EF[OUT]->push_back(beg_eta_EF[IN]->at(kBestMatch));
	    beg_phi_EF[OUT]->push_back(beg_phi_EF[IN]->at(kBestMatch));
	    beg_IsEM_EF[OUT]->push_back(beg_IsEM_EF[IN]->at(kBestMatch));
	    beg_cl_et_EF[OUT]->push_back(beg_cl_et_EF[IN]->at(kBestMatch));
	    beg_trkmatchnt_EF[OUT]->push_back(beg_trkmatchnt_EF[IN]->at(kBestMatch));
	    beg_nc_EF[OUT]++;
	  }

	  // T2 Inner Detector  
	  // Match eta and phi
	  kBestMatch = -1;
	  deltaEtaBestMatch = deltaEtaMinimumMatch;
	  deltaPhiBestMatch = deltaPhiMinimumMatch;
	  for (UInt_t k = 0; k < bT2IdNtracks[IN]; k++) {
	    deltaEta = doDeltaEta(bT2IdEtac[IN]->at(k), bL1Em_eta[IN]->at(j));
	    deltaPhi = doDeltaPhi(bT2IdPhic[IN]->at(k), bL1Em_phi[IN]->at(j));
	    if ( (deltaEta < deltaEtaBestMatch) &&
		 (deltaPhi < deltaPhiBestMatch) ) {
	      kBestMatch = k;
	      deltaEtaBestMatch = deltaEta;
	      deltaPhiBestMatch = deltaPhi;
	    }
	  }
	  
	  if (kBestMatch > -1) {  // Was there a match??
	    // Yes!
	    bT2IdEtac[OUT]->push_back(bT2IdEtac[IN]->at(kBestMatch));
	    bT2IdPhic[OUT]->push_back(bT2IdPhic[IN]->at(kBestMatch));
	    bT2IdPt[OUT]->push_back(bT2IdPt[IN]->at(kBestMatch));
	    bT2IdAlgo[OUT]->push_back(bT2IdAlgo[IN]->at(kBestMatch));
	    bT2IdNtracks[OUT]++;
	  }
	}
      }

      if (passedAppliedLvl1) {
	roiAfterAppliedLvl1Index++;
      }
      
    }

    L1Em_nRoI[OUT] = nPassed;
    bT2CaNclus[OUT] = nPassed;
    bRinger_NClusters[OUT] = nPassed;

    if (nPassed != 0) {
      outputTree->Fill();
    }

    // GetEntry was not called for the output, so we must clear all std::vectors!
    L1Core[OUT]->clear();
    EmCore[OUT]->clear();
    EmIsolation[OUT]->clear();
    HdCore[OUT]->clear();
    HdIsolation[OUT]->clear();
    bL1Em_eta[OUT]->clear();
    bL1Em_phi[OUT]->clear();

    bType[OUT]->clear();
    bPtGen[OUT]->clear();
    bEtaGen[OUT]->clear();
    bPhiGen[OUT]->clear();

    bT2CaEmE[OUT]->clear();
    bT2CaEta[OUT]->clear();
    bT2CaPhi[OUT]->clear();
    bT2CaRcore[OUT]->clear();
    bT2CaEratio[OUT]->clear();
    bT2CaHadE[OUT]->clear();

    bT2CaEmES0[OUT]->clear();
    bT2CaEmES1[OUT]->clear();
    bT2CaEmES2[OUT]->clear();
    bT2CaEmES3[OUT]->clear();
    bT2CaHadES0[OUT]->clear();
    bT2CaHadES1[OUT]->clear();
    bT2CaHadES2[OUT]->clear();
    bT2CaHadES3[OUT]->clear();
    bT2CaWidth[OUT]->clear();
    bT2CaF73[OUT]->clear();

    bT2IdEtac[OUT]->clear();
    bT2IdPhic[OUT]->clear();
    bT2IdPt[OUT]->clear();
    bT2IdAlgo[OUT]->clear();

    beg_e_EF[OUT]->clear();
    beg_eta_EF[OUT]->clear();
    beg_phi_EF[OUT]->clear();
    beg_IsEM_EF[OUT]->clear();
    beg_cl_et_EF[OUT]->clear();
    beg_trkmatchnt_EF[OUT]->clear();

    bEle_e[OUT]->clear();
    bEle_eta[OUT]->clear();
    bEle_phi[OUT]->clear();
    bEle_IsEM[OUT]->clear();

    bRinger_LVL1_Id[OUT]->clear();
    bRinger_NCells[OUT]->clear();
    bRinger_Roi_Id[OUT]->clear();
    bRinger_LVL1_Eta[OUT]->clear();
    bRinger_LVL1_Phi[OUT]->clear();
    bRinger_LVL2_Et[OUT]->clear();
    bRinger_LVL2_Eta[OUT]->clear();
    bRinger_LVL2_Phi[OUT]->clear();

    bRinger_EtaResCells[OUT]->clear();
    bRinger_PhiResCells[OUT]->clear();
    bRinger_DetCells[OUT]->clear();
    bRinger_EtaCells[OUT]->clear();
    bRinger_PhiCells[OUT]->clear();
    bRinger_ECells[OUT]->clear();
    bRinger_Rings[OUT]->clear();
  }

  // Save new Tree to output file and free memory
  outputTree->AutoSave("Overwrite");
  
  std::cout << "Done filtering and writing." << std::endl << TotalRoIPassed << "/" << TotalRoI << " RoIs passed the cuts (" << (((double) TotalRoIPassed*100)/((double) TotalRoI)) << "% passed)." << std::endl;

  delete inputTree;
  delete outputFile;

  return;
}
