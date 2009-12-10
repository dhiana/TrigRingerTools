//Dear emacs, this is -*- c++ -*-

/**
 * @file InputNeuron.cxx
 *
 * @brief The Input Neuron class declaration
 *
 * This file contains the declaration for the Input Neuron class.
 */

#include "TrigRingerTools/network/InputNeuron.h"
#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"

network::InputNeuron::InputNeuron (const double& subtract,
				   const double& divide,
				   const unsigned int* id)
  : network::Neuron(id),
    m_osynapse(0),
    m_state(1, 0),
    m_subtract(subtract),
    m_divide(divide)
{
}

network::InputNeuron::InputNeuron (const config::Neuron& config)
  : network::Neuron(config),
    m_osynapse(0),
    m_state(1, 0),
    m_subtract(config.subtract()),
    m_divide(config.divide())
{
}

network::InputNeuron::~InputNeuron ()
{
  if (m_osynapse.size() != 0) {
    RINGER_DEBUG1("Trying to destroy neuron that is still connected!"
		<< ". Exception thrown.");
    throw RINGER_EXCEPTION("connected neuron destruction");
  }
}

void network::InputNeuron::run (const data::Ensemble& data)
{
  RINGER_DEBUG2("Passing data through InputNeuron[" << id() 
	      << "]. Saving state...");
#ifdef RINGER_DEBUG
  if ( data.size() != m_state.size() ) {
      RINGER_DEBUG3("Reseting capacity of InputNeuron from " << m_state.size()
		  << " to " << data.size());
  }
#endif
  m_state = data;
  //apply normalization factors
  m_state -= m_subtract;
  m_state /= m_divide;
  RINGER_DEBUG1("Applying normalization if input `(x-" << m_subtract
		<< "')/" << m_divide << " at InputNeuron[" << id() << "]...");
  //run
  RINGER_DEBUG2("InputNeuron[" << id() << "] in/output = " << m_state);
  for (std::vector<Synapse*>::iterator it= m_osynapse.begin();
       it != m_osynapse.end(); ++it) {
    RINGER_DEBUG2("InputNeuron[" << id()
		  << "] feeds forward signal to Synapse["
		  << (*it)->id() << "]");
    (*it)->pass(m_state);
  }
}

void network::InputNeuron::train (const data::Ensemble& /*error*/)
{
  RINGER_DEBUG1("Teaching InputNeuron[" << id() << "] is an empty action!");
}

/**
 * @todo Shall we throw exceptions here? Think about...
 */
network::Neuron& network::InputNeuron::in_connect (network::Synapse* /*l*/)
{
  RINGER_DEBUG1("In-connect InputNeuron is a forbideen action. Nothing done!");
  return *this;
}

network::Neuron& network::InputNeuron::in_disconnect (network::Synapse* /*l*/)
{
  RINGER_DEBUG1("In-disconnect InputNeuron is a forbideen act. Nothing done!");
  return *this;
}

network::Neuron& network::InputNeuron::out_connect (network::Synapse* l)
{
  std::vector<network::Synapse*>::iterator it;
  if ( (it = Neuron::get_synapse(m_osynapse, l)) != m_osynapse.end() ) {
    RINGER_DEBUG1("Cannot connect with the same Synapse twice. "
		<< "No actions taken. ");
    return *this;
  }

  m_osynapse.push_back(l);
  return *this;
}

network::Neuron& network::InputNeuron::out_disconnect (network::Synapse* l)
{
  std::vector<network::Synapse*>::iterator it;
  if ( (it = Neuron::get_synapse(m_osynapse, l)) == m_osynapse.end() ) {
    RINGER_DEBUG1("Trying to disconnect something not connected. "
		<< "No actions taken. ");
    return *this;
  }

  m_osynapse.erase(it);
  return *this;
}

config::Neuron network::InputNeuron::dump (void) const
{
  return config::Neuron(id(), config::INPUT, 0, 0, 0, m_subtract, m_divide);
}

bool network::InputNeuron::dot(std::ostream& os) const
{
  return dot(os, false, 1);
}

bool network::InputNeuron::dot(std::ostream& os, bool bias, data::Feature bias_value) const
{
  unsigned int counter = 0;
  for (std::vector<Synapse*>::const_iterator it = m_osynapse.begin();
       it != m_osynapse.end(); ++it) {
    if ((*it)->output()) {
      if (!bias) {
        os << "normalization" << id() << " -> " << id() << " [weight=20] ;" << std::endl
           << "normalization" << id() << " [label=\"(x-" << m_subtract
           << ")/" << m_divide << "\",arrowtail=dot,shape=box,style=filled,color=darkgreen,fontcolor=white] ;"
           << std::endl;
        os << id() << " -> " << (*it)->output()->id()  
           << " [weight=20,label=\"(" << (*it)->id() << ") " 
           << (*it)->weight() << "\"] ;" << std::endl;
      } else {
        os << "subgraph cluster" << id() << counter << " {" << std::endl
           << " style = invis;" << std::endl;
        //<< " label=\"MM\";\n color = lightgrey;\n style = filled;" << std::endl;
        os << " bias" << id() << counter << " -> " << (*it)->output()->id()
           << " [weight=1,constraint=false] ;" << std::endl;
        os << " bias" << id() << counter 
           << " [label=\"(" << id() << ") bias = " << (*it)->weight() * bias_value
          //<< " [label=\"bias = " << (*it)->weight() 
           << "\",arrowtail=dot,shape=plaintext] ;"
           << std::endl << "}" << std::endl;
        ++counter;
      }
      (*it)->output()->dot(os);
    }
  }
  if (!bias) os << id() << " [color=blue,fontcolor=white,style=filled,"
                << "shape=circle,width=0.1] ;" << std::endl;  
  return true;
}
