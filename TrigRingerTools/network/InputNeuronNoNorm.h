//Dear emacs, this is -*- c++ -*-

/**
 * @file InputNeuronNoNorm.h
 *
 * @brief The Input Neuron without normalization class declaration
 *
 * This file contains the declaration for the Input Neuron (without normalization) class.
 */

#ifndef NETWORK_INPUTNEURONNONORM_H
#define NETWORK_INPUTNEURONNONORM_H

#include "TrigRingerTools/network/Neuron.h"
#include "TrigRingerTools/data/Ensemble.h"
#include <vector>

namespace network {

  class Synapse;

  /**
   * Describes the Input Neuron without normalization class.
   *
   * The Input Neuron is the entity responsible for inputting data
   * into the network during run phase. This neuron cannot be trained,
   * but to conform to the general aspect of a Neuron, it inherits
   * from Neuron.
   */
  class InputNeuronNoNorm: public Neuron {

  public:
    
    /**
     * Builds a new input neuron.
     *
     * @param subtract The subtraction factor to apply at input
     * @param divide The division factor to apply at input
     * @param id The optional identifier to this neuron.
     */
    InputNeuronNoNorm (const unsigned int* id=0);

    /**
     * Builds a new output neuron from a configuration.
     *
     * @param config The configuration node from where to extract my
     * parameters
     */ 
    InputNeuronNoNorm (const config::Neuron& config);

    /**
     * Virtualisation of destructor.
     */
    virtual ~InputNeuronNoNorm ();

    /**
     * Implementation of abstract base class method.
     *
     * This method loads the data from the only parameter, and calls
     * all output Synapse's to the current Neuron in order to forward the
     * running request.
     */
    virtual void run (const data::Ensemble& data);

    /**
     * Implementation of abstract base class method.
     *
     * This method does nothing.
     */
    virtual void train (const data::Ensemble& error);

    /**
     * Returns the last input for this neuron, if any.
     */
    virtual inline const data::Ensemble& state () const { return m_state; }

    /**
     * Implementation of abstract base class method.
     *
     * This method does nothing.
     */
    virtual Neuron& in_connect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     */
    virtual Neuron& out_connect (Synapse* l);

    /**
     * Implementation of abstract base class method.
     *
     * This method does nothing.
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

  protected:
    /**
     * This will dump the connections of this neuron to an output stream, in
     * dot-file format.
     *
     * @param os The output stream to consider
     * @param bias If this is a bias neuron, I have to do differently
     * @param bias_value The value of the bias node
     */
    virtual bool dot(std::ostream& os, bool bias, 
		     data::Feature bias_value) const;

  private: ///not implemented
    InputNeuronNoNorm (const InputNeuronNoNorm& other);
    InputNeuronNoNorm& operator= (const InputNeuronNoNorm& other);

  private: ///representation
    std::vector<Synapse*> m_osynapse; ///< The output Synapse's
    data::Ensemble m_state; ///< The current state of the Neuron
  };

}

#endif //NETWORK_INPUTNEURONNONORM_H
