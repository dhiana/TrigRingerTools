//Dear emacs, this is -*- c++ -*-

/**
 * @file data/DatabaseXml.h
 *
 * Loads a database in memory. The database file consists of a header
 * description and a set of entries each of which, contains one or more data
 * sets classified.
 */

#ifndef DATA_DATABASEXML_H
#define DATA_DATABASEXML_H

#include <string>
#include <map>
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/Header.h"
#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/XMLProcessor.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/RoIPatternSet.h"
#include <cmath>

namespace data {

  /**
   * Builds a new database from an XML configuration file
   */
  template <class TSet> class DatabaseXml : public Database<TSet> {

  protected: //representation
    
    data::Header* m_header; ///< The header information for this database

  public:

    /**
     * Builds an empty database
     *
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseXml (sys::Reporter *reporter);

    /**
     * Builds a new interface from a configuration file
     *
     * @param filename The name of the file to parse with the configuration
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseXml (const std::string& filename, sys::Reporter *reporter);

    /**
     * Builds a new database out of scratch parameters
     *
     * @param header This database header
     * @param data The PatternSets for this database, classified
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseXml (const data::Header* header,
	         const std::map<std::string, TSet*>& data,
	         sys::Reporter *reporter);

    /**
     * Copy constructor
     *
     * @param other The other database to copy data from
     */
    DatabaseXml (const DatabaseXml& other);

    /**
     * Destructor virtualisation
     */
    virtual ~DatabaseXml();

    /**
     * Returns a class that represents the Header entity
     */
    inline const data::Header* header() const { return m_header; }

    /**
     * Implements persistency functions.
     */

    bool read (const std::string& filename);
    bool save (const std::string& filename);

  private: //forbidden

    /**
     * Assignment
     */
    DatabaseXml& operator= (const DatabaseXml& other);

  };

}

/**
 * Dumps a database with a nice representation on a ostream
 *
 * @param os The output stream to use
 * @param db The database to dump
 */
template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::DatabaseXml<TSet>& db);

//------------------------------
// Template Implementation
//------------------------------

template <class TSet>
data::DatabaseXml<TSet>::DatabaseXml (sys::Reporter *reporter)
  : Database<TSet>(reporter),
    m_header(0)
{
}

template <class TSet>
data::DatabaseXml<TSet>::DatabaseXml (const std::string& filename, 
				      sys::Reporter *reporter)
  : Database<TSet>(reporter),
    m_header(0)
{
  read(filename);
}

template <class TSet>
data::DatabaseXml<TSet>::DatabaseXml (const data::Header* header, 
				      const std::map<std::string, TSet*>& data,
				      sys::Reporter *reporter)
  : Database<TSet>(data, reporter),
    m_header(0)
{
  m_header = new data::Header(*header);
}

template <class TSet>
data::DatabaseXml<TSet>::DatabaseXml (const DatabaseXml& other)
  : Database<TSet>((Database<TSet>) other),
    m_header(0)
{
  m_header = new data::Header(*other.m_header);
}

template <class TSet>
data::DatabaseXml<TSet>::~DatabaseXml() 
{
  if (m_header)
    delete m_header;
}

template <class TSet>
bool data::DatabaseXml<TSet>::save (const std::string& filename)
{
  RINGER_DEBUG2("Trying to save database at \"" << filename << "\".");
  std::string schema = sys::getenv("DATAPATH");
  if (schema.length() == 0) {
    RINGER_DEBUG1("I cannot find the standard schema path. Have you set"
		  << " DATAPATH? Exception thrown.");
    throw RINGER_EXCEPTION("DATAPATH not set");
  }
  
  schema = sys::look_in_path("database.xsd", schema);
  if (!schema.size())
  {
    throw RINGER_EXCEPTION("Schema file database.xsd not found in DATAPATH!");
  }

  sys::XMLProcessor xmlproc(schema, this->m_reporter);
  sys::xml_ptr root = xmlproc.new_document("database");
  sys::put_attribute_text(root, "version", "0.1");
  sys::put_node(root, this->m_header->node(root));
  sys::xml_ptr data = sys::put_element(root, "data");
  size_t index = 0;
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = this->m_data.begin(); it != this->m_data.end(); ++it) {
    RINGER_DEBUG2("XML'ing class \"" << it->first << "\".");
    sys::put_node(data, it->second->dump(root, it->first, index));
    index += it->second->size();
  }
  sys::put_node(root, data);
  RINGER_DEBUG2("Finally saving file...");
  if (!xmlproc.write(root, filename)) return false;
  RINGER_DEBUG2("File \"" << filename << "\" was saved.");
  return true;
}

