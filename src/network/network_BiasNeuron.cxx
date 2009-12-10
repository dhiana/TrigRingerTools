//Dear emacs, this is -*- c++ -*-

/**
 * @file BiasNeuron.cxx
 *
 * @brief The Bias Neuron class definition
 *
 * This file contains the definition for the Bias Neuron class.
 */

#include "TrigRingerTools/network/BiasNeuron.h"
#include "TrigRingerTools/sys/debug.h"

network::BiasNeuron::BiasNeuron (const data::Feature& bias,
				 const unsigned int* id)
  : InputNeuron(0, 1, id),
    m_bias(bias),
    m_state(1, bias)
{
}

network::BiasNeuron::BiasNeuron (const config::Neuron& config)
  : InputNeuron(config),
    m_bias(config.bias()),
    m_state(1, m_bias)
{
}

network::BiasNeuron::~BiasNeuron ()
{
}

void network::BiasNeuron::run (const data::Ensemble& data)
{
  if (data.size() != m_state.size()) {
    RINGER_DEBUG3("Resizing capacity of BiasNeuron from " << m_state.size()
		<< " to " << data.size());
    m_state = data::Ensemble(data.size(), m_bias);
  }
  InputNeuron::run(m_state); ///< Always run with the same data
}

config::Neuron network::BiasNeuron::dump (void) const
{
  return config::Neuron(id(), config::BIAS, 0, 0, m_bias);
}

bool network::BiasNeuron::dot(std::ostream& os) const
{
  InputNeuron::dot(os, true, m_bias);
  return true;
}
