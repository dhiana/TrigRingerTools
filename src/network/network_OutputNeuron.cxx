//Dear emacs, this is -*- c++ -*-

/**
 * @file OutputNeuron.cxx
 *
 * @brief The base Neuron class definition
 *
 * This file contains the definition for the OutputNeuron base class.
 */

#include <vector>

#include "TrigRingerTools/network/OutputNeuron.h"
#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"

//for back-propagation techniques
#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/network/NeuronBackProp.h"

network::OutputNeuron::OutputNeuron 
(const config::NeuronStrategyType& strategy,
 const config::Parameter* parameters,
 const unsigned int* id)
  : Neuron(id),
    m_counter(0),
    m_isynapse(0),
    m_strategy(strategy),
    m_master(0),
    m_state(1, 0)
{
  switch (m_strategy) {
  case config::NEURON_BACKPROP:
    {
      m_master = new strategy::NeuronBackProp(parameters);
    }
    break;
  default:
    RINGER_DEBUG1("This is an unknown neuron strategy -> " << strategy);
    throw RINGER_EXCEPTION("Unknown strategy for neurons");
  }
}

network::OutputNeuron::OutputNeuron (const config::Neuron& config)
  : Neuron(config),
    m_counter(0), 
    m_isynapse(0),
    m_strategy(config.strategy()),
    m_master(0),
    m_state(1, 0)
{
  switch (m_strategy) {
  case config::NEURON_BACKPROP: 
    {
      m_master = new strategy::NeuronBackProp(config.parameters());
    }
    break;
  default:
    RINGER_DEBUG1("This is an unknown neuron strategy -> " << m_strategy);
    throw RINGER_EXCEPTION("Unknown strategy for neurons");
  }
}

network::OutputNeuron::~OutputNeuron ()
{
  if (m_isynapse.size() != 0) {
    RINGER_DEBUG1("Trying to destroy still connected Neuron!");
    throw RINGER_EXCEPTION("destroy connected output neuron");
  }
  delete m_master;
}

void network::OutputNeuron::run (const data::Ensemble& data)
{
  RINGER_DEBUG2("Passing data through OutputNeuron[" << id() 
	      << "]. Saving state...");
  if ( data.size() != m_state.size() ) {
      RINGER_DEBUG3("Resizing capacity of OutputNeuron from " << m_state.size()
		  << " to " << data.size());
      m_state = data::Ensemble(data.size());
      m_counter = 0;
  }
  else if ( m_counter == 0 ) {
    m_state = 0; //reset
    RINGER_DEBUG2("OutputNeuron[" << id() << "] run-reset = " << m_state); 
  }
  m_state += data; ///update output
  ++m_counter; ///got one more

  if ( m_counter == m_isynapse.size() ) { ///finalise
    m_counter = 0; //reset
    RINGER_DEBUG2("OutputNeuron[" << id() << "] activation = " << m_state);
    m_master->run(m_state);
    RINGER_DEBUG2("OutputNeuron[" << id() << "] output = " << m_state);
  }
}

void network::OutputNeuron::train (const data::Ensemble& error)
{
  RINGER_DEBUG2("Teaching OutputNeuron " << id() << ".");
  RINGER_DEBUG2("OutputNeuron[" << id() << "] error signal = " << error);
  m_master->teach(m_state, error);
  RINGER_DEBUG2("OutputNeuron[" << id() << "] delta = " << m_state);
  for (std::vector<network::Synapse*>::iterator it = m_isynapse.begin();
       it != m_isynapse.end(); ++ it) {
    RINGER_DEBUG2("OutputNeuron[" << id() 
		<< "] feeds backward signal to Synapse["
		<< (*it)->id() << "]");
    (*it)->learn(m_state);
  }
}

network::Neuron& network::OutputNeuron::in_connect (network::Synapse* l)
{
  std::vector<network::Synapse*>::iterator it;
  if ( (it = Neuron::get_synapse(m_isynapse, l)) != m_isynapse.end() ) {
    RINGER_DEBUG1("Cannot connect with the same Synapse twice. "
		<< "No actions taken. ");
    return *this;
  }

  m_isynapse.push_back(l);
  return *this;
}

network::Neuron& network::OutputNeuron::out_connect (network::Synapse* /*l*/) 
{
  RINGER_DEBUG1("Trying to connect something at the output." 
	      << "No actions taken.");
  return *this;
}

network::Neuron& network::OutputNeuron::in_disconnect 
(network::Synapse* l)
{
  std::vector<network::Synapse*>::iterator it;
  if ( (it = Neuron::get_synapse(m_isynapse, l)) == m_isynapse.end() ) {
    RINGER_DEBUG1("Trying to disconnect something not connected. " 
		<< "No actions taken. ");
    return *this;
  }

  m_isynapse.erase(it);
  return *this;
}

network::Neuron& network::OutputNeuron::out_disconnect
(network::Synapse* /*l*/)
{
  RINGER_DEBUG1("Trying to disconnect something at the output?? "
	      << "No actions taken.");
  return *this;
}

config::Neuron network::OutputNeuron::dump (void) const
{
  switch (m_strategy) {
  case config::NEURON_BACKPROP:
    {
      const strategy::NeuronBackProp* master =
	dynamic_cast<const strategy::NeuronBackProp*>(m_master);
      config::NeuronBackProp tmp = master->dump();
      return config::Neuron(id(), config::OUTPUT, &m_strategy, &tmp);
    }
    break;
  default:
    RINGER_DEBUG1("I cannot dump a configuration based on a unexisting"
		<< " strategy type = " << m_strategy << ". Exception thrown.");
    throw RINGER_EXCEPTION("Unknown strategy type");
  }
}

bool network::OutputNeuron::dot(std::ostream& os) const
{
  ++const_cast<unsigned int&>(m_counter);
  if (m_counter == m_isynapse.size()) {
    //os << id() << " [label=\"(" << id() << ") " << m_master->dot() 
    //   << "\"] ;" << std::endl;
    // +------------------------------+
    // |  neuron_id    |  <output>    |
    // |  <input>      |  act_function|
    // |  sum_symbol   |              |
    // +------------------------------+
    os << id() << " [shape=Mrecord, label=\"{{" << id() 
       << " | <input> (+)} | <output> " << m_master->dot() << " "
       << "}\"] ;" << std::endl;
    os << id() << ":output -> outlet" << id() << std::endl;
    os << "outlet" << id() << " [label=\"" << id() << "\",color=\"red\",fontcolor=white,"
       << "style=filled,shape=circle,width=0.1]" << std::endl;
    const_cast<unsigned int&>(m_counter) = 0;
  }
  return true;
}



