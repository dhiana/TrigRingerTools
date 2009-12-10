//Dear emacs, this is -*- c++ -*-

/**
 * @file InputNeuronNoNorm.cxx
 *
 * @brief The Input Neuron class declaration
 *
 * This file contains the declaration for the Input Neuron class.
 */

#include "TrigRingerTools/network/InputNeuronNoNorm.h"
#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"

network::InputNeuronNoNorm::InputNeuronNoNorm (const unsigned int* id)
  : network::Neuron(id),
    m_osynapse(0),
    m_state(1, 0)
{
}

network::InputNeuronNoNorm::InputNeuronNoNorm (const config::Neuron& config)
  : network::Neuron(config),
    m_osynapse(0),
    m_state(1, 0)
{
}

network::InputNeuronNoNorm::~InputNeuronNoNorm ()
{
  if (m_osynapse.size() != 0) {
    RINGER_DEBUG1("Trying to destroy neuron that is still connected!"
		<< ". Exception thrown.");
    throw RINGER_EXCEPTION("connected neuron destruction");
  }
}

void network::InputNeuronNoNorm::run (const data::Ensemble& data)
{
  RINGER_DEBUG2("Passing data through InputNeuronNoNorm[" << id() 
	      << "]. Saving state...");
#ifdef RINGER_DEBUG
  if ( data.size() != m_state.size() ) {
      RINGER_DEBUG3("Reseting capacity of InputNeuronNoNorm from " << m_state.size()
		  << " to " << data.size());
  }
#endif
  m_state = data;
  //run
  RINGER_DEBUG2("InputNeuronNoNorm[" << id() << "] in/output = " << m_state);
  for (std::vector<Synapse*>::iterator it= m_osynapse.begin();
       it != m_osynapse.end(); ++it) {
    RINGER_DEBUG2("InputNeuronNoNorm[" << id()
		  << "] feeds forward signal to Synapse["
		  << (*it)->id() << "]");
    (*it)->pass(m_state);
  }
}

void network::InputNeuronNoNorm::train (const data::Ensemble& /*error*/)
{
  RINGER_DEBUG1("Teaching InputNeuronNoNorm[" << id() << "] is an empty action!");
}

/**
 * @todo Shall we throw exceptions here? Think about...
 */
network::Neuron& network::InputNeuronNoNorm::in_connect (network::Synapse* /*l*/)
{
  RINGER_DEBUG1("In-connect InputNeuronNoNorm is a forbideen action. Nothing done!");
  return *this;
}

network::Neuron& network::InputNeuronNoNorm::in_disconnect (network::Synapse* /*l*/)
{
  RINGER_DEBUG1("In-disconnect InputNeuronNoNorm is a forbideen act. Nothing done!");
  return *this;
}

network::Neuron& network::InputNeuronNoNorm::out_connect (network::Synapse* l)
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

network::Neuron& network::InputNeuronNoNorm::out_disconnect (network::Synapse* l)
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

config::Neuron network::InputNeuronNoNorm::dump (void) const
{
  return config::Neuron(id(), config::INPUT, 0, 0, 0, 0, 1);
}

bool network::InputNeuronNoNorm::dot(std::ostream& os) const
{
  return dot(os, false, 1);
}

bool network::InputNeuronNoNorm::dot(std::ostream& os, bool bias, data::Feature bias_value) const
{
  unsigned int counter = 0;
  for (std::vector<Synapse*>::const_iterator it = m_osynapse.begin();
       it != m_osynapse.end(); ++it) {
    if ((*it)->output()) {
      if (!bias) {
        os << "normalization" << id() << " -> " << id() << " [weight=20] ;" << std::endl
           << "normalization" << id() << " [label=\"x\",arrowtail=dot,shape=box,style=filled,color=darkgreen,fontcolor=white] ;"
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

