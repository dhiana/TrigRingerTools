//Dear emacs, this is -*- c++ -*-

/**
 * @file mlp-train.cxx
 *
 * Builds an unbiased MLP from scratch, based on a database (number of
 * features and classes to separate) and train the neural network to recognize
 * the patterns in that database.
 */

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/DatabaseXml.h"
#include "TrigRingerTools/data/NormalizationOperator.h"
#include "TrigRingerTools/data/util.h"
#include "TrigRingerTools/data/SumExtractor.h"
#include "TrigRingerTools/network/MLP.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/config/SynapseRProp.h"
#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/config/Header.h"
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include "TrigRingerTools/sys/Plain.h"

typedef struct param_t {
  std::string traindb; ///< database to use for training
  std::string testdb; ///< database to use for testing
  std::string startnet; ///< name of the starting neural net file
  std::string endnet; ///< name of the ending neural net file
  std::string bestnet; ///< name of the best neural net file
  std::string mseevo; ///< where to save the neural network MSE evolution
  std::string spevo; ///< where to save the neural network SP evolution
  std::string output; ///< where to save the last output after training
  std::string energy; ///< where to save the energies of the clusters
  long int nhidden; ///< number of hidden neurons
  long int epoch; ///< each epoch size
  bool msestop; ///< use MSE product stop criteria instead of SP stabilisation
  bool compress; ///< if I should use compressed or extended output 
  long int stopiter; ///< number of iterations w/o variance to stop
  data::Feature stopthres; ///< the threshold to consider for stopping
  long int sample; ///< the sample interval for MSE or SP
  long int hardstop; ///< where to hard stop the training
} param_t;

/**
 * Checks and validates program options.
 *
 * @param p The parameters, already parsed
 * @param reporter The reporter to use when reporting problems to the user
 */
bool checkopt (param_t& par, sys::Reporter* reporter)
{
  if (!par.traindb.size()) {
    RINGER_DEBUG1("No train DB file given! Throwing...");
    throw RINGER_EXCEPTION("Train Database file not given.");
  }
  if (!par.testdb.size()) {
    RINGER_DEBUG1("No test DB file given! Throwing...");
    throw RINGER_EXCEPTION("Test Database file not given.");
  }
  if (!sys::exists(par.traindb)) {
    RINGER_DEBUG1("Train Database file " << par.traindb << " doesn't exist.");
    throw RINGER_EXCEPTION("Train Database file doesn't exist");
  }
  if (!sys::exists(par.testdb)) {
    RINGER_DEBUG1("Test Database file " << par.testdb << " doesn't exist.");
    throw RINGER_EXCEPTION("Test Database file doesn't exist");
  }
  if (par.sample <= 0) {
    RINGER_DEBUG1("Trying to set the sampling interval to " << par.sample);
    throw RINGER_EXCEPTION("The sampling interval should be > 0");
  }
  if (par.stopthres <= 0) {
    RINGER_DEBUG1("I cannot use a stop threshold less than zero");
    throw RINGER_EXCEPTION("Stop threshold is less than zero");
  }
  if (!par.startnet.size()) {
    par.startnet = sys::stripname(par.traindb) + ".start.xml";
    RINGER_DEBUG1("Setting start file name to " << par.startnet);
  }
  if (!par.endnet.size()) {
    par.endnet = sys::stripname(par.traindb) + ".end.xml";
    RINGER_DEBUG1("Setting end file name to " << par.endnet);
  }
  if (!par.bestnet.size()) {
    par.bestnet = sys::stripname(par.traindb) + ".best.xml";
    RINGER_DEBUG1("Setting best net file name to " << par.bestnet);
  }
  if (!par.energy.size()) {
    par.energy = sys::stripname(par.traindb) + ".energy.xml";
    RINGER_DEBUG1("Setting transverse energy file name to " << par.energy);
  }
  if (!par.mseevo.size()) {
    par.mseevo = sys::stripname(par.traindb) + ".mse.txt";
    RINGER_DEBUG1("Setting MSE evolution file name to " << par.mseevo);
  }
  if (!par.spevo.size()) {
    par.spevo = sys::stripname(par.traindb) + ".sp.txt";
    RINGER_DEBUG1("Setting SP evolution file name to " << par.spevo);
  }
  if (!par.output.size()) {
    par.output = sys::stripname(par.traindb) + ".out.xml";
    RINGER_DEBUG1("Setting output file name to " << par.output);
  }
  if (!par.nhidden) {
    RINGER_DEBUG1("I cannot work with 0 hidden neurons. Exception thrown.");
    throw RINGER_EXCEPTION("No number of hidden neurons specified");
  }
  if (par.stopiter == 0) {
    RINGER_DEBUG1("I cannot wait 0 cycles to stop, please use a number"
		  << " between 1 and infinity.");
    throw RINGER_EXCEPTION("Cycles to stop cannot be zero");
  }
  if (!par.hardstop) {
    RINGER_DEBUG1("I cannot work with an unbound network training."
		  << " Please provide me a hardstop.");
    throw RINGER_EXCEPTION("No hardstop parameter specified.");
  }
  RINGER_DEBUG1("Command line options have been validated.");
  return true;
}

