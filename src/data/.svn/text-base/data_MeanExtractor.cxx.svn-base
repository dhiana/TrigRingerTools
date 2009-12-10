//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/MeanExtractor.cxx
 *
 * Implements mean extraction
 */

#include "TrigRingerTools/data/MeanExtractor.h"
#include <gsl/gsl_statistics_double.h>

data::Feature data::MeanExtractor::operator() (const data::Pattern& in) const
{
  const gsl_vector* v = abuse(in);
  return gsl_stats_mean(v->data, v->stride, v->size);
}



