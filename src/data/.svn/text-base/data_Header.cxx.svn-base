//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/Header.cxx
 *
 * Implements header reading.
 */

#include "TrigRingerTools/data/Header.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"

data::Header::Header(sys::xml_ptr_const node)
{
  sys::xml_ptr_const it = sys::get_first_child(node);
  m_author = sys::get_element_string(it); 
  it = sys::get_next_element(it);
  m_name = sys::get_element_string((sys::xml_ptr)it);
  it = sys::get_next_element(it);
  m_version = sys::get_element_string((sys::xml_ptr)it);
  it = sys::get_next_element(it);
  m_created = sys::get_element_date((sys::xml_ptr)it);
  it = sys::get_next_element(it);
  m_lastSaved = sys::get_element_date((sys::xml_ptr)it);
  it = sys::get_next_element(it);
  if (it) m_comment = sys::get_element_string((sys::xml_ptr)it);
  RINGER_DEBUG3("Loaded header information for database \"" << m_name
	      << "\" version \"" << m_version << "\" from \"" 
	      << m_author << "\" last saved on \"" 
	      << sys::stringtime(&m_lastSaved) << "\"");
}

data::Header::Header(const std::string& author,
         	     const std::string& name,
		     const std::string& version,
		     const time_t& created,
		     const std::string& comment)
  : m_author(author),
    m_name(name),
    m_version(version),
    m_created(created),
    m_lastSaved(time(0)),
    m_comment(comment)
{
  RINGER_DEBUG3("Loaded header draft information for database \"" << m_name
	      << "\" version \"" << m_version << "\" from \"" 
	      << m_author << "\" last saved on \"" 
	      << sys::stringtime(&m_lastSaved) << "\"");
}

data::Header::Header (const Header& other)
  : m_author(other.m_author),
    m_name(other.m_name),
    m_version(other.m_version),
    m_created(other.m_created),
    m_lastSaved(time(0)),
    m_comment(other.m_comment)
{
  RINGER_DEBUG3("Built header draft information from another header \"" 
	      << m_name << "\" version \"" << m_version << "\" from \"" 
	      << m_author << "\" last saved on \"" 
	      << sys::stringtime(&m_lastSaved) << "\"");
}

data::Header& data::Header::operator= (const Header& other)
{
  m_author = other.m_author;
  m_name = other.m_name;
  m_version = other.m_version;
  m_created = other.m_created;
  m_lastSaved = time(0);
  m_comment = other.m_comment;
  RINGER_DEBUG3("Copied header draft information from another header \"" 
	      << m_name << "\" version \"" << m_version << "\" from \"" 
	      << m_author << "\" last saved on \"" 
	      << sys::stringtime(&m_lastSaved) << "\"");
  return *this;
}

sys::xml_ptr data::Header::node (sys::xml_ptr any)
{
  sys::xml_ptr root = sys::make_node(any, "header");
  sys::put_element_text(root, "author", m_author);
  sys::put_element_text(root, "name", m_name);
  sys::put_element_text(root, "version", m_version);
  sys::put_element_date(root, "created", m_created);
  sys::put_element_date(root, "lastSaved", time(0));
  if (m_comment.length()) //optional comment to insert
    sys::put_element_text(root, "comment", m_comment);
  return root;
}
