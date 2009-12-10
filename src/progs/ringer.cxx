//Dear emacs, this is -*- c++ -*-

/**
 * @file ringer.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-23 09:03:57 $
 *  
 * Simplificado por Dhiana Deva - dhiana.deva@gmail.com
 *
 * Arranges calorimetry cell data (roiformat dump) in concentric rings of
 * configurable size and center and dump an XML file containing the ring'ified
 * information for every RoI and their metadata (LVL1 and RoI identifiers and
 * etaxphi information).
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
#include "TrigRingerTools/roiformat/roi_iterator.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "TrigRingerTools/data/PatternSet.h"


typedef struct param_t {
  std::string roidump; ///< roi dump file to read data from 
  std::string ringconfig; ///< ring configuration XML file
  std::string deadchannelsdump; ///< ring configuration XML file
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
  if (p.deadchannelsdump.size()) {
    if (!sys::exists(p.deadchannelsdump)) {
      throw RINGER_EXCEPTION("No ring configuration file");
    }
  }
  return true;
}

float absVal(float x){
  if(x < 0)
    x = -x;
  return x;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", "", "", false, 0.1, 0.1 };
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option("ring-config", 'c', par.ringconfig, 
			"location of the Ring Configuration XML file to use");
  opt_parser.add_option("eta-window", 'e', par.eta_window, 
			"Width of the window for peak searching in eta");
  opt_parser.add_option("global-center", 'g', par.global_center,
			"Use etaxphi center as provided by layer 2 (default is false)");
  opt_parser.add_option("phi-window", 'p', par.phi_window, 
			"Width of the window for peak searching in phi");
  opt_parser.add_option("roi-dump", 'r', par.roidump,
			"location of the RoI dumpfile to read data from");
  opt_parser.add_option("dead-channels", 'h', par.deadchannelsdump,
        	"location of the Dead Channels file to read data from");
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

  try {
    //Reading RoI with RoIIterator
    const std::string outputNT = "new_ntuple.root";
    roiformat::RoIIterator *it = new roiformat::RoIIterator(outputNT);
    
    //Adding files in roidump directory
    it->add(par.roidump);
    
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

    std::vector<roiformat::Cell> deadcells;
    // Reading dead channels list
    if (par.deadchannelsdump.size()) {
        int det;
        float eta, phi;
        std::ifstream arq(par.deadchannelsdump.c_str());
        arq >> det >> eta >> phi;
        while (!arq.eof())
        {
          roiformat::Cell deadcell( static_cast<roiformat::Cell::Sampling>( det ) , eta , phi , 0., 0., 0., 0., 0.);
          deadcells.push_back(deadcell);
          arq >> det >> eta >> phi;
        }
        arq.close();
    }
    
    //Looping through RoIs
    while(it->next()){
      size_t nCells = it->nCells();
      std::vector<roiformat::Cell> cells;
      std::vector<unsigned char> det_vec;
      std::vector<float> eta_vec;
      std::vector<float> phi_vec;
      std::vector<float> et_vec;
      bool dead_cell = false;
      const float dead_window = 0.0001;
      
      // Retrieving informations from iterator
      it->detectors(det_vec);
      it->eta(eta_vec);
      it->phi(phi_vec);
      it->energy(et_vec);

      // Format Cells
      for (size_t j = 0; j<nCells; j++)
      {
        // Verifies if cell is not on dead channels list.
        roiformat::Cell cell( static_cast<roiformat::Cell::Sampling>( static_cast<int>( det_vec.at(j) )) , eta_vec.at(j) , phi_vec.at(j) , 0., 0., 0., 0., et_vec.at(j));
        if (deadcells.size()>0) {
          dead_cell = false;
          for(Int_t i=0; i < deadcells.size(); i++){
            if( (cell.sampling() == deadcells[i].sampling()) && ( absVal(cell.eta() - deadcells[i].eta()) < dead_window ) && ( absVal(cell.phi()-deadcells[i].phi()) < dead_window) ){
              dead_cell = true;
            }
          }
          if (!dead_cell)
            cells.push_back(cell);
        } else cells.push_back(cell);
      }

      // Formats RoI
      const roiformat::RoI roi(cells, 0, 0, it->lvl1_eta(), it->lvl1_phi());

      // Finding center
      double eta, phi;
      bool ok = rbuild::find_center(reporter, &roi, eta, phi);
      ok &= par.global_center; // only use layer 2 center, if requested

      // Constructing rings
      rbuild::build_rings(reporter, &roi, rset, ok, eta, phi, par.eta_window, par.phi_window);

      // Normalizing rings
      rbuild::normalize_rings(reporter, rset);

      // Accessing rings
      std::vector<float> new_rings;
      bool rings_not_init = true; 
      data::Pattern these_rings(1);
      for (std::vector<rbuild::RingSet>::iterator jt=rset.begin(); jt!=rset.end(); ++jt) {
        if (rings_not_init) {
          these_rings = jt->pattern();
          rings_not_init = false;
        }
        else these_rings.append(jt->pattern());
      }
      
      // Constructing vector with the rings
      for(size_t i=0; i<these_rings.size();i++)
        new_rings.push_back(these_rings[i]);
      
      // Dumping new rings to new ntuple
      it->saveRoI(new_rings); 
    }
    delete(it);    
  }
  catch (sys::Exception& ex) {
    RINGER_FATAL(reporter, ex.what());
  }

  //dump ring cells in plain text format
  RINGER_REPORT(reporter, "Successful exit. Bye");
  delete reporter;
}
