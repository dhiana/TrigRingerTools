//Dear emacs, this is -*- c++ -*-

/**
 * @file RingConfig.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.3 $
 * $Date: 2007-04-06 16:15:01 $
 *
 * @brief Defines a RingSet configuration.
 */

#ifndef RINGER_RBUILD_RINGCONFIG_H
#define RINGER_RBUILD_RINGCONFIG_H

#include "TrigRingerTools/roiformat/Cell.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include <vector>
#include <string>

namespace rbuild {

  /**
   * Parameters for ring building
   */
  class RingConfig {

 public: //enumerations
  
    enum Normalisation {
      EVENT = 0, ///< normalises the ring energies with the total event energy
      SECTION = 1, ///< normalises the ring energies with the section energy
      SET = 2, ///< normalises the ring energies with the set energy
      SEQUENTIAL = 3, ///< normalises the ring energies with a special scheme
      NONE = 4 ///< no normalisation is applied
    };
    
    typedef enum Normalisation Normalisation;
    
    enum Section {
      EM = 0, ///< electromagnetic section of the ATLAS calorimetry
      HADRONIC = 1 ///< hadronic section of the ATLAS calorimetry
    };
    
  public: //interface

    /**
     * Defines a new configuration
     *
     * @param eta_size The ring size, in eta
     * @param phi_size The ring size, in phi
     * @param n The number of ring values to create
     * @param name A name for this configuration
     * @param norm The normalisation strategy to use for this set
     * @param section The calorimeter section this set belongs to
     * @param detectors Detectors I should be used for.
     */
    RingConfig (const double& eta_size, const double& phi_size,
		const size_t n, std::string name, const Normalisation& norm,
		const Section& section,
		const std::vector<roiformat::Cell::Sampling>& detectors);

    /**
     * Builds from an XML node pointer
     *
     * @param node The node from where I should get my configuration
     */
    RingConfig (sys::xml_ptr_const node);

    /**
     * Empty ring configuration. Pay attention when using it...
     */
    RingConfig ();

    /**
     * Destructor virtualisation
     */
    virtual ~RingConfig();

    /**
     * accessors
     */
    inline const double& eta_size(void) const { return m_etasz; }
    inline const double& phi_size(void) const { return m_phisz; }
    inline size_t max(void) const { return m_max; }
    inline const std::string& name(void) const { return m_name; }
    inline Normalisation normalisation(void) const { return m_norm; }
    inline Section section(void) const { return m_section; }
    const std::vector<roiformat::Cell::Sampling>& detectors(void) const
    { return m_detector; }

  private: //representation

    double m_etasz; ///< the width of rings, in eta
    double m_phisz; ///< the width of rings, in phi
    size_t m_max; ///< the amount of rings to produce
    std::string m_name; ///< the name of this configuration
    Normalisation m_norm; ///< the normalisation scheme for this ring set
    Section m_section; ///< the section this ring set belongs to
    std::vector<roiformat::Cell::Sampling> m_detector; ///< I'm good for those
    
  };

  /**
   * Converts a normalisation type into a nice string representation
   *
   * @param norm the variable to convert
   */
  std::string norm2str (const RingConfig::Normalisation& norm);

  /**
   * Converts a string into a Normalisation type
   *
   * @param s the string to convert
   */
  RingConfig::Normalisation str2norm (const std::string& s);

  /**
   * Converts a section type into a nice string representation
   *
   * @param section the variable to convert
   */
  std::string section2str (const RingConfig::Section& section);

  /**
   * Converts a string into a Section type
   *
   * @param s the string to convert
   */
  RingConfig::Section str2section (const std::string& s);

}

#endif /* RINGER_RBUILD_RINGCONFIG_H */
