//Dear emacs, this is -*- c++ -*-

/**
 * @file RingConfig.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.2 $
 * $Date: 2007-04-06 16:15:02 $
 *
 * Parameters for ring configuration
 */

#include "TrigRingerTools/rbuild/RingConfig.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"

rbuild::RingConfig::RingConfig (const double& eta_size,
				const double& phi_size,
				const size_t n, std::string name, 
				const Normalisation& norm,
				const Section& section,
				const std::vector<roiformat::Cell::Sampling>&
				detectors)
  : m_etasz(eta_size),
    m_phisz(phi_size),
    m_max(n),
    m_name(name),
    m_norm(norm),
    m_section(section),
    m_detector(detectors)
{
  RINGER_DEBUG3("\"" << m_name << "\" is width(eta=" << m_etasz
		<< ";phi=" << m_phisz << "), length=" << m_max);
}

rbuild::RingConfig::RingConfig ()
  : m_etasz(),
    m_phisz(),
    m_max(),
    m_name("No given name"),
    m_detector()
{
}

rbuild::RingConfig::~RingConfig()
{
}

rbuild::RingConfig::RingConfig (sys::xml_ptr_const node)
{
  //Read length and name -- easiest
  m_max = sys::get_attribute_uint(node, "length");
  m_name = sys::get_attribute_string(node, "name");
  m_section = str2section(sys::get_attribute_string(node, "section"));
  m_norm = str2norm(sys::get_attribute_string(node, "normalisation"));

  sys::xml_ptr_const c = sys::get_first_child(node); //parameters...
  while (!sys::is_element(c)) c = sys::get_next_element(c);
  if (sys::get_element_name(c) == "width") {
    m_etasz = sys::get_attribute_double(c, "eta");
    m_phisz = sys::get_attribute_double(c, "phi");
  }
  c = sys::get_next_element(c);
  while (!sys::is_element(c)) c = sys::get_next_element(c);
  if (sys::get_element_name(c) == "detector") {
    c = sys::get_first_child(c); //entries
    while (c) {
      while (!sys::is_element(c)) c = sys::get_next_element(c);
      if (sys::get_element_name(c) == "entry") {
	m_detector.push_back(roiformat::str2sampling
			     (sys::get_element_string(c)));
	RINGER_DEBUG3("Ring \"" << m_name << "\" will include data from \""
		      << sampling2str(m_detector[m_detector.size()-1])
		      << "\"");
      }
      c = sys::get_next_element(c);
    }
  } //read all entries from "detector" node

  RINGER_DEBUG3("Loaded Ring Configuration \"" << m_name << "\"");
  RINGER_DEBUG2("\"" << m_name << "\" is width(eta=" << m_etasz
		<< ";phi=" << m_phisz << "), length=" << m_max
		<< " and responsible for " << m_detector.size() 
		<< " samplings.");
}

std::string rbuild::norm2str (const rbuild::RingConfig::Normalisation& norm)
{
  std::string retval;
  switch (norm) {
  case rbuild::RingConfig::EVENT:
    retval = "EVENT";
    break;
  case rbuild::RingConfig::SET:
    retval = "SET";
    break;
  case rbuild::RingConfig::SECTION:
    retval = "SECTION";
    break;
  case rbuild::RingConfig::SEQUENTIAL:
    retval = "SEQUENTIAL";
    break;
  case rbuild::RingConfig::NONE:
    retval = "NONE";
    break;
  }
  return retval;
}

rbuild::RingConfig::Normalisation rbuild::str2norm (const std::string& s)
{
  if (s == "EVENT") return RingConfig::EVENT;
  else if (s == "SET") return RingConfig::SET;
  else if (s == "SECTION") return RingConfig::SECTION;
  else if (s == "SEQUENTIAL") return RingConfig::SEQUENTIAL;
  return RingConfig::NONE;
}

std::string rbuild::section2str (const rbuild::RingConfig::Section& section)
{
  std::string retval;
  switch (section) {
  case rbuild::RingConfig::EM:
    retval = "EM";
    break;
  case rbuild::RingConfig::HADRONIC:
    retval = "HADRONIC";
    break;
  }
  return retval;
}

rbuild::RingConfig::Section rbuild::str2section (const std::string& s)
{
  if (s == "EM") return RingConfig::EM;
  return RingConfig::HADRONIC;
}
