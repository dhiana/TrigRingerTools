//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/NeuronBackProp.cxx
 *
 * Implements Back Propagation parameter readout.
 */

#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"

config::NeuronBackProp::NeuronBackProp(sys::xml_ptr_const node)
{
  std::string funct = sys::get_attribute_string(node, "activationFunction");
  if (funct == "tanh") {
    RINGER_DEBUG2("I will use tanh as the activation function.");
    m_af = config::NeuronBackProp::TANH;
  }
  else if (funct == "sigmoid") {
    RINGER_DEBUG2("I will use sigmoid as the activation function.");
    m_af = config::NeuronBackProp::SIGMOID;
  }
  else if (funct == "linear") {
    RINGER_DEBUG2("I will use identity as the activation function.");
    m_af = config::NeuronBackProp::LINEAR;
  }
  else {
    RINGER_DEBUG1("Backpropagation activation function \"" << funct
		<< "\" is unknown to RINGER. Exception thrown.");
    throw RINGER_EXCEPTION("Unknown backprop activation function");
  }
}

config::NeuronBackProp::NeuronBackProp
(const ActivationFunction& af)
  : m_af(af)
{
}

config::NeuronBackProp::NeuronBackProp(const NeuronBackProp& other)
  : Parameter(), m_af(other.m_af)
{
}

config::NeuronBackProp& config::NeuronBackProp::operator= 
(const NeuronBackProp& other)
{
  m_af = other.m_af;
  return *this;
}

config::Parameter* config::NeuronBackProp::clone () const
{
  return new NeuronBackProp(m_af);
}

sys::xml_ptr config::NeuronBackProp::node (sys::xml_ptr any)
{
  sys::xml_ptr root = sys::make_node(any, "backPropagation");
  switch (m_af) {
  case TANH:
    sys::put_attribute_text(root, "activationFunction", "tanh");
    break;
  case SIGMOID:
    sys::put_attribute_text(root, "activationFunction", "sigmoid");
    break;
  case LINEAR:
    sys::put_attribute_text(root, "activationFunction", "linear");
    break;
  }
  return root;
}

