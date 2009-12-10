//Dear emacs, this is -*- c++ -*- 

/**
 * @file RingSet.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: ssnyder $
 * $Revision: 1.18 $
 * $Date: 2009-01-12 03:26:23 $
 *
 * Implements a set of rings
 */

#include "TrigRingerTools/rbuild/RingSet.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/roiformat/Cell.h"
#include <cmath>

rbuild::RingSet::RingSet (const RingConfig& config)
  : m_config(config),
    m_val(config.max()),
    m_cachedOverEtasize(1/config.eta_size()),
    m_cachedOverPhisize(1/config.phi_size())
    
{
  RINGER_DEBUG2("Created new RingSet with configuration parameters from \""
		<< m_config.name() << "\".");
}

rbuild::RingSet::RingSet (const RingConfig& config, const data::Pattern &rings)
  : m_config(config),
    m_val(rings),
    m_cachedOverEtasize(1/config.eta_size()),
    m_cachedOverPhisize(1/config.phi_size())
{
  RINGER_DEBUG2("Created new RingSet with configuration parameters from \""
		<< m_config.name() << "\".");
}

rbuild::RingSet::RingSet ()
  : m_config(),
    m_val(0),
    m_cachedOverEtasize(1),
    m_cachedOverPhisize(1)
{
  RINGER_DEBUG2("Created new empty RingSet.");
}

rbuild::RingSet::~RingSet()
{
  RINGER_DEBUG2("Destroyed RingSet with configuration parameters from \""
		<< m_config.name() << "\".");
}

void rbuild::RingSet::add (const std::vector<const roiformat::Cell*>& c,
			   const double& eta_center, const double& phi_center)
{
  typedef std::vector<const roiformat::Cell*> vec_type;
  typedef std::vector<roiformat::Cell::Sampling> samp_type;

  RINGER_DEBUG1("Starting add procedure for cell vector with " << c.size()
		<< " entries. The current RingSet is configured for:");
  //   for (samp_type::const_iterator jt=m_config.detectors().begin();
  //        jt!=m_config.detectors().end(); ++jt) 
  //     RINGER_DEBUG3(" ## " << roiformat::sampling2str(*jt));
  unsigned int fit_counter = 0;

  RINGER_DEBUG1("Considering center at (eta,phi) = (" << eta_center << "," << phi_center << ")");

  // Used later to multiply by the ring energy and get Et instead of E
  const double one_over = 1 / std::cosh(std::fabs(eta_center));
  
  //are we, possibly at the wrap-around region for phi?
  const bool wrap = roiformat::check_wrap_around(phi_center, false);
  if (wrap) {
    RINGER_DEBUG3("Possible Ring window at the phi wrap around" << " region *DETECTED*.");
  }
  const bool reverse_wrap = roiformat::check_wrap_around(phi_center, true);
  if (reverse_wrap) {
    RINGER_DEBUG3("Possible (reverse) Ring window at the phi wrap around" << " region *DETECTED*.");
  }

  //for all cells
  for (vec_type::const_iterator it=c.begin(); it!=c.end(); ++it) {
 
    //check if the cell is in a sampling I should handle. This is also useless
    //in a environment where the user knows the cell samplings (s)he is
    //feeding in. In this case, we could optimize the procedure eliminating
    //this check.

    /*
    bool belongs = false;
    for (samp_type::const_iterator jt=m_config.detectors().begin(); jt!=m_config.detectors().end(); ++jt)
    {
      if ((*it)->sampling() == *jt)
      {
        belongs = true;
        break;
      }
    }
    if (!belongs) continue; //go to next cell
    */

    //if I get here, is because I have to include the cell energy. It
    //calculates which ring the cells should be added to, then, it adds
    //up the cell value there and goes to the next cell. No need to do
    //anything later, because the sums are already correct!
    
    double phi_use = (*it)->phi(); //use this value for phi (wrap protection)
    if (wrap) phi_use = roiformat::fix_wrap_around(phi_use, false);
    else if (reverse_wrap) phi_use = roiformat::fix_wrap_around(phi_use, true);

    // Measure delta eta and delta phi to find out on which ring we are
    unsigned int i = 0;
    const double deltaEta = ((*it)->eta() - eta_center)*m_cachedOverEtasize;
    const double deltaPhi = (phi_use - phi_center)*m_cachedOverPhisize;
    const double deltaGreater = std::max(fabs(deltaEta), fabs(deltaPhi));
    i = static_cast<unsigned int>( std::floor (deltaGreater) );
    // Certain compilers have problems with round(), so let's do it ourselves
    if ( (deltaGreater - (double) i) > 0.5) ++i;

    if (i < m_val.size()) {
      //give us Et instead of E
      RINGER_DEBUG1(**it << " -> falls on ring[" << i << "]");
      m_val[i] += (((*it)->energy()) * one_over);
      ++fit_counter;
    }
    
  } //end for all cells

  //return happily
  RINGER_DEBUG2("A total of " << fit_counter << " (" 
		<<  (100*fit_counter)/c.size() << " %) cells were pertinent.");
  
  return;
}

