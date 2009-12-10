//Dear emacs, this is -*- c++ -*-

/**
 * @file OutputNeuron.h
 *
 * @brief The base Neuron class definition
 *
 * This file contains the declaration for the OutputNeuron base class.
 */

#ifndef NETWORK_OUTPUTNEURON_H
#define NETWORK_OUTPUTNEURON_H

#include "TrigRingerTools/network/Neuron.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/network/NeuronStrategy.h"
#include <vector>
#include "TrigRingerTools/config/type.h"

namespace network {

  /**
   * Declares the Output Neuron. That type of Neuron sits at the end of the
   * Network, being its state representative of (part of) the Network
   * output. It is able to run and train, but does not accept output
   * connections.
   *
   * @see network::Neuron
   */
  class OutputNeuron : public Neuron {

  public:
    
    /**
     * Builds a new output neuron with the given parameters.
     *
     * @param strategy The strategy to use for running data through and
     * backwards this neuron.
     * @param parameters Parameters to be set according to the strategy chosen
     * @param id The optional identifier to this neuron.
     */ 
    OutputNeuron (const config::NeuronStrategyType& strategy,
		  const config::Parameter* parameters,
		  const unsigned int* id=0);


    /**
     * Builds a new output neuron from a configuration.
     *
     * @param config The configuration node from where to extract my
     * parameters
     */ 
    OutputNeuron (const config::Neuron& config);

    /**
     * Virtualisation of destructor.
     */
    virtual ~OutputNeuron ();

    /**
     * Implementation of abstract base class method.
     *
     * At this level, this method receives 1 to many inputs from
     * different Synapse's, adding them up at the private state
     * variable. If the number of inputs is equal to the number of
     * input Synapse's, the counter is reset and the state is activated
     * using the given forward step function.
     *
     * @see data::Pattern::apply()
     * @see network::Synapse
     */
    virtual void run (const data::Ensemble& data);

    /**
     * Implementation of abstract base class method.
     *
     * This method calls a learning Strategy to understand how to set
     * its own state and call the incomming Synapse's, forwarding the
     * request to train over the network. 
     *
     * @param error The input from the outside world
     */
    virtual void train (const data::Ensemble& error);

    /**
     * Implementation of abstract base class method.
     */
    virtual inline const data::Ensemble& state () const { return m_state; }

    /**
     * Implementation of abstract base class method.
     */
    virtual Neuron& in_connect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     *
     * This method does nothing.
     */
    virtual Neuron& out_connect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     */
    virtual Neuron& in_disconnect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     *
     * This method does nothing.
     */
    virtual Neuron& out_disconnect (Synapse* l);

    /**
     * This will dump the current configuration into a config::Neuron
     */
    virtual config::Neuron dump(void) const;

    /**
     * This will dump the connections of this neuron to an output stream, in
     * dot-file format.
     *
     * @param os The output stream to consider
     */
    virtual bool dot(std::ostream& os) const;

  private: ///Not implemented
    OutputNeuron (const OutputNeuron& other);
    OutputNeuron& operator= (const OutputNeuron& other);

  private: ///representation
    unsigned int m_counter; ///< A counter to the number of received inputs
    std::vector<Synapse*> m_isynapse; ///< The input Synapse's
    const config::NeuronStrategyType m_strategy; ///< type definition
    strategy::Neuron* m_master; ///< How this Neuron runs and learns
    data::Ensemble m_state; ///< The current state of the Neuron
  };

}

#endif //NETWORK_OUTPUTNEURON_H
