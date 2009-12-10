//Dear emacs, this is -*- c++ -*-

/**
 * @file data/RemoveMeanOperator.h
 *
 * @brief This operator removes the mean of a Pattern.
 */

#ifndef DATA_REMOVEMEANOPERATOR_H
#define DATA_REMOVEMEANOPERATOR_H

#include "TrigRingerTools/data/PatternOperator.h"

namespace data {

  class RemoveMeanOperator : public PatternOperator {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~RemoveMeanOperator() {}

    /**
     * Generates a Pattern with the same length as the previous, but with the
     * mean removed from each Pattern feature.
     *
     * @param in The Pattern to transform using this operator
     * @param out The Pattern to output
     */
    virtual void operator() (const data::Pattern& in,
							 data::Pattern& out) const;

  };

}

#endif /* DATA_REMOVEMEANOPERATOR_H */
