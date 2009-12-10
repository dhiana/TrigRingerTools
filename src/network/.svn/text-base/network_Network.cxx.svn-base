//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/Network.cxx
 *
 * Implements the common network class
 */

#include "TrigRingerTools/network/Network.h"
#include "TrigRingerTools/network/InputNeuron.h"
#include "TrigRingerTools/network/BiasNeuron.h"
#include "TrigRingerTools/network/OutputNeuron.h"
#include "TrigRingerTools/network/HiddenNeuron.h"
#include "TrigRingerTools/network/Synapse.h"

#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/util.h"

#include "TrigRingerTools/data/RandomInteger.h"

#include <fstream>

/**
 * A static random integer generator
 */
static data::RandomInteger static_rnd;

network::Network::Network (const std::string& config, 
			   sys::Reporter* reporter)
  : m_config(0),
    m_reporter(reporter),
    m_neuron(),
    m_synapse()
{
  RINGER_REPORT(m_reporter, "Loading network configuration...");
  m_config = new config::Network(config, reporter);
  /**
   * BUILD NEURONS FIRST
   */
  for (std::vector<config::Neuron*>::const_iterator it =
	 m_config->neurons().begin(); it != m_config->neurons().end(); ++it) {
    switch ((*it)->type()) {
    case config::INPUT:
      m_input.push_back(new InputNeuron(**it));
      m_neuron[(*it)->id()] = m_input[m_input.size()-1];
      RINGER_DEBUG1("Created input neuron " << (*it)->id());
      break;
    case config::INPUTNONORM:
      m_inputnonorm.push_back(new InputNeuronNoNorm(**it));
      m_neuron[(*it)->id()] = m_inputnonorm[m_inputnonorm.size()-1];
      RINGER_DEBUG1("Created non-normalized input neuron " << (*it)->id());
      break;
    case config::BIAS:
      m_bias.push_back(new BiasNeuron(**it));
      m_neuron[(*it)->id()] = m_bias[m_bias.size()-1];
      RINGER_DEBUG1("Created bias neuron " << (*it)->id());
      break;
    case config::HIDDEN:
      m_neuron[(*it)->id()] = new HiddenNeuron(**it);
      RINGER_DEBUG1("Created hidden neuron " << (*it)->id());
      break;
    case config::OUTPUT:
      m_output.push_back(new OutputNeuron(**it));
      m_neuron[(*it)->id()] = m_output[m_output.size()-1];
      RINGER_DEBUG1("Created output neuron " << (*it)->id());
      break;
    default:
      throw RINGER_EXCEPTION("Unknown neuron type");
    }
  }
  RINGER_REPORT(m_reporter, m_config->neurons().size() << " neurons created.");

  /**
   * NOW BUILD THE SYNAPSES AND CONNECT NEURONS TOGETHER
   */
  for (std::vector<config::Synapse*>::const_iterator it = m_config->synapses().begin();it != m_config->synapses().end();++it) {
    m_synapse[(*it)->id()] = new Synapse(**it);
    //connect
    std::map<unsigned int, Neuron*>::iterator from = m_neuron.find((*it)->from());
    if (from == m_neuron.end()) {
      RINGER_DEBUG1("Synapse " << (*it)->id() 
		  << " tries to start from unexisting neuron " 
		  << (*it)->from() << ". Check your configuration. "
		  << "Exception thrown.");
      throw RINGER_EXCEPTION("Unconfigured \"from\" neuron in synapse.");
    }
    std::map<unsigned int, Neuron*>::iterator to = m_neuron.find((*it)->to());
    if (to == m_neuron.end()) {
      RINGER_DEBUG1("Synapse " << (*it)->id() 
		  << " tries to end at unexisting neuron " 
		  << (*it)->to() << ". Check your configuration. "
		  << "Exception thrown.");
      throw RINGER_EXCEPTION("Unconfigured \"to\" neuron in synapse.");
    }
    m_synapse[(*it)->id()]->connect(m_neuron[(*it)->from()], m_neuron[(*it)->to()]);
    RINGER_DEBUG1("Created synapse " << (*it)->id() << " connecting neuron "  << (*it)->from() << " to neuron " << (*it)->to());
  }
  RINGER_REPORT(m_reporter, m_config->synapses().size() << " synapses created.");
  time_t ls = m_config->header()->lastSaved();
  RINGER_REPORT(m_reporter, "Incarnated neural network \"" 
	      << m_config->header()->name() << "\" version \"" 
	      << m_config->header()->version() << "\" last saved at \""
	      << sys::stringtime(&ls) << "\" ");
}

