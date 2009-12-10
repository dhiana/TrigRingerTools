//Dear emacs, this is -*- c++ -*-

/**
 * @file rbuild/Config.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.5 $
 * $Date: 2007-04-12 21:03:54 $
 *
 * @brief Configuration reader that generates RingConfig's
 */

#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/rbuild/RingConfig.h"
#include <string>
#include <map>

#ifndef RINGER_RBUILD_CONFIG_H
#define RINGER_RBUILD_CONFIG_H

namespace rbuild {

  /**
   * Implements an interface to read data from XML configuration files
   */
  class Config {
    
  public: //< interface

    /**
     * Reads from an XML configuration file
     *
     * @param filename The name of the file to parse with the configuration
     * @param reporter The reporter to give to the configuration system
     */
    Config (const std::string& filename, sys::Reporter* reporter);

    /**
     * Destructor virtualisation
     */
    virtual ~Config () {};

    /**
     * Returns the valid configurations I've read from the file
     */
    inline const std::map<unsigned int, rbuild::RingConfig>& config (void) 
      const { return m_config; }

  private: //representation

    sys::Reporter* m_reporter; ///< if present, the reporter to use
    std::map<unsigned int, rbuild::RingConfig> m_config; ///< my configs

  };

}

#endif /* CONFIG_H */
