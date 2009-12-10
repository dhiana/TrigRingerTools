//Dear emacs, this is -*- c++ -*-

/**
 * @file ringer-run.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.4 $
 * $Date: 2008-03-26 21:05:49 $
 *
 * Arranges calorimetry cell data (roiformat dump) in concentric rings of
 * configurable size and center and dump an XML file containing the ring'ified
 * information for every RoI and their metadata (LVL1 and RoI identifiers and
 * etaxphi information).
 */

#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/roiformat/Database.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/network/MLP.h"
#include <iostream>
#include <popt.h>
#include <cstdio>
#include <cmath>
#include <sys/time.h>

/**
 * Returns the difference between two time values stored in structures of type
 * `struct timeval', in microseconds
 *
 * @param x The first operand
 * @param y The second operand
 */
double timeval_diff (struct timeval& x, struct timeval& y)
{
  /* Perform the carry for the later subtraction by updating Y. */
  if (x.tv_usec < y.tv_usec) {
    int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
    y.tv_usec -= 1000000 * nsec;
    y.tv_sec += nsec;
  }
  if (x.tv_usec - y.tv_usec > 1000000) {
    int nsec = (x.tv_usec - y.tv_usec) / 1000000;
    y.tv_usec += 1000000 * nsec;
    y.tv_sec -= nsec;
  }
  
  /* Compute the time remaining to wait.
     `tv_usec' is certainly positive. */
  double retval = (x.tv_sec - y.tv_sec) * 1e6;
  retval += x.tv_usec - y.tv_usec;
  if (x.tv_sec < y.tv_sec) retval *= -1;
  return retval;
}

typedef struct param_t {
  std::string roidump; ///< roi dump file to read data from 
  std::string ringconfig; ///< ring configuration XML file
  bool time; ///< <code>true</code> if I should time the execution
  std::string timings; ///< where to place timing information
  std::string net; ///< name of the neural net file
  std::string output; ///< where to save the output 
  bool global_center; ///< should I calculate a peak on each calo layer?
  double eta_window; ///< the eta size of the window for peak finding
  double phi_window; ///< the phi size of the window for peak finding
} param_t;

