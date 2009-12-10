//Dear emacs, this is -*- c++ -*-

/**
 * @file data/RoIPatternSet.h
 *
 * @brief Describes a pattern set that is composed of RoI's and describe their
 * properties. This is a direct descendent of the PatternSet class and reads
 * extra information not available directly in a PatternSet. The intention is
 * use this kind of objects when there needs to be a strong relation between
 * neural network inputs, outputs and meta-information beloging to an event.
 */

#ifndef DATA_ROIPATTERNSET_H
#define DATA_ROIPATTERNSET_H

#include "TrigRingerTools/data/PatternSet.h"
#include "TrigRingerTools/data/SimplePatternSet.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/Plain.h"

namespace data {

  /**
   * This class represents a PatternSet that contains, for every Pattern, an
   * extra set of attributes that describe their RoI properties.
   *
   * @see Pattern
   * @see PatternSet
   */
  class RoIPatternSet : public data::PatternSet {

  public: //typedef

    typedef struct RoIAttribute {
      unsigned int lvl1_id; ///< The LVL1 Trigger global event identifier
      unsigned int roi_id; ///< The LVL1 Trigger RoI identifier at this event
      double eta; ///< The center of the RoI in eta
      double phi; ///< The center of the RoI in phi
    } RoIAttribute;

  public: //construction and destruction

    /**
     * Creates an empty RoIPatternSet with given sizes and an optional
     * initialization value. The attribute fields of every Pattern are filled
     * by the default constructors.
     *
     * @param size The number of Patterns inside of the RoIPatternSet.
     * @param p_size The size of each Pattern inside the RoIPatternSet.
     * @param init An optional initialization value
     */
    RoIPatternSet(const size_t& size, const size_t& p_size,
		  const double& init=0);

    /** 
     * Creates an RoIPatternSet from another RoIPatternSet. This is the copy
     * constructor.
     *
     * @param other The RoIPatternSet to be cloned.
     */
    RoIPatternSet(const RoIPatternSet& other);

    /** 
     * Reads an RoIPatternSet from an XML file node
     *
     * @param node The root node where I'm in
     */
    RoIPatternSet(sys::xml_ptr_const node);

    /** 
     * Reads an RoIPatternSet from an ROOT file
     *
     * @param infoBranches  All information needed to access the ROOT file information.
     *                      See PatternSet.h
     */
    RoIPatternSet(const RootClassInfo &infoBranches);
      
    /**
     * Creates an RoIPatternSet from another RoIPatternSet, by selecting a set
     * of RoI patterns of interest.
     *
     * @param other The RoIPatternSet to copy data from
     * @param pats The set of patterns to take from the original set.
     */
    RoIPatternSet(const RoIPatternSet& other,
		  const std::vector<size_t>& pats);

    /**
     * Defines comparision variables
     */
    typedef enum variable_t {
      ETA = 0, 
      PHI = 1
    } variable_t;

    /**
     * Creates an RoIPatternSet from a set of Patterns, taking into
     * consideration the area delimited by the given parameters. The interval
     * taken will be [start, end)
     *
     * @param other The PatternSet to copy the Patterns from
     * @param start The starting point of the interval
     * @param end The ending point of the interval
     * @param var The variable type to be used for the comparison. The default
     * comparision variable is `eta'.
     */
    RoIPatternSet(const RoIPatternSet& other, double start, double end,
		  variable_t var=ETA);

    /**
     * Creates an RoIPatternSet from a set of Patterns, litterally
     *
     * @param pats The set of patterns to use for building this set.
     * @param attr The attributes that go with each of the patterns above
     */
    RoIPatternSet(const std::vector<Pattern*>& pats,
		  const std::vector<RoIAttribute>& attr);

    /**
     * Creates an RoIPatternSet from a SimplePatternSet
     *
     * @param pset The set to copy data from
     * @param attr The attributes that go with each of the patterns above
     */
    RoIPatternSet(const data::SimplePatternSet& pset,
		  const std::vector<RoIAttribute>& attr);

    /**
     * The default destructor.
     */
    virtual ~RoIPatternSet();

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
     * Makes a copy of this PatternSet in dynamic memory
     */
    virtual PatternSet* clone (void) const;

