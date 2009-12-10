//Dear emacs, this is -*- c++ -*- 

/**
 * @file Config.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.6 $
 * $Date: 2008-02-15 22:07:12 $
 *
 * Implements configuration readout.
 */

#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/XMLProcessor.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include <cstdlib>

rbuild::Config::Config (const std::string& filename, sys::Reporter* reporter)
  : m_reporter(reporter)
{
	std::string schema = sys::getenv("DATAPATH");
	if (schema.length() == 0)
	{
		RINGER_DEBUG1("I cannot find the standard schema path. Have you set" << " DATAPATH? Exception thrown.");
		throw RINGER_EXCEPTION("DATAPATH environment variable not set");
  }
  
  schema = sys::look_in_path("ring.xsd", schema);
  if (!schema.size())
  {
    throw RINGER_EXCEPTION("Schema file ring.xsd not found in DATAPATH!");
  }

	sys::XMLProcessor xmlproc(schema, m_reporter);
	RINGER_DEBUG2("Trying to parse " << filename);
	sys::xml_ptr root = xmlproc.read(filename);
	if (!root)
	{
		RINGER_WARN(m_reporter, "XML network file " << filename << " cannot be parsed. Exception thrown.");
		throw RINGER_EXCEPTION("Cannot parse XML network file");
  }
	
	for (sys::xml_ptr_const it=sys::get_first_child(root); it; it=sys::get_next_element(it))
	{
		if (sys::is_element(it) && sys::get_element_name(it) == "header") continue;
		if (sys::is_element(it) && sys::get_element_name(it) == "config")
		{
			//loads the job layout
			for (sys::xml_ptr_const jt=sys::get_first_child(it); jt; jt=sys::get_next_element(jt))
			{
				if (!sys::is_element(jt)) continue;
				std::string name = sys::get_element_name(jt);
				
				if (sys::is_element(jt) && name == "set")
				{
					typedef std::map<unsigned int, rbuild::RingConfig> map_type;
					unsigned int id = sys::get_attribute_uint(jt, "id");
					map_type::const_iterator it = m_config.find(id);
					
					if ( it != m_config.end() )
					{ //oops, that one already exists!
						//obs: this could be simplified when XML uniqueness start to
						//work under libxml2. It didn't as version 2.6.8
						RINGER_WARN(m_reporter, "RingSet with id=" << id << " already" << " exists. Ignoring new entry.");
						continue;
					}
					//ok, its a unique entry
					m_config[id] = rbuild::RingConfig(jt);
				}
      }
    }
  }

	RINGER_DEBUG2("Ring configuration file \"" << filename << "\" has " << m_config.size() << " valid entry(ies).");
	RINGER_DEBUG3("Configuration created from file \"" << filename << "\".");
}





