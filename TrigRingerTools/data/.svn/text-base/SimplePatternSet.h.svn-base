//Dear emacs, this is -*- c++ -*-

/**
 * @file data/SimplePatternSet.h
 *
 * @brief Declares the SimplePatternSet class.
 */

#ifndef DATA_SIMPLEPATTERNSET_H
#define DATA_SIMPLEPATTERNSET_H

#include <gsl/gsl_matrix.h>
#include <iostream>
#include "TrigRingerTools/data/Pattern.h"
#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/data/PatternSet.h"
#include "TrigRingerTools/sys/Plain.h"

namespace data {

  /**
   * This class represents a set of data::Pattern's.
   *
   * A <b>data::Pattern set</b> is an entity that holds, virtually, any number
   * of data::Pattern's. The set can be queried for special methods, allows
   * random access with a reasonable speed and data::Pattern's to be inserted
   * and removed from it. Some sort of normalisation strategies are also
   * possible. A SimplePatternSet can also be used to produce smaller
   * <b>sets</b> that match a certain criteria. This class and related methods
   * are strongly based on the GNU Scientific Library (GSL)
   * vector/matrix/block objects. Please, refer to the GSL manual to
   * understand better its limitations and virtudes (try at your shell
   * <code>info gsl</code> or <code>info gsl-ref</code>).
   *
   * While data::Pattern's represent each event available in a
   * SimplePatternSet, a data::Ensemble represents a given Feature for every
   * event in a SimplePatternSet. Through this abstraction, it is possible to
   * also manipulate a SimplePatternSet with respect to its "columns", erasing
   * or setting them.
   *
   * All types developed to be used in conjunction with this class may be
   * derived types through the use of GSL's views of blocks, matrices and
   * vectors.
   */
  class SimplePatternSet : public PatternSet {

  public: //construction and destruction

    /**
     * Creates an empty SimplePatternSet with given sizes and an optional
     * initialization value.
     *
     * @param size The number of Patterns inside of the SimplePatternSet.
     * @param p_size The size of each Pattern inside the SimplePatternSet.
     * @param init An optional initialization value
     */
    SimplePatternSet(const size_t& size, const size_t& p_size, 
		     const double& init=0);

    /** 
     * Creates a SimplePatternSet from another SimplePatternSet. This is the
     * copy constructor.
     *
     * @param other The SimplePatternSet to be cloned.
     */
    SimplePatternSet(const SimplePatternSet& other);

    /** 
     * Reads a SimplePatternSet from an XML file node
     *
     * @param node The root node where I'm in
     */
    SimplePatternSet(sys::xml_ptr_const node);

    /**
     * Creates a SimplePatternSet from another SimplePatternSet, by selecting
     * a set of patterns of interest.
     *
     * @param other The SimplePatternSet to copy data from
     * @param pats The set of patterns to take from the original set.
     */
    SimplePatternSet(const SimplePatternSet& other,
		     const std::vector<size_t>& pats);

    /**
     * Creates a SimplePatternSet from a set of Patterns, litterally
     *
     * @param pats The set of patterns to use for building this set.
     */
    SimplePatternSet(const std::vector<Pattern*>& pats);

    /**
     * The default destructor.
     */
    virtual ~SimplePatternSet();

  public: //virtual interfaces

    /**
     * This method returns the set size, i.e., the number of data::Pattern's
     * it contains.
     */
    virtual size_t size () const;

    /**
     * Returns the current size of each Pattern on the set.
     */
    virtual size_t pattern_size () const;

    /**
     * This method returns a constant reference of the data::Pattern required,
     * checking the range of the set before returning, by value, the required
     * data::Pattern. It's an error to address a non-existing position inside
     * the set. The data::Pattern is <b>not</b> created by data copying.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    virtual const Pattern pattern (const size_t& pos) const;

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
    virtual const Ensemble ensemble (const size_t& pos) const;

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
    virtual void set_pattern (const size_t& pos, const Pattern& pat);

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
    virtual void set_ensemble (const size_t& pos, const Ensemble& ens);

    /**
     * This method deletes a data::Pattern from the set. It's an error to call
     * this method with a data::Pattern number that doesn't exist. It takes
     * some computation time to delete() a Pattern from a set, so be sure
     * before doing it.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    virtual void erase_pattern (const size_t& pos);

    /**
     * This method deletes a data::Ensemble from the set. It's an error to
     * call this method with a data::Ensemble number that doesn't exist. It
     * takes some computation time to delete() an Ensemble from a set, so be
     * sure before doing it.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    virtual void erase_ensemble (const size_t& pos);

    /**
     * Makes a copy of this SimplePatternSet in dynamic memory
     */
    virtual PatternSet* clone (void) const;

    /**
     * Makes a selective copy of this SimplePatternSet in dynamic memory
     *
     * @param pats The set of patterns to take from the original set.
     */
    virtual PatternSet* clone (const std::vector<size_t>& pats) const;

    /**
     * Shuffles the order of data inside this SimplePatternSet.
     */
    virtual void shuffle (void);

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
			       const size_t start_id=0) const;

    /**
     * Dump the set to a ROOT file
     *
     * @param info Structure that holds ROOT file information.
     */
    virtual void dump (const RootClassInfo &/*info*/) const { }

    /**
     * Applies the given PatternOperator to all my Pattern's.
     *
     * @param op The operator to apply
     */
    virtual void apply_pattern_op (const data::PatternOperator& op);

    /**
     * Applies the given PatternOperator to all my Ensemble's.
     * 
     * @param op The operator to apply
     */
    virtual void apply_ensemble_op (const data::PatternOperator& op);

    /**
     * Dumps a pattern set with a nice representation on a ostream
     *
     * @param os The output stream to use
     */
    virtual std::ostream& stream_out (std::ostream& os) const;
	
    /**
     * Dumps a pattern set with a nice representation on a ostream
     *
     * @param f The output stream to use
     */
    virtual sys::Plain& stream_out (sys::Plain& f) const;

  public: //particular class interfaces

    /**
     * Merges two distinct SimplePatternSet's.
     *
     * This method will copy the given SimplePatternSet Pattern's into the
     * current set, enlarging it. We check if the Pattern sizes are the same
     * previous to the copying. This method returns a reference to the current
     * set being manipulated.
     *
     * @param other The SimplePatternSet to be copied
     */
    SimplePatternSet& merge (const SimplePatternSet& other);

    /**
     * Sets this SimplePatternSet starting from another SimplePatternSet, by
     * selecting a set of patterns of interest.
     *
     * @param other The SimplePatternSet to copy data from 
     * @param pats The set of patterns to take from the original set.
     */
    SimplePatternSet& assign(const SimplePatternSet& other,
			     const std::vector<size_t>& pats);

    /**
     * This method defines how to copy a SimplePatternSet.
     *
     * @param other The SimplePatternSet to be copied.
     */
    SimplePatternSet& operator= (const SimplePatternSet& other);

    /**
     * Subtracts, from this SimplePatternSet, the value given
     *
     * @param other The SimplePatternSet to be used in the operation.
     */
    SimplePatternSet& operator-= (const SimplePatternSet& other);

  private: //representation
    gsl_matrix* m_data; ///< my internal data
    
  };
  
}

#endif //DATA_SIMPLEPATTERNSET_H
