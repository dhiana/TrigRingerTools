//Dear emacs, this is -*- c++ -*-

/**
 * @file data/Ensemble.h
 *
 * @brief Declares the Ensemble class (or type??). 
 */

#ifndef DATA_ENSEMBLE_H
#define DATA_ENSEMBLE_H

#include "TrigRingerTools/data/Pattern.h"

namespace data {

  /**
   * A simple alias that makes things look better. In this case, Ensemble means a collumn (a variable
   * of a pattern).
   */
  typedef data::Pattern Ensemble;
  
}

#endif //DATA_ENSEMBLE_H
