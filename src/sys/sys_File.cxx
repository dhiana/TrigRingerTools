//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/File.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.6 $
 * $Date: 2008-10-22 00:11:11 $
 *
 * A common interface to file readout.
 */

#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/Plain.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/CBNT.h"

sys::File::File (const std::string& filename, std::ios_base::openmode m,
		 const char sep, const std::string &extra)
  : m_fimpl(0),
    m_filename(filename),
    m_extra(extra)
{
  //no choices for the time being, only 1 reader implemented
  if (filename.find(".rfd.gz") != std::string::npos &&
      filename.find(".rfd.gz") == filename.size() - 7) { //a compressed file!
    RINGER_DEBUG1("Implementation to read gzip'ep files is not yet there!");
    throw RINGER_EXCEPTION("Cannot read gzip'ed files");
  }
  else if (filename.find(".rfd.bz2") != std::string::npos &&
	   filename.find(".rfd.bz2") == filename.size() - 8) { 
    //a compressed file!
    RINGER_DEBUG1("Implementation to read bzip2'ep files is not yet there!");
    throw RINGER_EXCEPTION("Cannot read bzip2'ed files");
  }
  else if (filename.find(".root") != std::string::npos) { // Covers .root, .root.*, etc...
    // a ROOT CBNT file
    try {
      m_fimpl = new sys::CBNT(filename, m, m_extra);
    } catch (sys::Exception &e) {
      RINGER_DEBUG1("Opening \"" << m_filename << "\" caused an exception.");
      RINGER_DEBUG1(e.what());
      RINGER_DEBUG1("This became an *invalid* file.");
      delete m_fimpl;
      m_fimpl = 0;
    }
  }
  else { //try as a plain file!
    try {
      m_fimpl = new sys::Plain(filename, m, sep);
      RINGER_DEBUG3("Plain file \"" << filename << "\" opened successfuly.");
    }
    catch (sys::Exception& e) {
      RINGER_DEBUG1("Openning \"" << m_filename << "\" caused an exception.");
      RINGER_DEBUG1(e.what());
      RINGER_DEBUG1("This became a *invalid* file.");
      delete m_fimpl;
      m_fimpl = 0;
    }
  }
}

void sys::File::open (const std::string& filename, std::ios_base::openmode m, const char sep, const std::string &extra)
{
  m_fimpl = 0;
  m_filename = filename;
  m_extra = extra;

  //no choices for the time being, only 1 reader implemented
  if (filename.find(".rfd.gz") != std::string::npos &&
      filename.find(".rfd.gz") == filename.size() - 7) { //a compressed file!
    RINGER_DEBUG1("Implementation to read gzip'ep files is not yet there!");
    throw RINGER_EXCEPTION("Cannot read gzip'ed files");
  }
  else if (filename.find(".rfd.bz2") != std::string::npos &&
	   filename.find(".rfd.bz2") == filename.size() - 8) { 
    //a compressed file!
    RINGER_DEBUG1("Implementation to read bzip2'ep files is not yet there!");
    throw RINGER_EXCEPTION("Cannot read bzip2'ed files");
  }
  else if (filename.find(".root") != std::string::npos &&
           filename.find(".root") == filename.size() - 5) {
    // a ROOT CBNT file
    try {
      m_fimpl = new sys::CBNT(filename, m, m_extra);
      RINGER_DEBUG3("ROOT file \"" << filename << "\" opened successfuly.");
    } catch (sys::Exception &e) {
      RINGER_DEBUG1("Opening \"" << m_filename << "\" caused an exception.");
      RINGER_DEBUG1(e.what());
      RINGER_DEBUG1("This became an *invalid* file.");
      delete m_fimpl;
      m_fimpl = 0;
    }
  }
  else { //try as a plain file!
    try {
      m_fimpl = new sys::Plain(filename, m, sep);
      RINGER_DEBUG3("Plain file \"" << filename << "\" opened successfuly.");
    }
    catch (sys::Exception& e) {
      RINGER_DEBUG1("Openning \"" << m_filename << "\" caused an exception.");
      RINGER_DEBUG1(e.what());
      RINGER_DEBUG1("This became a *invalid* file.");
      delete m_fimpl;
      m_fimpl = 0;
    }
  }
}

sys::File::~File ()
{
  delete m_fimpl;
}

std::string sys::File::filename_no_ext (void) const
{
  if (m_filename.find(".rfd") != std::string::npos) {
    return m_filename.substr(0, m_filename.rfind(".rfd"));
  } else if (m_filename.find(".root") != std::string::npos) {
    return m_filename.substr(0, m_filename.rfind(".root"));
  }
  return std::string();
}

bool sys::File::eof (void) const
{
  if ( m_fimpl ) return m_fimpl->eof();
  return true;
}

bool sys::File::good (void) const
{
  if ( m_fimpl ) return m_fimpl->good();
  if ( eof() ) return false;
  return false;
}

bool sys::File::readmore (void) const
{
  if ( m_fimpl ) return m_fimpl->readmore();
  return false;
}

bool sys::File::is_open(void) const
{
  if (!m_fimpl) return false;
  return m_fimpl->is_open();
}



