//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/NormaliseOperator.cxx
 *
 * Implements the Mean Removal operator for Databases.
 */

#include "TrigRingerTools/data/NormaliseOperator.h"

void data::NormaliseOperator::operator() (const data::Pattern& in, 
					  data::Pattern& out) const
{
  out = in / m_max;
}

