//Dear emacs, this is -*- c++ -*-

/**
 * @file data/PatternSet.h
 *
 * @brief Declares the PatternSet interface.
 */

#ifndef DATA_PATTERNSET_H
#define DATA_PATTERNSET_H

#include <iostream>
#include <string>
#include "TrigRingerTools/data/Pattern.h"
#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/Plain.h"

namespace data {

  class RootClassInfo {
    public: // Temporary!
    std::string FileName;
    std::string TreeName;

    RootClassInfo(std::string fn = "file.root",   std::string tn = "CollectionTree")
    : FileName(fn),
      TreeName(tn)
    {
    }
    
  };

  /**
   * This class represents a set of data::Pattern's.
   *
   * A <b>data::Pattern set</b> is an entity that holds, virtually, any number
   * of data::Pattern's and attributes. The set can be queried for special
   * methods, allows random access with a reasonable speed and data::Pattern's
   * to be inserted and removed from it. Some sort of normalisation strategies
   * are also possible. A PatternSet can also be used to produce smaller
   * <b>sets</b> that match a certain criteria. This class and related methods
   * are strongly based on the GNU Scientific Library (GSL)
   * vector/matrix/block objects. Please, refer to the GSL manual to
   * understand better its limitations and virtudes (try at your shell
   * <code>info gsl</code> or <code>info gsl-ref</code>).
   *
   * While data::Pattern's represent each event available in a PatternSet, a
   * data::Ensemble represents a given Feature for every event in a
   * PatternSet. Through this abstraction, it is possible to also manipulate a
   * PatternSet with respect to its "columns", erasing or setting them.
   *
   * All types developed to be used in conjunction with this class may be
   * derived types through the use of GSL's views of blocks, matrices and
   * vectors.
   */
  class PatternSet {

  public: //interface

    /**
     * Default constructor
     */
    PatternSet() { }

    /**
     * Destructor virtualization
     */
    virtual ~PatternSet () {}

    /**
     * This method sets a specific data::Pattern inside the set to a new
     * value, also given as parameter. The new data::Pattern is checked for
     * size compatibility with the old one. If both have the same size, the
     * value is assigned, otherwise, the operation is <b>not</b>
     * performed. The range is also checked.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>.
     * @param pat The new value of the data::Pattern
     */
    virtual void set_pattern (const size_t& pos, const Pattern& pat) = 0;

    /**
     * This method sets a specific data::Ensemble inside the set to a new
     * value, also given as parameter. The new data::Ensemble is checked for
     * size compatibility with the old one. If both have the same size, the
     * value is assigned, otherwise, the operation is <b>not</b>
     * performed. The range is also checked.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>.
     * @param ens The new value of the data::Ensemble
     */
    virtual void set_ensemble (const size_t& pos, const Ensemble& ens) = 0;

    /**
     * This method deletes a data::Pattern from the set. It's an error to call
     * this method with a data::Pattern number that doesn't exist. It takes
     * some computation time to delete() a Pattern from a set, so be sure
     * before doing it.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    virtual void erase_pattern (const size_t& pos) = 0;

    /**
     * This method deletes a data::Ensemble from the set. It's an error to
     * call this method with a data::Ensemble number that doesn't exist. It
     * takes some computation time to delete() an Ensemble from a set, so be
     * sure before doing it.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    virtual void erase_ensemble (const size_t& pos) = 0;

    /**
     * This method returns the set size, i.e., the number of data::Pattern's
     * it contains.
     */
    virtual size_t size () const = 0;

    /**
     * Returns the current size of each Pattern on the set.
     */
    virtual size_t pattern_size () const = 0;

    /**
     * Makes a copy of this PatternSet in dynamic memory
     */
    virtual PatternSet* clone (void) const = 0;

    /**
     * Makes a selective copy of this PatternSet in dynamic memory
     *
     * @param pats The set of patterns to take from the original set.
     */
    virtual PatternSet* clone (const std::vector<size_t>& pats) const = 0;

    /**
     * Shuffles the order of data inside this PatternSet.
     */
    virtual void shuffle (void) = 0;

    /**
     * This method returns a constant reference of the data::Pattern required,
     * checking the range of the set before returning, by value, the required
     * data::Pattern. It's an error to address a non-existing position inside
     * the set. The data::Pattern is <b>not</b> created by data copying.
     *     
     * @param pos The relative position inside the set, starting from 
     * <code>0</code>
     */
    virtual const Pattern pattern (const size_t& pos) const = 0;

    /**
     * This method returns a constant reference of the data::Ensemble
     * required, checking the range of the set before returning, by value, the
     * required data::Ensemble. It's an error to address a non-existing
     * position inside the set. The data::Ensemble is <b>not</b> created by
     * data copying.
     *     
     * @param pos The relative position inside the set, starting from 
     * <code>0</code>
     */
    virtual const Ensemble ensemble (const size_t& pos) const = 0;

    /**
     * Dumps the set as a set of XML nodes
     *
     * @param any Any node in the XML tree.
     * @param cname The class name to use when dumping
     * @param start_id The initial number to take in consideration when
     * writing the entry identifiers.
     */
    virtual sys::xml_ptr dump (sys::xml_ptr any,
			       const std::string& cname,
			       const size_t start_id=0) const = 0;

    /**
     * Dumps the set to a ROOT file
     *
     * @param any Any node in the XML tree.
     * @param cname The class name to use when dumping
     * @param start_id The initial number to take in consideration when
     * writing the entry identifiers.
     */
    virtual void dump (const RootClassInfo &info) const = 0;

    /**
     * Applies the given PatternOperator to all my Pattern's.
     * 
     * @param op The operator to apply
     */
    virtual void apply_pattern_op (const data::PatternOperator& op) = 0;

    /**
     * Applies the given PatternOperator to all my Ensemble's.
     * 
     * @param op The operator to apply
     */
    virtual void apply_ensemble_op (const data::PatternOperator& op) = 0;

    /**
     * Dumps a pattern set with a nice representation on a ostream
     *
     * @param os The output stream to use
     */
    virtual std::ostream& stream_out (std::ostream& os) const = 0;
	
    /**
     * Dumps a pattern set with a nice representation on a ostream
     *
     * @param f The output stream to use
     */
    virtual sys::Plain& stream_out (sys::Plain& f) const = 0;

  };
  
}

/**
 * Dumps a pattern set with a nice representation on a ostream
 *
 * @param os The output stream to use
 * @param p The set to dump
 */
inline std::ostream& operator<< (std::ostream& os, const data::PatternSet& p)
{ return p.stream_out(os); }

/**
 * Dumps a pattern set with a nice representation on a ostream
 *
 * @param f The output sys::File to use
 * @param p The set to dump
 */

inline sys::Plain& operator<< (sys::Plain& f, const data::PatternSet& p)
{ return p.stream_out(f); }

#endif //DATA_PATTERNSET_H
