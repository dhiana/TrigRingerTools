//Dear emacs, this is -*- c++ -*- 

/**
 * @file sys/debug.h
 *
 * This file contains the definition of debug macros, that are enabled when
 * you compile your program with the <code>-DRINGER_DEBUG=X</code> option (where
 * X can be 1, 2 or 3). The best way to achieve that debugging capability is
 * by using macros, as they may be easily removed from the system when on
 * production.
 */

#ifndef RINGER_SYS_DEBUG_H
#define RINGER_SYS_DEBUG_H

#include <iostream>

/**
 * By default, switch off the debug output if nothing is said
 */
#ifndef RINGER_DEBUG
#define RINGER_DEBUG 0
#endif

namespace sys {

  /**
   * Defines if I should or not print the debug messages to std::cout
   *
   * @param level The level that has to be checked
   */
  bool nr_print_debug(unsigned int level);
}

/**
 * Highest level debugging output
 */
#if (RINGER_DEBUG>=1)
#define RINGER_DEBUG1(m) if (sys::nr_print_debug(1)) std::cout << " [DBG1] (" \
                                 << "/" << __FILE__ << "+" << __LINE__ \
                                 << ") " << m << std::endl
#else
#define RINGER_DEBUG1(m)
#endif

/**
 * Middle level debugging output
 */
#if (RINGER_DEBUG>=2)
#define RINGER_DEBUG2(m) if (sys::nr_print_debug(2)) std::cout << " [DBG2] (" \
                                 << "/" << __FILE__ << "+" << __LINE__ \
                                 << ") " << m << std::endl
#else
#define RINGER_DEBUG2(m)
#endif

/**
 * Lower level debugging output
 */
#if (RINGER_DEBUG>=3)
#define RINGER_DEBUG3(m) if (sys::nr_print_debug(3)) std::cout << " [DBG3] (" \
                                 << "/" << __FILE__ << "+" << __LINE__ \
                                 << ") " << m << std::endl
#else
#define RINGER_DEBUG3(m)
#endif

#endif //RINGER_SYS_DEBUG_H
