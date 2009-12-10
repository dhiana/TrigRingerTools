/*
 * The purpose of this program is to replace Ringer cells' energy information with the energy calculated
 * on the ring itself. This eases the view of the rings on CaloView Tool, showing the ring configuration in 3D
 *
 * It presumes the data file has been generated according to the latest CBNT making algorithms on Athena.
 *
 * Author: Danilo.Enoque.Ferreira.de.Lima@cern.ch
 *
 */

#include "RingsOnCells.h"

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>
#include <map>
#include <iostream>
#include "TObject.h"

#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"

typedef struct param_t {
  std::string roidump; ///< roi dump file to read data from 
  std::string ringconfig; ///< ring configuration XML file
  bool global_center;
} param_t;

/**
 * Checks and validates program options.
 *
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (const param_t& p, sys::Reporter* reporter)
{
  if (!p.roidump.size()) throw RINGER_EXCEPTION("No dump to read data from");
  if (!sys::exists(p.roidump)) {
    RINGER_DEBUG1("RoI dump file " << p.roidump << " doesn't exist.");
    throw RINGER_EXCEPTION("RoI dump file doesn't exist");
  }
  if (!p.ringconfig.size()) {
    throw RINGER_EXCEPTION("No ring configuration file");
  }
  if (!sys::exists(p.ringconfig)) {
    RINGER_DEBUG1("Ring config file " << p.ringconfig << " doesn't exist.");
    throw RINGER_EXCEPTION("Ring config file doesn't exist");
  }
  return true;
}
int main(int argc, char **argv) {

  sys::Reporter *reporter = new sys::LocalReporter;
  
  param_t par = { "", "" , false };
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option("ring-config", 'c', par.ringconfig, 
			"location of the Ring Configuration XML file to use");
  opt_parser.add_option("roi-dump", 'r', par.roidump,
			"location of the RoI dumpfile to read data from");
  opt_parser.add_option("global-center", 'g', par.global_center,
			"use the Ringer_LVL2_Eta and Ringer_LVL2_Phi information from input file as global RoI center?");
  opt_parser.parse(argc, argv);

  try {
    if (!checkopt(par, reporter))
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  std::string bname = sys::stripname(par.roidump);
  bname += "-caloviewer.root";
  TChain *inputTree = new TChain("CollectionTree");
  inputTree->Add(par.roidump.c_str());
  TFile *outputFile = new TFile(bname.c_str(), "RECREATE");

  RingsOnCells f(inputTree, reporter, par.global_center);
  f.setBranches();
  f.setStatusOfBranches();
  RingsOnCells *foutput = f.run(par.ringconfig);
  
  // Save new Tree to output file and free memory
  foutput->fChain->Write("", TObject::kOverwrite);
  
  delete inputTree;
  delete outputFile;
  delete reporter;

  return 0;
}
