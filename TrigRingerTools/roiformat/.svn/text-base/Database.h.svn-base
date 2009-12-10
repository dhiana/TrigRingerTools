//Dear emacs, this is -*- c++ -*-

/**
 * @file roiformat/Database.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.4 $
 * $Date: 2007-04-12 21:03:54 $
 *
 * @brief An entity that can handle multiple RoI's spread around files.
 */

#ifndef RINGER_ROIFORMAT_DATABASE_H
#define RINGER_ROIFORMAT_DATABASE_H

#include <string>
#include <map>
#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/sys/Reporter.h"

namespace roiformat {

  /**
   * Manages multiple RoI's inside multiple files.
   */
  class Database {

  public: //interface

    /**
     * Starts a new, empty, database
     * @param reporter The reporter to use
     */
    Database (sys::Reporter* reporter);

    /**
     * Starts a new database from events already loaded
     *
     * @param vr The vector of RoI's to take for input
     * @param reporter The reporter to use
     */
    Database (const std::vector<const roiformat::RoI*>& vr,
	      sys::Reporter* reporter);

    /**
     * Loads a new set of RoI's into this db, clearing the previous one.
     *
     * @param f The filename containing the RoI's
     * @param start The number of the RoI to start from
     * @param quantity How many RoI's to load from this file
     */
    bool load (const std::string& f, unsigned int start=0, 
	       unsigned int quantity=0);

    /**
     * Saves all RoI's in the database to file. The file is appended if it
     * already exists.
     *
     * @param f The filename where to dump the RoI's.
     */
    bool save (const std::string& f) const;

    /**
     * Returns a string containing statistical information. For the time
     * being, this is just the number of RoI's in each loaded file.
     */
    std::string stats (void) const;

    /**
     * Performs a check in every RoI I've got, return <code>true</code> if
     * everything is ok, or <code>false</code> otherwise.
     */
    bool check (void) const;
    
    /**
     * Compare two databases w.r.t. the RoI's they contain, their number of
     * cells and identifiers.
     */
    bool operator== (const Database& other) const;

    /**
     * Get a handle to all RoI's, organized by LVL1 id and then by RoI id.
     *
     * @param vr Vector of RoI pointers I have.
     * @param start The first RoI to take
     * @param quantity The total number of RoI's the user wants.
     */
    void rois (std::vector<const roiformat::RoI*>& vr,
	       unsigned int start=0,
	       unsigned int quantity=0);

    /**
     * Returns a predetermined RoI from the database, based on its identifiers
     *
     * @param lvl1_id The level 1 identifier for the RoI of interest
     * @param roi_id The RoI identifier for the RoI of interest
     */
    const roiformat::RoI* get (unsigned int lvl1_id, 
			       unsigned int roi_id) const;

    /**
     * The number of RoI's I have loaded
     */
    inline size_t size () const { return m_nroi; }

  private: //not allowed (just reload...)

    /**
     * Copy constructor
     */
    Database (const Database& other);

    /**
     * Assignment operation
     */
    Database& operator= (const Database& other);

  private: //representation

    /**
     * My RoI's, organised by LVL1 Identifier and then by RoI identifier.
     */
    std::map<unsigned, std::map<unsigned int, roiformat::RoI> > m_rois;
    unsigned int m_nroi; ///< The total number of RoI's loaded
    sys::Reporter* m_reporter; ///< The reporter to use

  };

}

#endif /* RINGER_ROIFORMAT_DATABASE_H */
