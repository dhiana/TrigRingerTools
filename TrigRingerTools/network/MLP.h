//Dear emacs, this is -*- c++ -*-

/**
 * @file MLP.h
 *
 * @brief This class describes an easy interface to build the classical
 * (vanilla) Multi-Layer Perceptron network, which is feed-forward and
 * fully-connected with no feedback links.
 */

#ifndef NETWORK_MLP_H
#define NETWORK_MLP_H

#include "TrigRingerTools/network/Network.h"
#include "TrigRingerTools/network/NeuronStrategy.h"
#include "TrigRingerTools/network/SynapseStrategy.h"
#include <vector>

namespace network {

  /**
   * An easier way to build an MLP network, totally connected and exclusively
   * feedforward (no backfeeding allowed).
   */ 
  class MLP : public Network {

  public: //interface

    /**
     * Constructor, builds a new MLP, without biasing on neurons and with a
     * very simple structure.
     *
     * @param input The number of input lines
     *
     * @param hidden The number of neurons in each hidden layer
     *
     * @param output The number of output neurons
     *
     * @param neu_strat_type The global neuron strategy type for this network 
     * @param neu_params The global neuron strategy parameters to
     * apply for this network 
     *
     * @param syn_strat_type The global synapse strategy type for this network
     * @param syn_params The global synapse strategy parameters to apply for
     * this network
     *
     * @param subtract an optional vector that contains "input" entries, one
     * for each input. This is the subtraction factor to apply for every input
     * the network receives.
     * @param divide an optional vector that contains "input" entries, one
     * for each input. This is the division factor to apply for every input
     * the network receives.
     *
     * @param reporter The reporter to inform about changes or errors.
     */
    MLP (const size_t input,
	 const std::vector<size_t>& hidden,
	 const size_t output,
	 const config::NeuronStrategyType& neu_strat_type,
	 const config::Parameter* neu_params,
	 const config::SynapseStrategyType& syn_strat_type,
	 const config::Parameter* syn_params,
	 const data::Pattern& input_subtract,
	 const data::Pattern& input_divide,
	 sys::Reporter* reporter);

    /**
     * Constructor, builds a new MLP, with optional layer biasing and
     * different strategies for hidden and output layer neurons.
     *
     * @param input The number of input lines
     *
     * @param hidden The number of neurons in each hidden layer
     *
     * @param output The number of output neurons
     *
     * @param bias The layers to bias (this vector has to have a maximum size
     * given by the number of hidden layers plus the output layer in the network. The initial bias
     * is selected randomly between -0.1 and +0.1.
     *
     * @param hidneu_strat_type The global hidden neuron strategy type for this
     * network 
     * @param hidneu_params The global hidden neuron strategy parameters to
     * apply for this network 
     * @param outneu_strat_type The global output neuron strategy type for this
     * network 
     * @param outneu_params The global output neuron strategy parameters to
     * apply for this network 
     *
     * @param syn_strat_type The global synapse strategy type for this network
     * @param syn_params The global synapse strategy parameters to apply for
     * this network
     *
     * @param subtract an optional vector that contains "input" entries, one
     * for each input. This is the subtraction factor to apply for every input
     * the network receives.
     * @param divide an optional vector that contains "input" entries, one
     * for each input. This is the division factor to apply for every input
     * the network receives.
     *
     * @param reporter The reporter to inform about changes or errors.
     */
    MLP (const size_t input,
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
	 sys::Reporter* reporter);

    /**
     * Just to virtualise the destructor
     */
    virtual ~MLP() {}

  };

}

#endif /* NETWORK_MLP_H */