/**
 * Checks and validates program options.
 *
 * @param argc The number of arguments given to the program execution
 * @param argv The arguments given to program execution
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (param_t& p, sys::Reporter* reporter)
{
  if (p.timings.size()) p.time = true;

  //checks
  if (!p.roidump.size()) throw RINGER_EXCEPTION("No dump to read data from");

  if (!sys::exists(p.roidump)) {
    RINGER_DEBUG1("RoI dump file " << p.roidump << " doesn't exist.");
    throw RINGER_EXCEPTION("RoI dump file doesn't exist");
  }

  if (!p.output.size()) {
    p.output = sys::stripname(p.roidump) + ".out.xml";
    RINGER_DEBUG1("Setting output file name to " << p.output);
  }

  if (!p.net.size()) {
    RINGER_DEBUG1("I cannot work without a network file. Exception thrown.");
    throw RINGER_EXCEPTION("No network file specified");
  } 

  if (!p.ringconfig.size()) 
    throw RINGER_EXCEPTION("No ring configuration file");

  if (!sys::exists(p.ringconfig)) {
    RINGER_DEBUG1("Ring config file " << p.ringconfig << " doesn't exist.");
    throw RINGER_EXCEPTION("Ring config file doesn't exist");
  }

  if (p.time)
    RINGER_REPORT(reporter, "Timings will be output at \"" 
		  << p.timings << "\".");
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", "", false, "", "", "", false, 0.1, 0.1};
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option("ring-config", 'c', par.ringconfig, 
			"location of the Ring Configuration XML file to use");
  opt_parser.add_option("eta-window", 'e', par.eta_window, 
			"Width of the window for peak searching in eta");
  opt_parser.add_option("global-center", 'g', par.global_center,
			"Use etaxphi center as provided by layer 2 (default is false)");
  opt_parser.add_option("net", 'n', par.net, "where to read the network");
  opt_parser.add_option("output", 'o', par.output, 
			"where to write the output of the MLP neural-network");
  opt_parser.add_option("phi-window", 'p', par.phi_window, 
			"Width of the window for peak searching in phi");
  opt_parser.add_option("roi-dump", 'r', par.roidump,
			"location of the RoI dumpfile to read data from");
  opt_parser.add_option("timings", 't', par.timings,
			"should timings be output by this application?");
  opt_parser.parse(argc, argv);

  try {
    if (!checkopt(par, reporter))
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  //loads the Network
  RINGER_REPORT(reporter, "Loading network \"" << par.net << "\"...");
  network::Network net(par.net, reporter);
  
  //start processing
  std::string bname = sys::stripname(par.roidump);

  try {

    //load roidump first
    roiformat::Database roidump(reporter);
    roidump.load(par.roidump);
    RINGER_REPORT(reporter, "Loaded RoI dump database at \"" << par.roidump 
		  << "\".");
    //then config
    rbuild::Config config(par.ringconfig, reporter);
    RINGER_REPORT(reporter, "Loaded Ring configuration at \""
		  << par.ringconfig << "\".");
    //do the processing:
    //1. For each configured ring set, create a *real* RingSet
    std::vector<rbuild::RingSet> rset;
    const std::map<unsigned int, rbuild::RingConfig>& rconfig = 
      config.config();
    unsigned int nrings = 0;
    for (std::map<unsigned int, rbuild::RingConfig>::const_iterator 
	   it=rconfig.begin(); it!=rconfig.end(); ++it) {
      rset.push_back(it->second);
      nrings += it->second.max();
    } //creates, obligatorily, ordered ring sets

    //2. Pass the relevant cells through the relevant RingSet(s).
    std::vector<const roiformat::RoI*> rois;
    roidump.rois(rois); //get a handle to all RoI's available
    data::RoIPatternSet outdb(roidump.size(), net.output_size());

    struct timeval start_time, peak_time, ring_time, norm_time, net_time;
    data::RoIPatternSet timedb(roidump.size(), 4);

    size_t i=0;
    for (std::vector<const roiformat::RoI*>::const_iterator
	   it=rois.begin(); it!=rois.end(); ++it) {
      RINGER_REPORT(reporter, "RoI -> L1Id #" << (*it)->lvl1_id() 
		    << " - RoI #" << (*it)->roi_id());
      gettimeofday(&start_time, 0);
      double eta, phi; //center values
      bool ok = rbuild::find_center(reporter, *it, eta, phi);
      ok &= par.global_center;
      gettimeofday(&peak_time, 0);
      rbuild::build_rings(reporter, *it, rset, ok, eta, phi, 
			  par.eta_window, par.phi_window);
      gettimeofday(&ring_time, 0);
      rbuild::normalize_rings(reporter, rset);
      gettimeofday(&norm_time, 0);

      //extra "magic" to place the entry at the output DB
      bool rings_not_init = true;
      data::Pattern these_rings(1);
      data::Pattern net_output(1);
      for (std::vector<rbuild::RingSet>::iterator 
	     jt=rset.begin(); jt!=rset.end(); ++jt) {
	//Now store the values as a data::Pattern before returning
	if (rings_not_init) {
	  these_rings = jt->pattern();
	  rings_not_init = false;
	}
	else these_rings.append(jt->pattern());
      } //for each RingSet (third iteration)

      //pass the normalized patternset through the network
      net.run(these_rings, net_output);
      gettimeofday(&net_time, 0);

      data::Pattern timings(4, 0);
      timings[0] = timeval_diff(peak_time, start_time);
      timings[1] = timeval_diff(ring_time, peak_time);
      timings[2] = timeval_diff(norm_time, ring_time);
      timings[3] = timeval_diff(net_time, norm_time);

      data::RoIPatternSet::RoIAttribute attr;
      attr.lvl1_id = (*it)->lvl1_id();
      attr.roi_id = (*it)->roi_id();
      attr.eta = (*it)->eta();
      attr.phi = (*it)->phi();
      outdb.set_pattern(i, net_output, attr);
      timedb.set_pattern(i, timings, attr);
      ++i;

    } //for all RoI's

    //prepare the database to dump
    std::string comment = "Originated from file ";
    comment += par.roidump;
    data::Header h("Andre ANJOS", bname, "2.0", time(0), comment);
    std::map<std::string, data::RoIPatternSet*> psmap;
    psmap[bname] = &outdb;
    data::DatabaseXml<data::RoIPatternSet> db(&h, psmap, reporter);
    db.save(par.output);
    RINGER_REPORT(reporter, "Output file \"" << par.output
		  << "\" was correctly saved and closed.");

    if (par.time) {
      //prepare the timings database to dump
      std::string comment = "Timings for file ";
      comment += par.roidump;
      data::Header h("Andre ANJOS", bname, "2.0", time(0), comment);
      std::map<std::string, data::RoIPatternSet*> tpsmap;
      tpsmap[bname] = &timedb;
      data::DatabaseXml<data::RoIPatternSet> tdb(&h, tpsmap, reporter);
      tdb.save(par.timings);
      RINGER_REPORT(reporter, "Timings file \"" << par.timings
		    << "\" was correctly saved and closed.");
    }

  } //try clause

  catch (sys::Exception& ex) {
    RINGER_FATAL(reporter, ex.what());
  }
  //dump ring cells in plain text format
  RINGER_REPORT(reporter, "Successful exit. Bye");
  delete reporter;
}




