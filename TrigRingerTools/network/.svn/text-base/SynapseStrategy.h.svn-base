//Dear emacs, this is -*- c++ -*-

/**
 * @file SynapseStrategy.h 
 *
 * @brief This file contains the declaration of the Strategy package
 * and the (abstract) definition of the Learn entity.
 */

#ifndef STRATEGY_SYNAPSE_H
#define STRATEGY_SYNAPSE_H

#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/Feature.h"

/**
 * The Strategy package includes algorithms for network learning and
 * running. The algorithms are classes derived from base abstract
 * classes that define the each algorithm type interface. The
 * algorithms, in general implement the operator() in order to look
 * like functions. The parameter initialization happens upon algorithm
 * instanciation.
 */
namespace strategy {

  /**
   * How a network::Synapse can learn, abstractically.  Defines the base class
   * of algorithms that can be used for a network::Synapse to
   * learn. Essentially, algorithms of this class are initialized with
   * constant parameters (or maybe none) and implement operator(). This way
   * they can work like a function.
   */
  class SynapseStrategy {
    
  public:

    /**
     * Destructor virtualization
     *
     */
    virtual ~SynapseStrategy() {}

    /**
     * Defined the basic learning interface. This will returnd the adjustment
     * for the value of weight to train the neural network.
     *
     * @param input  The current state of the input neuron (last output).
     * @param lesson Something given by my caller.
     */
    virtual data::Feature teach (const data::Ensemble& input,
				 const data::Ensemble& lesson) = 0;
    
  };

}

#endif //STRATEGY_SYNAPSE_H
