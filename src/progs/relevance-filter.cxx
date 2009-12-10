//Dear emacs, this is -*- c++ -*-

/**
 * @file relevance-filter.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-22 16:53:56 $
 *
 * Filters an RoI database based on the test relevance of their features,
 * removing their less relevant ensembles and saving a new database.
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
#include "TrigRingerTools/sys/Plain.h"

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
  double thres; ///< the value of the threshold cut
  std::string relevance; ///< where are the relevances are
  std::string output; ///< where to place the output
  bool reverse; ///< revert the behaviour, erasing relevant ensembles
} param_t;

/**
 * Checks and validates program options.
 *
 * @param argc The number of arguments given to the program execution
 * @param argv The arguments given to program execution
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (int& argc, char**& argv, param_t& p, sys::Reporter* reporter)
{
  //defaults for each option
  char* db=0;
  double thres=0;
  char* relevance=0;
  bool reverse=false;

  //return `arg' is set to !=0, so the system processes everything in the
  //while loop bellow.
  struct poptOption optionsTable[] = {
    { "db", 'd', POPT_ARG_STRING, &db, 'd',
      "location of the database to split", "path" },
    { "relevance", 'r', POPT_ARG_STRING, &relevance, 'r',
      "file containing the relevance for each ensemble", "path" },
    { "threshold", 't', POPT_ARG_DOUBLE, &thres, 't',
      "threshold for relevance cut", "double > 0" },
    { "reverse", 'v', POPT_ARG_NONE, 0, 'v',
      "Reverts the behaviour of this application, making it erase revelant ensembles instead of non-relevant ones", 0 },
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
    case 'r': //relevance
      RINGER_DEBUG1("Relevance will be fetched from " << relevance);
      if (!sys::exists(relevance)) {
	RINGER_DEBUG1("Relevance file " << relevance << " doesn't exist.");
	throw RINGER_EXCEPTION("Relevance file doesn't exist");
      }
      break;
    case 't': //threshold
      if (thres <= 0) {
	RINGER_DEBUG1("Setting the threshold to less or equal zero is"
		      << " prohibted. Exception thrown.");
	throw RINGER_EXCEPTION("Forbidden threshold");
      }
      RINGER_DEBUG1("Threshold set to " << thres);
      break;
    case 'v': //revert
      reverse = true;
      RINGER_DEBUG1("Reverse behaviour flag detected. I'll preserve"
		    << " non-relevant ensembles instead of erasing them.");
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
  if (!thres) {
    RINGER_DEBUG1("I cannot work with an eta threshold of ZERO."
		  << " Exception thrown.");
    throw RINGER_EXCEPTION("Valid eta threshold specified?");
  } else p.thres = thres;
  std::string prefix = stripname(db, ".xml");
  std::ostringstream thres_str;
  thres_str << thres;
  p.output = prefix + ".relevance-" + thres_str.str() + ".xml";
  if (!relevance) {
    RINGER_DEBUG1("I cannot work without a relevance file. Please specify."
		  << " Aborting execution with an exception.");
    throw RINGER_EXCEPTION("No relevance file.");
  } else p.relevance = relevance;
  p.reverse = reverse;
  poptFreeContext(optCon);

  RINGER_DEBUG1("Command line options have been read.");
  return true;
}

/**
 * Loads a set of ensemble relevances from a file, preserving the order for
 * each entry
 *
 * @param fname The filename containing the eta locations
 * @param m The map where to put the order and the relevances
 */
void load_relevances (const std::string& fname,
		      std::map<unsigned int, double>& m)
{
  sys::Plain in(fname);
  //skips first line
  std::string tmp;
  in >> tmp >> tmp >> tmp;
  while (!in.eof() && in.good()) {
    unsigned int order;
    double relevance;
    double dummy;
    in >> order;
    if (in.eof()) break;
    in >> relevance >> dummy;
    m[order] = relevance;
  }
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();
  param_t par;

  try {
    if (!checkopt(argc, argv, par, reporter))
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  try {
    //gets the relevance inputs
    std::map<unsigned int, double> relevance;
    load_relevances(par.relevance, relevance);
    //test maximum relevance value
    double max_relevance = 0;
    for (std::map<unsigned int, double>::const_iterator 
	   it = relevance.begin(); it != relevance.end(); ++it) {
      if (it->second > max_relevance) max_relevance = it->second;
    }
    if (par.thres > max_relevance) {
      RINGER_FATAL(reporter, "The maximum relevance value I found at " 
		   << par.relevance << " is " << max_relevance << ". The"
		   << " resulting output will be void. Please reconsider your"
		   << " threshold (" << par.thres << "). Aborting.");
    }
       
    //loads the DB
    data::DatabaseXml<data::RoIPatternSet> db(par.db, reporter);
    
    if (db.data("electron")->pattern_size() != relevance.size()) {
      RINGER_FATAL(reporter, "The number of relevance entries in " 
		   << par.relevance << " (" << relevance.size() 
		   << ") differs from the " << " database pattern size (" 
		   << db.data("electron")->pattern_size() << "). Aborting.");
    }

    RINGER_REPORT(reporter, "Cutting on " << relevance.size()
		  << " ensembles.");

    //backwards, because when I remove ensembles, they cease to exist, and
    //that will, potentially, create an exception of "unexisting" ensemble.
    for (std::map<unsigned int, double>::reverse_iterator
	   it = relevance.rbegin(); it != relevance.rend(); ++it) {
      RINGER_REPORT(reporter, "Processing ensemble `" << it->first
		    << "'.");
      const std::map<std::string, data::RoIPatternSet*>& data = db.data();
      //@warning: this is a hack
      std::map<std::string, data::RoIPatternSet*>* change = 
	const_cast<std::map<std::string, data::RoIPatternSet*>*>(&data);
      for (std::map<std::string, data::RoIPatternSet*>::iterator
	     jt = change->begin(); jt != change->end(); ++jt) {
	if (it->second < par.thres && !par.reverse) { //cut
	  RINGER_DEBUG1("Removing ensemble `" << it->first << "'.");
	  jt->second->erase_ensemble(it->first);
	}
	else if (it->second >= par.thres && par.reverse) { //cut
	  RINGER_DEBUG1("Removing ensemble `" << it->first << "'.");
	  jt->second->erase_ensemble(it->first);
	}
      }
    }

    RINGER_REPORT(reporter, "The resulting ensemble size after cutting is " 
		  << db.data("electron")->pattern_size() << ".");
    //save db
    db.save(par.output);
    RINGER_REPORT(reporter, "Saved new database with name " << par.output
		  << ".");
    

  }
  catch (const sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, 
		 "This was a top-level catch for a RINGER exception, "
	       << "I have to exit, bye.");
  }
  catch (const std::exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "This was a top-level catch for a std exception, "
	       << "I have to exit, bye.");
  }
  catch (...) {
    RINGER_FATAL(reporter, 
		 "This was a top-level catch for a unknown exception, "
		 << "I have to exit, bye.");
  }
  
  delete reporter;
}
