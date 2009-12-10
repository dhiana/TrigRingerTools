//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/RemoveMeanOperator.cxx
 *
 * Implements mean removal operator.
 */

#include "TrigRingerTools/data/RemoveMeanOperator.h"
#include <gsl/gsl_statistics_double.h>

void data::RemoveMeanOperator::operator() (const data::Pattern& in, 
					   data::Pattern& out) const
{
  out = in;
  const gsl_vector* v = abuse(in);
  out -= gsl_stats_mean(v->data, v->stride, v->size);
}



