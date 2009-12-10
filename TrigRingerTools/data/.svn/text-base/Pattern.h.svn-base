//Dear emacs, this is -*- c++ -*-

/**
 * @file data/Pattern.h 
 *
 * @brief This file contains the definition of what is a data::Pattern
 */

#ifndef DATA_PATTERN_H
#define DATA_PATTERN_H

#include "TrigRingerTools/data/Feature.h"
#include <vector>
#include <gsl/gsl_vector.h>
#include <iostream>
#include "TrigRingerTools/sys/File.h"
#include "TrigRingerTools/sys/Plain.h"

/**
 * Contains declarations of Data structures.
 *
 * The Data namespace will gather all data types into a single
 * functionality group. If you plan to make new data types, include
 * them at this name space for the sake of organisation.
 */
namespace data {

  class SimplePatternSet; ///< forward
  class PatternOperator; ///< forward
  class FeatureExtractor; ///< forward
  
  /**
   * The Pattern is defined in terms of Feature's.
   *
   * In Neural Network's jargon, the <b>pattern</b> is an entity that contains
   * a set of features that represent an event. Being the <b>pattern</b> just
   * an ensemble, its C++ representation cannot be far from a simple
   * vector. The required functionality is pretty much the same, although its
   * implementation relies on GSL matrix views of a data set.
   *
   * If a Pattern is created from a set of Feature's, a new block of data is
   * allocated.
   *
   * @see Feature
   * @see PatternSet
   */
  class Pattern {
    
  public:
    
    /**
     * Contructs a Pattern with all the same entries.
     *
     * @param s The number of Feature's in the Pattern
     * @param v The value of each Feature in the Pattern
     */
    Pattern (const size_t& s, const Feature v=0);

    /**
     * Constructs a Pattern from a vector of Feature's.
     *
     * This constructor defines how a Pattern should be created. A
     * number of Feature's should be given. After the Pattern is
     * created, it cannot be resized, so watch out!
     *
     * @param feat The input features.  
     */
    Pattern(const std::vector<Feature>& feat);

    /**
     * Constructs a Pattern from other Pattern (copy).
     *
     * @param other The other Pattern.
     */
    Pattern(const Pattern& other);

    /**
     * Constructs a Pattern from an already existent memory block (no data copy is done). This constructor
     * will just point to teh memory block passed.
     *
     * @param other The other Pattern. This other patters remains responsible for freeing any dynamically
     * allocated memory block.
     * @param size The size of the pattern.
     */
//    Pattern(Feature *other, size_t featSize);


    /**
     * This is just to make the destructor virtual
     */
    virtual ~Pattern();

    /**
     * Apply the given function to the current Pattern.
     *
     * Applies the given function, conforming to the interface
     * description, to the Pattern. The function is applied to each
     * member of the Pattern. This function returns a reference to
     * self.
     *
     * @param fct The function to apply
     */
    Pattern& apply (Feature (*fct)(Feature));

    /**
     * Returns the pattern size.
     */
    size_t size () const;

    /**
     * Copies the Pattern. 
     *
     * Implementation returns reference to *this, so be calm... This
     * copying resizes the internal implementation if necessary.
     *
     * @param other The Pattern to be copied.  
     */
    /*virtual*/ Pattern& operator= (const Pattern& other);

    /**
     * Assigns a constant value to each of the Pattern Feature's. 
     *
     * @param value The value to be copied to each Feature
     */
    Pattern& operator= (const Feature& value);

    /**
     * Adds the parameter value from all Feature's.
     * @param val The value to be added from the Feature's
     */
    Pattern& operator+= (const Feature& val);

    /**
     * Removes the parameter value from all Feature's.
     * @param val The value to be removed from the Feature's
     */
    Pattern& operator-= (const Feature& val);

    /**
     * Multiplies the parameter value to all Feature's.
     * @param val The value to be multiplied to the Feature's
     */
    Pattern& operator*= (const Feature& val);

    /**
     * Divides all Feature's by the parameter value.
     * @param val The value to divide the Feature's
     */
    Pattern& operator/= (const Feature& val);

    /**
     * Sums and assigns all Feature's from the other Pattern to this.
     *
     * It's an error to sum and assign Pattern's with different
     * sizes, but that's not inforced within the current
     * implementation.
     *
     * @param other The operand
     */
    Pattern& operator+= (const Pattern& other);

    /**
     * Subtracts and assigns all Feature's from the other Pattern to
     * this.  
     *
     * It's an error to subtract and assign Pattern's with different
     * sizes, but that's not inforced within the current
     * implementation.
     *
     * @param other The operand
     */
    Pattern& operator-= (const Pattern& other);

