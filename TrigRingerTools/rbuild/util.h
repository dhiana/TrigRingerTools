//Dear emacs, this is -*- c++ -*-

/**
 * @file rbuild/util.h
 *
 * @brief A set of utilities for ring building and normalization.
 */

#ifndef RBUILD_UTIL_H
#define RBUILD_UTIL_H

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/rbuild/RingSet.h"

namespace rbuild {

  /**
   * Defines the energy threshold for divisions, in MeV's
   */
  const double ENERGY_THRESHOLD = 0.001;

  /**
   * Calculates and applies the sequential normalisation vector that results
   * from the following arithmetic:
   *
   * @f[
   * N0 = E
   * N1 = E - E0
   * N2 = E - E0 - E1 = N1 - E1
   * N3 = E - E0 - E1 - E2 = N2 - E2
   * ...
   * NX = N(X-1) - E(X-1)
   * @f]
   *
   * Where Ni are the normalisation factors, E is the total layer energy and Ei
   * are the energy values for each ring. 
   *
   * @param reporter The reporter to use for reporting normalization problems
   * @param rings The calculated ring values one wishes to normalise
   * @param stop_energy The threshold to judge when to stop this normalisation
   * strategy and start using the total layer energy instead, in MeV.
   */
  void sequential (sys::Reporter* reporter, data::Pattern& rings, 
		   const data::Feature& stop_energy=100.0);

  /**
   * Calculates the center of interation based on the second e.m. layer
   * information or return false, indicating a center could not be found.
   *
   * @param reporter A system-wide reporter to use
   * @param roi The RoI to study
   * @param eta The eta value calibrated to the center
   * @param phi The phi value calibrated to the center
   *
   * @return <code>true</code> if everything goes Ok, or <code>false</code>
   * otherwise.
   */
  bool find_center(sys::Reporter* reporter, const roiformat::RoI* roi, 
		   double& eta, double& phi);

  /**
   * Calculates based on the RoI input and on the center previously calculated.
   *
   * @param reporter A system-wide reporter to use
   * @param roi The RoI dump to use as starting point
   * @param rset The ring set configuration to use for creating the rings
   * @param own_center If this value is set to <code>false</code> a layer based
   * center is calculated for the ring center. Otherwise, the values given on
   * the following variables are considered.
   * @param eta The center to consider when building the rings
   * @param phi The center to consider when building the rings
   * @param eta_window The window size in eta, to use when considering peak finding
   * @param phi_window The window size in phi, to use when considering peak finding
   */
  void build_rings(sys::Reporter* reporter,
		   const roiformat::RoI* roi,
		   std::vector<rbuild::RingSet>& rset, bool own_center,
		   const double& eta, const double& phi, 
		   const double& eta_window,
		   const double& phi_window);

  /**
   * Apply normalization based on the ring set configuration
   *
   * @param reporter A system-wide reporter to use
   * @param rset The ring set configuration to use for creating the rings
   */
  void normalize_rings(sys::Reporter* reporter,
		       std::vector<rbuild::RingSet>& rset);

}

#endif /* RBUILD_UTIL_H */
