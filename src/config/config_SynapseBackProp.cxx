//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/SynapseBackProp.cxx
 *
 * Implements Back Propagation parameter readout.
 */

#include "TrigRingerTools/config/SynapseBackProp.h"
#include <cstdlib>

config::SynapseBackProp::SynapseBackProp(sys::xml_ptr_const node)
{
  m_learn_rate = sys::get_attribute_double(node, "learnRate");
  m_momentum = sys::get_attribute_double(node, "momentum");
  m_learn_rate_decay = sys::get_attribute_double(node, "learnRateDecay");
}

config::SynapseBackProp::SynapseBackProp
(const double& learning_rate, const double& momentum,
 const double& learning_rate_decay)
  : m_learn_rate(learning_rate),
    m_momentum(momentum),
    m_learn_rate_decay(learning_rate_decay)
{
}

config::SynapseBackProp::SynapseBackProp (const SynapseBackProp& other)
  : Parameter(),
    m_learn_rate(other.m_learn_rate),
    m_momentum(other.m_momentum),
    m_learn_rate_decay(other.m_learn_rate_decay)
{
}

config::SynapseBackProp& config::SynapseBackProp::operator= 
  (const SynapseBackProp& other)
{
  m_learn_rate = other.m_learn_rate;
  m_momentum = other.m_momentum;
  m_learn_rate_decay = other.m_learn_rate_decay;
  return *this;
}

config::Parameter* config::SynapseBackProp::clone () const
{
  return new SynapseBackProp(m_learn_rate, m_momentum, m_learn_rate_decay);
}

sys::xml_ptr config::SynapseBackProp::node (sys::xml_ptr any)
{
  sys::xml_ptr root = sys::make_node(any, "backPropagation");
  sys::put_attribute_double(root, "learnRate", m_learn_rate);
  sys::put_attribute_double(root, "momentum", m_momentum);
  sys::put_attribute_double(root, "learnRateDecay", m_learn_rate_decay);
  return root;
}
