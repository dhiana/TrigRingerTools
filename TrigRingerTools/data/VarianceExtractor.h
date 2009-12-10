//Dear emacs, this is -*- c++ -*-

/**
 * @file data/VarianceExtractor.h
 *
 * @brief Calculates the variance of a Pattern.
 */

#ifndef DATA_VARIANCEEXTRACTOR_H
#define DATA_VARIANCEEXTRACTOR_H

#include "TrigRingerTools/data/FeatureExtractor.h"

namespace data {

  class VarianceExtractor : public FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~VarianceExtractor() {}

    /**
     * Evaluates the unbiased Pattern variance.
     *
     * The Pattern variance is evaluated with a call to this
     * method. The variance method considered here is t he "best
     * unbiased estimate":
     * @f[
     * \sigma^{2} = \frac{1}{N-1} \sum_{i=1}^{N} (x_i-\mu_x)^2
     * @f]
     * where:
     * - @f$ \mu_x @f$ is the mean as described in mean();
     * - @f$N @f$ is the number of samples in the set and 
     * - @f$x_i@f$ is the value of each individual sample.
     *
     * @param in The Pattern from which evaluate the variance
     */
    virtual data::Feature operator() (const data::Pattern& in) const;

  };

}

#endif /* DATA_VARIANCEEXTRACTOR_H */
