//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/util.cxx
 *
 * Implements the facilities described on the header file.
 */

#include "TrigRingerTools/data/util.h"
#include "TrigRingerTools/data/MeanExtractor.h"
#include "TrigRingerTools/data/MaxExtractor.h"
#include "TrigRingerTools/data/MinExtractor.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include <gsl/gsl_pow_int.h>
#include <cmath>

data::Feature data::mean_square (const data::PatternSet& p)
{
  data::MeanExtractor mean;
  data::Pattern tmp(p.pattern_size());
  data::Feature retval = 0;
  for (size_t i=0; i<p.size(); ++i) {
    tmp = p.pattern(i);
    tmp.apply(gsl_pow_2);
    retval += mean(tmp);
  }
  retval /= (p.size()*p.pattern_size());
  return retval;
}

data::Feature data::root_mean_square (const data::PatternSet& p)
{
  return sqrt(data::mean_square(p)); 
}

data::Feature data::abs_mean (const data::PatternSet& p)
{
  data::MeanExtractor mean;
  data::Pattern tmp(p.pattern_size());
  data::Feature retval = 0;
  for (size_t i=0; i<p.size(); ++i) {
    tmp = p.pattern(i);
    tmp.apply(fabs);
    retval += mean(tmp);
  }
  retval /= (p.size());
  return retval;
}

double data::sp (const data::PatternSet& output, 
		 const data::PatternSet& target,
		 double& eff1, double& eff2, double& thres)
{
  const double SCAN_STEP = 0.001; //1 per thousand of the target delta
  if (output.pattern_size() > 1 || target.pattern_size() > 1) {
    RINGER_DEBUG1("I can only calculate the SP product if the pattern sizes"
		  " of `output' and `target' are equal to 1. Exception"
		  " thrown.");
    throw RINGER_EXCEPTION("Cannot calculate SP product");
  }
  const data::Pattern o = output.ensemble(0);
  const data::Pattern t = target.ensemble(0);
  data::MinExtractor minima;
  data::Feature min = minima(t);
  data::MaxExtractor maxima;
  data::Feature max = maxima(t);
  data::Feature step = (max-min)*SCAN_STEP;
  data::Feature middle = (max+min)/2;
  double sp = 0;
  for (data::Feature i = min; i <= max; i += step) {
    unsigned int success1 = 0;
    unsigned int failure1 = 0;
    unsigned int success2 = 0;
    unsigned int failure2 = 0;
    for (unsigned int j = 0; j < t.size(); ++j) {
      if (t[j] > middle) {
	if (o[j] >= i) ++success2;
	else ++failure2;
      }
      else {
	if (o[j] < i) ++success1;
	else ++failure1;
      }
    }
    //calculates the SP product for this threshold
    double temp_eff1 = ((double)success1)/(success1 + failure1);
    double temp_eff2 = ((double)success2)/(success2 + failure2);
    double temp_sp = (temp_eff1 + temp_eff2) * (temp_eff1 * temp_eff2);
    if (temp_sp > sp) {
      sp = temp_sp;
      eff1 = temp_eff1;
      eff2 = temp_eff2;
      thres = i;
    }
  }
  return sp;
}
