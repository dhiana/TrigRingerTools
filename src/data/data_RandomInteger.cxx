//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/RandomInteger.cxx
 *
 * @brief Implements the RandomInteger class.
 */

#include "TrigRingerTools/data/RandomInteger.h"
#include <ctime>
#include <cstdlib>

/**
 * Initialisation
 */
static data::RandomInteger s_rnd(time(0));

data::RandomInteger::RandomInteger (const size_t& seed)
  : m_seed(seed)
{
  if (seed != 0) srand(seed);
  else srand(time(0));
}

size_t data::RandomInteger::draw (const size_t& max) const
{
  double cache = (static_cast<double>(rand())/RAND_MAX)*max;
  return static_cast<size_t>(cache);
}

void data::RandomInteger::draw 
(const size_t& max, std::vector<size_t>& c) const
{
  for (size_t i=0; i<c.size(); ++i) c[i] = draw(max);
}

