//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/HiddenNeuron.cxx
 *
 * @brief Defines the HiddenNeuron class.
 */

#include <vector>

#include "TrigRingerTools/network/HiddenNeuron.h"
#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"

//for back-propagation techniques
#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/network/NeuronBackProp.h"

network::HiddenNeuron::HiddenNeuron 
(const config::NeuronStrategyType& strategy,
 const config::Parameter* parameters,
 const unsigned int* id)
  : Neuron(id),
    m_counter(0), 
    m_isynapse(0),
    m_osynapse(0),
    m_strategy(strategy),
    m_master(0),
    m_state(1, 0),
    m_error(1, 0)
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

network::HiddenNeuron::HiddenNeuron (const config::Neuron& config)
  : Neuron(config),
    m_counter(0), 
    m_isynapse(0),
    m_osynapse(0),
    m_strategy(config.strategy()),
    m_master(0),
    m_state(1, 0),
    m_error(1, 0)
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

/**
 * @todo When destroying, disconnect all network::Synapses and warn.
 */
network::HiddenNeuron::~HiddenNeuron ()
{
  if (m_isynapse.size() != 0) {
    RINGER_DEBUG1("Trying to destroy still in-connected Neuron!");
    throw RINGER_EXCEPTION("destroy in-connected hidden neuron");
  }
  if (m_osynapse.size() != 0) {
    RINGER_DEBUG1("Trying to destroy still out-connected Neuron!");
    throw RINGER_EXCEPTION("destroy out-connected hidden neuron");
  }
  delete m_master;
}

void network::HiddenNeuron::run (const data::Ensemble& data)
{
  RINGER_DEBUG2("Passing data through HiddenNeuron[" << id() 
	      << "] and saving state...");
  if ( data.size() != m_state.size() ) {
    RINGER_DEBUG3("Resizing capacity of HiddenNeuron from " << m_state.size()
		<< " to " << data.size());
    m_state = data::Ensemble(data.size());
    m_counter = 0;
  }
  else if ( m_counter == 0 ) {
    m_state = 0; //reset
    RINGER_DEBUG3("HiddenNeuron[" << id() << "] run-reset = " << m_state);
  }
  m_state += data; //update output
  ++m_counter; ///got one more

  if ( m_counter == m_isynapse.size() ) { ///finalise
    m_counter = 0; //reset
    RINGER_DEBUG2("HiddenNeuron[" << id() << "] activation = " << m_state);
    m_master->run(m_state); ///activate
    RINGER_DEBUG2("HiddenNeuron[" << id() << "] output = " << m_state);
    for (std::vector<network::Synapse*>::iterator it = m_osynapse.begin();
	 it != m_osynapse.end(); ++ it) {
      RINGER_DEBUG2("HiddenNeuron[" << id() 
		  << "] feeds forward signal to Synapse["
		  << (*it)->id() << "]");
      (*it)->pass(m_state);
    }
  }
}

void network::HiddenNeuron::train (const data::Ensemble& error)
{
  RINGER_DEBUG2("Teaching HiddenNeuron[" << id() << "]. Saving state...");
  if ( error.size() != m_error.size() ) {
    RINGER_DEBUG3("Resizing learning capacity of HiddenNeuron from " 
		<< m_error.size() << " to " << error.size());
    m_error = data::Ensemble(error.size());
    m_counter = 0;
  }
  else if ( m_counter == 0 ) {
     m_error = 0; //reset
     RINGER_DEBUG3("HiddenNeuron[" << id() << "] train-reset = " << m_error);
  }
  m_error += error; //update error state
  ++m_counter; //got one more

  if ( m_counter == m_osynapse.size() ) { ///finalise
    m_counter = 0; //reset
    m_master->teach(m_state, m_error);
    RINGER_DEBUG2("HiddenNeuron[" << id() << "] prop. error = " << m_error);
    RINGER_DEBUG2("HiddenNeuron[" << id() << "] delta = " << m_state);
    for (std::vector<network::Synapse*>::iterator it = m_isynapse.begin();
	 it != m_isynapse.end(); ++ it) {
      RINGER_DEBUG2("HiddenNeuron[" << id() 
		  << "] feeds backward signal to Synapse["
		  << (*it)->id() << "]");
      (*it)->learn(m_state);
    }
  }
}

network::Neuron& network::HiddenNeuron::in_connect (network::Synapse* l)
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

network::Neuron& network::HiddenNeuron::out_connect (network::Synapse* l) 
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

network::Neuron& network::HiddenNeuron::in_disconnect 
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

network::Neuron& network::HiddenNeuron::out_disconnect
(network::Synapse* l)
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

config::Neuron network::HiddenNeuron::dump (void) const
{
  switch (m_strategy) {
  case config::NEURON_BACKPROP:
    {
      const strategy::NeuronBackProp* master =
	dynamic_cast<const strategy::NeuronBackProp*>(m_master);
      config::NeuronBackProp tmp = master->dump();
      return config::Neuron(id(), config::HIDDEN, &m_strategy, &tmp);
    }
    break;
  default:
    RINGER_DEBUG1("I cannot dump a configuration based on a unexisting"
		<< " strategy type = " << m_strategy << ". Exception thrown.");
    throw RINGER_EXCEPTION("Unknown strategy type");
  }
}

bool network::HiddenNeuron::dot(std::ostream& os) const
{
  ++const_cast<unsigned int&>(m_counter);
  if (m_counter == m_isynapse.size()) {
    for (std::vector<Synapse*>::const_iterator it = m_osynapse.begin();
	 it != m_osynapse.end(); ++it) {
      if ((*it)->output()) {
	os << id() << ":output -> " << (*it)->output()->id() 
	   << " [weight=10,label=\"(" << (*it)->id() << ") " 
	   << (*it)->weight() << "\"]" << std::endl;
	(*it)->output()->dot(os);
      }
    }
    //os << id() << " [label=\"(" << id() << ") " << m_master->dot() 
    //   << "\"] ;" << std::endl;
    // The general structure is:
    // +------------------------------+
    // |  neuron_id    |  <output>    |
    // |  <input>      |  act_function|
    // |  sum_symbol   |              |
    // +------------------------------+
    os << id() << " [shape=Mrecord, label=\"{{" << id() 
       << " | <input> (+)} | <output> " << m_master->dot() << " "
       << "}\"] ;" << std::endl;
    const_cast<unsigned int&>(m_counter) = 0;
  }
  return true;
}
