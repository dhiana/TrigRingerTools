//Dear emacs, this is -*- c++ -*-

/**
 * @file network/network/NeuronBackProp.h
 *
 * Defines the concrete Back Propagation Learn Algorithm for Neuron's.
 */

#ifndef STRATEGY_NEURONBACKPROP_H
#define STRATEGY_NEURONBACKPROP_H

#include "TrigRingerTools/network/NeuronStrategy.h"
#include "TrigRingerTools/config/NeuronBackProp.h"

namespace strategy {

  /**
   * Declares how a network::Neuron learns in Back Propagation.
   *
   * This class declares the BackPropagation strategy for operating during the
   * running <b>and</b> learning phases of a Neural Network setup.
   */
  class NeuronBackProp : public strategy::Neuron {

  public:
    /**
     * Constructs from initial parametrisation
     *
     * @param af The propagation function class to use.
     */
    NeuronBackProp (const config::NeuronBackProp::ActivationFunction& af);

    /**
     * Constructs from a configuration object
     *
     * @param config The configuration object to extract my parameters from
     */
    NeuronBackProp (const config::Parameter* config);

    /**
     * Defines the basic learning interface for neurons.
     *
     * @param data The data to transform according to this Strategy
     */
    virtual void run (data::Ensemble& data) const;

    /**
     * Defines the basic learning interface for neurons operating
     * during a backpropagation step.
     *
     * The backpropagation step goes as follows:
     * -# Take the lesson from an external agent;
     *    @f[ l^{(l)} @f]
     * -# Multiply it by the derivative of the output function
     *    calculated with respect to the activation.
     *    @f[
     *    l^{(l)} \times \phi'(v^{(l)}_n)
     *    @f]
     *    For that we need the activation for this particular
     *    neuron, <b>or</b>, what is certainly faster, express the
     *    derivative in terms of the previous output of this
     *    neuron. For example:
     *    @f[
     *    \phi(x) = tanh(x)
     *    @f]
     *    @f[
     *    \frac{d\phi(x)}{dx} = 1 - tanh^2(x)
     *    @f]
     * -# Call all the input network::Synapse's with the calculated
     * values, so they can back propagate it too.
     *
     * @param output The current neuron state, which is the same as the output
     * of the previous run.
     * @param lesson Something given by my caller.
     */
    virtual void teach (data::Ensemble& output,	const data::Ensemble& lesson) 
      const;

    /**
     * Dumps the current configuration on this object
     */
    config::NeuronBackProp dump (void) const;

    /**
     * Returns the "dot" representation of this node
     */
    virtual std::string dot (void) const;

  private: //function types

    /**
     * Apply operation <code>forward</code> over argument;
     */
    typedef data::Feature (*forward) (data::Feature);
    
    /**
     * Apply operation <code>backward</code> over argument;
     */
    typedef data::Feature (*backward) (data::Feature);

  private:
    config::NeuronBackProp::ActivationFunction m_af; ///< function class
    forward m_forward; ///< my forward activation function (run)
    backward m_backward; ///< my backward activantion function (train)
  };

}

#endif //STRATEGY_NEURONBACKPROP_H