network::Network::Network (const std::vector<network::Neuron*>& neurons,
			   const std::vector<network::Synapse*>& synapses,
			   sys::Reporter* reporter)
  : m_config(0),
    m_reporter(reporter),
    m_neuron(),
    m_synapse()
{
  adopt(neurons, synapses);
}

network::Network::Network (sys::Reporter* reporter)
  : m_config(0),
    m_reporter(reporter),
    m_neuron(),
    m_synapse()
{
}

network::Network::~Network ()
{
  delete m_config;
  for(std::map<unsigned int, Synapse*>::iterator it = m_synapse.begin(); it != m_synapse.end(); ++it) {
    delete it->second;
  }
  for(std::map<unsigned int, Neuron*>::iterator it = m_neuron.begin(); it != m_neuron.end(); ++it) {
    delete it->second;
  }
}

bool network::Network::save (const std::string& file,
			     const config::Header* header) const
{
  RINGER_DEBUG3("Saving network state at file \"" << file << "\".");
  std::vector<config::Neuron*> neuron_config;
  std::vector<config::Synapse*> synapse_config;
  for (std::map<unsigned int, Neuron*>::const_iterator it =
	 m_neuron.begin(); it != m_neuron.end(); ++it) {
    neuron_config.push_back(new config::Neuron(it->second->dump()));
  }
  for (std::map<unsigned int, Synapse*>::const_iterator it =
	 m_synapse.begin(); it != m_synapse.end(); ++it) {
    synapse_config.push_back(new config::Synapse(it->second->dump()));
  }

  //check for the header business
  bool header_allocated = false;
  const config::Header* touse = 0;
  if (header) touse = header;
  else if (m_config) touse = m_config->header();
  else { //build a dummy header
    touse = new config::Header("UNSET AUTHOR", "UNSET NAME", "0.0", time(0),
			       "UNSET COMMENT");
    header_allocated = true;
  }
  config::Network new_config(touse, synapse_config, neuron_config, m_reporter);
  if (header_allocated) delete touse;

  //save and delete the acquired resources
  bool retval = new_config.save(file);
  for (std::vector<config::Neuron*>::iterator it =
	 neuron_config.begin(); it != neuron_config.end(); ++it) {
    delete *it;
  }
  for (std::vector<config::Synapse*>::iterator it =
	 synapse_config.begin(); it != synapse_config.end(); ++it) {
    delete *it;
  }
  if (!retval) {
    RINGER_WARN(m_reporter, "I could not save network state in \"" 
	      << file << "\". Exception thrown.");
    throw RINGER_EXCEPTION("couldn't save network state");
  }
  RINGER_DEBUG3("Network state saved.");
  return true;
}

bool network::Network::dot (const std::string& filename) const
{
  RINGER_DEBUG2("Trying to build a dot representation at \"" 
		<< filename << "\".");
  if (sys::exists(filename)) sys::backup(filename); 
  std::ofstream os(filename.c_str()); 
  if (!os) { 
    RINGER_DEBUG1("I cannot dump a dot-file at \"" << filename 
		<< "\". Exception thrown.");
    throw RINGER_EXCEPTION("Cannot open dot-file to write");
  }
  //start to dump
  os << "digraph network {" << std::endl 
     << " rankdir=LR; ranksep=\"1.0 equally\";concentrate=true;" << std::endl;
  //get the input neurons first
  for (std::vector<InputNeuron*>::const_iterator it = m_input.begin(); 
       it != m_input.end(); ++it) (*it)->dot(os);
  for (std::vector<InputNeuronNoNorm*>::const_iterator it = m_inputnonorm.begin(); 
       it != m_inputnonorm.end(); ++it) (*it)->dot(os);
  for (std::vector<BiasNeuron*>::const_iterator it = m_bias.begin(); 
       it != m_bias.end(); ++it) (*it)->dot(os);
  os << "}";
  os.close();
  RINGER_DEBUG2("Dot-file \"" << filename << "\" was saved.");
  return true;
}

