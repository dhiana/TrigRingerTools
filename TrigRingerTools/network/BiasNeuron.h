//Dear emacs, this is -*- c++ -*-

/**
 * @file BiasNeuron.h
 *
 * @brief The Bias Neuron class declaration
 *
 * This file contains the declaration for the Bias Neuron class.
 */

#ifndef BIASNEURON_H
#define BIASNEURON_H

#include "TrigRingerTools/network/InputNeuron.h"

namespace network {

  /**
   * This class declares the BiasNeuron type. A Bias Neuron is just a
   * normal input Neuron without inputs needed from the environment.
   */
  class BiasNeuron : public InputNeuron {

  public:
    
    /**
     * Builds a new bias neuron
     *
     * @param bias The value to be send to the output Synapse's
     * @param id The optional identifier to this neuron.
     */
    BiasNeuron (const data::Feature& bias, const unsigned int* id=0);

    /**
     * Builds a new output neuron from a configuration.
     *
     * @param config The configuration node from where to extract my
     * parameters
     */ 
    BiasNeuron (const config::Neuron& config);

    /**
     * Destructor virtualisation.
     */
    virtual ~BiasNeuron ();

    /**
     * Implemenation of base class. Uses data to get the dimension of the
     * ensemble to send.
     */
    virtual void run (const data::Ensemble& data);

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
    using InputNeuron::dot;
    virtual bool dot(std::ostream& os) const;

  private: ///not implemented
    BiasNeuron (const BiasNeuron& other);
    BiasNeuron& operator= (const BiasNeuron& other);

  private: ///representation
    data::Feature m_bias; ///< my bias
    data::Ensemble m_state; ///< The current state of the Neuron
    
  };

}

#endif //BIASNEURON_H
