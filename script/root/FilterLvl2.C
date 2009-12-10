/*
 * The purpose of this script is run T2Calo Egamma cuts on a CBNT data file.
 * It presumes the data file has been generated according to the latest CBNT making algorithms on Athena,
 * after having run the Offline Algorithms (doESD = True or doAOD = True), the Trigger (LVL1, LVL2 Calo,
 * EF Calo and EF ID) and the Truth.
 * To make the filtering it is necessary to have only LVL1 branches and T2Calo branches. Therefore, this
 * script can be changed easily to remove unnecessary branches.
 *
 *
 * To execute this script, ideally, one must have an ntuple (e.g. xyz.root) and one must know which LVL1
 * threshold was used to generate it.
 * First generate the CollectionTree.C and CollectionTree.h files from the ntuple (EXAMPLES are provided):
 * $ root xyz.root
 * root [0] CollectionTree->MakeClass();
 * Now the necessary CollectionTree.* files were generated. One must compile the script:
 * root [1] .L FilterLvl2.C+
 * Next, run the script:
 * root [2] FilterLvl2("xyz.root", "xyzOut.root");
 * xyzOut will be generated. This presumes the TTree is named CollectionTree and that the LVL1 cut applied
 * on Athena prior to this script is EM01. The LVL1 cut can be changed easily:
 * root [3] FilterLvl2("xyz.root", "xyzOut.root", "EM11");
 *
 */


#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>
#include <map>
#include <iostream>
#include "CollectionTree.h"
#include "Threshold.h"
#include "FilterLvl2Tree.h"

void FilterLvl2(const TString &inputName, const TString &outputName,
		const TString &appliedLvl1Threshold = "EM01",
		const TString &tree = "CollectionTree") {

  // Description of input parameters:
  // inputName is/are the input file(s) (it may include wildcards).
  // outputName is the output file, where filtered data will be saved.
  // tree is the name of the TTree where information was saved (usually "CollectionTree")
  // appliedLvl1Thresold is the threshold applied by Athena whence the saving of data

  // TODO: Add other menus

  loadThresholdMap();

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

  Long64_t TotalRoIPassed, TotalRoI;

  FilterLvl2Tree f(inputTree);
  FilterLvl2Tree *foutput = f.filter(theAppliedLvl1Threshold->second, TotalRoI, TotalRoIPassed);
  
  // Save new Tree to output file and free memory
  foutput->fChain->AutoSave("Overwrite");
  
  std::cout << "Done filtering and writing." << std::endl << TotalRoIPassed << "/" << TotalRoI << " RoIs passed the cuts (" << (((double) TotalRoIPassed*100)/((double) TotalRoI)) << "% passed)." << std::endl;

  delete inputTree;
  delete outputFile;

  return;
}
