//Dear emacs, this is -*- c++ -*-

/**
 * @file xml2text.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-22 16:53:56 $
 *
 * Converts an XML database into a text file, where every entry is on a single
 * line. No headers or such.
 */

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include <popt.h>
#include <cstdio>
#include "TrigRingerTools/sys/Plain.h"

/**
 * Returns the basename of a file, without its extension and diretory prefixes
 * 
 * @param fullname The name of the file
 */
std::string stripname (const std::string& fullname)
{
  //find location of last '/'
  size_t start = fullname.rfind("/");
  if (start == std::string::npos) start = 0;
  else ++start;
  //stopped here
  size_t end = fullname.rfind(".xml");
  return fullname.substr(start, end-start);
}

typedef struct param_t {
  bool separate; ///< puts each class in a different file
  std::string input; ///< the name of the input XML file
  std::string outprefix; ///< the prefix to be used for the output file(s)
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
  p.separate = false;
  char* input = 0;
  char* outprefix = 0;
  struct poptOption optionsTable[] = {
    { "input", 'i', POPT_ARG_STRING, &input, 'i',
      "The name of the input XML file", "path" },
    { "output", 'o', POPT_ARG_STRING, &outprefix, 'o',
      "The prefix to be used for the output file", 
      "string: default is \"input-file.txt\"" },
    { "separate", 's', POPT_ARG_NONE, 0, 's',
      "Separates each data class in a different file", "default is false" },
    POPT_AUTOHELP
    { 0, 0, 0, 0, 0 }
  };

  poptContext optCon = poptGetContext(NULL, argc, (const char**)argv,
				      optionsTable, 0);

  if (argc < 2) {
    poptPrintUsage(optCon, stderr, 0);
    return false;
  }

  char c;
  while ((c = poptGetNextOpt(optCon)) > 0) {
    switch (c) {
    case 'i': //name of the input file
      RINGER_DEBUG1("The input will be take from file \"" << input << "\".");
      if (!sys::exists(input)) {
	RINGER_DEBUG1("Input file " << input << " doesn't exist.");
	throw RINGER_EXCEPTION("Input file doesn't exist");
      }
      break;
    case 'o': //the output file prefix
      RINGER_DEBUG1("The output prefix was set to \"" << outprefix << "\".");
      break;
    case 's': //separate output
      RINGER_DEBUG1("The output file will be separated in classes.");
      p.separate = true;
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
  if (!input) {
    RINGER_DEBUG1("I cannot work without an input file. Exception thrown.");
    throw RINGER_EXCEPTION("No input file specified");
  } else p.input = input;
  if (!outprefix) {
    p.outprefix = stripname(p.input);
    RINGER_DEBUG1("Setting output prefix to " << p.outprefix);
  } else p.outprefix = outprefix;

  poptFreeContext(optCon);

  RINGER_DEBUG1("Command line options have been read.");
  return true;
}

int main (int argc, char** argv)
{
  typedef std::map<std::string, data::RoIPatternSet*> map_type;
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

    data::DatabaseXml<data::RoIPatternSet> db(par.input, reporter);
    if (par.separate) {
      const map_type& data = db.data();
      for (map_type::const_iterator it=data.begin(); it!=data.end(); ++it) {
	std::string output = par.outprefix + "." + it->first + ".txt";
	sys::Plain out(output, std::ios_base::out|std::ios_base::trunc, ' ');
	out << *(it->second);
	out << "\n";
      }
    }
    else {
      std::string output = par.outprefix + ".txt";
      sys::Plain out(output, std::ios_base::out|std::ios_base::trunc, ' ');
      const map_type& data = db.data();
      for (map_type::const_iterator it=data.begin(); it!=data.end(); ++it) {
	out << *(it->second);
	out << "\n";
      }
    }
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I cannot cope with the exception at"
		 << " the top-level. Aborting...");
  }

  RINGER_REPORT(reporter, "Finished successfully. Bye.");
  delete reporter;
}