int main (int argc, char** argv)
{
  sys::Reporter *reporter = new sys::LocalReporter();

  param_t par = { "", "", "", "", "", "", "", "", "",
                  4, 50, false, true, 50, 0.001, 10, 10000 };
  sys::OptParser opt_parser(argv[0]);
  opt_parser.add_option
    ("hard-stop", 'b', par.hardstop,
     "number of epochs after which to hard stop the training session");
  opt_parser.add_option
    ("epoch", 'c', par.epoch,
     "how many entries per training step should I use");
  opt_parser.add_option
    ("traindb", 'd', par.traindb,
     "location of the database to use for training");
  opt_parser.add_option
    ("end-net", 'e', par.endnet, 
     "where to write the last network");
  opt_parser.add_option
    ("best-net", 'g', par.bestnet, 
     "where to write the best network");
  opt_parser.add_option
    ("energy", 'j', par.energy,
     "the name of the output file for transverse energies of input clusters");
  opt_parser.add_option
    ("stop-iteration", 'i', par.stopiter,
     "how many times to wait for non-variation to be considered a stop sign");
  opt_parser.add_option
    ("mse-evolution", 'm', par.mseevo,
     "where to write the MSE evolution data, during training");
  opt_parser.add_option
    ("sample-interval", 'n', par.sample,
     "when to sample the training process for the MSE evolution or SP");
  opt_parser.add_option
    ("output", 'o', par.output,
     "where to write the output of the last network");
  opt_parser.add_option
    ("sp-evolution", 'p', par.spevo,
     "where to write the SP evolution data, during training");
  opt_parser.add_option
    ("hidden", 'r', par.nhidden,
     "how many hidden neurons should I use for the network");
  opt_parser.add_option
    ("start-net", 's', par.startnet,
     "where to write the start status of the network");
  opt_parser.add_option
    ("mse-stop", 't', par.msestop,
     "if I should use MSE stop criteria instead of SP (default)");
  opt_parser.add_option
    ("testdb", 'u', par.testdb,
     "location of the database to use for testing");
  opt_parser.add_option
    ("stop-threshold", 'w', par.stopthres,
     "the stop threshold to consider for flagging a potential stop");
  opt_parser.add_option
    ("compress-output", 'z', par.compress,
     "should compress the output, e.g. 2 classes -> 1 output for the network");
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
  data::DatabaseXml<data::RoIPatternSet> traindb(par.traindb, reporter);
  data::DatabaseXml<data::RoIPatternSet> testdb(par.testdb, reporter);
  std::vector<std::string> cnames;
  traindb.class_names(cnames);
  RINGER_DEBUG1("Test set size is " << testdb.size());
  
  //tune input DB's for size/randomness
  data::Database<data::RoIPatternSet> 
    traindb2(traindb); //save original train database
  traindb.normalise();
  //calculate the normalization factor based on the train set, but only after
  //size normalization
  data::NormalizationOperator norm_op(traindb);
  RINGER_DEBUG1("Train set size is " << traindb2.size());

  //checks db size
  if (traindb.size() < 2) {
    RINGER_FATAL(reporter, "The database you loaded contains only 1 class of"
		 " events. Please, reconsider your input file.");
  }

  //checks one more thing
  if (traindb.size() > 2 && !par.msestop) {
    RINGER_FATAL(reporter, "I cannot use the SP product with in a multi"
		 "-class scenario. Please, either re-program me or reconsider"
		 " your options.");
  }

  //Builds the MLP network, 1 input for each feature, a compressed output
  //schema for each class (2 classes -> 1 output)
  std::vector<size_t> hlayer(1, par.nhidden);
  config::NeuronStrategyType nstrat = config::NEURON_BACKPROP;
  config::NeuronBackProp::ActivationFunction actfun = 
    config::NeuronBackProp::TANH;
  config::Parameter* nsparam = new config::NeuronBackProp(actfun);
  config::SynapseStrategyType sstrat = config::SYNAPSE_RPROP;
  config::Parameter* ssparam = new config::SynapseRProp(0.1);
  std::vector<bool> biaslayer(2, true);
  unsigned int nout = traindb.size();
  if (par.compress) nout = lrint(std::ceil(log2(traindb.size())));
  network::MLP net(traindb.pattern_size(), hlayer, nout,
		   biaslayer, nstrat, nsparam, nstrat, nsparam,
		   sstrat, ssparam, norm_op.mean(), norm_op.stddev(), 
		   reporter);

  data::RoIPatternSet train(1, 1);
  traindb.merge(train);
  RINGER_REPORT(reporter, "Normalised train set size is " << train.size());
  data::RoIPatternSet train2(1, 1);
  traindb2.merge(train2);
  RINGER_REPORT(reporter, "Original train set size is " << train2.size());
  RINGER_DEBUG1("Train set size is " << train.size());
  data::RoIPatternSet target(1, 1);
  traindb.merge_target(par.compress, -1, +1, target);
  RINGER_REPORT(reporter, "Normalized train target set size is " 
		<< target.size());
  data::RoIPatternSet target2(1, 1);
  traindb2.merge_target(par.compress, -1, +1, target2);
  RINGER_REPORT(reporter, "Original train target set size is "
		<< target2.size());
  RINGER_DEBUG1("Train target set size is " << target.size());
  data::RoIPatternSet test(1, 1);
  testdb.merge(test);
  RINGER_DEBUG1("Test set size is " << test.size());
  data::RoIPatternSet test_target(1, 1);
  testdb.merge_target(par.compress, -1, +1, test_target);
  RINGER_DEBUG1("Test target set size is " << test_target.size());
  
  try {
    sys::Plain mseevo(par.mseevo, std::ios_base::trunc|std::ios_base::out);
    mseevo << "epoch test-mse train-mse" << "\n";
    sys::Plain spevo(par.spevo, std::ios_base::trunc|std::ios_base::out);
    spevo << "epoch test-sp train-sp" << "\n";
    data::SimplePatternSet output(target2.simple());
    config::Header net_header("Andre DOS ANJOS", par.output, "1.0", time(0),
			      "Start set");
    net.save(par.startnet, &net_header);
    unsigned int stopnow = par.stopiter;

    //Trains until the MSE or SP product stabilizes
    double val = 1;
    double var = 1;

    double prev = 0; //previous
    size_t i = 0;
    double best_val = val;
    while (stopnow) {
      net.train(train.simple(), target.simple(), par.epoch);
      --par.hardstop;
      if (!par.hardstop) {
	RINGER_REPORT(reporter, "Hard-stop limit has been reached. Stopping"
		      << " the training session by force.");
	stopnow = 0;
	continue;
      }

      if (i%par.sample == 0) { //time to check performance
	double eff1, eff2, thres;
	double eff1_test, eff2_test, thres_test;
	{
	  //test set analysis
	  output = test_target.simple();
	  net.run(test.simple(), output);
	  double sp_val;
	  if (traindb.size() == 2)
	    sp_val = data::sp(output, test_target, eff1_test, eff2_test, thres_test);
	  double mse_val = data::mse(output, test_target.simple());
	  mseevo << (unsigned int)i << " " << mse_val;
	  spevo << (unsigned int)i << " " << sp_val;
	  prev = val;
	  if (!par.msestop) val = sp_val;
	  else val = mse_val;
	  var = std::fabs(val-prev)/prev;
	  bool savenet = false;
	  if (!par.msestop) { //the greater, the better
	    if (val > best_val) {
	      best_val = val;
	      savenet = true;
	    }
	  }
	  else {
	    if (var < best_val) {
	      best_val = val;
	      savenet = true;
	    }
	  }
	  if (savenet) { //save the current network because it is the best
	    //save result
	    RINGER_REPORT(reporter, "Saving best network so far at \""
			  << par.bestnet << "\"...");
	    config::Header end_header("Andre DOS ANJOS", par.output, 
				      "1.0", time(0), "Trained network");
	    net.save(par.bestnet, &end_header);
	  }
	}
	{
	  //train set analysis
	  net.run(train2.simple(), output);
	  double sp_val;
	  if (traindb.size() == 2) 
	    sp_val = data::sp(output, target2, eff1, eff2, thres);
	  double mse_val = data::mse(output, target2.simple());
	  mseevo << " " << mse_val << "\n";
	  spevo << " " << sp_val << "\n";
	}
	if (var < par.stopthres) {
	  --stopnow;
	  RINGER_REPORT(reporter, "Detected possible stop in " << stopnow
			<< " more iterations.");
	}
	else stopnow = par.stopiter;
	if (!par.msestop) {
	  RINGER_REPORT(reporter, "[epoch " << i << "/test] SP = " << val
			<< " (variation = " << var << ") (for threshold=" 
			<< thres_test << " -> " << cnames[0]
			<< " eff=" << eff1_test*100 << "% and " << cnames[1]
			<< " eff=" << eff2_test*100 << "%)");
	}
	else RINGER_REPORT(reporter, "[epoch " << i << "] MSE = " << val
			   << " (variation = " << var << ")");
      }

      ++i; //go to next epoch
    }

    //save result
    RINGER_REPORT(reporter, "Saving last network \""
                  << par.endnet << "\"...");
    config::Header end_header("Andre DOS ANJOS", par.output, 
                              "1.0", time(0), "Trained network");
    net.save(par.endnet, &end_header);

    RINGER_REPORT(reporter,
		  "Saving training and testing outputs and targets.");
    
    //reload the best network saved so far.
    network::Network bestnet(par.endnet, reporter);
    data::SimplePatternSet simple_train_output(target2.simple());
    bestnet.run(train2.simple(), simple_train_output);
    data::RoIPatternSet train_output(simple_train_output, train2.attributes());
    double mse_train = data::mse(train_output, target2);
    double sp_train = 0;
    double train_eff1 = 0;
    double train_eff2 = 0;
    double train_thres = 0;
    if (traindb.size() == 2) 
      sp_train = data::sp(train_output, target2, train_eff1, train_eff2, 
			  train_thres);
    data::SimplePatternSet simple_test_output(target.simple());
    bestnet.run(test.simple(), simple_test_output);
    data::RoIPatternSet test_output(simple_test_output, test.attributes());
    double mse_test = data::mse(test_output, test_target);
    double sp_test = 0;
    double test_eff1 = 0;
    double test_eff2 = 0;
    double test_thres = 0;
    if (traindb.size() == 2) sp_test = data::sp(test_output, test_target,
                                                test_eff1, test_eff2, test_thres);

    //energy estimations
    data::RoIPatternSet train_energy(train_output.size(), 1);
    data::Ensemble train_energies(train_output.size(), 0);
    data::SumExtractor add;
    for (size_t i = 0; i < train_output.size(); ++i) {
      train_energies[i] = add(train2.pattern(i));
    }
    train_energy.set_ensemble(0, train_energies);
    train_energy.set_attribute(train2.attributes());

    data::RoIPatternSet test_energy(test_output.size(), 1);
    data::Ensemble test_energies(test_output.size(), 0);
    for (size_t i = 0; i < test_output.size(); ++i) {
      test_energies[i] = add(test.pattern(i));
    }
    test_energy.set_ensemble(0, test_energies);
    test_energy.set_attribute(test.attributes());

    std::map<std::string, data::RoIPatternSet*> data;
    data["train-output"] = &train_output;
    data["train-target"] = &target2;
    data["train-energy"] = &train_energy;
    data["test-output"] = &test_output;
    data["test-target"] = &test_target;
    data["test-energy"] = &test_energy;
    std::ostringstream comment;
    comment << "Outputs from training session based on " << par.traindb << ". "
	    << "Train set MSE = " << mse_train;
    if (traindb.size() == 2) comment << " and SP = " << sp_train;
    comment << ". Test set MSE = " << mse_test;
    if (testdb.size() == 2) comment << " and SP = " << sp_test;
    comment << ".";
    data::Header header("Andre DOS ANJOS", par.output, "1.0", time(0),
			comment.str());
    data::DatabaseXml<data::RoIPatternSet> output_db(&header, data, reporter);
    output_db.save(par.output);
    RINGER_REPORT(reporter, "Network output saved to \"" << par.output
		  << "\".");


    //print summary:
    if (traindb.size() == 2) {
      RINGER_REPORT(reporter, "Summary for " << cnames[0] << "/" << cnames[1]
		    << " discrimination with an MLP network:");
    }
    RINGER_REPORT(reporter, comment.str());
    if (traindb.size() == 2) { //print SP statistics
      RINGER_REPORT(reporter, "Train set gives, for threshold="
		    << train_thres << " -> " << cnames[0]
		    << " eff=" << train_eff1*100 << "% and " << cnames[1]
		    << " eff=" << train_eff2*100 << "%.");
      RINGER_REPORT(reporter, "Test set gives, for threshold="
		    << test_thres << " -> " << cnames[0]
		    << " eff=" << test_eff1*100 << "% and " << cnames[1]
		    << " eff=" << test_eff2*100 << "%.");
    }
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