void network::Network::run (const data::Pattern& input, data::Pattern& output)
{
  RINGER_DEBUG3("Running 1 pattern through \"" << m_config->header()->name() 
	      << "\" network.");
  unsigned int i=0;
  for (std::vector<InputNeuron*>::iterator it = m_input.begin();
       it != m_input.end(); ++it, ++i) {
    data::Ensemble data(1, input[i]);
    (*it)->run(data);
  }
  for (std::vector<InputNeuronNoNorm*>::iterator it = m_inputnonorm.begin();
       it != m_inputnonorm.end(); ++it, ++i) {
    data::Ensemble data(1, input[i]);
    (*it)->run(data);
  }
  data::Ensemble dummy(1, 1);
  for (std::vector<BiasNeuron*>::iterator it = m_bias.begin();
       it != m_bias.end(); ++it) (*it)->run(dummy);
  i = 0;
  if (output.size() != m_output.size()) {
    RINGER_DEBUG1("Resizing output... If you want to have faster processing"
		<< " please consider giving an output Pattern with the same"
		<< " number of positions as the number of output neurons in"
		<< " this network, i.e., " << m_output.size() << ". Currently"
		<< " it is " << output.size());
    output = data::Pattern(m_output.size(), 0);
  }
  for (std::vector<OutputNeuron*>::iterator it = m_output.begin();
       it != m_output.end(); ++it, ++i) {
    output[i] = ((*it)->state())[0];
  }
  RINGER_DEBUG3("Ran 1 pattern through network.");
}

void network::Network::run (const data::SimplePatternSet& input,
			    data::SimplePatternSet& output)
{
  RINGER_DEBUG3("Running " << input.size() << " pattern(s) through network.");
  unsigned int i=0;
  for (std::vector<InputNeuron*>::iterator it = m_input.begin();
       it != m_input.end(); ++it, ++i) {
    data::Ensemble data = input.ensemble(i);
    (*it)->run(data);
  }
  for (std::vector<InputNeuronNoNorm*>::iterator it = m_inputnonorm.begin();
       it != m_inputnonorm.end(); ++it, ++i) {
    data::Ensemble data = input.ensemble(i);
    (*it)->run(data);
  }
  data::Ensemble dummy(input.size(), 1);
  for (std::vector<BiasNeuron*>::iterator it = m_bias.begin();
       it != m_bias.end(); ++it) (*it)->run(dummy);
  i = 0;
  if (output.size() != input.size() || 
      output.pattern_size() != m_output.size()) {
    RINGER_DEBUG1("Resizing output... If you want to have faster processing"
		<< " please consider giving an output PatternSet with the same"
		<< " number of positions and ensembles as the number of"
		<< " output neurons and ensembles in the input set"
		<< ", i.e., size = " << input.size() << " and"
		<< " ensemble size = " << m_output.size()
		<< ". Currently the output size is " << output.size() 
		<< " and the pattern size is " << output.pattern_size() 
		<< ".");
    output = data::SimplePatternSet(input.size(), m_output.size(), 0);
  }
  for (std::vector<OutputNeuron*>::iterator it = m_output.begin();
       it != m_output.end(); ++it, ++i) {
    output.set_ensemble(i, (*it)->state());
  }
  RINGER_DEBUG3("Ran " << input.size() << " pattern(s) through network.");
}

void network::Network::train (const data::Pattern& data,
			      const data::Pattern& target)
{
  RINGER_DEBUG3("Training network with 1 Pattern");
  data::Pattern tmp(m_output.size(), 0);
  RINGER_DEBUG2("Input signal is " << data);
  run(data, tmp);
  RINGER_DEBUG2("Output signal is " << tmp);
  tmp -= target;
  tmp *= -1;
  RINGER_DEBUG2("Error signal is " << tmp);
  unsigned int i=0;
  for (std::vector<OutputNeuron*>::iterator it = m_output.begin();
       it != m_output.end(); ++it, ++i) {
    data::Ensemble train_data(1, tmp[i]);
    (*it)->train(train_data);
  }
  RINGER_DEBUG3("Network trained.");
}

