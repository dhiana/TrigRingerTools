//Dear emacs, this is -*- c++ -*- 

/**
 * @file roiformat/Cell.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.13 $
 * $Date: 2008-05-30 21:59:21 $
 *
 * @brief Implements a Cell that can be readout of a roiformat::File or just
 * exist in the empty space. Scratch constructors are provided.
 */

#ifndef RINGER_ROIFORMAT_CELL_H
#define RINGER_ROIFORMAT_CELL_H

#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/FileImplementation.h"
#include "TrigRingerTools/sys/Plain.h"
#include "TrigRingerTools/sys/CBNT.h"
#include <string>
#include <vector>
#include <cmath>

namespace roiformat {

  /**
   * safe PI wrap-around threshold for both egamma and jet objects
   */
  const double PI_THRESHOLD = 1.0;
  
  /**
   * 2*PI default value
  */
  const double TWO_PI = 2*M_PI;
  
  /**
  * Helper function to identify wrap around cases.
  * if reverse_wrap = true, then we check for reverse wrap_arround.
  */
  inline bool check_wrap_around(const double phi_center, const bool reverse_wrap)
  {
    if (reverse_wrap) return (phi_center < (PI_THRESHOLD - M_PI));
    else return (phi_center > (M_PI - PI_THRESHOLD));
  }
  
  /**
  * Helper function to fix wrap around cases.
  * if reverse_wrap = true, then we fix reverse wrap_arround cases.
  */
  inline double fix_wrap_around(const double phi_value, const bool reverse_wrap)
  {
    if (reverse_wrap)
    {
      if (phi_value > 0.)  return (phi_value - TWO_PI);
    }
    else
    {
      if (phi_value < 0.) return (phi_value + TWO_PI);
    }
    return phi_value;
  }

  /**
   * Describes every cell in a calorimeter
   */
  class Cell {

  public: //interface

    enum Sampling { 
      PSBARREL    =  0, ///< pre-sampler, barrel section
      EMBARREL1   = 1, ///< electromagnetic calo, barrel section, front layer
      EMBARREL2   = 2, ///< electromagnetic calo, barrel section, middle layer
      EMBARREL3   = 3, ///< electromagnetic calo, barrel section, back layer
      PSENDCAP    =  4, ///< pre-sampler, endcap section
      EMENDCAP1   = 5, ///< electromagnetic calo, endcap section, front layer
      EMENDCAP2   = 6, ///< electromagnetic calo, endcap section, front layer
      EMENDCAP3   = 7, ///< electromagnetic calo, endcap section, front layer
      HADENCAP0   = 8, ///< hadronic calo, endcap section (LAr), layer 0
      HADENCAP1   = 9, ///< hadronic calo, endcap section (LAr), layer 1
      HADENCAP2   = 10, ///< hadronic calo, endcap section (LAr), layer 2
      HADENCAP3   = 11, ///< hadronic calo, endcap section (LAr), layer 3
      TILEBARREL0 = 12, ///< hadronic calo, barrel (Tiles), layer 0
      TILEBARREL1 = 13, ///< hadronic calo, barrel (Tiles), layer 1
      TILEBARREL2 = 14, ///< hadronic calo, barrel (Tiles), layer 2
      TILEGAPSCI0 = 15, ///< hadronic calo, barrel (Tiles), gap scint. layer 0
      TILEGAPSCI1 = 16, ///< hadronic calo, barrel (Tiles), gap scint. layer 1
      TILEGAPSCI2 = 17, ///< hadronic calo, barrel (Tiles), gap scint. layer 2
      TILEEXTB0   = 18, ///< hadronic calo, extended barrel (Tiles), layer 0
      TILEEXTB1   = 19, ///< hadronic calo, extended barrel (Tiles), layer 1
      TILEEXTB2   = 20, ///< hadronic calo, extended barrel (Tiles), layer 2
      FORWCAL0    = 21, ///< forward calo, layer 0
      FORWCAL1    = 22, ///< forward calo, layer 1
      FORWCAL2    = 23, ///< forward calo, layer 2
      UNKNOWN     = 24  ///< unknown piece of data, not supposed to occur:)
    };

    /**
     * An alias to simplify our lives.
     */
    typedef enum Sampling Sampling;