    /**
     * Multiplies and assigns all Feature's from the other Pattern to
     * this. 
     *
     * It's an error to multiply and assign Pattern's with different
     * sizes, but that's not inforced within the current
     * implementation.
     *
     * @param other The operand
     */
    Pattern& operator*= (const Pattern& other);

    /**
     * Divides and assigns all Feature's from the other Pattern to
     * this. 
     *
     * It's an error to divide and assign Pattern's with different
     * sizes, but that's not inforced within the current
     * implementation.
     *
     * @param other The operand
     */
    Pattern& operator/= (const Pattern& other);

    /**
     * Adds the data::Feature's of two Pattern's.
     *
     * Implementation returns reference a new Pattern that is the sum
     * of the first and the second data::Feature's. It's an error
     * trying to sum Pattern's with different sizes. The behaviour for
     * the moment is undefined.
     *
     * @param first The other operand
     * @param second The second operand
     */
    friend Pattern operator+ (const Pattern& first, const Pattern& second)
    {
      data::Pattern tmp(first);
      return tmp += second;
    }

    /**
     * Subtracts the data::Feature's of two Pattern's. 
     *
     * Implementation returns reference a new Pattern that is the
     * subtraction of these by the other data::Feature's. It's an
     * error trying to subtract Pattern's with different sizes. The
     * behaviour for the moment is undefined. The evaluation goes like
     * this:
     *
     * @f[ 
     * result = *this - other
     * @f]
     *
     * @param first The first operand
     * @param second The second operand
     */
    friend Pattern operator- (const Pattern& first, const Pattern& second)
    {
      data::Pattern tmp(first);
      return tmp -= second;
    }

    /**
     * Multiplies the data::Feature's of two Pattern's.
     *
     * Implementation returns reference a new Pattern that is the
     * multiplication of the input Patterns data::Feture's. It's an
     * error trying to multiply Pattern's with different sizes. The
     * behaviour for the moment is undefined.
     *
     * @param first The first operand
     * @param second The second operand
     */
    friend Pattern operator* (const Pattern& first, const Pattern& second)
    {
      data::Pattern tmp(first);
      return tmp *= second;
    }

    /**
     * Divides the data::Feature's of two Pattern's.
     *
     * Implementation returns reference a new Pattern that is the
     * division of the first by the second Pattern
     * data::Feature's. It's an error trying to sum Pattern's with
     * different sizes. The behaviour for the moment is undefined. The
     * evaluation goes like this:
     *
     * @f[
     * result = \frac{*this}{other}
     * @f]
     *
     * @param first The first operand
     * @param second The second operand
     */
    friend Pattern operator/ (const Pattern& first, const Pattern& second)
    {
      data::Pattern tmp(first);
      return tmp /= second;
    }

    /**
     * PatternOperator is a friend to allow computation optimisations
     */
    friend class PatternOperator;

    /**
     * FeatureExtractor is a friend to allow computation optimisations
     */
    friend class FeatureExtractor;

    /**
     * Returns a reference to a given the Feature. 
     *
     * This operator is used when we want to change the object, so,
     * the LHS (Left Hand Side) of a operation.
     *
     * @param pos The Feature's relative position inside the Pattern.
     */
    Feature& operator[] (const size_t& pos);

    /**
     * Returns a const <b>reference</b> of a given Feature.
     *
     * This operator is used when we want <b>not</b> to change the
     * object, so, the RHS (Right Hand Side) of a operation.
     *
     * @param pos The Feature's relative position inside the Pattern.
     */
    const Feature& operator[] (const size_t& pos) const;

    /**
     * Appends the contents of the given Pattern to myself
     *
     * @param other The other Pattern to append to myself
     */
    void append (const Pattern& other);

  protected: //only a PatternSet and inherited classes can use this

    /**
     * Builds a Pattern out of a vector view. This is used to refer, for
     * instance to a matrix or a PatternSet.
     *
     * @warning This is not very clean and should be removed as soon as
     * another idea pops up or upon a redesign
     *
     * @param vector_view The vector view to use as construction parameter
     */
    Pattern (gsl_vector_view vector_view);

    /**
     * My private friend
     */
    friend class data::SimplePatternSet;

  private:
    gsl_vector_view m_view; ///< an optional view that might be set
    gsl_vector* m_vector; ///< the vector component of the "view"
  	gsl_vector m_static_vector; ///< the vector that will be used in case a block of already allocated memory is passed.
  };

}

/**
 * How to dump a pattern to a std::ostream, very useful for debugging.
 *
 * @param os The output stream to be used
 * @param p The pattern you want dumped
 */
std::ostream& operator<< (std::ostream& os, const data::Pattern& p);

/**
 * How to dump a pattern to a std::ostream, very useful for debugging.
 *
 * @param f The output file to be used
 * @param p The pattern you want dumped
 */
sys::Plain& operator<< (sys::Plain& f, const data::Pattern& p);

#endif //DATA_PATTERN_H

