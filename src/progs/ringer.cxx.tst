//Dear emacs, this is -*- c++ -*-

/**
 * @file ringer.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-23 09:03:57 $
 *
 * Arranges calorimetry cell data (roiformat dump) in concentric rings of
 * configurable size and center and dump an XML file containing the ring'ified
 * information for every RoI and their metadata (LVL1 and RoI identifiers and
 * etaxphi information).
 * 
 * Simplified by Dhiana Deva
 * 
 */

#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/data/DatabaseRoot.h"
#include "TrigRingerTools/roiformat/Database.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "TrigRingerTools/data/PatternSet.h"
#include "TrigRingerTools/roiformat/roi_iterator.h"

typedef struct param_t {
  std::string roidump; ///< roi dump file to read data from 
  std::string ringconfig; ///< ring configuration XML file
  std::string deadchannels; ///< dead channels list file
  bool global_center; ///< should I calculate a peak on each calo layer?
  double eta_window; ///< the eta size of the window for peak finding
  double phi_window; ///< the phi size of the window for peak finding
  bool dumproot;
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
  if (p.deadchannels.size()) {
    if (!sys::exists(p.deadchannels)) {
      RINGER_DEBUG1("Dead channels file" << p.deadchannels << "doesn't exist");
      throw RINGER_EXCEPTION("Dead channels file doesn't exist");
    }
  }
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", "", "", false, 0.1, 0.1 };
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option("ring-config", 'c', par.ringconfig, 
			"location of the Ring Configuration XML file to use");
  opt_parser.add_option("dead-channels", 'n', par.deadchannels,
            "location of the Dead Channels file to use");
  opt_parser.add_option("eta-window", 'e', par.eta_window, 
			"Width of the window for peak searching in eta");
  opt_parser.add_option("global-center", 'g', par.global_center,
			"Use etaxphi center as provided by layer 2 (default is false)");
  opt_parser.add_option("phi-window", 'p', par.phi_window, 
			"Width of the window for peak searching in phi");
  opt_parser.add_option("roi-dump", 'r', par.roidump,
			"location of the RoI dumpfile to read data from");
  opt_parser.add_option("dump-root", 'd', par.dumproot,
			"whether to dump to a ROOT file or XML file");
  opt_parser.parse(argc, argv);

  try {
    if (!checkopt(par, reporter))
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  RINGER_REPORT(reporter, "Dhiana testando...");

  //Reading RoI with RoIIterator
  const std::string outputNT = "dhiana_test.root";
  roiformat::RoIIterator *it = new roiformat::RoIIterator(outputNT);
  //Adding files in roidump directory
  it->add(par.roidump);

  try {
    // Reading configuration
    rbuild::Config config(par.ringconfig, reporter);
    RINGER_REPORT(reporter, "Loaded Ring configuration at \"" << par.ringconfig << "\".");
    // Configure ringer sets
    std::vector<rbuild::RingSet> rset;
    const std::map<unsigned int, rbuild::RingConfig>& rconfig = config.config();
    unsigned int nrings = 0;
    for (std::map<unsigned int, rbuild::RingConfig>::const_iterator it=rconfig.begin(); it!=rconfig.end(); ++it)
    {
      rset.push_back(it->second);
      nrings += it->second.max();
    }
    // Reading dead channels list
    std::vector<roiformat::Cell> deadcells;
    unsigned det;
    float eta, phi;
    std::ifstream arq("deathTest.txt");
    arq >> det >> eta >> phi;
    while (!arq.eof())
    {
      roiformat::Cell deadcell( static_cast<roiformat::Cell::Sampling>( static_cast<int>(det) ) , eta , phi , 0., 0., 0., 0., 0.);
      deadcells.push_back(deadcell);
      arq >> det >> eta >> phi;
    }
    arq.close(); 
    // Looping through RoIs
    while(it->next()){
    	// Formats Cells
        size_t nCells = it->nCells();
    	std::vector<roiformat::Cell> cells;
        std::vector<unsigned char> det_vec;
        std::vector<float> eta_vec;
        std::vector<float> phi_vec;
        std::vector<float> et_vec;
        it->detectors(det_vec);
        it->eta(eta_vec);
        it->phi(phi_vec);
        it->energy(et_vec);
    	for (size_t j = 0; j<nCells; j++)
    	{
    		roiformat::Cell cell( static_cast<roiformat::Cell::Sampling>( static_cast<int>( det_vec.at(j) )) , eta_vec.at(j) , phi_vec.at(j) , 0., 0., 0., 0., et_vec.at(j));
    		cells.push_back(cell);
    	}
        // Verifies dead channel list
    	// Formats RoI
    	const roiformat::RoI roi(cells, 0, 0, it->lvl1_eta(), it->lvl1_phi());
    	// Finding center
    	double eta, phi;
        bool ok = rbuild::find_center(reporter, &roi, eta, phi);
        ok &= par.global_center; // only use layer 2 center, if requested
        // Constructs rings
        rbuild::build_rings(reporter, &roi, rset, ok, eta, phi, par.eta_window, par.phi_window);
        // Normalize rings
        rbuild::normalize_rings(reporter, rset);
        // Accessing rings
        std::vector<double> new_rings;
        size_t k = 0;
        for (std::vector<rbuild::RingSet>::iterator it=rset.begin(); it!=rset.end(); it++)
        {
          data::Pattern pat = it->pattern();
          for (size_t j=0; j<pat.size(); j++) 
            new_rings.at(k++) = pat[j];
        }
        // Saves into new Ntuple
        it->saveRoI();
        //Deletar os vectors!
        //Mas não foi alocado dinamicamente...
      }                                           //while next
  }                                               //try
  catch (sys::Exception& ex) {
    RINGER_FATAL(reporter, ex.what());
  }
  delete it;
  RINGER_REPORT(reporter, "Successful exit. Bye");
  delete reporter;
}
