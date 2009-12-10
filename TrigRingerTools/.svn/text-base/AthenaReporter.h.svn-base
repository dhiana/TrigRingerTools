//Dear emacs, this is -*- c++ -*-

/**
 * @file AthenaReporter.h
 *
 * @brief This file contains the definition of what is a
 * sys::AthenaReporter. 
 */

#ifndef RINGER_SYS_ATHENAREPORTER_H
#define RINGER_SYS_ATHENAREPORTER_H

#include <iostream>
#include <string>

#include "TrigRingerTools/sys/Reporter.h"
#include "GaudiKernel/MsgStream.h"

namespace sys {

	/**
	* Defines a way to talk to a user that is sitting locally with
	* respect to the executing program.
	*/
	class AthenaReporter : public Reporter
	{

	private:
		MsgStream m_log;

	public:

		/**
		* The only constructor.
		* @param os The <em>normal</em> output stream
		* @param es The error stream
		*/
		AthenaReporter(IMessageSvc* svc, const std::string& source, int buffer_length=128)
			: m_log(svc, source, buffer_length) {}
			
		virtual ~AthenaReporter() {}
		
		bool report (const std::string& info)
		{
			m_log << MSG::INFO << info << endreq;
			return true;
		}
		
		bool warn (const std::string& info)
		{
			m_log << MSG::WARNING << info << endreq;
			return true;
		}

		bool fatal (const std::string& info)
		{
			m_log << MSG::FATAL << info << endreq;
			std::exit(1);
			return true;
		}

		bool except (const std::string& info)
		{
			m_log << MSG::WARNING << info << endreq;
			return true;
		}

	};

}

#endif /* RINGER_SYS_ATHENAREPORTER_H */
