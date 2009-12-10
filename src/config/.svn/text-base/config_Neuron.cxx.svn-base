//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/Neuron.cxx
 *
 * Implements neuron configuration read/write.
 */

#include "TrigRingerTools/config/Neuron.h"
#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"

config::Neuron::Neuron(sys::xml_ptr_const node)
  : m_params(0),
    m_bias(0),
    m_subtract(0),
    m_divide(1)
{
  sys::xml_ptr_const it = node;
  std::string type = sys::get_element_name(it);
  if (type == "input") m_type = config::INPUT;
  else if (type == "inputnonorm") m_type = config::INPUTNONORM;
  else if (type == "bias") m_type = config::BIAS;
  else if (type == "hidden") m_type = config::HIDDEN;
  else if (type == "output") m_type = config::OUTPUT;
  else {
    RINGER_DEBUG1("Neuron type " << type << " is unknown! Exception thrown.");
    throw RINGER_EXCEPTION("Unknown neuron type");
  }

  //Read identity
  m_id = sys::get_attribute_uint(node, "id");

  //Read bias attribute
  if (m_type == config::BIAS)
    m_bias = sys::get_attribute_double(node, "bias");

  //If input type, read subtract and divide normalization factors
  if (m_type == config::INPUT) {
    m_subtract = sys::get_attribute_double(node, "subtract");
    m_divide = sys::get_attribute_double(node, "divide");
  }

  if (m_type == config::HIDDEN || m_type == config::OUTPUT) {
    sys::xml_ptr_const c = sys::get_first_child(node); //parameters...
    if (!sys::is_element(c)) c = sys::get_next_element(c);
    if (sys::get_element_name(c) == "backPropagation") {
      m_strategy = config::NEURON_BACKPROP;
      m_params = new config::NeuronBackProp(c);
    }
    else {
      RINGER_DEBUG1("Unknown Hidden or Output neuron strategy => \"" 
		  << sys::get_element_name(c) << "\". Exception thrown.");
      throw RINGER_EXCEPTION("Unknown hidden or output neuron strategy");
    }
  }
  RINGER_DEBUG3("Loaded neuron \"" << m_id << "\"");
}

config::Neuron::Neuron (unsigned int id, const config::NeuronType& type,
			const config::NeuronStrategyType* strategy,
			const config::Parameter* params, const double& bias,
			const double& subtract, const double& divide)
  : m_id(id),
    m_type(type),
    m_strategy(),
    m_params(),
    m_bias(bias),
    m_subtract(subtract),
    m_divide(divide)
{
  if (m_type == config::HIDDEN || m_type == config::OUTPUT) {
    if (!strategy || !params) {
      RINGER_DEBUG1("Hidden or Output neurons need a strategy! "
		  << "Exception thrown.");
      throw RINGER_EXCEPTION("Unknown hidden or neuron strategy");
    }
    m_strategy = *strategy;
    m_params = params->clone();
  }
}

config::Neuron::Neuron(const Neuron& other)
  : m_id(other.m_id),
    m_type(other.m_type),
    m_strategy(other.m_strategy),
    m_params(0),
    m_bias(other.m_bias),
    m_subtract(other.m_subtract),
    m_divide(other.m_divide)
{
  if (m_type == config::HIDDEN || m_type == config::OUTPUT)
    m_params = other.m_params->clone();
}

config::Neuron& config::Neuron::operator= (const Neuron& other)
{
  m_id = other.m_id;
  m_type = other.m_type;
  m_strategy = other.m_strategy;
  if (m_type == config::HIDDEN || m_type == config::OUTPUT)
    m_params = other.m_params->clone();
  m_bias = other.m_bias;
  m_subtract = other.m_subtract;
  m_divide = other.m_divide;
  return *this;
}

config::Neuron::~Neuron () 
{
  delete m_params;
}

sys::xml_ptr config::Neuron::node (sys::xml_ptr any)
{
  sys::xml_ptr root = 0;
  switch (m_type) {
  case config::INPUT:
    root = sys::make_node(any, "input");
    sys::put_attribute_uint(root, "id", m_id);
    sys::put_attribute_double(root, "subtract", m_subtract);
    sys::put_attribute_double(root, "divide", m_divide);
    break;
  case config::INPUTNONORM:
    root = sys::make_node(any, "inputnonorm");
    sys::put_attribute_uint(root, "id", m_id);
    break;
  case config::BIAS:
    root = sys::make_node(any, "bias");
    sys::put_attribute_uint(root, "id", m_id);
    sys::put_attribute_double(root, "bias", m_bias);
    break;
  case config::HIDDEN:
    root = sys::make_node(any, "hidden");
    sys::put_attribute_uint(root, "id", m_id);
    sys::put_node(root, m_params->node(root));
    break;
  case config::OUTPUT:
    root = sys::make_node(any, "output");
    sys::put_attribute_uint(root, "id", m_id);
    sys::put_node(root, m_params->node(root));
    break;
  }
  return root;
}


