//Dear emacs, this is -*- c++ -*-

/**
 * @file eta-filter.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.4 $
 * $Date: 2008-03-26 21:05:49 $
 *
 * Filters an RoI database based on the eta locations of every object.
 */

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/util.h"
#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include <popt.h>
#include <sstream>
#include <cmath>
#include <iomanip>

/**
 * Returns the basename of a file, without its extension and diretory prefixes
 * 
 * @param fullname The name of the file
 * @param ext The extension to strip
 */
std::string stripname (const std::string& fullname, const std::string& ext)
{
  //find location of last '/'
  size_t start = fullname.rfind("/");
  if (start == std::string::npos) start = 0;
  else ++start;
  //stopped here
  size_t end = fullname.rfind(ext);
  return fullname.substr(start, end-start);
}

typedef struct param_t {
  std::string db; ///< database to apply the filtering
  double eta; ///< the absolute eta location to use for splitting
  std::string prefix; ///< the db output for objects with smaller eta value
} param_t;

/**
 * Checks and validates program options.
 *
 * @param argc The number of arguments given to the program execution
 * @param argv The arguments given to program execution
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (int& argc, char**& argv, param_t& p, sys::Reporter *reporter)
{
  //defaults for each option
  char* db=0;
  double eta=0;

  //return `arg' is set to !=0, so the system processes everything in the
  //while loop bellow.
  struct poptOption optionsTable[] = {
    { "db", 'd', POPT_ARG_STRING, &db, 'd',
      "location of the database to split", "path" },
    { "interval", 'i', POPT_ARG_DOUBLE, &eta, 'i',
      "the interval to split the database", "double > 0" },
    POPT_AUTOHELP
    { 0, 0, 0, 0, 0 }
  };

  poptContext optCon = poptGetContext(NULL, argc, (const char**)argv,
				      optionsTable, 0);

  if (argc == 1) {
    poptPrintUsage(optCon, stderr, 0);
    return false;
  }

  char c;
  while ((c = poptGetNextOpt(optCon)) > 0) {
    switch (c) {
    case 'd': //db
      RINGER_DEBUG1("Database name is " << db);
      if (!sys::exists(db)) {
	RINGER_DEBUG1("Database file " << db << " doesn't exist.");
	throw RINGER_EXCEPTION("Database file doesn't exist");
      }
      break;
    case 'i': //eta threshold
      if (eta <= 0) {
	RINGER_DEBUG1("Setting the eta interval to <= 0 is"
		      << " prohibted. Exception thrown.");
	throw RINGER_EXCEPTION("Forbidden eta interval");
      }
      RINGER_DEBUG1("Eta interval set to " << eta);
      break;
    }
  }

  if (c < -1) {
    /* an error occurred during option processing */
    RINGER_FATAL(reporter, "Error during option processing with popt! "
		 << poptBadOption(optCon, POPT_BADOPTION_NOALIAS) << ": "
		 << poptStrerror(c));
  }

  //checks
  if (!db) {
    RINGER_DEBUG1("I cannot work without a database file. Exception thrown.");
    throw RINGER_EXCEPTION("No database file specified");
  } else p.db = db;
  if (!eta) {
    RINGER_DEBUG1("I cannot work with an eta threshold of ZERO."
		  << " Exception thrown.");
    throw RINGER_EXCEPTION("Valid eta interval specified?");
  } else p.eta = eta;
  p.prefix = stripname(db, ".xml");
  poptFreeContext(optCon);

  RINGER_DEBUG1("Command line options have been read.");
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();
  param_t par;

  try {
    if (!checkopt(argc, argv, par, reporter))
		{
			RINGER_FATAL(reporter, "Terminating execution.");
		}
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  try {
    //loads the DB
    data::DatabaseXml<data::RoIPatternSet> db(par.db, reporter);
    
    //checks db size
    if (db.size() < 2) {
      RINGER_FATAL(reporter, "The database you loaded contains only 1 class of"
		   " events. Please, reconsider your input file.");
    }

    size_t order = 0; ///< defines the dumping ording
    for (double eta_start = -2.5; eta_start < 2.51; eta_start += par.eta) {
      //build output data sets for electrons
      data::RoIPatternSet electrons(*db.data("electron"), 
				    eta_start, eta_start+par.eta);
      data::RoIPatternSet jets(*db.data("jet"), eta_start, eta_start+par.eta);
    
      //dump new databases at specified locations
      std::map<std::string, data::RoIPatternSet*> new_data;
      new_data["electron"] = &electrons;
      new_data["jet"] = &jets;
      std::ostringstream comment;
      comment << "DB for objects with eta greater or equal than " 
	      << eta_start << " but smaller than " << eta_start+par.eta << ".";
      data::Header header("Andre DOS ANJOS", "Filtered database on eta",
			  "1.0", time(0), comment.str());
      data::DatabaseXml<data::RoIPatternSet> new_db(&header, new_data, reporter);
      std::ostringstream filename;
      filename << par.prefix << "-" << std::setw(2) << std::setfill('0') 
	       << order << ".xml";
      new_db.save(filename.str());
      RINGER_REPORT(reporter, "Saved new database " << filename.str());
      ++order;
    }

  }
  catch (const sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "This was a top-level catch for a RINGER exception, " << "I have to exit, bye.");
  }
  catch (const std::exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "This was a top-level catch for a std exception, " << "I have to exit, bye.");
  }
  catch (...) {
    RINGER_FATAL(reporter, 
		 "This was a top-level catch for a unknown exception, "
		 << "I have to exit, bye.");
  }
  
  delete reporter;
}
