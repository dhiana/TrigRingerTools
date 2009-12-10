//Dear emacs, this is -*- c++ -*-

/**
 * @file splitter.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.4 $
 * $Date: 2008-03-26 21:05:49 $
 *
 * Splits a database in two, according to the proportions given
 */

#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/data/SumExtractor.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>

typedef struct param_t {
  std::string db; ///< input database
  std::string out1; ///< first output database
  std::string out2; ///< second output database
  double percentage; ///< the splitting percentage
  double energy_cut; ///< at which eT to cut
} param_t;

/**
 * Checks and validates program options.
 *
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (const param_t& par, sys::Reporter* reporter)
{
  if (par.percentage <= -1.0 || par.percentage >= 1.0) {
    RINGER_DEBUG1("Trying to set the training percentage to "<< par.percentage);
    throw RINGER_EXCEPTION("This value should be between (-1,1)");
  }
  if (par.energy_cut < 0.0) {
    RINGER_DEBUG1("Trying to set the energy threshold to "<< par.energy_cut);
    throw RINGER_EXCEPTION("This value should be between (0,Inf)");
  }
  if (!par.db.size()) {
    RINGER_DEBUG1("No DB file given! Throwing...");
    throw RINGER_EXCEPTION("Database file not given.");
  }
  if (!sys::exists(par.db)) {
    RINGER_DEBUG1("Database file " << par.db << " doesn't exist.");
    throw RINGER_EXCEPTION("Database file doesn't exist");
  }
  if (!par.out1.size()) {
    RINGER_DEBUG1("First output filename not given! Throwing...");
    throw RINGER_EXCEPTION("First output file not given.");
  }
  if (!par.out2.size()) {
    RINGER_DEBUG1("Second output filename not given! Throwing...");
    throw RINGER_EXCEPTION("Second output file not given.");
  }
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", "", "", 0.5, 170000 };
  sys::OptParser opt_parser(argv[0]);

  opt_parser.add_option
    ("train-percentage", 'a', par.percentage,
     "how much of the database to use for first part of the split");
  opt_parser.add_option
    ("energy-cut", 'c', par.energy_cut,
     "energy threshold to cut events");
  opt_parser.add_option
    ("db", 'd', par.db,
     "location of the database to use for the operation");
  opt_parser.add_option
    ("out1", 't', par.out1, "name of the first output file");
  opt_parser.add_option
    ("out2", 'u', par.out2, "name of the second output file");
  opt_parser.parse(argc, argv);

  try {
    if (!checkopt(par, reporter))
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  //loads the DB
  data::DatabaseXml<data::RoIPatternSet> db(par.db, reporter);

  //execute the cutting operation
  for (std::map<std::string, data::RoIPatternSet*>::const_iterator
         it = db.data().begin(); it != db.data().end(); ++it) {
    for (size_t i = 0; i < it->second->size();) {
      data::SumExtractor sum;
      if ( sum(it->second->pattern(i)) > par.energy_cut ) {
        RINGER_REPORT(reporter, "Erasing pattern " << i << " from " 
                      << it->first << " set.");
        const_cast<data::RoIPatternSet*>(it->second)->erase_pattern(i);
      }
      else ++i;
    }
  }

  //splits the database in train and test
  data::DatabaseXml<data::RoIPatternSet>* traindb;
  data::DatabaseXml<data::RoIPatternSet>* testdb;
  std::vector<std::string> cnames;
  db.class_names(cnames);
  db.split(par.percentage, (data::Database<data::RoIPatternSet> *&) traindb, (data::Database<data::RoIPatternSet> *&) testdb);
  RINGER_REPORT(reporter, "Train set size is " << traindb->size());
  RINGER_REPORT(reporter, "Test set size is " << testdb->size());

  traindb->save(par.out1);
  RINGER_REPORT(reporter, "Output file \"" << par.out1
                << "\" was correctly saved and closed.");
  testdb->save(par.out2);
  RINGER_REPORT(reporter, "Output file \"" << par.out2
                << "\" was correctly saved and closed.");
  
  delete traindb;
  delete testdb;
  RINGER_REPORT(reporter, "Successful exit. Bye");
  delete reporter;
}
