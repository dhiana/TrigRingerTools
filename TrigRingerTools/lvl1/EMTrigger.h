//Dear emacs, this is -*- c++ -*-

/**
 * @file lvl1/EMTrigger.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">André DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.4 $
 * $Date: 2007-04-12 21:03:39 $
 *
 * @brief A configurable LVL1 e.m. trigger simulator
 */

#ifndef LVL1_EMTRIGGER_H
#define LVL1_EMTRIGGER_H

#include "TrigRingerTools/roiformat/Database.h"
#include "TrigRingerTools/sys/Reporter.h"
#include <vector>

/**
 * Entities that helps us with the simulation of the LVL1 e.m. trigger
 */
namespace lvl1 {

  /**
   * Provided a few thresholds, an object of this class can filter an RoI,
   * or an RoI database, indicating which of those RoI's would pass the LVL1
   * trigger. 
   */
  class EMTrigger {

  public: //interface
    
    /**
     * Constructor
     *
     * @param em_threshold The threshold (maximum 2x1 TT sum inside the 2x2 TT
     * RoI cluster defined around the RoI center) to be used as a minimum of
     * energy deposition in the e.m. section, in GeV (transverse energy)
     * @param em_isolation The threshold for e.m. isolation in the 12 TT
     * neighboring the 2x2 TT core, in GeV (transverse energy)
     * @param had_isolation The hadronic isolation to be accounted for the 4x4
     * TT area formed behind the e.m. section of the RoI, in GeV (transverse
     * energy) 
     */
    EMTrigger (const double& em_threshold, const double& em_isolation,
	       const double& had_isolation); 

    /**
     * Destructor virtualization
     */
    virtual ~EMTrigger() {}

    /**
     * Tells if a single RoI would pass this trigger
     *
     * @param rep The reporter to use for reporting errors and problems.
     * @param roi The roi to be tested
     */
    bool filter (sys::Reporter* rep, const roiformat::RoI& roi) const;

    /**
     * Tells which RoI's from an RoI databased would pass this trigger. This
     * will call filter().
     *
     * @param rep The reporter to use for reporting errors and problems.
     * @param roidb The RoI database to be used for the tests
     * @param passed A vector with RoI pointers that have passed this trigger
     *
     * @return The number of RoIs that have passed the trigger
     */ 
    size_t filter (sys::Reporter* rep, roiformat::Database& roidb,
		   std::vector<const roiformat::RoI*>& vr) const;

  private: //representation (all in MeV despite the constructor requires GeV!)

    double m_em_threshold; ///< The minimum energy in the core
    double m_em_isolation; ///< The required e.m. isolation in the surroundings
    double m_had_isolation; ///< The required hadronic isolation in the back

  };

}

#endif /* LVL1_EMTRIGGER_H */
