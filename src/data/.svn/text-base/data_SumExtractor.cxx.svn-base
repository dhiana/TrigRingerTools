//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/SumExtractor.cxx
 *
 * Implements sum extraction
 */

#include "TrigRingerTools/data/SumExtractor.h"
#include <gsl/gsl_statistics_double.h>

data::Feature data::SumExtractor::operator() (const data::Pattern& in) const
{
  const gsl_vector* v = abuse(in);
  //is there a faster way to do it? Not clear from the GSL info pages
  return v->size*gsl_stats_mean(v->data, v->stride, v->size);
}



