//Dear emacs, this is -*- c++ -*-

/**
 * @file mlp-relevance.cxx
 *
 * Loads an MLP built with another instance of neuralringer and calculates the
 * relevance for each variable, dividing the data in train and test.
 */

#include "TrigRingerTools/data/SimplePatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/data/RemoveDBMeanOperator.h"
#include "TrigRingerTools/data/util.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/network/Network.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <popt.h>
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
  std::string traindb; ///< database to use for training
  std::string testdb; ///< database to use for testing
  std::string out; ///< where to save the test set relevance
  data::Feature trainperc; ///< default amount of data to use for tranining
  std::string net; ///< the network file
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
  char* traindb=0;
  char* testdb=0;
  char* out=0;
  char* net=0;

  //return `arg' is set to !=0, so the system processes everything in the
  //while loop bellow.
  struct poptOption optionsTable[] = {
    { "traindb", 'd', POPT_ARG_STRING, &traindb, 'd',
      "location of the database to use for training", "path" },
    { "testdb", 'e', POPT_ARG_STRING, &testdb, 'e',
      "location of the database to use for testing", "path" },
    { "network", 'n', POPT_ARG_STRING, &net, 'n',
      "The network description to use", "path" },
    { "out", 'o', POPT_ARG_STRING, &out, 'o',
      "Where to save the relevance output", 
      "path: default is net-name.relevance.txt" },
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
    case 'd': //traindb
      RINGER_DEBUG1("Train database name is " << traindb);
      if (!sys::exists(traindb)) {
	RINGER_DEBUG1("Train database file " << traindb << " doesn't exist.");
	throw RINGER_EXCEPTION("Train database file doesn't exist");
      }
      break;
    case 'e': //testdb
      RINGER_DEBUG1("Test database name is " << testdb);
      if (!sys::exists(testdb)) {
	RINGER_DEBUG1("Database file " << testdb << " doesn't exist.");
	throw RINGER_EXCEPTION("Database file doesn't exist");
      }
      break;
    case 'n': //network
      RINGER_DEBUG1("Network name is " << net);
      if (!sys::exists(net)) {
	RINGER_DEBUG1("Network file " << net << " doesn't exist.");
	throw RINGER_EXCEPTION("Network file doesn't exist");
      }
      break;
    case 'o': //where to save the relevance output
      RINGER_DEBUG1("Saving relevance calculations to \"" 
		    << out << "\".");
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
  if (!traindb) {
    RINGER_DEBUG1("I cannot work without a train database file. Exception thrown.");
    throw RINGER_EXCEPTION("No train database file specified");
  } else p.traindb = traindb;
  if (!testdb) {
    RINGER_DEBUG1("I cannot work without a test database file. Exception thrown.");
    throw RINGER_EXCEPTION("No test database file specified");
  } else p.testdb = testdb;
  if (!net) {
    RINGER_DEBUG1("I cannot work without a network file to start from.");
    throw RINGER_EXCEPTION("No network file specified");
  } else p.net = net;
  if (!out) {
    p.out = stripname(p.net) + ".relevance.txt";
    RINGER_DEBUG1("Setting output name to " << p.out);
  } else p.out = out;
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
      RINGER_FATAL(reporter, "Terminating execution.");
  }
  catch (sys::Exception& ex) {
    RINGER_EXCEPT(reporter, ex.what());
    RINGER_FATAL(reporter, "I can't handle that exception. Aborting.");
  }

  //loads the DB
  data::DatabaseXml<data::SimplePatternSet> traindb(par.traindb, reporter);
  data::DatabaseXml<data::SimplePatternSet> testdb(par.testdb, reporter);

  //loads the network
  network::Network net(par.net, reporter);
  bool compressed_output = false;
  if (net.output_size() < traindb.size()) compressed_output = true;
  
  //split the data base in train and test
  std::vector<std::string> cnames;
  traindb.class_names(cnames);

  RINGER_DEBUG1("Train DB size is " << traindb.size());
  RINGER_DEBUG1("Test DB size is " << testdb.size());

  data::SimplePatternSet train(1, 1);
  traindb.merge(train);
  RINGER_DEBUG1("Train set size is " << train.size());
  data::SimplePatternSet train_target(1, 1);
  traindb.merge_target(compressed_output, -1, +1, train_target);

  data::SimplePatternSet test(1, 1);
  testdb.merge(test);
  RINGER_DEBUG1("Test set size is " << test.size());
  data::SimplePatternSet test_target(1, 1);

  testdb.merge_target(compressed_output, -1, +1, test_target);

  try {
    sys::Plain out(par.out, std::ios_base::trunc|std::ios_base::out);
    data::SimplePatternSet test_output(test_target);
    data::SimplePatternSet train_output(train_target);
    out << "input test.relevance.mse train.relevance.mse "
        << "test.relevance.sp train.relevance.sp\n";
    //test set analysis
    net.run(test, test_output);
    net.run(train, train_output);
    data::SimplePatternSet test_copy(test);
    data::SimplePatternSet train_copy(train);
    data::SimplePatternSet test_copy_output(test_target);
    data::SimplePatternSet train_copy_output(train_target);
    data::Ensemble test_mean(test.size());
    data::Ensemble train_mean(train.size());
    double eff1 = 0.0;
    double eff2 = 0.0;
    double thres = 0.0;
    double test_sp = data::sp(test_output, test_target, eff1, eff2, thres);
    double train_sp = data::sp(train_output, train_target, eff1, eff2, thres);
    for (size_t i = 0; i < test.pattern_size(); ++i) {
      RINGER_REPORT(reporter, "Evaluating relevance for feature `" 
		    << i << "'.");
      //for each input variable
      //reset inputs
      test_copy = test; 
      train_copy = train;
      //set desired ensemble to zero: remember, I removed the mean!
      test_copy.set_ensemble(i, test_mean);
      train_copy.set_ensemble(i, train_mean);
      //run changed copies through the network
      net.run(test_copy, test_copy_output);
      net.run(train_copy, train_copy_output);
      out << (unsigned int)i << " " 
          << data::mse(test_output, test_copy_output) << " "
          << data::mse(train_output, train_copy_output) << " "
          << (test_sp - 
              data::sp(test_copy_output, test_target, eff1, eff2, thres))
          << " "
          << (train_sp - 
              data::sp(train_copy_output, train_target, eff1, eff2, thres))
          << "\n";
    }
    RINGER_REPORT(reporter, "Cleaning up and exiting...");
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



