//Dear emacs, this is -*- c++ -*-

/**
 * @file lvl1-filter.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.3 $
 * $Date: 2007-04-12 21:04:32 $
 *
 * A program that applies a simple e.m. LVL1 trigger on the RFD RoI's.
 */

#include "TrigRingerTools/lvl1/EMTrigger.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/roiformat/Database.h"

typedef struct param_t {
  std::string roidump; ///< roi dump file to read data from 
  double em_threshold; ///< the e.m. threshold to use, in GeV
  double em_isolation; ///< the e.m. isolation, in GeV
  double had_isolation; ///< the hadronic isolation, in GeV
  std::string output; ///< the output file to use
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
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", 20.0, 4.0, 2.0, "/dev/stdout" };
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option("hadronic-threshold", 'c', par.had_isolation,
			"The hadronic isolation in GeV");
  opt_parser.add_option("em-threshold", 'e', par.em_threshold,
			"The e.m. threshold in GeV");
  opt_parser.add_option("em-isolation", 'i', par.em_isolation,
			"The e.m. isolation in GeV");
  opt_parser.add_option("output", 'o', par.output,
			"The output file to use");
  opt_parser.add_option("roi-dump", 'r', par.roidump,
			"location of the RoI dumpfile to read data from");
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
    //load roidump first
    roiformat::Database roidump(reporter);
    roidump.load(par.roidump);
    RINGER_REPORT(reporter, "Loaded RoI dump database at \"" << par.roidump 
		  << "\".");
    RINGER_REPORT(reporter, "Input data base contains " 
		  << roidump.size() << " RoI's.");
    //then config
    lvl1::EMTrigger trigger(par.em_threshold, par.em_isolation, 
			    par.had_isolation);
    std::vector<const roiformat::RoI*> rois_that_passed;
    trigger.filter(reporter, roidump, rois_that_passed);
    roiformat::Database roisave(rois_that_passed, reporter);
    RINGER_REPORT(reporter, "Output data base contains " 
		  << roisave.size() << " RoI's.");
    RINGER_REPORT(reporter, "RoI usage: " 
		  << (100*roisave.size())/roidump.size() << "%");
    roisave.save(par.output);
  }

  catch (sys::Exception& ex) {
    RINGER_FATAL(reporter, ex.what());
  }
  //dump ring cells in plain text format
  RINGER_REPORT(reporter, "Successful exit. Bye");
  delete reporter;
}
