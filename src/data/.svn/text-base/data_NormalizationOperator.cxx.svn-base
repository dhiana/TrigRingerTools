//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/NormalizationOperator.cxx
 *
 * Implements the Mean Removal operator for Databases.
 */

#include "TrigRingerTools/data/NormalizationOperator.h"

void data::NormalizationOperator::operator() (const data::Pattern& in, 
					      data::Pattern& out) const
{
  out = in - m_mean;
  out /= m_sd;
}
