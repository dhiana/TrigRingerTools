//Dear emacs, this is -*- c++ -*-

/**
 * @file debug.cxx
 *
 * Defines a few debug utilities
 */

#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include <cstdlib>

bool sys::nr_print_debug(unsigned int level)
{
  std::string var = sys::getenv("RINGER_DEBUG");
  if (var.size() == 0) return false;
  
  //if you get here, you know there is something
  unsigned int v = std::strtoul(var.c_str(), 0, 0);
  if (level <= v) return true;
  return false;
}
