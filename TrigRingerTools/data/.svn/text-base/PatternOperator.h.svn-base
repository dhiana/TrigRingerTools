//Dear emacs, this is -*- c++ -*-

/**
 * @file data/PatternOperator.h
 *
 * @brief Virtual base class that rules Pattern transformations.
 */

#ifndef DATA_PATTERNOPERATOR_H
#define DATA_PATTERNOPERATOR_H

#include "TrigRingerTools/data/Pattern.h"
#include <gsl/gsl_vector.h>

namespace data {

  /**
   * This will transform the input Pattern into a new output Pattern. The
   * result of this transformation is <b>not</b> necesarily bi-directional.
   */
  class PatternOperator {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~PatternOperator() {}

    /**
     * Defines the only operation on a Pattern.
     *
     * @param in The Pattern to transform using this operator
     * @param out The Pattern to output
     */
    virtual void operator() (const data::Pattern& in, 
			     data::Pattern& out) const =0;

  protected:
    /**
     * Allows children to benefit from member access inside the Pattern.
     *
     * @param p The vector to abuse
     * @return The internal representation of the Pattern
     */
    inline const gsl_vector* abuse(const data::Pattern& p) const 
    { return p.m_vector; }

  };

}

#endif /* DATA_PATTERNOPERATOR_H */
