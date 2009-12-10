//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/SynapseRProp.h
 *
 * @brief Describes parameters for a synapse in rPropagation mode.
 */

#ifndef CONFIG_SYNAPSERPROP_H
#define CONFIG_SYNAPSERPROP_H

#include "TrigRingerTools/config/Parameter.h"
#include "TrigRingerTools/sys/xmlutil.h"

namespace config {

  /**
   * Describes how to load back propagation parameters
   */
  class SynapseRProp : public Parameter {

  public: //interface
    
    /**
     * How to start up from a node
     *
     * @param node The XML node where my parameters are
     */
    SynapseRProp(sys::xml_ptr_const node);

    /**
     * How to start up from simple values
     *
     * @param weight_update The value to be used as a start up weight
     * update. Normally should start at 0.1
     */
    SynapseRProp(const double& weight_update=0.1);

    /**
     * How to copy (construct) this set of parameters
     *
     * @param other The parameters to copy
     */
    SynapseRProp(const SynapseRProp& other);

    /**
     * Destructor virtualisation
     */
    virtual ~SynapseRProp() {}

    /**
     * How to copy this set of parameters
     *
     * @param other The parameters to copy
     */
    SynapseRProp& operator= (const SynapseRProp& other);

    /**
     * Returns the learning rate
     */
    double weight_update() const { return m_weight_update; }

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

    double m_weight_update; ///< my weight update

  };

}

#endif /* CONFIG_SYNAPSERPROP_H */
