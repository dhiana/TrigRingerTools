//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/ReporterImplementation.h
 *
 * @brief This file contains the definition of what is a
 * sys::ReporterImplementation.
 */

#ifndef RINGER_SYS_REPORTERIMPLEMENTATION_H
#define RINGER_SYS_REPORTERIMPLEMENTATION_H

#include <string>

namespace sys {

  /**
   * Defines the interface between the current reporting system and
   * the sys::Reporter.
   */
  class ReporterImplementation {
    
  public:
    /**
     * Destructor virtualisation
     */
    virtual ~ReporterImplementation() {}

    /**
     * Write something to the reporter.
     * @param info What to write to the available stream.
     */
    virtual bool write (const std::string& info) =0;

    /**
     * Write something to the reporter, using a different strategy
     * though.
     * @param info What to write to the available stream.
     */
    virtual bool write_error (const std::string& info) =0;

  };

}

#endif //RINGER_SYS_REPORTERIMPLEMENTATION_H
