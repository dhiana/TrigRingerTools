//Dear emacs, this is -*- c++ -*- 

/**
 * @file rbuild/util.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: ssnyder $
 * $Revision: 1.11 $
 * $Date: 2009-01-12 03:26:23 $
 *
 * Implements ring building utilities
 */

#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/data/SumExtractor.h"
#include "TrigRingerTools/data/MaxExtractor.h"
#include "TrigRingerTools/data/MinExtractor.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Reporter.h"

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
void rbuild::sequential (sys::Reporter* /*reporter*/,
			 data::Pattern& rings, 
			 const data::Feature& stop_energy)
{

  //if the ENERGY_THRESHOLD is greater than `stop', use stop instead.
  data::Feature stop = stop_energy;
  if (rbuild::ENERGY_THRESHOLD > stop) {
    RINGER_DEBUG1("I detected that the \"stop\" value is *smaller* than the"
		  << " global energy threshold (" << stop_energy
		  << " < " << rbuild::ENERGY_THRESHOLD 
		  << "). This is an invalid"
		  << " consideration. I'll overwrite the \"stop\" value with"
		  << " the energy threshold and proceed.");
    stop = rbuild::ENERGY_THRESHOLD;
  }

  data::Pattern norm(rings.size(), 0);
  data::SumExtractor sum;
  norm[0] = std::fabs(sum(rings));
  //if the sum is less than stop, apply layer normalisation to all rings
  if (norm[0] < stop) {
    //if the normalization factor is smaller than stop, I have to verify if it
    //is still too small. A small value will make things go bad... So, I
    //verify if the sum is smaller than the layer maxima, if that is the case,
    //I have to reconsider the normalization factor to a more reasonable
    //value, e.g. the layer absolute maximum
    data::MaxExtractor mxex;
    data::MinExtractor mnex;
    data::Feature max = mxex(rings);
    data::Feature min = mnex(rings);
    if (norm[0] < max) {
      data::Feature new_norm = std::fabs(max);
      if (std::fabs(min) > new_norm) new_norm = std::fabs(min);
      RINGER_DEBUG1("Replacing normalization factor ring-sum (" 
		  << norm[0] << " MeV) by layer absolute maxima (" 
		  << new_norm << " MeV)");
      norm[0] = new_norm;
    }

    //if the sum is even less than the threshold, do not apply any
    //normalisation at all here!
    if (norm[0] < rbuild::ENERGY_THRESHOLD) {
      RINGER_DEBUG1("Layer sum is less than \"" 
		  << rbuild::ENERGY_THRESHOLD << "\"," 
		  << " skipping normalisation for this set.");
      return;
    }

    RINGER_DEBUG1("Layer sum is less than \"" << stop << "\","
		  << " normalising all rings with total layer energy.");
    rings /= norm[0];
    return;
  }

  //I'm ok to proceed with the calculations
  bool fixed = false; //shall I stop to apply a variant normalisation factor?
  for (unsigned int i=1; i<rings.size(); ++i) {
    norm[i] = std::fabs(norm[i-1] - rings[i-1]);
    //if the normalization factor is less than `stop', just apply a fixed,
    //known to be great enough, value instead of that. Otherwise, the noise
    //will get too amplified.
    if (fixed || norm[i] < stop) {
      norm[i] = norm[0];
      if (!fixed) {
	fixed = true;
	RINGER_DEBUG2("Stopped sequential normalisation for values starting"
		      << " from ring[" << i << "], because my stop value is"
		      << " set to \"" << stop 
		      << "\" MeV and the factors got to that value.");
      }
    }
  }
  rings /= norm;
}

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
bool rbuild::find_center(sys::Reporter* /*reporter*/,
			 const roiformat::RoI* roi, 
			 double& eta, double& phi)
{
  std::vector<const roiformat::Cell*> cells;
  roi->cells(roiformat::Cell::EMBARREL2, cells);
  roi->cells(roiformat::Cell::EMENDCAP2, cells);
  if (!cells.size()) {
    RINGER_DEBUG1("I couldn't find any cells for layer e.m. second layer" 
		  << " in RoI" << " with L1Id #" << roi->lvl1_id() 
		  << " and RoI #" << roi->roi_id());
    return false;
  }
  RINGER_DEBUG2("I've found " << cells.size() << " at e.m. second layer...");
  roiformat::max(cells, eta, phi);
  RINGER_DEBUG2("The maximum happens at eta=" << eta << " and phi=" << phi);
  return true;
}

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
void rbuild::build_rings(sys::Reporter* /*reporter*/,
			 const roiformat::RoI* roi,
			 std::vector<rbuild::RingSet>& rset, 
			 bool own_center,
			 const double& eta, const double& phi, 
			 const double& eta_window,
			 const double& phi_window)
{
  //for each RingSet (calculate primary ring values, w/o normalization)
  for (std::vector<rbuild::RingSet>::iterator 
	 jt=rset.begin(); jt!=rset.end(); ++jt) {
    jt->reset(); //reset this ringset
    const std::vector<roiformat::Cell::Sampling>& dets = jt->config().detectors();
    //for all detectors in a given RingSet
    std::vector<const roiformat::Cell*> cells;
    for (std::vector<roiformat::Cell::Sampling>::const_iterator kt=dets.begin(); kt!=dets.end(); ++kt) {
      //get all cells for that RoI, that are from detectors I'm interested
      roi->cells(*kt, cells);
    } //for relevant detectors

    if (!cells.size()) {
      RINGER_DEBUG1("I couldn't find any cells for ring set \""
		    << jt->config().name() << "\" in RoI"
		    << " with L1Id #" << roi->lvl1_id() 
		    << " and RoI #" << roi->roi_id());
      continue;
    }
    RINGER_DEBUG2("I've found " << cells.size() << " cells for ring set" << " \"" << jt->config().name() << "\"...");

    //add the ring values for those cells, based on the center given or
    //calculate its own center.
    if (!own_center) {
      double my_eta, my_phi;
      roiformat::max(cells, my_eta, my_phi, eta, phi, eta_window, phi_window);
      jt->add(cells, my_eta, my_phi);
    }
    else jt->add(cells, eta, phi);

  } //for each RingSet
}