    /**
     * Scratch build up. Take default values (zeroes)
     */
    Cell () {};

    /**
     * Builds a new cell, specifying every cell value
     *
     * @param sampling This cell's sampling
     * @param eta This cell's center in eta
     * @param phi This cell's center in phi
     * @param r This cell's radius, with respect to ATLAS center
     * @param deta The size of the cell, in eta
     * @param dphi The size of the cell, in phi
     * @param dr The size of the cell, in the radius direction
     * @param energy The cell's energy
     */
    Cell (Sampling sampling, const double& eta, const double& phi, 
	  const double& r, const double& deta, const double& dphi,
	  const double& dr, const double& energy);

    /**
     * Destroyes the cell
     */
    virtual ~Cell () {}

    friend class sys::FileImplementation;
    friend class sys::Plain;
    friend class sys::CBNT;

    /**
     * Comparison operator
     * @param other Cell to compare with this
     */
    bool operator== (const roiformat::Cell& other) const;

    /**
     * Association operator
     * @param other Reference cell. this wll be equal to other after this method executes.
     */
    Cell &operator= (const Cell& other)
    {
      m_samp = other.m_samp;
      m_eta = other.m_eta;
      m_phi = other.m_phi;
      m_r = other.m_r;
      m_deta = other.m_deta;
      m_dphi = other.m_dphi;
      m_dr = other.m_dr;
      m_energy = other.m_energy;
      return *this;
    }
    
    /**
     * Accessors
     */
    inline Sampling sampling(void) const { return m_samp; }
    inline const double& eta(void) const { return m_eta; }
    inline const double& phi(void) const { return m_phi; }
    inline const double& r(void) const { return m_r; }
    inline const double& deta(void) const { return m_deta; }
    inline const double& dphi(void) const { return m_dphi; }
    inline const double& dr(void) const { return m_dr; }
    inline const double& energy(void) const { return m_energy; } 

    /**
     * Streams in data into a cell.
     *
     * @param f The roiformat::File to take the data from.
     */
    sys::File& operator<< (sys::File& f);

  private: //representation

    Sampling m_samp; ///< This cell's sampling
    double m_eta; ///< The center of the cell, in eta
    double m_phi; ///< The center of the cell, in phi
    double m_r; ///< The center of the cell with respect to the central axis.
    double m_deta; ///< The size of the cell, in eta
    double m_dphi; ///< The size of the cell, in phi
    double m_dr; ///< The size of the cell, in radius direction
    double m_energy; ///< This cell's energy
    char m_sep; ///< The separator used when dumping
  };

  /**
   * Converts a string into a Cell::Sampling
   *
   * @param s string to convert
   */
  Cell::Sampling str2sampling (const std::string& s);
  
  /**
   * Converts a a Cell::Sampling into string
   *
   * @param s the sampling to convert
   */
  std::string sampling2str (const Cell::Sampling& s);

  /**
   * Returns the eta and phi of the cell with most energy deposition.
   * 
   * @param vcell A vector of constant cells
   * @param eta The eta value to be returned
   * @param phi The phi value to be returned
   */
  void max (const std::vector<const Cell*>& vcell, double& eta, double& phi);

  /**
   * Returns the eta and phi of the cell with highest energy deposition, but
   * which also falls into the region centered around the reference eta and
   * phi values given, as large as defined by the window size.
   * 
   * @param vcell A vector of constant cells
   * @param eta The eta value to be returned
   * @param phi The phi value to be returned
   * @param eta_ref The center of the reference window
   * @param phi_ref The center of the reference window
   * @param eta_window The width of the window in eta direction
   * @param phi_window The width of the window in phi direction
   */
  void max (const std::vector<const roiformat::Cell*>& vcell, 
	    double& eta, double& phi, const double& eta_ref, 
	    const double& phi_ref, const double& eta_window, 
	    const double& phi_window);

}

/** 
 * How to write my own data to a std output stream
 *
 * @param os The output stream where I should send my data to
 * @param c The cell where to take the data from
 */
std::ostream& operator<< (std::ostream& os, const roiformat::Cell& c);

#endif /* RINGER_ROIFORMAT_CELL_H */
