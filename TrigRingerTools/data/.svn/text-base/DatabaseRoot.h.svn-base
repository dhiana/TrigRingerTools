//Dear emacs, this is -*- c++ -*-

/**
 * @file data/DatabaseRoot.h
 *
 * Loads a database in memory. The database file consists of a
 * set of entries each of which, contains one or more data
 * sets classified. This class implements the ROOT persistency.
 */

#ifndef DATA_DATABASEROOT_H
#define DATA_DATABASEROOT_H

#include <string>
#include <vector>
#include <map>
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/PatternSet.h"
#include <cmath>

#include "TFile.h"
#include "TTree.h"

namespace data {

  /**
   * Builds a new database from a Root file
   */
  template <class TSet> class DatabaseRoot : public Database<TSet> {

  public:

    /**
     * Builds an empty database
     *
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseRoot (sys::Reporter *reporter);

    /**
     * Builds a new interface from a configuration file
     *
     * @param rootPath The paths, inside ROOT to be used to get the correct 
     *                 branch on the TTree for each class
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseRoot (const std::map<const std::string, const RootClassInfo>& rootPath,
                  sys::Reporter *reporter);

    /**
     * Builds a new database out of scratch parameters
     *
     * @param data The PatternSets for this database, classified
     * @param reporter The reporter to give to the configuration system
     */
    DatabaseRoot (const std::map<std::string, TSet*>& data,
	          sys::Reporter *reporter);

    /**
     * Copy constructor
     *
     * @param other The other database to copy data from
     */
    DatabaseRoot (const Database<TSet>& other);

    /**
     * Destructor virtualisation
     */
    virtual ~DatabaseRoot();

    bool read (const std::map<const std::string, const RootClassInfo>& rootPath);
    bool save (const std::map<const std::string, const RootClassInfo>& rootPath);

  private: //forbidden

    /**
     * Assignment
     */
    DatabaseRoot& operator= (const DatabaseRoot& other);

  protected: //representation
    
  };

}

/**
 * Dumps a database with a nice representation on a ostream
 *
 * @param os The output stream to use
 * @param db The database to dump
 */
template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::DatabaseRoot<TSet>& db);

//------------------------------
// Template Implementation
//------------------------------

template <class TSet>
data::DatabaseRoot<TSet>::DatabaseRoot (sys::Reporter *reporter)
  : Database<TSet>(reporter)
{
  RINGER_DEBUG2("data::DatabaseRoot::DatabaseRoot(sys::Reporter *reporter)");
}

template <class TSet>
data::DatabaseRoot<TSet>::DatabaseRoot (const std::map<const std::string, const RootClassInfo>& rootPath,
                                        sys::Reporter *reporter)
  : Database<TSet>(reporter)
{
  RINGER_DEBUG2("data::DatabaseRoot::DatabaseRoot(const std::map<const std::string, const RootClassInfo>& rootPath, sys::Reporter *reporter)");
  read(rootPath);
}

template <class TSet>
data::DatabaseRoot<TSet>::DatabaseRoot (const std::map<std::string, TSet*>& data,
				        sys::Reporter *reporter)
  : Database<TSet>(data, reporter)
{
}

template <class TSet>
data::DatabaseRoot<TSet>::DatabaseRoot (const Database<TSet>& other)
  : Database<TSet>(other)
{
  RINGER_DEBUG2("data::DatabaseRoot::DatabaseRoot(const Database<TSet>& other)");
}

template <class TSet>
data::DatabaseRoot<TSet>::~DatabaseRoot() 
{
}

template <class TSet>
bool data::DatabaseRoot<TSet>::read (const std::map<const std::string, const RootClassInfo>& rootPath)
{
  RINGER_DEBUG2("data::DatabaseRoot::read(...): Size of rootPath is " << rootPath.size());
  typedef const std::map<const std::string, const RootClassInfo> mapClasses;
  for (mapClasses::const_iterator i = rootPath.begin(); i != rootPath.end(); i++) {
    const std::string name = i->first;
    if (this->m_data.find(name) != this->m_data.end()) {
      RINGER_DEBUG1("Error! Class name \"" << name << "\" already exists!"
                    << " Exception thrown.");
      throw RINGER_EXCEPTION("Repeated DB class names");
    }

    RINGER_REPORT(this->m_reporter, "Loading entries for class \"" << name << "\".");
    //Instantiates a simple PatternSet, which is the most reasonable
    //assumption I can have in the lack of more information
    this->m_data[name] = new TSet(i->second);
    RINGER_REPORT(this->m_reporter, "Database class \"" << name << "\" has "
		  << this->m_data[name]->size() << " entries.");
  }

  //check pattern sizes
  if (this->m_data.size() != 0) {
    this->m_patsize = this->m_data.begin()->second->pattern_size();
    for (typename std::map<std::string, TSet*>::const_iterator
	   it = this->m_data.begin(); it != this->m_data.end(); ++it) {
      if (this->m_patsize != it->second->pattern_size()) {
	RINGER_DEBUG1("PatternSet from class \"" << it->first << "\" seems to"
		      << " have a different pattern size ("
		      << it->second->pattern_size() << ") then the rest of"
		      << " the database (" << this->m_patsize 
		      <<"). Exception thrown.");
	for (typename std::map<std::string, TSet*>::const_iterator it 
	       = this->m_data.begin(); it != this->m_data.end(); ++it) delete it->second;
	throw RINGER_EXCEPTION
	  ("Uncoherent database (different pattern sizes)");
      }
    }
  }

  return true;
}

template <class TSet>
bool data::DatabaseRoot<TSet>::save (const std::map<const std::string, const RootClassInfo>& rootPath)
{
  RINGER_DEBUG2("data::DatabaseRoot::save(...): Size of rootPath is " << rootPath.size());
  typedef const std::map<const std::string, const RootClassInfo> mapClasses;
  for (mapClasses::const_iterator i = rootPath.begin(); i != rootPath.end(); i++) {
    const std::string name = i->first;
    if (this->m_data.find(name) == this->m_data.end()) {
      RINGER_DEBUG1("Error! Class name \"" << name << "\" doesn't exist!"
                    << " Exception thrown.");
      throw RINGER_EXCEPTION("Trying to save non-existing class");
    }

    RINGER_REPORT(this->m_reporter, "Saving entries for class \"" << name << "\".");
    //Instantiates a simple PatternSet, which is the most reasonable
    //assumption I can have in the lack of more information
    this->m_data[name]->dump(i->second);
    RINGER_REPORT(this->m_reporter, "Database class \"" << name << "\" has "
		  << " been saved.");
  }

  return true;
}

template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::DatabaseRoot<TSet>& db)
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = db.data().begin(); it != db.data().end(); ++it)
    os << "Database class [" << it->first << "] is:" << std::endl
       << *it->second << std::endl;
  return os;
}


#endif /* DATA_DATABASEROOT_H */
