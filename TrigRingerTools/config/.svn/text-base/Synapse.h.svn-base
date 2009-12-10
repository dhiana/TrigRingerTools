//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/Synapse.h
 *
 * @brief Defines a way to read synapse configurations from XML files.
 */

#ifndef CONFIG_SYNAPSE_H
#define CONFIG_SYNAPSE_H

#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/config/SynapseBackProp.h"
#include "TrigRingerTools/config/SynapseRProp.h"
#include "TrigRingerTools/sys/xmlutil.h"

namespace config {

  /**
   * Defines a Synapse configuration
   */
  class Synapse {

  public:
    /**
     * The XML side constructor
     *
     * @param node The node from where to extract the parameters.
     */
    Synapse (sys::xml_ptr_const node);

    /**
     * Constructs a Synapse from scratch
     */
    Synapse (unsigned int id, unsigned int from, unsigned int to,
	     const double& weight, 
	     const config::SynapseStrategyType& strategy, 
	     const Parameter* params);

    /**
     * The copy constructor
     */
    Synapse (const Synapse& other);

    /**
     * The copy operator
     */
    Synapse& operator= (const Synapse& other);

    /**
     * Destructor virtualisation
     */
    virtual ~Synapse ();

    /**
     * Returns the XML node representation for this Synapse.
     *
     * @param any Any node in the XML document.
     */
    sys::xml_ptr node (sys::xml_ptr any);

    /**
     * Dumps the current configuration on this item
     */
    config::Synapse dump() const;

    /**
     * Return information
     */
    inline unsigned int id() const { return m_id; }
    inline unsigned int from() const { return m_from; }
    inline unsigned int to() const { return m_to; }
    inline config::SynapseStrategyType strategy() const { return m_strategy; }
    inline double weight() const { return m_weight; }
    inline const Parameter* parameters() const { return m_params; }

  private:
    unsigned int m_id; ///< the unique synapse identifier
    unsigned int m_from; ///< the -start- neuron
    unsigned int m_to; ///< the -end- neuron
    config::SynapseStrategyType m_strategy; ///< the learning/running strategy
    Parameter* m_params; ///< my training parameters
    double m_weight; ///< my configured weight
  };

}

#endif /* CONFIG_SYNAPSE_H */
