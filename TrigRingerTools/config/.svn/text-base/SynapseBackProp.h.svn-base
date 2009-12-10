//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/SynapseBackProp.h
 *
 * @brief Describes parameters for a synapse in backpropagation mode.
 */

#ifndef CONFIG_SYNAPSEBACKPROP_H
#define CONFIG_SYNAPSEBACKPROP_H

#include "TrigRingerTools/config/Parameter.h"
#include "TrigRingerTools/sys/xmlutil.h"

namespace config {

  /**
   * Describes how to load back propagation parameters
   */
  class SynapseBackProp : public Parameter {

  public: //interface
    
    /**
     * How to start up from a node
     *
     * @param node The XML node where my parameters are
     */
    SynapseBackProp(sys::xml_ptr_const node);

    /**
     * How to start up from simple values
     *
     * @param learning_rate The learning rate for synapses
     * @param momentum The optional value to give for the momentum
     * @param learning_rate_decay The optional value to express how learning
     * decays with the iterations. 
     */
    SynapseBackProp(const double& learning_rate,
		    const double& momentum=0,
		    const double& learning_rate_decay=0);

    /**
     * How to copy (construct) this set of parameters
     *
     * @param other The parameters to copy
     */
    SynapseBackProp(const SynapseBackProp& other);

    /**
     * Destructor virtualisation
     */
    virtual ~SynapseBackProp() {}

    /**
     * How to copy this set of parameters
     *
     * @param other The parameters to copy
     */
    SynapseBackProp& operator= (const SynapseBackProp& other);

    /**
     * Returns the learning rate
     */
    double learning_rate() const { return m_learn_rate; }

    /**
     * Returns the momentum
     */
    double momentum() const { return m_momentum; }

    /**
     * Returns the learning rate decay
     */
    double learning_rate_decay() const { return m_learn_rate_decay; }

    /**
     * Clones this object
     */
    virtual Parameter* clone () const;

    /**
     * Gets the XML representation for these parameters.
     *
     * @param any Any node on the XML structure you want to create a new
     * node.
     */
    virtual sys::xml_ptr node (sys::xml_ptr any);

  private: //representation

    double m_learn_rate; ///< my learning rate
    double m_momentum; ///< my momentum
    double m_learn_rate_decay; ///< my learning rate decay

  };

}

#endif /* CONFIG_SYNAPSEBACKPROP_H */
