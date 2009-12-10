//Dear emacs, this is -*- c++ -*-

/**
 * @file network/network/SynapseRProp.h 
 *
 * @brief This file contains the declaration of the Back Propagation
 * Learning algorithm. It is used for network::Synapse learning in Back
 * Propagation mode.
 */

#ifndef STRATEGY_SYNAPSERPROP_H
#define STRATEGY_SYNAPSERPROP_H

#include "TrigRingerTools/network/SynapseStrategy.h"
#include "TrigRingerTools/data/Feature.h"
#include "TrigRingerTools/config/SynapseRProp.h"

namespace strategy {

  /**
   * Defines how a network::Synapse can learn using the Resilient Propagation
   * Method. This method is better explained in: A direct Adaptative method for
   * Faster backpropagation learning: the RProp algorithm, by Martin
   * Riedmiller and Heinrich Braun, published at the 1993 (III) IEEE
   * International Conference on Neural Networks, pages 586-591 - May.
   */
  class SynapseRProp : public strategy::SynapseStrategy {
    
  public:

    /**
     * The <em>scratch</em> constructor for this algorithm.
     *
     * @param weight_update The value to be used as a start up weight
     * update. Normally should start at 0.1
     */
    SynapseRProp (const data::Feature& weight_update);

    /**
     * Builds an new object with this type starting from configuration
     * parameters.
     *
     * @param config The configuration parameters to use as input
     */
    SynapseRProp (const config::Parameter* config);

    /**
     * Destructor virtualisation
     */
    virtual ~SynapseRProp() {}

    /**
     * Implements the basic learning interface for Resilient BackPropagation 
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
    config::SynapseRProp dump () const;

  private:
    data::Feature m_weight_update; ///< The current weight update
    data::Feature m_prev_delta; ///< The previous lesson I got
    data::Feature m_prev_deriv; ///< The previous derivative I got
  };

}

#endif //STRATEGY_SYNAPSERPROP_H
