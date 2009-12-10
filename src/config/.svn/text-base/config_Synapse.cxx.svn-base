//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/Synapse.cxx
 *
 * Implements synapse configuration read/write.
 */

#include "TrigRingerTools/config/Synapse.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/data/RandomInteger.h"
#include <string>
#include <cstdlib>

static data::RandomInteger static_rnd; //default initializer, do it once

config::Synapse::Synapse(sys::xml_ptr_const node)
  : m_params(0)
{
  m_id = sys::get_attribute_uint(node, "id");
  m_from = sys::get_attribute_uint(node, "from");
  m_to = sys::get_attribute_uint(node, "to");

  //Read initial weight, if exists, or select it randomly between -1 and 1
  sys::xml_ptr_const c = sys::get_first_child(node);
  if (!sys::is_element(c)) c = sys::get_next_element(c);
  if (sys::get_element_name(c) == "weight") {
    m_weight = sys::get_element_double(c);
    RINGER_DEBUG2("Loaded initial weight for synapse " << m_id
		<< " = " << m_weight);
    c = sys::get_next_element(c);
  }
  else { //select it randomly between -1 and 1
    m_weight = static_rnd.draw(RAND_MAX);
    m_weight -= ((double)RAND_MAX)/2;
    m_weight /= ((double)RAND_MAX)/2;
    RINGER_DEBUG2("Selected random initial weight for synapse " << m_id
		<< " = " << m_weight);
  }
  
  if (sys::get_element_name(c) == "backPropagation") {
      m_strategy = config::SYNAPSE_BACKPROP;
      m_params = new config::SynapseBackProp(c);
  }
  else if (sys::get_element_name(c) == "rBackPropagation") {
      m_strategy = config::SYNAPSE_RPROP;
      m_params = new config::SynapseRProp(c);
  }
  else {
    RINGER_DEBUG1("I cannot find the strategy for synapse "
		<< m_id << "! Exception thrown.");
    throw RINGER_EXCEPTION("Undefined synapse strategy");
  }
  RINGER_DEBUG3("Loaded synapse \"" << m_id << "\"");
}

config::Synapse::Synapse (unsigned int id,
			  unsigned int from,
			  unsigned int to,
			  const double& weight,
			  const config::SynapseStrategyType& strategy,
			  const config::Parameter* params)
  : m_id(id),
    m_from(from),
    m_to(to),
    m_strategy(strategy),
    m_params(0),
    m_weight(weight)
{
  if (params) m_params = params->clone();
}

config::Synapse::Synapse (const Synapse& other)
  : m_id(other.m_id),
    m_from(other.m_from),
    m_to(other.m_to),
    m_strategy(other.m_strategy),
    m_params(0),
    m_weight(other.m_weight)
{
  m_params = other.m_params->clone();
}

config::Synapse& config::Synapse::operator= (const Synapse& other)
{
  m_id = other.m_id;
  m_from = other.m_from;
  m_to = other.m_to;
  m_strategy = other.m_strategy;
  m_weight = other.m_weight;
  if (m_params) {
    delete m_params;
    m_params = other.m_params->clone();
  }
  return *this;
}

config::Synapse::~Synapse () {
  delete m_params;
}

sys::xml_ptr config::Synapse::node (sys::xml_ptr any)
{
  sys::xml_ptr root = sys::make_node(any, "synapse");
  sys::put_attribute_uint(root, "id", m_id);
  sys::put_attribute_uint(root, "from", m_from);
  sys::put_attribute_uint(root, "to", m_to);
  sys::put_element_double(root, "weight", m_weight);
  sys::put_node(root, m_params->node(root));
  return root;
}
