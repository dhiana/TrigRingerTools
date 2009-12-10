//Dear emacs, this is -*- c++ -*-

/**
 * @file network/network/SynapseBackProp.h 
 *
 * @brief This file contains the declaration of the Back Propagation
 * Learning algorithm. It is used for network::Synapse learning in Back
 * Propagation mode.
 */

#ifndef STRATEGY_SYNAPSEBACKPROP_H
#define STRATEGY_SYNAPSEBACKPROP_H

#include "TrigRingerTools/network/SynapseStrategy.h"
#include "TrigRingerTools/data/Feature.h"
#include "TrigRingerTools/config/SynapseBackProp.h"

namespace strategy {

  /**
   * Defines how a network::Synapse can learn using the Back
   * Propagation Method. This method is better explained in S.Haykin's
   * <b>Neural Networks</b> book. In short, that's what we have to do:
   *
   * -# Take the <em>lesson</em> that is given by the output neuron
   *    (the one that called me initially). The <em>lesson</em> is the 
   *    <em>local gradient</em> of a neuron;
   *    @f[ l^{(l)} @f]
   * -# Multiply the lesson by the <em>output</em> of the input layer;
   *    @f[
   *    l^{(l)} \times y^{(l-1)}
   *    @f]
   * -# Multiply the result of the previous step by the learning rate;
   *    @f[
   *    \eta \times l^{(l)} \times y^{(l-1)}
   *    @f]
   * -# Add the result of the previous setup to the current weight,
   *    possibly weighting the sum with a momentum ponderator.
   *    @f[
   *    w_{n+1} = (1-\mu) \times (w_{n} + \eta \times l^{(l)} 
   *    \times y^{(l-1)}) + (\mu) \times w_{n-1}
   *    @f]
   * -# Adjust the learning rate using the decay parameter;
   *    @f[
   *    \eta_{n+1} = \eta_{n} \times \gamma if E_{n} > E_{n-1}
   *    \eta_{n+1} = \eta_{n} \times (1 + \frac{1-\gamma}{10}) otherwise
   *    @f]
   */
  class SynapseBackProp : public strategy::SynapseStrategy {
    
  public:

    /**
     * The <em>scratch</em> constructor for this algorithm.
     *
     * @param lrate The Learning Rate.
     * @param momentum The learning momentum.
     * @param decay The Learning Rate decay parameter for this learning method.
     */
    SynapseBackProp (const data::Feature& lrate,
		     const data::Feature& momentum,
		     const data::Feature& decay);

    /**
     * Builds an new object with this type starting from configuration
     * parameters.
     *
     * @param config The configuration parameters to use as input
     */
    SynapseBackProp (const config::Parameter* config);

    /**
     * Destructor virtualisation
     */
    virtual ~SynapseBackProp() {}

    /**
     * Implements the basic learning interface for Back Propagation
     *
     * @param input  The state of the input neuron (last output).
     * @param lesson Something given by my caller (local gradients for
     * the previous levels).
     */
    virtual data::Feature teach (const data::Ensemble& input,
				 const data::Ensemble& lesson);

    /**
     * Dumps my configuration parameters on this configuration item
     */
    config::SynapseBackProp dump () const;

  private:
    data::Feature m_lrate; ///< The current learning rate
    data::Feature m_momentum; ///< This synapses momentum
    data::Feature m_decay; ///< The current learning rate decay
    data::Feature m_prev_delta; ///< The previous lesson I got
  };

}

#endif //STRATEGY_SYNAPSEBACKPROP_H
