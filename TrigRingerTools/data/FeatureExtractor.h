//Dear emacs, this is -*- c++ -*-

/**
 * @file data/FeatureExtractor.h
 *
 * @brief Defines an operator that calculates quantities from Patterns, in
 * much the same way as PatternOperator's do. The only difference being that
 * operator() returns a single value here.
 */

#ifndef DATA_FEATUREEXTRACTOR_H
#define DATA_FEATUREEXTRACTOR_H

#include "TrigRingerTools/data/Pattern.h"

namespace data {

  class FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~FeatureExtractor() {}

    /**
     * Defines the only operation on a Pattern.
     *
     * @param in The Pattern to transform using this operator
     */
    virtual data::Feature operator() (const data::Pattern& in) const =0;

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

#endif /* DATA_FEATUREEXTRACTOR_H */
