//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/EnergyNormaliseOperator.cxx
 *
 * Implements Pattern energy normalisation
 */

#include "TrigRingerTools/data/EnergyNormaliseOperator.h"
#include <gsl/gsl_blas.h>

void data::EnergyNormaliseOperator::operator() (const data::Pattern& in, 
						data::Pattern& out) const
{
  out = in;
  const gsl_vector* v = abuse(in);
  out /= gsl_blas_dnrm2(v);
}