/**
 * Apply normalization based on the ring set configuration
 *
 * @param reporter A system-wide reporter to use
 * @param rset The ring set configuration to use for creating the rings
 */
void rbuild::normalize_rings(sys::Reporter* reporter,
			     std::vector<rbuild::RingSet>& rset)
{
  //at this point, I have all ring sets, separated
  double emsection = 0; // energy at e.m. section
  double hadsection = 0; // energy at hadronic section

  //for each RingSet (first iteration) -- apply set dependent norms.
  for (std::vector<rbuild::RingSet>::iterator jt=rset.begin(); jt!=rset.end(); ++jt)
  {
    //calculate the relevant energies
    double setenergy = 0;
    for (size_t i=0; i<jt->pattern().size(); ++i) setenergy += jt->pattern()[i];
    if (jt->config().section() == rbuild::RingConfig::EM) emsection += setenergy;
    else hadsection += setenergy;

    //what is the normalisation strategy here? Can I do something already?
    switch(jt->config().normalisation())
    {
      case rbuild::RingConfig::SET:
        if (setenergy > rbuild::ENERGY_THRESHOLD) jt->pattern() /= fabs(setenergy);
        else
        {
	        RINGER_DEBUG1("Ignoring normalisation command \""
		                    << norm2str(jt->config().normalisation())
		                    << "\" for RingSet \"" << jt->config().name() 
		                    << " because the denominator is bellow the " 
		                    << rbuild::ENERGY_THRESHOLD << " MeV threshold (value="
		                    << setenergy << ").");
        }
      break;
      case rbuild::RingConfig::SEQUENTIAL:
        sequential(reporter, jt->pattern());
      break;
      default: //do nothing
      break;
    }
    RINGER_DEBUG1("Set energy for \"" << jt->config().name() << "\" = " << setenergy);
  } //for each RingSet (first iteration)

  double event = emsection + hadsection; // event energy
  RINGER_DEBUG1("Event energy = " << event);
  RINGER_DEBUG1("E.m. energy = " << emsection);
  RINGER_DEBUG1("Hadronic energy = " << hadsection);

  //for each RingSet (third iteration) -- now accumulate and store
  for (std::vector<rbuild::RingSet>::iterator jt=rset.begin(); jt!=rset.end(); ++jt)
  {
    //what is the normalisation strategy here? Do the rest of options
    switch(jt->config().normalisation())
    {
      case rbuild::RingConfig::EVENT:
        if (event > rbuild::ENERGY_THRESHOLD) jt->pattern() /= fabs(event);
        else
        {
	        RINGER_DEBUG1("Ignoring normalisation command \""
		                    << norm2str(jt->config().normalisation())
		                    << "\" for RingSet \"" << jt->config().name() 
		                    << " because the denominator is bellow the " 
		                    << rbuild::ENERGY_THRESHOLD << " MeV threshold (value="
		                    << event << ").");
        }
      break;
      case rbuild::RingConfig::SECTION:
        if (jt->config().section() == rbuild::RingConfig::EM)
        {
	        if (emsection > rbuild::ENERGY_THRESHOLD) jt->pattern() /= fabs(emsection);
	        else
	        {
	          RINGER_DEBUG1("Ignoring normalisation command \""
		                      << norm2str(jt->config().normalisation())
		                      << "\" for RingSet \"" << jt->config().name() 
		                      << " because the denominator is bellow the "
		                      << rbuild::ENERGY_THRESHOLD << " MeV threshold (value="
		                      << emsection << ").");
	        }
        }
        else
        {
	        if (hadsection > rbuild::ENERGY_THRESHOLD) jt->pattern() /= fabs(hadsection);
	        else
	        {
	          RINGER_DEBUG1("Ignoring normalisation command \""
		                      << norm2str(jt->config().normalisation())
		                      << "\" for RingSet \"" << jt->config().name() 
		                      << " because the denominator is bellow the "
		                      << rbuild::ENERGY_THRESHOLD 
		                      << " MeV threshold (value=" << hadsection << ").");
	        }
        }
      break;
      default: //do nothing
      break;
    }
  }
}
