//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/VarianceExtractor.cxx
 *
 * Implements the variance calculation on a Pattern
 */

#include "TrigRingerTools/data/VarianceExtractor.h"
#include <gsl/gsl_statistics_double.h>

data::Feature data::VarianceExtractor::operator() 
  (const data::Pattern& in) const
{
  const gsl_vector* v = abuse(in);
  return gsl_stats_variance(v->data, v->stride, v->size);
}



