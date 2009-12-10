//Dear emacs, this is -*- c++ -*-

/**
 * @file roiformat/src/RoI.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.8 $
 * $Date: 2008-05-30 22:03:35 $
 *
 * Implements the RoI component.
 */

#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/sys/debug.h"

roiformat::RoI::RoI ()
  : m_cells(),
    m_samp(),
    m_lvl1_id(0),
    m_roi_id(0),
    m_eta(0),
    m_phi(0),
    m_sampNeedUpdate(false)
{
}

roiformat::RoI::RoI (unsigned int lvl1_id, unsigned int roi_id, 
                     const double& eta, const double& phi)
  : m_cells(),
    m_samp(),
    m_lvl1_id(lvl1_id),
    m_roi_id(roi_id),
    m_eta(eta),
    m_phi(phi),
    m_sampNeedUpdate(false)
{
  RINGER_DEBUG2("Created RoI {LVL1ID: " << m_lvl1_id << " RoI: " 
                << m_roi_id << "} from scratch (empty).");
}
							   

roiformat::RoI::RoI (const std::vector<roiformat::Cell>& vc,
		     unsigned int lvl1_id, unsigned int roi_id,
		     const double& eta, const double& phi)
  : m_cells(vc),
    m_samp(),
    m_lvl1_id(lvl1_id),
    m_roi_id(roi_id),
    m_eta(eta),
    m_phi(phi),
    m_sampNeedUpdate(false)
{
  typedef std::vector<roiformat::Cell> vec_type;
  for (vec_type::const_iterator it=m_cells.begin(); it!=m_cells.end(); ++it) {
    roiformat::Cell::Sampling s = it->sampling();
    m_samp[s].push_back(&(*it));
  }
  RINGER_DEBUG2("Created RoI {LVL1ID: " << m_lvl1_id << " RoI: " 
	      << m_roi_id << "} from scratch.");
}

roiformat::RoI::RoI (const RoI& other)
  : m_cells(other.m_cells),
    m_samp(),
    m_lvl1_id(other.m_lvl1_id),
    m_roi_id(other.m_roi_id),
    m_eta(other.m_eta),
    m_phi(other.m_phi),
    m_sampNeedUpdate(false)
{
  typedef std::vector<roiformat::Cell> vec_type;
    for (vec_type::const_iterator it=m_cells.begin(); it!=m_cells.end(); ++it) {
    roiformat::Cell::Sampling s = it->sampling();
    m_samp[s].push_back(&(*it));
  }
  RINGER_DEBUG2("Copied RoI {LVL1ID: " << m_lvl1_id << " RoI: " 
	      << m_roi_id << "} from another RoI.");
}

void roiformat::RoI::insertCell(const roiformat::Cell &c) {
  // Inserts a cell on m_cells and marks m_samp to be updated
  m_cells.push_back(c);
  m_sampNeedUpdate = true;
}

roiformat::RoI& roiformat::RoI::operator= (const RoI& other)
{
  m_cells = other.m_cells;
  m_lvl1_id = other.m_lvl1_id;
  m_roi_id = other.m_roi_id;
  m_eta = other.m_eta;
  m_phi = other.m_phi;
  m_samp.clear(); ///< remove *all* entries for this map
  typedef std::vector<roiformat::Cell> vec_type;
  for (vec_type::const_iterator it=m_cells.begin(); it!=m_cells.end(); ++it) {
    roiformat::Cell::Sampling s = it->sampling();
    m_samp[s].push_back(&(*it));
  }
  m_sampNeedUpdate = false;
  RINGER_DEBUG2("Assigned RoI {LVL1ID: " << m_lvl1_id << " RoI: " 
	      << m_roi_id << "} from another RoI.");
  return *this;
}

const std::vector<const roiformat::Cell*>* roiformat::RoI::cells
(const roiformat::Cell::Sampling& s) const 
{ 
  updateSamp();
  typedef std::map<roiformat::Cell::Sampling, 
    std::vector<const roiformat::Cell*> > map_type; 
  map_type::const_iterator it = m_samp.find(s);
  if (it == m_samp.end()) return 0;
  return &(const_cast<roiformat::RoI*>(this)->m_samp[s]);
}

void roiformat::RoI::cells (std::vector<const roiformat::Cell*>& vc) const
{
  updateSamp();
  typedef std::vector<roiformat::Cell> vec_type;
  for (vec_type::const_iterator it=m_cells.begin(); it!=m_cells.end(); ++it) {
    vc.push_back(&(*it));
  }
}

void roiformat::RoI::cells (const roiformat::Cell::Sampling& s,
			    std::vector<const roiformat::Cell*>& vc) const
{
  updateSamp();
  typedef std::map<roiformat::Cell::Sampling, 
    std::vector<const roiformat::Cell*> > map_type; 
  map_type::const_iterator it = m_samp.find(s);
  if (it == m_samp.end()) return;
  //append
  typedef std::vector<const roiformat::Cell*> vec_type;
  vec_type tv = const_cast<roiformat::RoI*>(this)->m_samp[s];
  vc.insert(vc.end(), tv.begin(), tv.end());
}

bool roiformat::RoI::check (void) const
{
  updateSamp();
  //check for UNKNOWN calo
  typedef std::map<roiformat::Cell::Sampling, 
    std::vector<const roiformat::Cell*> > map_type;
  map_type::const_iterator mt = m_samp.find(roiformat::Cell::UNKNOWN);
  if ( mt != m_samp.end() ) {
    RINGER_DEBUG1("Found calo " << mt->second.size() << "cells of type UNKNOWN at"
		<< " {RoI: " << m_roi_id << " LVL1ID: " << m_lvl1_id);
    return false;
  }
  //check overlap of cells -- not implemented!
  return true;
}

std::ostream &operator<< (std::ostream &o, roiformat::RoI &r) {
  o << "{RoI " << r.roi_id() << ",LVL1ID " << r.lvl1_id() << ",Eta " << r.eta()
    << ",Phi " << r.phi() << "}" << std::endl;
  std::vector<const roiformat::Cell *> vc;
  r.cells(vc);
  for (std::vector<const roiformat::Cell *>::const_iterator i = vc.begin(); i != vc.end(); ++i) {
    o << "Cell #" << i - vc.begin() << std::endl;
    o << *(*i) << std::endl;
  }
  return o;
}

/*
sys::File& operator>> (sys::File& f, roiformat::RoI& r)
{
  return r << f;
}

sys::File& operator<< (sys::File& f, const roiformat::RoI& r)
{
  return r >> f;
}
*/

