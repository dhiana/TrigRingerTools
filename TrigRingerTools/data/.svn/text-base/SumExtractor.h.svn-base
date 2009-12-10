//Dear emacs, this is -*- c++ -*-

/**
 * @file data/SumExtractor.h
 *
 * @brief This FeatureExtractor calculates the sum of a Pattern.
 */

#ifndef DATA_SUMEXTRACTOR_H
#define DATA_SUMEXTRACTOR_H

#include "TrigRingerTools/data/FeatureExtractor.h"

namespace data {

  class SumExtractor : public FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~SumExtractor() {}

    /**
     * Evaluates the Pattern sum.
     *
     * The sum of a Pattern is the arithmetic sum taken for all
     * values inside the set.
     *
     * @f[
     * s_x = \sum_{i=1}^{N} x_i
     * @f]
     * Where:
     * - @f$N @f$ is the number of samples in the set and 
     * - @f$x_i@f$ is the value of each individual sample.
     *
     * @param in The Pattern from which evaluate the sum
     */
    virtual data::Feature operator() (const data::Pattern& in) const;

  };

}

#endif /* DATA_SUMEXTRACTOR_H */
