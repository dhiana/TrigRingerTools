//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/Network.cxx
 *
 * Implements the neural network configuration interface class.
 */

#include "TrigRingerTools/config/Network.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/XMLProcessor.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include <cstdlib>

config::Network::Network(const std::string& filename,
			 sys::Reporter* reporter)
  : m_reporter(reporter),
    m_header(0),
    m_synapse(),
    m_neuron()
{
  std::string schema = sys::getenv("DATAPATH");
  if (schema.length() == 0) {
    RINGER_DEBUG1("I cannot find the standard schema path. Have you set"
		<< " DATAPATH? Exception thrown.");
    throw RINGER_EXCEPTION("DATAPATH not set");
  }
  
  schema = sys::look_in_path("network.xsd", schema);
  if (!schema.size())
  {
    throw RINGER_EXCEPTION("Schema file network.xsd not found in DATAPATH!");
  }
  
  sys::XMLProcessor xmlproc(schema, m_reporter);
  RINGER_DEBUG2("Trying to parse " << filename);
  sys::xml_ptr root = xmlproc.read(filename);
  if (!root) {
    RINGER_WARN(m_reporter, "XML network file " << filename 
	      << " cannot be parsed. Exception thrown.");
    throw RINGER_EXCEPTION("Cannot parse XML network file");
  }
  for (sys::xml_ptr_const it=sys::get_first_child(root); it; 
       it=sys::get_next_element(it)) {
    if (sys::is_element(it) && sys::get_element_name(it) == "header") {
      //loads the header information
      m_header = new config::Header(it);
      continue;
    }
    if (sys::is_element(it) && sys::get_element_name(it) == "layout") {
      //loads the network layout
      size_t ne = 0;
      size_t sy = 0;
      for (sys::xml_ptr_const jt=sys::get_first_child(it); jt; 
	   jt=sys::get_next_element(jt)) {
	if (!sys::is_element(jt)) continue;
	std::string name = sys::get_element_name(jt);
	if (sys::is_element(jt) && (name == "input" || name == "inputnonorm" || 
	                            name == "bias" ||
				    name == "hidden" || name == "output")) {
	  m_neuron.push_back(new Neuron(jt));
	  ++ne;
	}
	if (sys::is_element(jt) && name == "synapse") {
	  m_synapse.push_back(new Synapse(jt));
	  ++sy;
	}
      }
      RINGER_DEBUG2("Network file \"" << filename << "\" has "
		  << ne << " neuron(s).");
      RINGER_DEBUG2("Network file \"" << filename << "\" has " 
		  << sy << " synapse(s).");
    }
  }

  RINGER_DEBUG3("Configuration created from file \"" << filename << "\".");
}

config::Network::Network (const config::Header* header, 
			  const std::vector<Synapse*>& synapses,
			  const std::vector<Neuron*>& neurons,
			  sys::Reporter* reporter)
  : m_reporter(reporter),
    m_header(0),
    m_synapse(),
    m_neuron()
{
  if (header) m_header = new config::Header(*header);
  for (std::vector<Synapse*>::const_iterator it = synapses.begin(); 
       it != synapses.end(); ++it) m_synapse.push_back(new Synapse(**it));
  for (std::vector<Neuron*>::const_iterator it = neurons.begin(); 
       it != neurons.end(); ++it) m_neuron.push_back(new Neuron(**it));
  RINGER_DEBUG3("Configuration created from scratch.");
}

config::Network::~Network()
{
  delete m_header;
  for (std::vector<Synapse*>::iterator it = m_synapse.begin(); 
       it != m_synapse.end(); ++it) delete *it;
  for (std::vector<Neuron*>::iterator it = m_neuron.begin(); 
       it != m_neuron.end(); ++it) delete *it;
}

bool config::Network::save (const std::string& filename)
{
  RINGER_DEBUG2("Trying to save parsed document at \"" << filename << "\".");
  std::string schema = sys::getenv("DATAPATH");
  if (schema.length() == 0) {
    RINGER_DEBUG1("I cannot find the standard schema path. Have you set"
		<< " DATAPATH? Exception thrown.");
    throw RINGER_EXCEPTION("DATAPATH not set");
  }
  
  schema = sys::look_in_path("network.xsd", schema);
  if (!schema.size())
  {
    throw RINGER_EXCEPTION("Schema file network.xsd not found in DATAPATH!");
  }
  
  sys::XMLProcessor xmlproc(schema, m_reporter);
  sys::xml_ptr root = xmlproc.new_document("network");
  sys::put_attribute_text(root, "version", "0.3");
  sys::put_node(root, m_header->node(root));
  sys::xml_ptr layout = sys::put_element(root, "layout");
  for (std::vector<Neuron*>::iterator it = m_neuron.begin(); 
       it != m_neuron.end(); ++it) sys::put_node(layout, (*it)->node(layout));
  for (std::vector<Synapse*>::iterator it = m_synapse.begin(); 
       it != m_synapse.end(); ++it) sys::put_node(layout, (*it)->node(layout));
  RINGER_DEBUG2("Finally saving file...");
  if (!xmlproc.write(root, filename)) return false;
  RINGER_DEBUG2("File \"" << filename << "\" was saved.");
  return true;
}
