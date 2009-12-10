//Dear emacs, this is -*- c++ -*-

/**
 * @file data/MeanExtractor.h
 *
 * @brief This FeatureExtractor calculates the mean of a Pattern.
 */

#ifndef DATA_MEANEXTRACTOR_H
#define DATA_MEANEXTRACTOR_H

#include "TrigRingerTools/data/FeatureExtractor.h"

namespace data {

  class MeanExtractor : public FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~MeanExtractor() {}

    /**
     * Evaluates the Pattern mean.
     *
     * The mean of a Pattern is the arithmetic mean taken for all
     * values inside the set.
     *
     * @f[
     * \mu_x = \frac{1}{N} \sum_{i=1}^{N} x_i
     * @f]
     * Where:
     * - @f$N @f$ is the number of samples in the set and 
     * - @f$x_i@f$ is the value of each individual sample.
     *
     * @param in The Pattern from which evaluate the mean
     */
    virtual data::Feature operator() (const data::Pattern& in) const;

  };

}

#endif /* DATA_MEANEXTRACTOR_H */
