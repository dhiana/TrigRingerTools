//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/Synapse.cxx
 *
 * @brief This file holds the implementation details of the class
 * Synapse.
 */

#include "TrigRingerTools/network/Synapse.h"
#include "TrigRingerTools/network/Neuron.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/network/SynapseBackProp.h"
#include "TrigRingerTools/network/SynapseRProp.h"

unsigned int network::Synapse::s_id = 0; ///< initialisation

network::Synapse::Synapse (const data::Feature& init,
			   const config::SynapseStrategyType& strategy,
			   const config::Parameter* params, 
			   const unsigned int* id)
  : m_id(),
    m_weight(init),
    m_in(0),
    m_out(0),
    m_learns(true),
    m_state(1, 0),
    m_strategy(strategy),
    m_teacher(0)
{
  if (id) { //set the given id
    m_id = *id;
    s_id = *id + 1;
  }
  else m_id = s_id++; //get a new id

  switch(m_strategy) {
  case config::SYNAPSE_BACKPROP:
    m_teacher = new strategy::SynapseBackProp(params);
    break;
  case config::SYNAPSE_RPROP:
    m_teacher = new strategy::SynapseRProp(params);
    break;
  default:
    RINGER_DEBUG1("Unknown strategy type \"" << strategy 
		  << "\"! Exception thrown.");
    throw RINGER_EXCEPTION("Unknown strategy type");
  }
  RINGER_DEBUG3("Built synapse " << m_id << " with initial weight = " << init
	      << ", capacity = " << m_state.size());
}

network::Synapse::Synapse (const config::Synapse& config)
  : m_id(config.id()),
    m_weight(config.weight()),
    m_in(0),
    m_out(0),
    m_learns(true),
    m_state(1, 0),
    m_strategy(config.strategy()),
    m_teacher(0)
{
  switch(m_strategy) {
  case config::SYNAPSE_BACKPROP:
    m_teacher = new strategy::SynapseBackProp(config.parameters());
    break;
  case config::SYNAPSE_RPROP:
    m_teacher = new strategy::SynapseRProp(config.parameters());
    break;
  }
  RINGER_DEBUG3("Built synapse " << m_id << " with initial weight = " 
	      << m_weight << ", capacity = " << m_state.size());
}

network::Synapse::~Synapse()
{
  delete m_teacher;
  RINGER_DEBUG3("Destroying synapse " << m_id);
  disconnect();
}

void network::Synapse::learning (const bool& switch_to)
{
  m_learns = switch_to;
}

void network::Synapse::pass (const data::Ensemble& data)
{
  RINGER_DEBUG3("Passing data through synapse " << m_id 
		<< ". Saving state...");
#ifdef RINGER_DEBUG
  //helps counting resizements
  if ( data.size() != m_state.size() ) {
      RINGER_DEBUG3("Resizing capacity of Synapse from "
		  << m_state.size() << " to " << data.size());
  }
#endif
  m_state = data;
  m_state *= m_weight;

  /// Calls the output synapse
  m_out->run(m_state);
}

void network::Synapse::learn (const data::Ensemble& lesson)
{
  RINGER_DEBUG3("Teaching synapse " << m_id << ". Saving state...");
#ifdef RINGER_DEBUG
  //helps counting resizements
  if ( lesson.size() != m_state.size() ) {
      RINGER_DEBUG3("Reseting capacity of Synapse from " << m_state.size()
		  << " to " << lesson.size());
  }
#endif
  m_state = lesson; //copies the lesson to the current state
  data::Feature weight_change = 0;
  if (m_learns) {
    // Calls the input neuron, *before* adjusting the synaptic weight, to get
    // the weight adjustment right.
    weight_change = m_teacher->teach(m_in->state(), lesson);
  }
  // Forwards the error signal backward, before applying the weight adjustment
  m_state *= m_weight;
  RINGER_DEBUG2("Synapse[" << id() << "] feeds backward " << m_state
	      << " to Neuron[" << m_in->id() << "].");
  m_in->train(m_state);
  // Adjusts the weight
  m_weight += weight_change;
  RINGER_DEBUG2("Synapse[" << id() << "] weight is " << m_weight << " now.");
}

unsigned int network::Synapse::new_id (void)
{
  return s_id++;
}

unsigned int network::Synapse::id (void) const
{
  return m_id;
}

bool network::Synapse::operator== (const network::Synapse& other) const
{
  return m_id == other.m_id;
}

bool network::Synapse::operator!= (const network::Synapse& other) const
{
  return m_id != other.m_id;
}

void network::Synapse::connect (network::Neuron* start, 
				network::Neuron* end)
{
  RINGER_DEBUG3("Synapse " << m_id << " connects -start- neuron " 
	      << start->id() << " to -end- neuron " << end->id());

  if (m_in) m_in->out_disconnect(this);
  if (start) {
    start->out_connect(this);
    m_in = start;
  }
  else {
    RINGER_DEBUG1("Start neuron doesn't exist! Exception thrown.");
    throw RINGER_EXCEPTION("start neuron does not exist");
  }

  if (m_out) m_out->in_disconnect(this);
  if (end) {
    end->in_connect(this);
    m_out = end;
  }
  else {
    RINGER_DEBUG1("End neuron doesn't exist! Exception thrown.");
    throw RINGER_EXCEPTION("end neuron does not exist");
  }
}

void network::Synapse::disconnect ()
{
  if ( m_in && m_out ) {
    RINGER_DEBUG3("Synapse " << m_id << " disconnects -start- neuron "
		<< m_in->id() << " from -end- neuron " << m_out->id());
  }
	     
  if (m_in) m_in->out_disconnect(this);
  m_in = 0;
  if (m_out) m_out->in_disconnect(this);
  m_out = 0;
}
  
config::Synapse network::Synapse::dump(void) const
{
  switch (m_strategy) {
  case config::SYNAPSE_BACKPROP:
    {
      const strategy::SynapseBackProp* teacher =
	dynamic_cast<const strategy::SynapseBackProp*>(m_teacher);
      config::SynapseBackProp tmp = teacher->dump();
      return config::Synapse(m_id, m_in->id(), m_out->id(), m_weight,
			     m_strategy, &tmp);
    }
    break;
  case config::SYNAPSE_RPROP:
    {
      const strategy::SynapseRProp* teacher =
        dynamic_cast<const strategy::SynapseRProp*>(m_teacher);
      config::SynapseRProp tmp = teacher->dump();
      return config::Synapse(m_id, m_in->id(), m_out->id(), m_weight,
                             m_strategy, &tmp);
    }
    break;
  default:
    RINGER_DEBUG1("I cannot dump a configuration based on a unexisting"
		<< " strategy type = " << m_strategy << ". Exception thrown.");
    throw RINGER_EXCEPTION("Unknown strategy type");
  }
} 
