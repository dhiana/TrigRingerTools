//Dear emacs, this is -*- c++ -*-

/**
 * @file config/src/SynapseRProp.cxx
 *
 * Implements Back Propagation parameter readout.
 */

#include "TrigRingerTools/config/SynapseRProp.h"
#include <cstdlib>

config::SynapseRProp::SynapseRProp(sys::xml_ptr_const node)
{
  m_weight_update = sys::get_attribute_double(node, "weightUpdate");
}

config::SynapseRProp::SynapseRProp(const double& weight_update)
  : m_weight_update(weight_update)
{
}

config::SynapseRProp::SynapseRProp (const SynapseRProp& other)
  : Parameter(),
    m_weight_update(other.m_weight_update)
{
}

config::SynapseRProp& config::SynapseRProp::operator= 
  (const SynapseRProp& other)
{
  m_weight_update = other.m_weight_update;
  return *this;
}

config::Parameter* config::SynapseRProp::clone () const
{
  return new SynapseRProp(m_weight_update);
}

sys::xml_ptr config::SynapseRProp::node (sys::xml_ptr any)
{
  sys::xml_ptr root = sys::make_node(any, "rBackPropagation");
  sys::put_attribute_double(root, "weightUpdate", m_weight_update);
  return root;
}
