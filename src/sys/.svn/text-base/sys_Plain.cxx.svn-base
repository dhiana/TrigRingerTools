//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/Plain.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-22 16:53:56 $
 *
 * Implements the plain text file readout
 */

#include <iomanip>
#include "TrigRingerTools/sys/Plain.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/roiformat/Cell.h"

sys::Plain::Plain (const std::string& filename, 
			 std::ios_base::openmode m,
                   const char sep)
  : m_filename(filename),
    m_file(filename.c_str(), m),
    m_sep(sep)
{
  if ( !m_file ) {
    RINGER_DEBUG1("I could *not* open the file \"" << m_filename 
		  << "\". Exception thrown.");
    throw RINGER_EXCEPTION("Cannot open file.");
  }

  //The output file has a default scientific notation for doubles
  m_file << std::scientific << std::setprecision(10);
  
  RINGER_DEBUG3("File \"" << m_filename << "\" opened successfuly.");
}

sys::Plain::~Plain ()
{
  m_file.close();
  RINGER_DEBUG3("File \"" << m_filename << "\" closed successfuly.");
}

sys::FileImplementation &sys::Plain::operator<< (const roiformat::RoI &roi) {
  roi.updateSamp();
  *this << "RoI: " << roi.roi_id() << " eta: " << roi.eta() << " phi: " << roi.phi()
        << " LVL1 ID: " << roi.lvl1_id() << "\n";
  typedef std::vector<roiformat::Cell> vec_type;
  for (vec_type::const_iterator it = roi.m_cells.begin(); it != roi.m_cells.end(); ++it) {
    *this << (*it);
  }
  return *this;
}

sys::FileImplementation &sys::Plain::operator>> (roiformat::RoI &roi) {
  std::string s;
  roi.m_cells.erase(roi.m_cells.begin(), roi.m_cells.end()); ///< clean-up
  *this >> s >> roi.m_roi_id >> s >> roi.m_eta >> s >> roi.m_phi >> s >> s >> roi.m_lvl1_id;
  RINGER_DEBUG2("Reading RoI {LVL1ID: " << roi.m_lvl1_id << " RoI: "
                << roi.m_roi_id << " eta: " << roi.m_eta << " phi: " << roi.m_phi
                << "} from File.");
  while (readmore() && !eof() && good()) {
    roiformat::Cell c;
    *this >> c;
    roi.m_cells.push_back(c);
  }
  roi.m_sampNeedUpdate = true;
  roi.updateSamp();
  RINGER_DEBUG2("Read RoI {LVL1ID: " << roi.m_lvl1_id << " RoI: "
                << roi.m_roi_id << "} from Plain file.");
  return *this;
}

sys::FileImplementation &sys::Plain::operator<< (const roiformat::Cell &cell) {
  std::string t = "";
  t += separator();
  *this << cell.sampling() << t << cell.eta() << t << cell.phi() << t << cell.r() << t
        << cell.deta() << t << cell.dphi() << t << cell.dr() << t << cell.energy() << "\n";
  RINGER_DEBUG3("Cell written to Plain file {" << cell.sampling() << "," << cell.eta()
                << "," << cell.phi() << "," << cell.r() << "," << cell.deta() << ","
		<< cell.dphi() << "," << cell.dr() << "," << cell.energy() << "}");
  return *this;
}

sys::FileImplementation &sys::Plain::operator>> (roiformat::Cell &cell) {
  char t;
  unsigned int sampling;
  *this >> sampling >> t >> cell.m_eta >> t >> cell.m_phi >> t >> cell.m_r >> t >> cell.m_deta
        >> t >> cell.m_dphi >> t >> cell.m_dr >> t >> cell.m_energy;
  cell.m_samp = (roiformat::Cell::Sampling) sampling;
  RINGER_DEBUG3("Cell read from Plain file {" << cell.m_samp << "," << cell.m_eta << "," << cell.m_phi
                << "," << cell.m_r << "," << cell.m_deta << "," << cell.m_dphi << "," << cell.m_dr
		<< "," << cell.m_energy << "}");
  return *this;
}

sys::Plain& sys::Plain::operator>> (int& i)
{
  m_file >> i;
  return *this;
}

sys::Plain& sys::Plain::operator>> (unsigned int& ui)
{
  m_file >> ui;
  return *this;
}

sys::Plain& sys::Plain::operator>> (double& d)
{
  m_file >> d;
  return *this;
}

sys::Plain& sys::Plain::operator>> (std::string& s)
{
  m_file >> s;
  return *this;
}

sys::Plain& sys::Plain::operator>> (char& c)
{
  m_file >> c;
  return *this;
}

sys::Plain& sys::Plain::operator<< (int i)
{
  m_file << i;
  return *this;
}

sys::Plain& sys::Plain::operator<< (unsigned int ui)
{
  m_file << ui;
  return *this;
}

sys::Plain& sys::Plain::operator<< (const double& d)
{
  m_file << d;
  return *this;
}

sys::Plain& sys::Plain::operator<< (char c)
{
  m_file << c;
  return *this;
}

sys::Plain& sys::Plain::operator<< (const std::string& s)
{
  m_file << s;
  return *this;
}

bool sys::Plain::eof (void) const
{
  return m_file.eof();
}

bool sys::Plain::good (void) const
{
  return m_file.good();
}

bool sys::Plain::readmore (void) const
{
  //peek next char to see if its an 'R', if is, return false, else return
  //true. Put back the character in any case...
  char c;
  sys::Plain* p = const_cast<sys::Plain*>(this);
  p->m_file >> c;
  p->m_file.putback(c);
  if (c == 'R') return false;
  return true;
}

