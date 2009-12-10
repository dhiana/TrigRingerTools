//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/MaxExtractor.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.2 $
 * $Date: 2007-04-06 16:15:02 $
 *
 * Implements the maxima extractor for a Pattern.
 */

#include "TrigRingerTools/data/MaxExtractor.h"

data::Feature data::MaxExtractor::operator() (const data::Pattern& in) const
{
  return gsl_vector_max(abuse(in));
}



