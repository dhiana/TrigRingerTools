//Dear emacs, this is -*- c++ -*-

/**
 * @file data/MaxExtractor.h
 *
 * @brief This FeatureExtractor calculates the max of a Pattern.
 */

#ifndef DATA_MAXEXTRACTOR_H
#define DATA_MAXEXTRACTOR_H

#include "TrigRingerTools/data/FeatureExtractor.h"

namespace data {

  class MaxExtractor : public FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~MaxExtractor() {}

    /**
     * Evaluates the Pattern max.
     *
     * The max of a Pattern is the arithmetic max taken for all
     * values inside the set.
     *
     * @param in The Pattern from which evaluate the max
     */
    virtual data::Feature operator() (const data::Pattern& in) const;

  };

}

#endif /* DATA_MAXEXTRACTOR_H */
