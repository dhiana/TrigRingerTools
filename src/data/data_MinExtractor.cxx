//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/MinExtractor.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.2 $
 * $Date: 2007-04-06 16:15:02 $
 *
 * Implements the minima extractor for a Pattern.
 */

#include "TrigRingerTools/data/MinExtractor.h"

data::Feature data::MinExtractor::operator() (const data::Pattern& in) const
{
  return gsl_vector_min(abuse(in));
}



