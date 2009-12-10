//Dear emacs, this is -*- c++ -*-

/**
 * @file rbuild/RingSet.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a>
 * $Author: dferreir $
 * $Revision: 1.7 $
 * $Date: 2008-12-05 09:39:34 $
 *
 * @brief Describes a set of rings with associated values.
 */

#ifndef RINGER_RBUILD_RINGSET_H 
#define RINGER_RBUILD_RINGSET_H

#include "TrigRingerTools/roiformat/Cell.h"
#include <vector>
#include "TrigRingerTools/rbuild/RingConfig.h"
#include "TrigRingerTools/data/Pattern.h"

/**
 * Encompasses all classes related to ring building and manipulation
 */
namespace rbuild {

  /**
   * Builds a new set of rings
   */
  class RingSet {

  public: //interface

    /**
     * Builds a new RingSet, starting from a ring definition. The RingSet
     * starts empty.
     *
     * @param config This RingSet configuration
     */
    RingSet (const RingConfig& config);

    /**
     * Builds a new RingSet, starting from a ring definition. The RingSet
     * starts with the supplied rings.
     *
     * @param[in] config This RingSet configuration
     * @param[in] rings The initial rings values. The size of the rings must match the
     *            one specified in the config object to avoid problems.
     */
    RingSet (const RingConfig& config, const data::Pattern &rings);


    /**
     * to create an empty RingSet is possible, but watch out, you can't do
     * much with it...
     */
    RingSet ();

    /**
     * Virtualises the destructor
     */
    virtual ~RingSet();

    /**
     * Access the configuration for this object
     */
    inline const rbuild::RingConfig& config (void) const { return m_config; }

    /**
     * Adds some set of cells to this RingSet.
     *
     * @param c The cells to add
     * @param eta_center Where, in eta, I should center my rings
     * @param phi_center Where, in phi, I should center my rings
     */
    void add (const std::vector<const roiformat::Cell*>& c,
	      const double& eta_center, const double& phi_center);

    /**
     * Returns the (current) ring values.
     */
    inline data::Pattern& pattern (void) { return m_val; }

    /**
     * Resets all current values
     */
    inline void reset (void) { m_val = 0.0; }

  private: //representation

    rbuild::RingConfig m_config; ///< my own configuration for ring building
    data::Pattern m_val; ///< my current values
    
    mutable float m_cachedOverEtasize; ///< cached value of 1/m_config.eta_size() for optimizations
    mutable float m_cachedOverPhisize; ///< cached value of 1/m_config.phi_size() for optimizations

  };

}

#endif /* RINGER_RBUILD_RINGSET_H */
