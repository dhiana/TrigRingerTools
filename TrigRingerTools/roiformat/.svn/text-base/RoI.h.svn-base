//Dear emacs, this is -*- c++ -*-

/**
 * @file roiformat/RoI.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: dferreir $
 * $Revision: 1.10 $
 * $Date: 2009-01-27 19:46:22 $
 *
 * @brief Represents a collection of <b>unique</b> cells, forming a region of
 * interest in the detector.
 */

#ifndef RINGER_ROIFORMAT_ROI_H
#define RINGER_ROIFORMAT_ROI_H

#include "TrigRingerTools/roiformat/Cell.h"
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/FileImplementation.h"
#include "TrigRingerTools/sys/Plain.h"
#include "TrigRingerTools/sys/CBNT.h"
#include <vector>
#include <map>

#ifdef ATHENA_VERSION
#include "CLIDSvc/CLASS_DEF.h"
#include "DataModel/DataVector.h"
#endif


namespace roiformat {

  /**
   * A RoI is a collection of roiformat::Cell's.
   */
  class RoI {

  public: //interface

    /**
     * Builds an empty RoI, without any roiformat::Cell's inside
     */
    RoI ();

    /**
      * Builds an empty RoI, without any roiformat::Cell's inside, but
      * sets eta, phi, roi ID and LVL1 id.
      * @param lvl1_id The LVL1 identifier
      * @param roi_id The RoI identifier inside the event
      * @param eta The eta location spotted by LVL1
      * @param phi The phi location spotted by LVL1
      */ 
    RoI (unsigned int lvl1_id, unsigned int roi,
         const double& eta, const double& phi);
						    

    /**
     * Builds a RoI from a vector of roiformat::Cell's and parameters.
     *
     * @param vc The vector of roiformat::Cell's as input
     * @param lvl1_id The LVL1 identifier
     * @param roi_id The RoI identifier inside the event
     * @param eta The eta location spotted by LVL1
     * @param phi The phi location spotted by LVL1
     */
    RoI (const std::vector<roiformat::Cell>& vc,
	 unsigned int lvl1_id, unsigned int roi_id,
	 const double& eta, const double& phi);

    /**
     * Copy constructor
     */
    RoI (const RoI& other);

    /**
      * Inserts a Cell on this RoI
      * @param c The cell to be inserted
      */
    void insertCell(const roiformat::Cell &c);

    /**
     * Assignment operator
     */
    RoI& operator= (const RoI& other);

    /**
     * Returns the roiformat::Cell's from a specific sampling. The returned
     * object cannot be changed by the user, that would have to copy it in
     * this case. Note that the non-existance of the given detector on the RoI
     * will oblige this procedure to return <code>0</code>
     * (<code>NULL</code>), so you have to check it.
     *
     * @param s The sampling layer you are interested on
     */
    const std::vector<const roiformat::Cell*>* cells
    (const roiformat::Cell::Sampling& s) const;

    /**
     * Returns all roiformat::Cell's as links.
     *
     * @param vc Where to link the cells you are interested on.
     */
    void cells (std::vector<const roiformat::Cell*>& vc) const;

    /**
     * Returns sampling roiformat::Cell's as links.
     *
     * @param s The sampling layer you are interested on
     * @param vc Where to link the cells you are interested on.
     */
    void cells (const roiformat::Cell::Sampling& s,
		std::vector<const roiformat::Cell*>& vc) const;

    /**
     * The RoI identifier inside the event
     */
    inline unsigned int roi_id (void) const { return m_roi_id; }

    /**
     * The LVL1 identifier for this RoI
     */
    inline unsigned int lvl1_id (void) const { return m_lvl1_id; }

    /**
     * The eta value, found with the poor granularity of LVL1.
     */
    inline double eta (void) const { return m_eta; }

    /**
     * The phi value, found with the poor granularity of LVL1.
     */
    inline double phi (void) const { return m_phi; }

    /**
     * Check this RoI for:
     * -# Overlapping cells
     * -# Occurrence of UNKNOWN calorimeter samplings
     */
    bool check (void) const;

    friend class sys::FileImplementation;
    friend class sys::Plain;
    friend class sys::CBNT;

  private:
    /**
     * Update m_samp
     */
    void updateSamp() const
    {
      if (m_sampNeedUpdate)
      {
        typedef std::vector<roiformat::Cell> vec_type;
        m_samp.clear();
        for (vec_type::const_iterator it=m_cells.begin(); it!=m_cells.end(); ++it)
        {
          roiformat::Cell::Sampling s = it->sampling();
          m_samp[s].push_back(&(*it));
        }
        m_sampNeedUpdate = false;
      }
    }


  private: //representation

    /**
     * The RoI Cell's are organised per layer to facilitate the access.
     */
    std::vector<roiformat::Cell> m_cells; ///< All my cells
   
    // Thanks to Denis for providing this "mutable" idea
    mutable std::map<roiformat::Cell::Sampling,
	             std::vector<const roiformat::Cell*> > m_samp; ///< per layer

    unsigned int m_lvl1_id; ///< The LVL1 identifier for this RoI
    unsigned int m_roi_id; ///< The LVL1 identifier for this RoI
    double m_eta; ///< The LVL1 eta location for this RoI
    double m_phi; ///< The LVL1 phi location for this RoI

    mutable bool m_sampNeedUpdate; ///< Flag that indicates that m_samp needs to be updated.
                                   ///< This allows further optimizations.
  };

}

std::ostream &operator<< (std::ostream &o, roiformat::RoI &r);

/** 
 * How to read my own data from a roiformat::File
 *
 * @param f The roiformat::File to read the data from
 * @param r The RoI where to put the data
 */
//sys::File& operator>> (sys::File& f, roiformat::RoI& r);

/** 
 * How to write my own data to a roiformat::File
 *
 * @param f The roiformat::File to write the data to
 * @param r The RoI where to take the data from
 */
//sys::File& operator<< (sys::File& f, const roiformat::RoI& r);

#ifdef ATHENA_VERSION
CLASS_DEF( roiformat::RoI , 198719673 , 1 )
CLASS_DEF( DataVector<roiformat::RoI> , 200512408 , 1 )
#endif


#endif /* RINGER_ROIFORMAT_ROI_H */