    /**
     * Makes a selective copy of this PatternSet in dynamic memory
     *
     * @param pats The set of patterns to take from the original set.
     */
    virtual PatternSet* clone (const std::vector<size_t>& pats) const;

    /**
     * Shuffles the order of data inside this PatternSet.
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
     * Dumps the set to a ROOT file.
     *
     * @param info Information about the ROOT file.
     */
    virtual void dump (const RootClassInfo &info) const;

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

  public: //class-based methods

    /**
     * This method returns a constant reference of the RoIAttribute required,
     * checking the range of the set before returning the required set.  It's
     * an error to address a non-existing position inside the set.
     * 
     * @param pos The relative position inside the set, starting from
     * <code>0</code>
     */
    const RoIAttribute& attribute (const size_t& pos) const;

    /**
     * This method returns a constant reference of all attributes inside
     */
    inline const std::vector<RoIAttribute>& attributes () const 
    { return m_attr; }

    /**
     * This method sets a specific RoIAttribute inside the set to a new value,
     * also given as parameter. The Pattern range is checked.
     *
     * @param pos The relative position inside the set, starting from
     * <code>0</code>.
     * @param attr The new attributes of Pattern `pos'
     */
    void set_attribute (const size_t& pos, const RoIAttribute& attr);

    /**
     * This method sets a all RoIAttribute's inside the set to a new value,
     * also given as parameter. The Pattern range is checked.
     *
     * @param attr The new attributes for all Pattern's
     */
    void set_attribute (const std::vector<RoIAttribute>& attr);

    /**
     * Sets both the features and attributes of a Pattern.
     *
     * @param pos The relative position of the Pattern inside this ensemble
     * @param pat The new value of the Pattern to be set. This will be range
     * checked for compatibility with the current PatternSet.
     * @param attr The new attributes of this Pattern.
     */
    void set_pattern (const size_t& pos, const data::Pattern& pat, 
		      const RoIAttribute& attr);

    /**
     * Merges two distinct RoIPatternSet's.
     *
     * This method will copy the given RoIPatternSet Pattern's and attributes
     * into the current set, enlarging it. We check if the Pattern sizes are
     * the same previous to the copying. This method returns a reference to
     * the current set being manipulated.
     *
     * @param other The RoIPatternSet to be copied
     */
    RoIPatternSet& merge (const RoIPatternSet& other);

    /**
     * Sets this RoIPatternSet starting from another RoIPatternSet, 
     * by selecting a set of patterns of interest.
     *
     * @param other The RoIPatternSet to copy data from
     * @param pats The set of patterns to take from the original set.
     */
    RoIPatternSet& assign(const RoIPatternSet& other,
			  const std::vector<size_t>& pats);
	
    /**
     * This method defines how to copy an RoIPatternSet.
     *
     * @param other The RoIPatternSet to be copied.
     */
    RoIPatternSet& operator= (const RoIPatternSet& other);

    /**
     * Subtracts, from this RoIPatternSet, the value given
     *
     * @param other The RoIPatternSet to be used in the operation.
     */
    inline RoIPatternSet& operator-= (const RoIPatternSet& other)
    { m_set -= other.m_set; return *this; }

    /**
     * Gets a handle to the SimplePatternSet version, without the attributes
     * for this PatternSet.
     */
    inline const data::SimplePatternSet& simple (void) const
    { return m_set; }

  private: //representation
    data::SimplePatternSet m_set; ///< My pattern related data
    std::vector<RoIAttribute> m_attr; ///< My attributes

  };

}

/**
 * Dumps a data::RoIPatternSet::RoIAttribute with a nice representation on a
 * ostream
 *
 * @param os The output stream to use
 * @param attr The attributes to dump
 */
std::ostream& operator<< (std::ostream& os, 
			  const data::RoIPatternSet::RoIAttribute& attr);

/**
 * Dumps a data::RoIPatternSet::RoIAttribute with a nice representation on a
 * sys::Plain
 *
 * @param os The system file
 * @param attr The attributes to dump
 */
sys::Plain& operator<< (sys::Plain& os, 
		       const data::RoIPatternSet::RoIAttribute& attr);

#endif /* DATA_ROIPATTERNSET_H */
