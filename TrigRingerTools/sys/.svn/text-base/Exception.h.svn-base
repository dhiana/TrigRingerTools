//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/Exception.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.3 $
 * $Date: 2007-04-06 16:15:02 $
 *
 * @brief Describes possible exceptions in system in an abstract, general
 * form.
 */

#ifndef RINGER_SYS_EXCEPTION_H
#define RINGER_SYS_EXCEPTION_H

#include <stdexcept>

namespace sys {

  /**
   * Defines our standard exception type. It should include tools for complete
   * logging of the failure.
   */
  class Exception : std::exception {

  public: ///< interface

    /**
     * Constructor
     *
     * @param package The package name as a C-string
     * @param file The class (or filename) originating the exception as a
     * C-string 
     * @param lineno The line number in the source file where the exception
     * whas first thrown as a C-string
     * @param info A descriptive log as a C-string.
     */
    Exception (const std::string& package, const std::string& file,
	       const unsigned int& lineno, const std::string& info);

    /**
     * Destructor virtualisation
     */
    virtual ~Exception() throw() ;

    /**
     * Describes the exception
     */
    virtual const char* what() const throw() { return m_compose.c_str(); }

    /**
     * Tells us which package it is from
     */
    inline const char* package() const { return m_package.c_str(); }

    /**
     * Tells us which file this exception came from
     */
    inline const char* file() const { return m_file.c_str(); }

    /**
     * Tells us which line number on the file, the exception came from.
     */
    inline const char* lineno() const { return m_lineno.c_str(); }

    /**
     * What is the informational string about the exception
     */
    inline const char* info() const { return m_info.c_str(); }

    /**
     * The date when this exception was generated
     */
    inline const char* date() const { return m_date.c_str(); }

  private: //representation
    std::string m_package; ///< the package name
    std::string m_file; ///< the file name in the package
    std::string m_lineno; ///< the line number in the file
    std::string m_info; ///< a descriptive log of the exception
    std::string m_date; ///< the time where the exception was generated
    std::string m_compose; ///< the composed message string

  };

}

/**
 * A nicer handle to package exceptions
 */
#define RINGER_EXCEPTION(x) sys::Exception(__PACKAGE__, __FILE__, \
	                                   __LINE__, x)

#endif /* RINGER_SYS_EXCEPTION_H */
