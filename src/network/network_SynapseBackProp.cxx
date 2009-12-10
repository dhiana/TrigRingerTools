//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/SynapseBackProp.cxx
 *
 * @brief This file contains the definition of the Back Propagation
 * Learning algorithm. It is used for network::Synapse learning in Back
 * Propagation mode.
 */

#include "TrigRingerTools/network/SynapseBackProp.h"
#include "TrigRingerTools/config/SynapseBackProp.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/MeanExtractor.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include <cmath>

strategy::SynapseBackProp::SynapseBackProp 
(const data::Feature& lrate, const data::Feature& momentum,
 const data::Feature& decay)
  : m_lrate(lrate), 
    m_momentum(momentum), 
    m_decay(decay),
    m_prev_delta(0)
{
  //checks on ranges are performed at XML configuration parsing!
  RINGER_DEBUG3("Instantiated with Learning Rate = " << m_lrate
	      << ", Momentum = " << m_momentum << " and LR Decay = " 
	      << m_decay << ".");
}

strategy::SynapseBackProp::SynapseBackProp (const config::Parameter* config)
  : m_lrate(0), 
    m_momentum(0), 
    m_decay(0), 
    m_prev_delta(0)
{
  const config::SynapseBackProp* bpparams =
    dynamic_cast<const config::SynapseBackProp*>(config);
  m_lrate = bpparams->learning_rate();
  m_momentum = bpparams->momentum();
  m_decay = bpparams->learning_rate_decay();
  //checks on ranges are performed at XML configuration parsing!
  RINGER_DEBUG3("Instantiated with Learning Rate = " << m_lrate
	      << ", Momentum = " << m_momentum << " and LR Decay = "
	      << m_decay << ".");
}

data::Feature strategy::SynapseBackProp::teach (const data::Ensemble& input,
						const data::Ensemble& lesson)
{
  RINGER_DEBUG3("SynapseBackProp::teach called.");
  data::MeanExtractor mean;
  data::Feature delta = m_lrate * mean(lesson * input);
  RINGER_DEBUG2("Calculating synaptic weight adjustment with change = "
		<< delta << ", learning rate = " << m_lrate
		<< ", momentum = " << m_momentum
		<< " and learning rate decay = " << m_decay);
  data::Feature retval = (1-m_momentum)*delta + (m_momentum)*m_prev_delta;
  RINGER_DEBUG3("Previous change saved (" << delta << ")");
  if (delta <= m_prev_delta) {
    data::Feature mult = (1 + (1 - m_decay)/10);
    RINGER_DEBUG3("Increasing learning rate with factor " << mult);
    m_lrate *= mult;
  }
  else {
    RINGER_DEBUG3("Decreasing learning rate with factor " << m_decay);
    m_lrate *= m_decay;
  }
  RINGER_DEBUG2("Next learning rate is going to be " << m_lrate);
  m_prev_delta = delta;
  return retval;
}

config::SynapseBackProp strategy::SynapseBackProp::dump (void) const
{
  return config::SynapseBackProp(m_lrate, m_momentum, m_decay);
}
