//Dear emacs, this is -*- c++ -*-

/**
 * @file merge.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.4 $
 * $Date: 2008-03-26 21:05:49 $
 *
 * Merges a set of databases into a common XML file.
 */

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include <map>
#include <string>
#include <sstream>
#include <popt.h>

/**
 * Defines a set of options for program execution
 */
typedef struct param_t {
  bool single; ///< single class in the output file
  std::string output; ///< the output filename
  std::map<std::string, std::string> classmap; ///< the class organisation
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
  bool multi = false;
  char* output = 0;
  struct poptOption optionsTable[] = {
    { "multi-class", 'm', POPT_ARG_NONE, 0, 'm',
      "The resulting DB is a multiclass database", 0},
    { "output", 'o', POPT_ARG_STRING, &output, 'o',
      "The resulting DB will be dumped to this filename", "path"}, 
    POPT_AUTOHELP
    { 0, 0, 0, 0, 0 }
  };

  poptContext optCon = poptGetContext(NULL, argc, (const char**)argv,
				      optionsTable, 0);
  poptSetOtherOptionHelp(optCon,
			 "[OPTION]* <file1> [<class1>] [<fileN> [class<N>]]+");

  if (argc == 1) {
    poptPrintUsage(optCon, stderr, 0);
    return false;
  }

  char c;
  while ((c = poptGetNextOpt(optCon)) > 0) {
    RINGER_DEBUG1("Reading option `" << c << "' from command line.");
    switch (c) {
    case 'm': //multi-class output
      multi = true;
      RINGER_DEBUG1("Final database is expected to be multiclass.");
      break;
    case 'o': //output file
      RINGER_DEBUG1("Output will be re-directed to file \"" << output 
		    << "\".");
      break;
    }
  }

  if (c < -1) {
    /* an error occurred during option processing */
    RINGER_FATAL(reporter, "Error during option processing with popt! "
		 << poptBadOption(optCon, POPT_BADOPTION_NOALIAS) << ": "
		 << poptStrerror(c));
  }

  p.single = !multi;

  if (p.single) {
    RINGER_DEBUG1("Operating in single merge class mode. "
		  << "Arguments *MUST* mean the DB names to merge!");
    while (poptPeekArg(optCon)) {
      p.classmap[poptGetArg(optCon)] = "merged-data";
    }
  }
  else {
    RINGER_DEBUG1("Operating in multi-class mode. "
		  << "Arguments *MUST* mean the DB names and classes");
    while (poptPeekArg(optCon)) {
      const char* dbname = poptGetArg(optCon);
      const char* cname = poptGetArg(optCon);
      if (!cname) {
	RINGER_DEBUG1("When operating in multi-db-mode, I expect you to"
		      << " give me DB file names and their classes at the"
		      << " new DB I'll merge. I'm missing class name for"
		      << " DB \"" << dbname << "\". Verify your inputs!");
	throw RINGER_EXCEPTION("Missing DB class name in multi-mode");
      }
      if (!sys::exists(dbname)) {
	RINGER_DEBUG1("Database file " << dbname << " doesn't exist.");
	throw RINGER_EXCEPTION("Database file doesn't exist");
      }
      p.classmap[dbname] = cname;
    }
  }

  //checks
  if (!p.classmap.size()) {
    RINGER_DEBUG1("I cannot proceed with an empty classmap. You should "
		  << "give me a number of arguments. Reconsider you options.");
    poptPrintUsage(optCon, stderr, 0);
    throw RINGER_EXCEPTION("No databases to start from.");
  }

  if (!output) {
    RINGER_DEBUG1("I cannot proceed without an output filename.");
    poptPrintUsage(optCon, stderr, 0);
    throw RINGER_EXCEPTION("No output filename.");
  }

  p.output = output;

  RINGER_DEBUG1("Command line options have been read.");
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();
  typedef std::map<std::string, data::RoIPatternSet*> map_type;
  typedef std::map<std::string, std::string> cmap_type;
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
    std::ostringstream comment;
    comment << "This file was produced with " << argv[0] << ". " ;
    map_type db_data;

    //for each of these entries
    for (cmap_type::const_iterator 
	   it = par.classmap.begin(); it != par.classmap.end(); ++it) {

      //load new database
      data::DatabaseXml<data::RoIPatternSet> start_db(it->first, reporter);
      RINGER_REPORT(reporter, "Putting data from file \"" << it->first 
		    << "\" into class \"" << it->second << "\"...");
      std::vector<std::string> cnames;
      start_db.class_names(cnames);
      if (cnames.size() == 0) {
	RINGER_WARN(reporter, "The file \"" << it->first << "\" seems"
		    " empty. I'll ignore it."); 
	continue;
      }
      if (cnames.size() > 1) {
	RINGER_WARN(reporter, "I can't work with multi-class input db's."
		    << " I'll ignore all contents of \"" << it->first
		    << "\", except class \"" << cnames[0] << "\".");
      }
      comment << "It contains data from " << it->first << " with class name "
	      << it->second << ". ";

      //check, if in the new database, I'll already have a place to stuff the
      //data, otherwise, create a new one
      if (db_data.find(it->second) != db_data.end()) {
	db_data.find(it->second)->second->merge(*start_db.data(cnames[0]));
      }
      else {
	db_data[it->second] = 
	  new data::RoIPatternSet(*start_db.data(cnames[0]));
      }
      RINGER_REPORT(reporter, "File \"" << it->first << "\" processed.");
    }

    //dump on a new file given by argv[1]
    data::Header h("Andre DOS ANJOS", "Merged database", "1.0",
		   time(0), comment.str());
    data::DatabaseXml<data::RoIPatternSet> dump(&h, db_data, reporter);
    RINGER_REPORT(reporter, "Dumping merged database into \"" << par.output
		  << "\".");
    dump.save(par.output);
  }

  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I cannot cope with the exception at"
		 << " the top-level. Aborting...");
  }

  RINGER_REPORT(reporter, "Finished successfully. Bye.");
  delete reporter;
}
