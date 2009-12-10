//Dear emacs, this is -*- c++ -*-

/**
 * @file roiformat/src/Database.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: ssnyder $
 * $Revision: 1.4 $
 * $Date: 2009-01-12 03:26:24 $
 *
 * Implements a Database of RoI's.
 */

#include "TrigRingerTools/roiformat/Database.h"
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include <sstream>

roiformat::Database::Database (sys::Reporter* reporter) 
  : m_nroi(0), 
    m_reporter(reporter)
{
  RINGER_DEBUG1("Created empty RoI dump Database.");
}

roiformat::Database::Database (const std::vector<const roiformat::RoI*>& vr,
			       sys::Reporter* reporter)
  : m_nroi(vr.size()),
    m_reporter(reporter)
{
  typedef std::vector<const roiformat::RoI*> vec_type;
  for (vec_type::const_iterator it=vr.begin(); it!=vr.end(); ++it) {
    RINGER_REPORT(m_reporter, "Loading RoI -> L1Id #" << (*it)->lvl1_id()
		  << " and RoI #" << (*it)->roi_id());
    m_rois[(*it)->lvl1_id()][(*it)->roi_id()] = **it;
  }
  RINGER_DEBUG2("Created new database with " << vr.size()
	      << " from vector of RoI's.");
}

bool roiformat::Database::load (const std::string& f, unsigned int /*start*/, 
				unsigned int /*quantity*/)
{
  //unloads what was previously loaded
  m_rois.clear();
  m_nroi = 0;

  //loads the new file
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  sys::File file(f);
  roiformat::RoI r;
  unsigned int counter = 0;
  unsigned int ignored = 0;
  while (! file.eof() && file.good() ) {
    file >> r;
    map_type::iterator it = m_rois.find(r.lvl1_id());
    if ( it != m_rois.end() ) {
      submap_type::iterator jt = it->second.find(r.roi_id());
      if ( jt != it->second.end() ) {
	RINGER_REPORT(m_reporter, 
		      "Discarding duplicated RoI at file \"" << f 
		      << "\" {LVL1 ID=" << r.lvl1_id() << ", RoI ID=" 
		      << r.roi_id() << "}");
	++ignored;
	continue;
      }
    }
    RINGER_REPORT(m_reporter, "Loading RoI -> L1Id #" << r.lvl1_id()
		  << " and RoI #" << r.roi_id());
    m_rois[r.lvl1_id()][r.roi_id()] = r;
    ++counter;
  }
  
  RINGER_DEBUG1("Created new database with " << counter << " RoI's (ignored "
	      << ignored << " RoI's) from file \"" << f << "\".");
  m_nroi = counter;
  return true;
}

bool roiformat::Database::save (const std::string& f) const
{
  sys::File file(f, std::ios_base::out|std::ios_base::app, ',');
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  for (map_type::const_iterator it=m_rois.begin(); it!=m_rois.end(); ++it) {
    for (submap_type::const_iterator jt=it->second.begin(); 
	 jt!=it->second.end(); ++jt) file << jt->second;
  }
  RINGER_DEBUG1("Wrote database with " << m_nroi << " RoI's into file \"" 
	      << f << "\".");
  return true;
}

std::string roiformat::Database::stats (void) const
{
  std::ostringstream oss;
  oss << m_nroi << " RoI's.";
  return oss.str();
}

bool roiformat::Database::check (void) const
{
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  for (map_type::const_iterator it=m_rois.begin(); it!=m_rois.end(); ++it) {
    for (submap_type::const_iterator jt=it->second.begin(); 
	 jt!=it->second.end(); ++jt) if (!jt->second.check()) return false;
  }
  return true;
}

bool roiformat::Database::operator== (const Database& other) const
{
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  map_type::const_iterator oit = other.m_rois.begin();
  for (map_type::const_iterator it=m_rois.begin(); it!=m_rois.end(); 
       ++it, ++oit) {
    submap_type::const_iterator ojt = oit->second.begin();
    for (submap_type::const_iterator jt=it->second.begin();
	 jt!=it->second.end(); ++jt, ++ojt) 
      if (jt->second.lvl1_id() != ojt->second.lvl1_id() || 
	  jt->second.roi_id() != ojt->second.roi_id()) {
	RINGER_DEBUG1("RoI " << jt->second.roi_id() << ", LVL1ID " 
		    << jt->second.lvl1_id()
		    << " seems not to exist in the other Database. Stop!");
	return false;
      }
  }
  RINGER_DEBUG1("Databases have the same identifiers.");
  return true;
}

void roiformat::Database::rois (std::vector<const roiformat::RoI*>& vr,
				unsigned int start,
				unsigned int quantity)
{
  if (!quantity) quantity = m_nroi;
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  for (map_type::const_iterator it=m_rois.begin(); it!=m_rois.end(); ++it) {
    for (submap_type::const_iterator jt=it->second.begin(); 
	 jt!=it->second.end(); ++jt) {
      if (!quantity) return; // stop when quantity becomes zero
      if (start) --start; 
      if (!start) {
	vr.push_back(&jt->second);
	--quantity;
      }
    }
  }
  //done
}

const roiformat::RoI* roiformat::Database::get (unsigned int lvl1_id, 
						unsigned int roi_id) const
{
  typedef std::map<unsigned, std::map<unsigned int, roiformat::RoI> > map_type;
  typedef std::map<unsigned int, roiformat::RoI> submap_type;
  map_type::const_iterator it = m_rois.find(lvl1_id);
  if ( it != m_rois.end() ) {
    submap_type::const_iterator jt = it->second.find(roi_id);
    if ( jt != it->second.end() ) return &(jt->second);
  }
  return 0;
}
