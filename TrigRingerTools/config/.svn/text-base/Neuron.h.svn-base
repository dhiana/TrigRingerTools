//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/Neuron.h
 *
 * @brief Defines a way to read neuron information from XML configuration
 * files.
 */

#ifndef CONFIG_NEURON_H
#define CONFIG_NEURON_H

#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/config/Parameter.h"
#include "TrigRingerTools/sys/xmlutil.h"

namespace config {

  /**
   * Defines a neuron configuration
   */
  class Neuron {

  public:

    /**
     * Builds a neuron configuration from an XML DOM node
     *
     * @param node The node in the XML tree from where to extract this
     * neuron's configuration parameters.
     */
    Neuron (sys::xml_ptr_const node);

    /**
     * Builds a neuron from scratch
     *
     * @param id The unique identifier a neuron has to have
     * @param type The neuron type
     * @param strategy The strategy to use for this neuron
     * @param params The parameters for the neuron
     * @param bias The optional bias to insert into the neuron
     */
    Neuron (unsigned int id, const config::NeuronType& type,
	    const config::NeuronStrategyType* strategy=0,
	    const config::Parameter* params=0, const double& bias=0.0,
	    const double& subtract=0.0, const double& divide=1.0);

    /**
     * Builds a neuron configuration from another neuron
     */
    Neuron (const Neuron& other);

    /**
     * Copies a neuron configuration from another neuron
     */
    Neuron& operator= (const Neuron& other);

    /**
     * Destructor virtualisation
     */
    virtual ~Neuron();

    /**
     * Returns the node representation for this Neuron.
     *
     * @param any Any node in the XML tree where you would like to create the
     * new node.
     */
    sys::xml_ptr node (sys::xml_ptr any);

    /**
     * Answer stuff about this neuron
     */
    inline unsigned int id () const { return m_id; }
    inline config::NeuronType type() const { return m_type; }
    inline config::NeuronStrategyType strategy() const 
    { return m_strategy; }
    inline double bias () const { return m_bias; }
    inline double subtract () const { return m_subtract; }
    inline double divide () const { return m_divide; }
    inline const config::Parameter* parameters() const { return m_params; }

  private: //representation
    unsigned int m_id; ///< my unique identity
    config::NeuronType m_type; ///< my neuron type (enumeration)
    config::NeuronStrategyType m_strategy; ///< the activation function
    config::Parameter* m_params; ///< my strategy parameters
    double m_bias; ///< if I'm a bias neuron, I get this set
    double m_subtract; ///< if I'm an input neuron, I get this set
    double m_divide; ///< if I'm an input neuron, I get this set

  };

}

#endif /* NEURON_H */
