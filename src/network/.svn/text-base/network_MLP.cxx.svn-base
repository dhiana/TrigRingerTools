//Dear emacs, this is -*- c++ -*-

/**
 * @file MLP.cxx
 *
 * Implements the multi-layer perceptron.
 */

#include "TrigRingerTools/network/MLP.h"
#include "TrigRingerTools/network/InputNeuron.h"
#include "TrigRingerTools/network/BiasNeuron.h"
#include "TrigRingerTools/network/OutputNeuron.h"
#include "TrigRingerTools/network/HiddenNeuron.h"
#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/data/RandomInteger.h"

static data::RandomInteger rnd; //default initializer, do it once

/**
 * Creates a new synapse based on strategy types and parameters. The returned
 * value should be deleted by yourself.
 *
 * @param type The global synapse strategy type for this network
 * @param params The global synapse strategy parameters to apply for
 * this network
 */
network::Synapse* create_synapse (const config::SynapseStrategyType& type,
				  const config::Parameter* params, const unsigned *id)
{
  data::Feature weight = rnd.draw(RAND_MAX);
  weight -= ((data::Feature)RAND_MAX)/2;
  weight /= 10*((data::Feature)RAND_MAX)/2;
  return new network::Synapse(weight, type, params, id);
}

network::MLP::MLP (const size_t input,
		   const std::vector<size_t>& hidden,
		   const size_t output,
		   const config::NeuronStrategyType& neu_strat_type,
		   const config::Parameter* neu_params,
		   const config::SynapseStrategyType& syn_strat_type,
		   const config::Parameter* syn_params,
		   const data::Pattern& input_subtract,
		   const data::Pattern& input_divide,
		   sys::Reporter* reporter)
  : network::Network(reporter)
{
  RINGER_DEBUG2("Creating simple MLP network " << input << "-" << output << "...");
  std::vector<std::vector<network::Neuron*> > layers; //all neurons by layer
  std::vector<network::Neuron*> neurons; //all neurons, unorganized
  std::vector<network::Neuron*> current; //current layer being operated
  unsigned id = 0;

  //check the input normalization factor sizes
  if (input_subtract.size() != input)
    throw RINGER_EXCEPTION("Input subtraction factor and input size differ.");
  if (input_divide.size() != input)
    throw RINGER_EXCEPTION("Input division factor and input size differ.");

  //create input neurons
  for (size_t i=0; i<input; ++i) {
    network::Neuron* n = new network::InputNeuron(input_subtract[i], input_divide[i], &id);
    id++;
    neurons.push_back(n);
    current.push_back(n);
  }
  layers.push_back(current);
  current.erase(current.begin(), current.end()); //clear 'current' container
  RINGER_DEBUG2(layers[layers.size()-1].size() << " input neurons done.");

  //create hidden neurons
  for (size_t i=0; i<hidden.size(); ++i) {
    for (size_t j=0; j<hidden[i]; ++j) {
      network::Neuron* n = new network::HiddenNeuron(neu_strat_type, neu_params, &id);
      id++;
      neurons.push_back(n);
      current.push_back(n);
    }
    layers.push_back(current);
    current.erase(current.begin(), current.end()); //clear 'current' container
    RINGER_DEBUG2("Hidden layer[" << i << "] (" << layers[layers.size()-1].size() <<" neurons) done.");
  }
  RINGER_DEBUG2("Hidden neurons done.");

  //create output neurons
  for (size_t i=0; i<output; ++i) {
    network::Neuron* n = new network::OutputNeuron(neu_strat_type, neu_params, &id);
    id++;
    neurons.push_back(n);
    current.push_back(n);
  }
  layers.push_back(current);
  current.erase(current.begin(), current.end()); //clear 'current' container
  RINGER_DEBUG2(layers[layers.size()-1].size() << " output neurons done.");

  //Fully connect all layers
  std::vector<network::Synapse*> synapses; //all synapses, unorganized
  id = 0;
  for (size_t i=0; i<layers.size()-1; ++i) {
    for (size_t j=0; j<layers[i].size(); ++j) { //this layer
      for (size_t k=0; k<layers[i+1].size(); ++k) { //next layer
        network::Synapse* s = create_synapse(syn_strat_type, syn_params, &id);
        id++;
	      RINGER_DEBUG3("Connect [" << i << "][" << j << "] -> [" << i+1 << "][" << k << "]");
	      s->connect(layers[i][j], layers[i+1][k]);
	      synapses.push_back(s);
      }
    }
  }
  RINGER_DEBUG2("Connected all normal layers.");

  //Build parent network and that is it! Have fun:)
  network::Network::adopt(neurons, synapses);
  RINGER_DEBUG2("Sent information for parent network.");
}

