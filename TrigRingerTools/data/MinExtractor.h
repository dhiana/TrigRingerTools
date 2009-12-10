//Dear emacs, this is -*- c++ -*-

/**
 * @file data/MinExtractor.h
 *
 * @brief This FeatureExtractor calculates the min of a Pattern.
 */

#ifndef DATA_MINEXTRACTOR_H
#define DATA_MINEXTRACTOR_H

#include "TrigRingerTools/data/FeatureExtractor.h"

namespace data {

  class MinExtractor : public FeatureExtractor {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~MinExtractor() {}

    /**
     * Evaluates the Pattern min.
     *
     * The min of a Pattern is the arithmetic min taken for all
     * values inside the set.
     *
     * @param in The Pattern from which evaluate the min
     */
    virtual data::Feature operator() (const data::Pattern& in) const;

  };

}

#endif /* DATA_MINEXTRACTOR_H */
