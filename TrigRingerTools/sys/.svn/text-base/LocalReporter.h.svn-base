//Dear emacs, this is -*- c++ -*- 

/**
 * @file sys/LocalReporter.h
 *
 * @brief This file contains the definition of what is a
 * sys::LocalReporter. 
 */

#ifndef RINGER_SYS_LOCALREPORTER_H
#define RINGER_SYS_LOCALREPORTER_H

#include <iostream>
#include <string>

#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/util.h"

#include <cstdlib>

namespace sys {

	/**
	* Defines a way to talk to a user that is sitting locally with
	* respect to the executing program.
	*/
	class LocalReporter : public Reporter
	{
	public:

		/**
		* The only constructor.
		* @param os The <em>normal</em> output stream
		* @param es The error stream
		*/
		LocalReporter(std::ostream& os =std::cout, std::ostream& es =std::cerr)
		: m_os(os),
			m_es(es)
		{
			///Start-up report: the date
			std::ostringstream oss;
			oss << "This is the NeuralRinger v" << NR_VERSION << " Reporter.";
			report(oss.str());
#ifdef XERCES_XML_BACK_END
			report("NeuralRinger was compiled to use Apache's XercesC++.");
#else
			report("NeuralRinger was compiled to use Gnome's Libxml2.");
#endif /* XERCES_XML_BACK_END */
//			RINGER_DEBUG1("Debug level 1 is activated.");
//			RINGER_DEBUG2("Debug level 2 is activated.");
//			RINGER_DEBUG3("Debug level 3 is activated.");
		}

		virtual ~LocalReporter()
		{
			//Shutdown report.
			std::ostringstream oss;
			oss << "NeuralRinger v" << NR_VERSION << " Reporter has finished.";
			report(oss.str());
		}

		bool report (const std::string& info)
		{
			m_os << sys::currenttime("short") << ": " << info << '\n';
			return bool (m_os);
		}
		
		bool warn (const std::string& info)
		{
			m_os << sys::currenttime("short") << ": **WARNING** " << info << '\n';
			return (m_os);
		}
		
		bool fatal (const std::string& info)
		{
			m_es << sys::currenttime("short") << ": **FATAL** " << info << '\n';
			std::exit(1);
			return true;
		}
		
		bool except (const std::string& info)
		{
			m_os << sys::currenttime("short") << ": **EXCEPTION** " << info << '\n';
			return bool (m_os);
		}

	private: ///Representation
		std::ostream& m_os; ///< The standard output stream
		std::ostream& m_es; ///< The error stream
	};

}

#endif /* RINGER_SYS_LOCALREPORTER_H */