network::MLP::MLP (const size_t input,
		   const std::vector<size_t>& hidden,
		   const size_t output,
		   const std::vector<bool>& bias,
		   const config::NeuronStrategyType& hidneu_strat_type,
		   const config::Parameter* hidneu_params,
		   const config::NeuronStrategyType& outneu_strat_type,
		   const config::Parameter* outneu_params,
		   const config::SynapseStrategyType& syn_strat_type,
		   const config::Parameter* syn_params,
		   const data::Pattern& input_subtract,
		   const data::Pattern& input_divide,
		   sys::Reporter* reporter)
  : network::Network(reporter)
{
  RINGER_DEBUG2("Creating complex MLP network...");
  std::vector<std::vector<network::Neuron*> > layers; //all neurons by layer
  std::vector<network::Neuron*> neurons; //all neurons, unorganized
  std::vector<network::Neuron*> current; //current layer being operated
  unsigned nodeId = 0;
  unsigned synId = 0;

  //create input neurons
  for (size_t i=0; i<input; ++i) {
    network::Neuron* n = new network::InputNeuron(input_subtract[i], input_divide[i], &nodeId);
    nodeId++;
    neurons.push_back(n);
    current.push_back(n);
  }
  layers.push_back(current);
  current.erase(current.begin(), current.end()); //clear 'current' container
  RINGER_DEBUG2("Input neurons done.");

  //create hidden neurons
  for (size_t i=0; i<hidden.size(); ++i) {
    for (size_t j=0; j<hidden[i]; ++j) {
      network::Neuron* n = new network::HiddenNeuron(hidneu_strat_type, hidneu_params, &nodeId);
      nodeId++;
      neurons.push_back(n);
      current.push_back(n);
    }
    layers.push_back(current);
    current.erase(current.begin(), current.end()); //clear 'current' container
    RINGER_DEBUG2("Hidden layer[" << i << "] done.");
  }
  RINGER_DEBUG2("Hidden neurons done.");

  //create output neurons
  for (size_t i=0; i<output; ++i) {
    network::Neuron* n = new network::OutputNeuron(outneu_strat_type, outneu_params, &nodeId);
    nodeId++;
    neurons.push_back(n);
    current.push_back(n);
  }
  layers.push_back(current);
  current.erase(current.begin(), current.end()); //clear 'current' container
  RINGER_DEBUG2("Output neurons done.");

  //Checks for biases
  RINGER_DEBUG2("Checking for required bias");
  if (bias.size() > layers.size()-1) {
    RINGER_DEBUG1("The number of bias layers (" << bias.size() 
		  << ") is bigger than the total number of hidden + output "
		  << "layers (" << hidden.size() + 1 << "). Exception thrown.");
    throw RINGER_EXCEPTION("Different number of hidden/outupt layers and bias layers");
  }

  //Fully connect all layers
  std::vector<network::Synapse*> synapses; //all synapses, unorganized
  for (size_t i=0; i<layers.size()-1; ++i) {
    for (size_t j=0; j<layers[i].size(); ++j) { //this layer
      for (size_t k=0; k<layers[i+1].size(); ++k) { //next layer
	      network::Synapse* s = create_synapse(syn_strat_type, syn_params, &synId);
	      synId++;
	      s->connect(layers[i][j], layers[i+1][k]);
	      synapses.push_back(s);
      }
    }
  }
  RINGER_DEBUG2("Connected all normal layers.");

  //Do the biasing of every layer that was asked for
  network::Neuron* bneuron = 0;
  size_t bias_counter = 0;
  for (size_t i=0; i<bias.size(); ++i) {
    if (!bneuron)
    {
      bneuron = new network::BiasNeuron(1.0, &nodeId); //create only once
      nodeId++;
    }
    if (bias[i]) { //bias hidden layer i+1
      for (size_t j=0; j<layers[i+1].size(); ++j) {
	      network::Synapse* s = create_synapse(syn_strat_type, syn_params, &synId);
	      synId++;
	      s->connect(bneuron, layers[i+1][j]);
	      synapses.push_back(s);
	      ++bias_counter; //bias neuron has been used
      }
    }
  }
  if (!bias_counter) delete bneuron; //bias neuron not used
  else neurons.push_back(bneuron); //register use
  RINGER_DEBUG2("Added bias (if requested).");

  //Build parent network and that is it! Have fun:)
  network::Network::adopt(neurons, synapses);
  RINGER_DEBUG2("Sent information for parent network.");
}



