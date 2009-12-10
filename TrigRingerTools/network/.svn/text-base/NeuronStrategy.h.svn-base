//Dear emacs, this is -*- c++ -*-

/**
 * @file NeuronStrategy.h
 *
 * Declares the abstract NeuronStrategy class of algorithms.
 */

#ifndef STRATEGY_NEURON_H
#define STRATEGY_NEURON_H

#include "TrigRingerTools/data/Ensemble.h"

namespace strategy {

  /**
   * Defines the base class of algorithms that can be used for a
   * network::Neuron to run in both operation <b>and</b> learning
   * phases. Essentially, algorithms of this class are initialized with
   * constant parameters (or maybe none) and implement operator(). This way
   * they can work like a function.
   */
  class Neuron {
    
  public:

    /**
     * Destructor virtualization
     *
     */
    virtual ~Neuron() {}

    /**
     * Defines the basic learning interface for neurons.
     *
     * @param data The data to transform according to this Strategy
     */
    virtual void run (data::Ensemble& data) const = 0;

    /**
     * Defines the basic learning interface for neurons.
     *
     * @param output The current neuron state, which is the same as the output
     * of the previous run(s).
     * @param lesson Something given by my caller.
     */
    virtual void teach (data::Ensemble& output, const data::Ensemble& lesson) 
      const = 0;

    /**
     * Returns the "dot" representation of this node
     */
    virtual std::string dot (void) const=0;

  };

}

#endif //STRATEGY_NEURON_H
