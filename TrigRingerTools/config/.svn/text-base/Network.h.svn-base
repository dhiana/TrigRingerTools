//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/Network.h
 *
 * @brief Describes how to read and parse (using XMLProcessor) configuration
 * data for a neural network.
 */

#ifndef CONFIG_NETWORK_H
#define CONFIG_NETWORK_H

#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/config/Header.h"
#include "TrigRingerTools/config/Synapse.h"
#include "TrigRingerTools/config/Neuron.h"
#include <string>
#include <vector>

namespace config {

  /**
   * Implements an interface to read data from XML configuration files
   */
  class Network {
    
  public: //< interface

    /**
     * Builds a new interface from a configuration file
     *
     * @param filename The name of the file to parse with the configuration
     * @param reporter The reporter to give to the configuration system
     */
    Network (const std::string& filename, sys::Reporter* reporter);

    /**
     * Builds a new configuration out of scratch parameters
     *
     * @param header This network header
     * @param neurons This network neurons
     * @param synapses This network synapses
     * @param reporter The reporter to give to the configuration system
     */
    Network (const config::Header* header, 
	     const std::vector<Synapse*>& synapses,
	     const std::vector<Neuron*>& neurons,
	     sys::Reporter* reporter);

    /**
     * Destructor virtualisation
     */
    virtual ~Network ();

    /**
     * Returns a class that represents the Header entity
     */
    inline const Header* header() const { return m_header; }

    /**
     * Returns a list of synapses loaded from the configuration file
     */
    inline const std::vector<Synapse*>& synapses() const { return m_synapse; }

    /**
     * Returns a list of neurons loaded from the configuration file
     */
    inline const std::vector<Neuron*>& neurons() const { return m_neuron; }

    /**
     * Saves the configuration that represents this network.
     *
     * @param filename The name of the file where to save this network
     * configuration.
     */
    bool save (const std::string& filename);

    /**
     * Creates a dot-file representing the diagram of the network.
     *
     * @param filename The name of the file where to save this network
     * dot-file representation.
     */
    bool dot (const std::string& filename);

  private: //representation

    sys::Reporter* m_reporter; ///< if present, the reporter to use
    Header* m_header; ///< my header
    std::vector<Synapse*> m_synapse; ///< my synapses
    std::vector<Neuron*> m_neuron; ///< my neurons
  };

}

#endif /* CONFIG_NETWORK_H */