void network::Network::train (const data::SimplePatternSet& data,
			      const data::SimplePatternSet& target)
{
  RINGER_DEBUG3("(BATCH) Training network with " 
		<< data.size() << " Patterns");
  data::SimplePatternSet output(data.size(), m_output.size()); //empty
  run(data, output);
  data::SimplePatternSet error(target);
  error -= output; // calculates the error for this iteration
  for (unsigned int j=0; j<m_output.size(); ++j)
    m_output[j]->train(error.ensemble(j));
  RINGER_DEBUG3("Network trained.");
}

void network::Network::train (const data::SimplePatternSet& data,
			      const data::SimplePatternSet& target,
			      unsigned int epoch)
{
  RINGER_DEBUG3("(BATCH-RANDOM) Training network with " 
		<< epoch << " Patterns");
  std::vector<size_t> pats(epoch);
  static_rnd.draw(data.size(), pats); //get random positions
  data::SimplePatternSet input(data, pats); //get patterns for this iteration
  data::SimplePatternSet output(epoch, m_output.size()); //empty
  run(input, output); //set network state
  data::SimplePatternSet error(target, pats);
  error -= output; // calculates the error for this iteration
  for (unsigned int j=0; j<m_output.size(); ++j)
    m_output[j]->train(error.ensemble(j));
  RINGER_DEBUG3("Network trained.");
}

void network::Network::adopt (const std::vector<network::Neuron*>& neurons,
			      const std::vector<network::Synapse*>& synapses)
{
  //for sanity
  delete m_config;
  m_config = 0;
  for(std::map<unsigned int, Synapse*>::iterator it = m_synapse.begin();
      it != m_synapse.end(); ++it) {
    delete it->second;
  }
  for(std::map<unsigned int, Neuron*>::iterator it = m_neuron.begin();
      it != m_neuron.end(); ++it) {
    delete it->second;
  }
  m_synapse.clear();
  m_neuron.clear();
  m_input.erase(m_input.begin(), m_input.end());
  m_inputnonorm.erase(m_inputnonorm.begin(), m_inputnonorm.end());
  m_bias.erase(m_bias.begin(), m_bias.end());
  m_output.erase(m_output.begin(), m_output.end());

  //assign
  for (std::vector<network::Neuron*>::const_iterator it = neurons.begin();
       it != neurons.end(); ++it) {
    std::map<unsigned int, network::Neuron*>::iterator bad =
      m_neuron.find((*it)->id());
    if (bad != m_neuron.end()) {
      RINGER_WARN(m_reporter, 
		"Found doubled neuron id on network. Exception thrown.");
      throw RINGER_EXCEPTION("Duplicated neuron id");
    }
    m_neuron[(*it)->id()] = (*it);

    //bad, but there seems to be no other "nice" way around this which doesn't
    //imply passing more and more arguments...
    network::BiasNeuron* bias = dynamic_cast<network::BiasNeuron*>(*it);
    if (bias) {
      m_bias.push_back(bias);
      continue;
    }
    network::InputNeuron* input = dynamic_cast<network::InputNeuron*>(*it);
    if (input) {
      m_input.push_back(input);
      continue;
    }
    network::InputNeuronNoNorm* inputnonorm = dynamic_cast<network::InputNeuronNoNorm*>(*it);
    if (inputnonorm) {
      m_inputnonorm.push_back(inputnonorm);
      continue;
    }
    network::OutputNeuron* output = dynamic_cast<network::OutputNeuron*>(*it);
    if (output) {
      m_output.push_back(output);
      continue;
    }
  }
  for (std::vector<network::Synapse*>::const_iterator it = synapses.begin();
       it != synapses.end(); ++it) {
    std::map<unsigned int, network::Synapse*>::iterator bad = 
      m_synapse.find((*it)->id());
    if (bad != m_synapse.end()) {
      RINGER_WARN(m_reporter, 
		"Found doubled synapse id on network. Exception thrown.");
      throw RINGER_EXCEPTION("Duplicated synapse id");
    }
    m_synapse[(*it)->id()] = (*it);
  }
}
