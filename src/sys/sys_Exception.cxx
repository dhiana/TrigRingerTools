//Dear emacs, this is -*- c++ -*-

/**
 * @file Exception.cxx
 *
 * Implements the basic exception type.
 */

#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include <cstring>
#include <sstream>

sys::Exception::Exception (const std::string& package, 
			      const std::string& file,
			      const unsigned int& lineno, 
			      const std::string& info)
  : m_package(package),
    m_file(file),
    m_lineno(),
    m_info(info),
    m_date(sys::currenttime("short")), ///< we start empty
    m_compose()
{
  std::ostringstream tmp;
  tmp << lineno;
  m_lineno = tmp.str();
  m_compose = m_package + "/" + m_file + "+" + m_lineno + " -> " + m_info +
    " @" + m_date;
}

sys::Exception::~Exception () throw ()
{
}
