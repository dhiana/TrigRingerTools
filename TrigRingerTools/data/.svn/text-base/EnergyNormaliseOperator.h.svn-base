//Dear emacs, this is -*- c++ -*-

/**
 * @file data/EnergyNormaliseOperator.h
 *
 * @brief Normalises the Pattern so the square root of its features is 1.
 */

#ifndef DATA_ENERGYNORMALISEOPERATOR_H
#define DATA_ENERGYNORMALISEOPERATOR_H

#include "TrigRingerTools/data/PatternOperator.h"

namespace data {

  class EnergyNormaliseOperator : public PatternOperator {

  public:

    /**
     * Destructor virtualisation
     */
    virtual ~EnergyNormaliseOperator() {}

    /**
     * Generates a Pattern with the same length as the previous, but with its
     * energy normalised to the square sum of all features is 1.
     *
     * @param in The Pattern to transform using this operator
     * @param out The Pattern to output
     */
    virtual void operator() (const data::Pattern& in,
			     data::Pattern& out) const;

  };

}

#endif /* DATA_ENERGYNORMALISEOPERATOR_H */
