//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/Neuron.cxx
 *
 * @brief The base Neuron class definition
 *
 * This file contains the implementation of the unique identification
 * number genearator for Neuron's.
 */

#include "TrigRingerTools/network/Neuron.h"
#include "TrigRingerTools/network/Synapse.h"

unsigned int network::Neuron::s_id = 0; ///< initialisation

network::Neuron::Neuron(const unsigned int* id)
  : m_id()
{
  if (id) { //set the given identifier and the static attributor
    m_id = *id;
    s_id = *id + 1;
  }
  else m_id = s_id++;
}

network::Neuron::Neuron (const config::Neuron& config)
  : m_id(config.id())
{
}

network::Neuron::~Neuron()
{
}

std::vector<network::Synapse*>::iterator network::Neuron::get_synapse
(std::vector<network::Synapse*>& c, const network::Synapse* l)
{
  std::vector<network::Synapse*>::iterator it;
  for (it = c.begin(); it != c.end(); ++ it) 
    if (**it==*l) break; ///<compare id's
  return it;
}
