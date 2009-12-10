//Dear emacs, this is -*- c++ -*-

/**
 * @file HiddenNeuron.h
 *
 * @brief Declares the HiddenNeuron class.
 */

#ifndef HIDDENNEURON_H
#define HIDDENNEURON_H

#include "TrigRingerTools/network/Neuron.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/network/NeuronStrategy.h"
#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/config/Parameter.h"
#include <vector>

namespace network {

  /**
   * This type of Neuron is just like the OutputNeuron, but can output
   * to many Synapse's instead. Therefore it contains functionality to
   * deal also with a large number of outputs.
   *
   * @see network::Neuron
   */
  class HiddenNeuron : public Neuron {
    
  public:
    /**
     * Builds a new hidden neuron with the given parameters.
     *
     * @param strategy The strategy to use for running data through and
     * backwards this neuron.
     * @param parameters Parameters to be set according to the strategy chosen
     * @param id The optional identifier to this neuron.
     */ 
    HiddenNeuron (const config::NeuronStrategyType& strategy,
		  const config::Parameter* parameters,
		  const unsigned int* id=0);


    /**
     * Builds a new output neuron from a configuration.
     *
     * @param config The configuration node from where to extract my
     * parameters
     */ 
    HiddenNeuron (const config::Neuron& config);

    /**
     * Virtualisation of destructor.
     */
    virtual ~HiddenNeuron ();

    /**
     * Implementation of abstract base class method.
     *
     * At this level, this method receives 1 to many inputs from
     * different Synapse's, adding them up at the private state
     * variable. If the number of inputs is equal to the number of
     * input Synapse's, the counter is reset and the state is activated
     * using the given forward step function. The state is passed
     * through the network::Synapse::pass() method to other Neuron's.
     *
     * @see strategy::forward
     * @see data::Pattern::apply()
     * @see network::Synapse
     */
    virtual void run (const data::Ensemble& data);

    /**
     * Implementation of abstract base class method.
     *
     * In analogy with run(), this method does the contrary: it
     * receives the training request from one to many output Synapse's
     * and add them up, updating the internal state. It then calls a
     * learning Strategy to understand how to set its own state and
     * call the incomming Synapse's, forwarding the request to train over
     * the network.
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
     */
    virtual Neuron& out_connect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     */
    virtual Neuron& in_disconnect (Synapse* l);

    /**
     * Implementation of abstract base class method.
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

  private: ///not implemented
    HiddenNeuron (const HiddenNeuron& other);
    HiddenNeuron& operator= (const HiddenNeuron& other);

  private: ///representation
    unsigned int m_counter; ///< A counter to the number of received inputs
    std::vector<Synapse*> m_isynapse; ///< The input Synapse's
    std::vector<Synapse*> m_osynapse; ///< The output Synapse's
    config::NeuronStrategyType m_strategy; ///< This neuron's strategy
    strategy::Neuron* m_master; ///< How this Neuron runs and learns
    data::Ensemble m_state; ///< The current state of the Neuron
    data::Ensemble m_error; ///< The error state of the Neuron
  };

}

#endif //HIDDENNEURON_H
