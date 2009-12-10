//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/Reporter.h
 *
 * @brief This file contains the definition of what is a sys::Reporter. The
 * reporter should, among other things, report debugging messages
 * correctly. This is controlled by a small set of macros that correctly
 * address the debugging facilities of the Reporter. For using those, you have
 * to set a compilation macro accordingly: <code>DEBUG</code>=LOW, NORMAL or
 * HIGH. If this macro is undefined for the time of compilation, no
 * compilation options are coded in.
 */

#ifndef RINGER_SYS_REPORTER_H
#define RINGER_SYS_REPORTER_H

#include <string>
#include <sstream>
#include "TrigRingerTools/sys/ReporterImplementation.h"

/**
 * The <em>System</em> namespace contains system access objects.
 *
 * This namespace contains the functionality required by RINGER to get
 * in touch with the user environment, be it in form of messages that
 * are handled to the user or for reporting errors.
 */
namespace sys {

  /**
   * Defines the interface to the Reporting System.
   *
   * This class defines how the messages and errors are sent to the
   * monitoring user. Since the implementation of this class can
   * change depending on the environment, we have decoupled from its
   * interface by using the <em>bridge</em> pattern.
   */
  class Reporter {

  public:
    
    /**
     * Destructor virtualisation.
     */
    virtual ~Reporter() {};

  public:

    /**
     * Report something to the user.
     * @param info What to report.
     */
    virtual bool report (const std::string& info)=0;

    /**
     * Warn the user about a problem.
     * @param info What to warn about.
     */
    virtual bool warn (const std::string& info)=0;

    /**
     * Warn the user about a problem and std::exit() afterwards.
     * @param info What to report about the problem.
     */
    virtual bool fatal (const std::string& info)=0;

    /**
     * Warn the user about an exception.
     * @param info What to report about the problem.
     */
    virtual bool except (const std::string& info)=0;
    
		/**
		* Measures a string size and check if it is bellow a certain
		* limit. If not, break it in a special way to fit the screen.
		* @param s The string to check
		*/
		virtual std::string& check_string (std::string& s)
		{
			const int lsize = 72; ///< The lines maximum size
			int left = s.size() - lsize;
			size_t counter = 1;
			while ( left > 0 )
			{
				size_t pos = counter++ * lsize;
				s.insert(pos,"\\\n | ");
				///stopped here!
				left -= lsize;
			}
			return s;
		}
  };
  
}

/**
 * Make sure that we have verbose on if the user does not specify
 * anything else. If VERBOSE is set to 1 or above, warnings
 * messages are printed. If above or equal 2, report messages are
 * also printed. Fatal and exceptions messages are always printed.
 */
#ifndef RINGER_VERBOSE
#define RINGER_VERBOSE 2
#endif

/**
 * Defines a simpler way to report messages
 */
#if (RINGER_VERBOSE>=2) 
#define RINGER_REPORT(r,m) { \
    std::ostringstream s;  \
    s << m;                \
    r->report(s.str());	   }
#else
#define RINGER_REPORT(r,m)
#endif

/**
 * Defines a simpler way to report messages
 */
#define RINGER_FATAL(r,m) { \
    std::ostringstream s;  \
    s << m;                \
    r->fatal(s.str());	   }

/**
 * Defines a simpler way to report messages
 */
#define RINGER_EXCEPT(r,m) { \
    std::ostringstream s;  \
    s << m;                \
    r->except(s.str());	   }

/**
 * Defines a simpler way to report messages
 */
#if (RINGER_VERBOSE>=1)
#define RINGER_WARN(r,m) { \
    std::ostringstream s;  \
    s << m;                \
    r->warn(s.str());	   }
#else
#define RINGER_WARN(r,m)
#endif


#endif /* RINGER_SYS_REPORTER_H */ 

