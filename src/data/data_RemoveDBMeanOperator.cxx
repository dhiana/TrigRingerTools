//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/RemoveDBMeanOperator.cxx
 *
 * Implements the Mean Removal operator for Databases.
 */

#include "TrigRingerTools/data/RemoveDBMeanOperator.h"

void data::RemoveDBMeanOperator::operator() (const data::Pattern& in, 
					     data::Pattern& out) const
{
  out = in - m_mean;
}