template <class TSet>
bool data::DatabaseXml<TSet>::read (const std::string& filename)
{
  std::string schema = sys::getenv("DATAPATH");
  if (schema.length() == 0) {
    RINGER_DEBUG1("I cannot find the standard schema path. Have you set"
		  << " DATAPATH? Exception thrown.");
    throw RINGER_EXCEPTION("DATAPATH not set");
  }

  schema = sys::look_in_path("database.xsd", schema);
  if (!schema.size())
  {
    throw RINGER_EXCEPTION("Schema file database.xsd not found in DATAPATH!");
  }

  sys::XMLProcessor xmlproc(schema, this->m_reporter);

  RINGER_DEBUG2("Trying to parse " << filename);
  sys::xml_ptr root = xmlproc.read(filename);
  if (!root) {
    RINGER_WARN(this->m_reporter, "XML database file " << filename
		<< " cannot be parsed. Exception thrown.");
    throw RINGER_EXCEPTION("Cannot parse XML database file");
  }
  
  sys::xml_ptr_const top = sys::get_first_child(root);
  //read header info
  this->m_header = new data::Header(top);
  top = sys::get_next_element(top);

  //for all classes
  for (sys::xml_ptr_const jt=sys::get_first_child(top); jt; 
       jt=sys::get_next_element(jt)) {
    if (!sys::is_element(jt)) continue;
    std::string name = sys::get_attribute_string(jt, "name");
    if (this->m_data.find(name) != this->m_data.end()) {
      RINGER_DEBUG1("Error! Class name \"" << name << "\" already exists!"
		    << " Exception thrown.");
      throw RINGER_EXCEPTION("Repeated DB class names");
    }

    RINGER_REPORT(this->m_reporter, "Loading entries for class \"" << name << "\".");
    //Instantiates a simple PatternSet, which is the most reasonable
    //assumption I can have in the lack of more information
    this->m_data[name] = new TSet(jt);
    RINGER_REPORT(this->m_reporter, "Database class \"" << name << "\" has "
		  << this->m_data[name]->size() << " entries.");
  }

  //check pattern sizes (not a simple way to do it with libxml2 as 2.6.16...
  if (this->m_data.size() != 0) {
    this->m_patsize = this->m_data.begin()->second->pattern_size();
    for (typename std::map<std::string, TSet*>::const_iterator
	   it = this->m_data.begin(); it != this->m_data.end(); ++it) {
      if (this->m_patsize != it->second->pattern_size()) {
	RINGER_DEBUG1("PatternSet from class \"" << it->first << "\" seems to"
		      << " have a different pattern size ("
		      << it->second->pattern_size() << ") then the rest of"
		      << " the database (" << this->m_patsize 
		      << "). Exception thrown.");
	//unwind
	delete this->m_header;
	for (typename std::map<std::string, TSet*>::const_iterator it 
	       = this->m_data.begin(); it != this->m_data.end(); ++it) delete it->second;
	throw RINGER_EXCEPTION
	  ("Uncoherent database (different pattern sizes)");
      }
    }
  }

  RINGER_DEBUG2("Database file \"" << filename << "\" has " << this->m_data.size()
		<< " classes.");
  RINGER_DEBUG3("Database created from file \"" << filename << "\".");
  return true;
}

template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::DatabaseXml<TSet>& db)
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = db.data().begin(); it != db.data().end(); ++it)
    os << "Database class [" << it->first << "] is:" << std::endl
       << *it->second << std::endl;
  return os;
}


#endif /* DATA_DATABASEXML_H */
